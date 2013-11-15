/**
 * Tact example for handling capacitive sensing.
 * Copyright (C) 2013, Tomek Ness and Studio NAND
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Arduino sketch https://github.com/StudioNAND/tact-arduino-sketch
 * Processing lib https://github.com/StudioNAND/tact-processing
 *
 * This example is inspired by Mads Hobye's instructables tutorial - thanks!
 * http://www.instructables.com/id/Touche-for-Arduino-Advanced-touch-sensing/
 */

// Serial, symbols per second
#define BAUD_RATE 115200

// Bit set/clear macros
#define SET(x,y) (x |=(1<<y))
#define CLR(x,y) (x &= (~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))

// How many Tact sensor inputs are multiplexed?
#define NUMBER_OF_SENSORS 3

// Frequency spectrum start index
#define FREQUENCY_START 65
// Frequency spectrum length
#define FREQUENCY_LENGTH 32

#define MP_4051_S0 2
#define MP_4051_S1 3
#define MP_4051_S2 4

int frequencyLength = FREQUENCY_LENGTH;
int frequencyStart = FREQUENCY_START;
int results[FREQUENCY_LENGTH];

// Status flag set by client (i.e. Processing)
boolean clientRequested = false;

// Present command mode
// 0=celar; 1=getter; 2=setter
int commandMode = 0;

void setup () {
  // Start up serial communication
  Serial.begin (BAUD_RATE);

  // Set up frequency generator
  TCCR1A = 0b10000010;
  TCCR1B = 0b00011001;
  // ICR1 = 110;
  // OCR1A = 55;

  // Signal generator pins
  pinMode (7, INPUT);
  pinMode (8, OUTPUT);
  // Sync (test) pin
  pinMode (9, OUTPUT);

  pinMode (MP_4051_S0, OUTPUT);  // s0
  pinMode (MP_4051_S1, OUTPUT);  // s1
  pinMode (MP_4051_S2, OUTPUT);  // s2

  // Fill up result array with default=0
  for (int i = 0; i < frequencyLength; i++)
    results[i] = 0;
}

void loop () {

  // While there is anything in the 
  // pipe that has not been processed...
  while (Serial.available () > 0) {
    // Read the serial input
    serialEvent (Serial.read ());
  }

  // If signal data has been requested 
  // by the client (i.e. Processing) ...
  if (clientRequested) {

    // For each Tact sensor-input ...
    for (int count=0; count < NUMBER_OF_SENSORS; count++) {

      // Select the sensor-input / bit
      // (use this with arduino 0013+)
      digitalWrite (MP_4051_S0, bitRead (count, 0));
      digitalWrite (MP_4051_S1, bitRead (count, 1));
      digitalWrite (MP_4051_S2, bitRead (count, 2));

      // Announce which of the Tact-inputs that 
      // are multiplexed will be transmitted
      sendValue (3000 + count);

      for (unsigned int d = frequencyStart; d < frequencyLength+frequencyStart; d++) {
        // Reload new frequency
        TCNT1 = 0;
        ICR1 = d;
        OCR1A = d/2;

        // Restart generator
        SET (TCCR1B, 0);
        // Read response signal
        int v = analogRead (0);
        // Stop generator
        CLR (TCCR1B, 0);

        results[d-frequencyStart] = (float) v;
      }

      // Tell client that a result array 
      // is about to be dispatched
      sendValue (2000);
      // Wait ...
      delay (1);
      // Send signal spectrum
      for (int x=0;  x < frequencyLength;  x++)
        sendValue (results[x]);
      
      // Confirm that signal spectrum 
      // has been delivered, done!
      sendValue (2001);
    }

    // Set the request-indicator back 
    // to false until client resets it
    clientRequested = false;
  }
  // Toggle pin 9 after each 
  // sweep (good for scope)
  TOG (PORTB, 0);
}

