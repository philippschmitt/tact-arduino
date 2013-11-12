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

byte yMSB=0;
byte yLSB=0;
byte xMSB=0;
byte xLSB=0;
byte zeroByte=128;
byte Checksum=0;

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
  
  // New line feed clear the command mode
  if (in == 10) {
    commandMode = 0;
  }else{
    
    // Getter mode
    if (commandMode == 1) {
      // Get next reading when client
      // requests got in via ...
      if (in == 'n')
        clientRequested = true;
      // Get size of signal spectrum,
      // the frequency length
      else if (in == 'a')
        sendValue (5000 + frequencyLength);
      // Get start index of the signal spectrum
      else if (in == 's')
        sendValue (4000 + frequencyStart);
    }
    
    // Setter mode
    else if (commandMode == 2) {
      // Set frequency start index. Next bytes will be 
      // the actual value. Get those straight by calling:
      if (in == 's')
        frequencyStart = valueReceiveLoop ();
      // Change frequency length. Next bytes will be 
      // the actual value. Get those straight by calling:
      else if (in == 'a')
        frequencyLength = valueReceiveLoop ();
    }
    // No mode selected
    else if (commandMode == 0) {
      // Set commandMode to get mode
      if (in == 'g')
        commandMode = 1;
      // Set commandMode to get mode
      else if (in == 's')
        commandMode = 2;
    }
  }
}

/**
 * Reads integer values from Serial stream and 
 * thereby can be used to handle setter parameters.
 * (i.e. for signal spectrum start index or length)
 * 
 * @return The value parsed from Serial stream 
 *         as <code>int</code>.
 */
int valueReceiveLoop () {
  int buf = 0;
  boolean endofData = false;

  while(!endofData){
    // If there's still data in the pipe ...
    if (Serial.available() > 0 ) {
      // Read data from serial
      byte in = Serial.read();
      // If serial event is not a ";"
      // ASCII code for end of line, 
      // then add event to buffer
      if (in != ';')
        buf = buf * 10 + (int) (in-48);
      else
        endofData = true;
    }
  }
  return buf;
}

/* y = 01010100 11010100    (value is a 2 Byte integer)
 *     yMSB     yLSB        send seperately -> joined by client
 */
void sendValue (int value) {
  Serial.write (byte(lowByte(value)));   // send Low Byte  
  Serial.write (byte(highByte(value)));  // send high Byte   
}
