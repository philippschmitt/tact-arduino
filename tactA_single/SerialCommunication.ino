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
 * This example is inspired by Mads Hobye's instructables tutorial
 * http://www.instructables.com/id/Touche-for-Arduino-Advanced-touch-sensing/
 */

/**
 * Function for handling incoming serial events.
 * This includes parsing and respoding of/to commands 
 * and switching to different operating modes. Those 
 * are represented by "commandMode", which can be 0, 1 
 * or two. Both last ones stand for getter and setter. 
 * Threreby spectrum start index and length can be 
 * easily modified and adjusted.
 *
 * @param in the byte to process.
 * @return void
 */
void serialEvent(byte in){
  
  if (in == 'v') {
    sendInt (VERSION);
  }
  
  // Secrete handshake for the client to test 
  // if Tact driven Arduino is present.
  else if (in == 'h') {
    if (Serial.available() > 0)
      // If next char is an 'i'
      if (Serial.read() == 105)
        // Send '5'
        sendInt(53);
  }
  
  // Reset the command buffer index
  // and change status to "Receiving"
  else if (in == 'g') {
    cmdIndex = 0;
    state = STATE_RECEIVE_CMD;
  }
  
  // For newline, stop receiving command 
  // buffer values - start transmitting
  else if (in == 10) {
    state = STATE_TRANSMIT_SENSOR;
  }
  
  // Reads integer values from Serial stream
  else if (in > 47 && in < 59) {
    
    // Start the int value with the byte
    // that just has been received
    int value = (in - 48);
    
    // Stay in the while loop until
    // char ";" comes through the pipe
    while (true){
      // If there's still data in the pipe ...
      if (Serial.available() > 0) {
        // Read byte from serial stream
        in = Serial.read();
        // If serial event is not a ";" ASCII code 
        // then add event to value buffer
        if (in != ';') {
          value = value * 10 + (int) (in - 48);
        }else{
          // Integer stream just ended,
          // exit the while loop.
          break;
        }
      }
    }
    
    // Store integer value in command buffer
    cmdBuffer[cmdIndex] = value;
    // Update index pointer
    cmdIndex++;
  }
}

/* y = 01010100 11010100    (value is a 2 Byte integer)
 *     yMSB     yLSB        send seperately -> joined by client
 */
void sendInt (int value) {
  Serial.write (byte(lowByte(value)));   // send Low Byte  
  Serial.write (byte(highByte(value)));  // send high Byte   
}
