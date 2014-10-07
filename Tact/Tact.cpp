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
// also sets _useMultiplexer and _runCMD
Tact::Tact(bool useMultiplexer) : _useMultiplexer(useMultiplexer), _runCMD(false) {

	// serial controller
	// _runCMD = false;

	// initiate all fields in sensorList array to NULL
	for (int i = 0; i < MAX_SENSOR_COUNT; ++i) {
		_sensorList[i] = NULL;
	}
}


// Init new Tact Toolkit
void Tact::begin() {

	// Set up frequency generator
	TCCR1A = 0b10000010;
	TCCR1B = 0b00011001;

	// Signal generator pins
	pinMode (7, INPUT);
	pinMode (8, OUTPUT);
	// Sync (test) pin
	pinMode (9, OUTPUT);

	// Multiplexer pins
	if(_useMultiplexer) {
		pinMode (MP_4051_S0, OUTPUT);  // s0
		pinMode (MP_4051_S1, OUTPUT);  // s1
		pinMode (MP_4051_S2, OUTPUT);  // s2
	}

}

/*
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
*/

/*
void Tact::_serialEvent(const byte inByte) {

	// Do not listen if command is being processed
	if(_runCMD) {
		return;
	}

	switch (inByte) {
		case '\n':
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
*/

/*
// execute Serial Command
void Tact::_executeSerialCommand() {

	// command is not initial handshake
	if(_serialCmdKey  != 'V') {
		/*
		// TactSensor logic:
		// check, if requested sensor exists
		if( ! &_sensorList[_serialCmdBuffer[CMD_BUFFER_PIN]] ) {
			digitalWrite(A2, HIGH);
			// it doesn't. 
			// add it to the list
			addSensor(_serialCmdBuffer[CMD_BUFFER_PIN], _serialCmdBuffer[CMD_BUFFER_START], _serialCmdBuffer[CMD_BUFFER_COUNT], _serialCmdBuffer[CMD_BUFFER_STEP]);
		
		// update sensor settings, just in case they have been changed
		} else {
			updateSensor(_serialCmdBuffer[CMD_BUFFER_PIN], _serialCmdBuffer[CMD_BUFFER_START], _serialCmdBuffer[CMD_BUFFER_COUNT], _serialCmdBuffer[CMD_BUFFER_STEP]);
		}
		*//*

		// Announce which of the Tact-inputs that 
    	// are multiplexed will be transmitted
    	_sendInt (3000 + _serialCmdBuffer[CMD_BUFFER_PIN]);
	}

	// act depending on command key
	switch (_serialCmdKey) {
		
		// transmit spectrum
		case 'G':
			// refresh sensor
			_refresh( (*_sensorList[_serialCmdBuffer[CMD_BUFFER_PIN]]) );
			// Tell client that a result array 
          	// is about to be dispatched
          	_sendInt (2000 + _serialCmdBuffer[CMD_BUFFER_COUNT]);

          	// Go! Send signal spectrum ...
          	for (int x=0; x < _serialCmdBuffer[CMD_BUFFER_COUNT]; x++) {
            	_sendInt( (*_sensorList[_serialCmdBuffer[CMD_BUFFER_PIN]]).data[x] );
         	}

         	// Confirm that signal spectrum 
    		// has been delivered, done!
    		_sendInt (2999);

			break;

		// transmit peak
		case 'P':
			_sendInt(4000 + readPeak(_serialCmdBuffer[CMD_BUFFER_PIN]) );
			break;

		case 'B':
			_sendInt(5000 + readBias(_serialCmdBuffer[CMD_BUFFER_PIN]) );
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
*/


// Helper to send integers via Serial
/* y = 01010100 11010100    (value is a 2 Byte integer)
 *     yMSB     yLSB        send seperately -> joined by client
 */
 /*
void Tact::_sendInt (unsigned int value) {
	Serial.write (byte(lowByte(value)));   // send Low Byte  
	Serial.write (byte(highByte(value)));  // send high Byte   
}
*/

// Add single sensor with ID = 0
void Tact::addSensor(unsigned int _indexStart, unsigned int _indexCount, unsigned int _indexStep) {
	// add new sensor to sensor list
	_sensorList[0] = new TactSensor(0, _indexStart, _indexCount, _indexStep);
}


