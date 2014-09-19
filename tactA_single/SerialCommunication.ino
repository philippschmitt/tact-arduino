/**
 * Tact example for handling capacitive sensing.
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
 * Arduino sketch https://github.com/StudioNAND/tact-arduino-sketch
 * Processing lib https://github.com/StudioNAND/tact-processing
 *
 * This example is inspired by Mads Hobye's instructables tutorial
 * http://www.instructables.com/id/Touche-for-Arduino-Advanced-touch-sensing/
 */
 
void serialEvent (const byte inByte) {
  
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
      }else if (inByte > 64 && inByte < 91) {
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
      }else{
        // ERROR - unexpected token in parameter stream
      }
      break;
  }
}

/* y = 01010100 11010100    (value is a 2 Byte integer)
 *     yMSB     yLSB        send seperately -> joined by client
 */
void sendInt (int value) {
  Serial.write (byte(lowByte(value)));   // send Low Byte  
  Serial.write (byte(highByte(value)));  // send high Byte   
}

