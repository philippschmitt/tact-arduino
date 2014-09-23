/**
 * Tact Library for handling capacitive sensing.
 * Copyright (C) 2013, Tomek Ness, Jack Rusher and Studio NAND
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
 * Arduino library https://github.com/StudioNAND/tact-arduino-sketch
 * Processing library https://github.com/StudioNAND/tact-processing
 *
 * This example is inspired by Mads Hobye's instructables tutorial
 * http://www.instructables.com/id/Touche-for-Arduino-Advanced-touch-sensing/
 */


// Include Arduino and Tact.h Header
#include "Arduino.h"
#include "Tact.h"


// Constructor
Tact::Tact(int test) {
	
	// set sensor count
	_sensors = 0;
}


// Destructor
Tact::~Tact() {
	// nothing to destroy
}


// Init new Tact Toolkit
void Tact::begin() {

	// Set application state
	int state = STATE_IDLE;

	// Start up serial communication
	Serial.begin (BAUD_RATE);

	// Set up frequency generator
	TCCR1A = 0b10000010;
	TCCR1B = 0b00011001;

	// Signal generator pins
	pinMode (7, INPUT);
	pinMode (8, OUTPUT);
	// Sync (test) pin
	pinMode (9, OUTPUT);

}


// Add Sensor
TactSensor Tact::addSensor(int _indexStart, int _indexCount, int _indexStep) {

	// create cmdBuffer for new sensor
	int cmdBuffer[4] = {_sensors, _indexStart, _indexCount, _indexStep };

	// increase sensor counter by 1
	_sensors++;

	// init new sensor object
	TactSensor sensor(this, cmdBuffer);

	// give them something to play with
	return sensor;
}


// Process incoming Serial data
void Tact::serialEvent (const byte inByte) {
	/*
	switch (inByte) {
		
		case '\n':
			// Process command
			execute();
			break;

		case CMD_SEPARATOR:
			// If token is command-separator: then increment 
			// command-index to address the next field of 
			// the command-buffer
			cmdIndex++;
			break;

		// If none of the above
		default:
			// Test if byte represents a digit and 
			// update existing int value
			if (inByte > 47 && inByte < 58) {
				cmdBuffer[cmdIndex] = cmdBuffer[cmdIndex] * 10 + (int) (inByte - 48);
				// If in range from A-Z
			} else if (inByte > 64 && inByte < 91) {
				// Set command key/name.
				cmdKey = inByte;

				if (inByte == 'G' || inByte == 'P' || inByte == 'B') {
					state = STATE_RECEIVE_CMD;
				}

				// reset the command-index to -1
				cmdIndex = -1;
				// clear list of command parameters
				cmdBuffer[0] = 0;
				cmdBuffer[1] = 0;
				cmdBuffer[2] = 0;
				cmdBuffer[3] = 0;

			} else {
				// ERROR - unexpected token in parameter stream
			}
			break;
	}
	*/
}


/* y = 01010100 11010100    (value is a 2 Byte integer)
 *     yMSB     yLSB        send seperately -> joined by client
 */
void Tact::sendInt (int value) {
	/*
	Serial.write (byte(lowByte(value)));   // send Low Byte  
	Serial.write (byte(highByte(value)));  // send high Byte   
	*/
}


/**
 * Function to execute current set command, called 
 * when new-line byte (10) has been received. 
 */
void Tact::execute () {
	/*
	switch (cmdKey) {
		case 'G':
			state = STATE_TRANSMIT_SENSOR;
			break;

		case 'P':
			state = STATE_TRANSMIT_PEAK;
			break;

		case 'B':
			state = STATE_TRANSMIT_BIAS;
			break;

		case 'V':
			sendInt (5000 + VERSION);
			break;
	}
	*/
}


int Tact::readSensor(int _cmdBuffer[4]) {

	// Declare sensor value buffer 
    int results[ _cmdBuffer[CMD_BUFFER_INDEX_COUNT]];
    // Declare peak and bias vars
    int peak = 0;
    int bias = 0;

    for (unsigned int d = 0; d < _cmdBuffer[CMD_BUFFER_INDEX_COUNT]; d++) {
		// Reload new frequency
		TCNT1 = 0;
		ICR1 = _cmdBuffer[CMD_BUFFER_INDEX_START] + _cmdBuffer[CMD_BUFFER_INDEX_STEP] * d;
		OCR1A = ICR1 / 2;

		// Restart generator
		SET (TCCR1B, 0);
		// Read response signal
		results[d] = (float) analogRead(0);
		// Stop generator
		CLR (TCCR1B, 0);

		// Check if current result is higher than previously stored peak
		// if true, overwrite peak and bias
		if( results[d] > peak ) {
			peak = results[d];
			bias = d;
		}
    }

    return bias;
}

