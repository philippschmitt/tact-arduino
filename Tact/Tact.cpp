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
Tact::Tact() {
	
	// set sensor count to zero
	sensors = 0;

	// serial controller
	_runCMD = false;
}


// Init new Tact Toolkit
void Tact::begin() {
	
	// Set application state
	// int serialState = STATE_IDLE;

	// Start up serial communication
	// Serial.begin (BAUD_RATE);

	// Set up frequency generator
	TCCR1A = 0b10000010;
	TCCR1B = 0b00011001;

	// Signal generator pins
	pinMode (7, INPUT);
	pinMode (8, OUTPUT);
	// Sync (test) pin
	pinMode (9, OUTPUT);

	// Multiplexer pins
	pinMode (MP_4051_S0, OUTPUT);  // s0
	pinMode (MP_4051_S1, OUTPUT);  // s1
	pinMode (MP_4051_S2, OUTPUT);  // s2

}


// Start Tact with Serial
void Tact::beginSerial() {
	// Start up serial communication
  	Serial.begin (BAUD_RATE);
	// run regular startup
	begin();
}


// Serial Event Delegation
void Tact::readSerial() {
	// While there is anything in the 
	// pipe that has not been processed..
	while (Serial.available() > 0) {
		// Read incoming serial data
		_serialEvent (Serial.read());
	}

	// execute completed Serial commands
	if( _runCMD == true ) {
		_executeSerialCommand();
		_runCMD = false;
	}
}


void Tact::_serialEvent(const byte inByte) {

	if(_runCMD == true) {
		return;
	}
  
	switch (inByte) {
		case '\n':
			// Process command
			_runCMD = true;
			break;
		case CMD_SEPARATOR:
			// If token is command-separator: then increment 
			// command-index to address the next field of 
			// the command-buffer
			_serialCmdIndex++;
			break;
		// If none of the above
		default:
			// Test if byte represents a digit and 
			// update existing int value
			if (inByte > 47 && inByte < 58) {
				_serialCmdBuffer[_serialCmdIndex] = _serialCmdBuffer[_serialCmdIndex] * 10 + (int) (inByte - 48);
				// If in range from A-Z
			} else if (inByte > 64 && inByte < 91) {
				// Set command key/name.
				_serialCmdKey = inByte;
				// reset the command-index to -1
				_serialCmdIndex = -1;
				// clear list of command parameters
				_serialCmdBuffer[0] = 0;
				_serialCmdBuffer[1] = 0;
				_serialCmdBuffer[2] = 0;
				_serialCmdBuffer[3] = 0;
			} else {
				// ERROR - unexpected token in parameter stream
			}
			break;
	}
	
}


// execute Serial Command
void Tact::_executeSerialCommand() {

	// refresh sensor
	_refresh( (*_sensorList[_serialCmdBuffer[CMD_BUFFER_INDEX_PIN]]) );

	// Announce which of the Tact-inputs that 
    // are multiplexed will be transmitted
    _sendInt (3000 + _serialCmdBuffer[CMD_BUFFER_INDEX_PIN]);

	// act depending on command key
	switch (_serialCmdKey) {
		
		// transmit spectrum
		case 'G':

			// Tell client that a result array 
          	// is about to be dispatched
          	_sendInt (2000 + _serialCmdBuffer[CMD_BUFFER_INDEX_COUNT]);

          	// Go! Send signal spectrum ...
          	for (int x=0; x < _serialCmdBuffer[CMD_BUFFER_INDEX_COUNT]; x++) {
            	_sendInt( (*_sensorList[_serialCmdBuffer[CMD_BUFFER_INDEX_PIN]]).data[x] );
         	}

         	// Confirm that signal spectrum 
    		// has been delivered, done!
    		_sendInt (2999);

			break;

		// transmit peak
		case 'P':
			_sendInt(4000 + readPeak(_serialCmdBuffer[CMD_BUFFER_INDEX_PIN]) );
			break;

		case 'B':
			_sendInt(5000 + readBias(_serialCmdBuffer[CMD_BUFFER_INDEX_PIN]) );
			break;

		case 'V':
			_sendInt (5000 + VERSION);
			break;

		default:
			// no valid command
			// do nothing
			break;
	}
}


// Helper to send integers via Serial
/* y = 01010100 11010100    (value is a 2 Byte integer)
 *     yMSB     yLSB        send seperately -> joined by client
 */
void Tact::_sendInt (int value) {
	Serial.write (byte(lowByte(value)));   // send Low Byte  
	Serial.write (byte(highByte(value)));  // send high Byte   
}



// Add Sensor
void Tact::addSensor(int _indexStart, int _indexCount, int _indexStep) {
	// add new sensor to sensor list
	_sensorList[sensors] = new TactSensor(sensors, _indexStart, _indexCount, _indexStep);
	// increase sensor count by 1
	sensors++;
}


// read data from sensor and refresh data array of object TactSensor
void Tact::_refresh( TactSensor & sensor ) {

	// Select the sensor-input / bit
	// (use this with arduino 0013+)
	digitalWrite (MP_4051_S0, bitRead (sensor.cmdBuffer[CMD_BUFFER_INDEX_PIN], 0));
	digitalWrite (MP_4051_S1, bitRead (sensor.cmdBuffer[CMD_BUFFER_INDEX_PIN], 1));
	digitalWrite (MP_4051_S2, bitRead (sensor.cmdBuffer[CMD_BUFFER_INDEX_PIN], 2));

	// reset peak and bias to zero
	sensor.peak = 0;
	// sensor.bias = 0;

    for (unsigned int d = 0; d < sensor.cmdBuffer[CMD_BUFFER_INDEX_COUNT]; d++) {
		// Reload new frequency
		TCNT1 = 0;
		ICR1 = sensor.cmdBuffer[CMD_BUFFER_INDEX_START] + sensor.cmdBuffer[CMD_BUFFER_INDEX_STEP] * d;
		OCR1A = ICR1 / 2;

		// Restart generator
		SET (TCCR1B, 0);
		
		// Read response signal and write to sensor data container
		sensor.data[d] = (float) analogRead(0);

		// Stop generator
		CLR (TCCR1B, 0);

		// Check if current result is higher than previously stored peak
		// if true, overwrite peak and bias
    	if( sensor.data[d] > sensor.peak ) {
    		// Serial.println(5);
			sensor.peak = sensor.data[d];
        	sensor.bias = d;
    	}
    	
    }

    // Toggle pin 9 after each 
  	// sweep (good for scope)
  	TOG (PORTB, 0);
}


int Tact::readPeak(int _sensorID) {
	// refresh sensor data for current sensor
	_refresh( (*_sensorList[_sensorID]) );
	// return refreshed bias
	return (*_sensorList[_sensorID]).peak;
}


// Tact:Read Bias
int Tact::readBias(int _sensorID) {
	// refresh sensor data for current sensor
	_refresh( (*_sensorList[_sensorID]) );
	// return refreshed bias
	return (*_sensorList[_sensorID]).bias;
}


// Constructor for TactSensor
Tact::TactSensor::TactSensor(int _id, int _indexStart, int _indexCount, int _indexStep) {
	// fill config array
	cmdBuffer[CMD_BUFFER_INDEX_PIN] = _id;
	cmdBuffer[CMD_BUFFER_INDEX_START] = _indexStart;
	cmdBuffer[CMD_BUFFER_INDEX_COUNT] = _indexCount;
	cmdBuffer[CMD_BUFFER_INDEX_STEP] = _indexStep;

	// fill variables
	peak = 0;
	bias = 0;
}