// Add Sensor with custom ID
void Tact::addSensor(unsigned int _sensorID, unsigned int _indexStart, unsigned int _indexCount, unsigned int _indexStep) {
	// add new sensor to sensor list
	_sensorList[_sensorID] = new TactSensor(_sensorID, _indexStart, _indexCount, _indexStep);
}

/*
// Update sensor settings
void Tact::updateSensor(unsigned int _sensorID, unsigned int _indexStart, unsigned int _indexCount, unsigned int _indexStep) {
	// update sensor settings
	// if( (*_sensorList[_sensorID]).cmdBuffer[0] != _sensorID || (*_sensorList[_sensorID]).cmdBuffer[1] != _indexStart || (*_sensorList[_sensorID]).cmdBuffer[2] != _indexCount || (*_sensorList[_sensorID]).cmdBuffer[3] != _indexStep ) {
		(*_sensorList[_sensorID]).cmdBuffer[0] = _sensorID;
		(*_sensorList[_sensorID]).cmdBuffer[1] = _indexStart;
		(*_sensorList[_sensorID]).cmdBuffer[2] = _indexCount;
		(*_sensorList[_sensorID]).cmdBuffer[3] = _indexStep;

		// _refresh( (*_sensorList[_sensorID]) );
	// }
}
*/

// read data from sensor and refresh data array of object TactSensor
void Tact::_refresh( TactSensor * sensor ) {

	// using multiplex sensor?
	if(_useMultiplexer) {
		// Select the sensor-input / bit
		// (use this with arduino 0013+)
		digitalWrite (MP_4051_S0, bitRead (sensor->cmdBuffer[CMD_BUFFER_PIN], 0));
		digitalWrite (MP_4051_S1, bitRead (sensor->cmdBuffer[CMD_BUFFER_PIN], 1));
		digitalWrite (MP_4051_S2, bitRead (sensor->cmdBuffer[CMD_BUFFER_PIN], 2));
	}
	
	// reset peak and bias to zero
	sensor->peak = 0;
	// sensor.bias = 0;

    for (unsigned int d = 0; d < sensor->cmdBuffer[CMD_BUFFER_COUNT]; d++) {
		// Reload new frequency
		TCNT1 = 0;
		ICR1 = sensor->cmdBuffer[CMD_BUFFER_START] + sensor->cmdBuffer[CMD_BUFFER_STEP] * d;
		OCR1A = ICR1 / 2;

		// Restart generator
		SET (TCCR1B, 0);
		
		// Read response signal and write to sensor data container
		sensor->data[d] = (float) analogRead(0);

		// Stop generator
		CLR (TCCR1B, 0);

		// Check if current result is higher than previously stored peak
		// if true, overwrite peak and bias
    	if( sensor->data[d] > sensor->peak ) {
    		// Serial.println(5);
			sensor->peak = sensor->data[d];
        	sensor->bias = d;
    	}
    	
    }

    // Toggle pin 9 after each 
  	// sweep (good for scope)
  	TOG (PORTB, 0);
}


int Tact::readPeak(unsigned int _sensorID) {
	// refresh sensor data for current sensor
	_refresh( _sensorList[_sensorID] );
	// return refreshed bias
	return (*_sensorList[_sensorID]).peak;
}


// Tact:Read Bias
int Tact::readBias(unsigned int _sensorID) {
	// refresh sensor data for current sensor
	_refresh( _sensorList[_sensorID] );
	// return refreshed bias
	return (*_sensorList[_sensorID]).bias;
}


// read Spectrum and write values to provided array
void Tact::readSpectrum(unsigned int _sensorID, int *targetArray) {
	// refresh sensor data for current sensor
	_refresh( _sensorList[_sensorID] );
	// work directly with retArray or memcpy into it from elsewhere like
  	memcpy(targetArray, (*_sensorList[_sensorID]).data, (*_sensorList[_sensorID]).cmdBuffer[CMD_BUFFER_COUNT]); 
}


// Constructor for TactSensor
Tact::TactSensor::TactSensor(unsigned int _id, unsigned int _indexStart, unsigned int _indexCount, unsigned int _indexStep) {
	// fill config array
	cmdBuffer[CMD_BUFFER_PIN] = _id;
	cmdBuffer[CMD_BUFFER_START] = _indexStart;
	cmdBuffer[CMD_BUFFER_COUNT] = _indexCount;
	cmdBuffer[CMD_BUFFER_STEP] = _indexStep;

	// fill variables
	peak = 0;
	bias = 0;
}


