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
Tact::Tact(bool useMultiplexer) {

	// initiate all fields in sensorList array to NULL
	for (int i = 0; i < MAX_SENSOR_COUNT; ++i) {
		_sensorList[i] = NULL;
	}

	// set flag: use multiplexed sensor or not?
	_useMultiplexer = useMultiplexer;
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


// Start Tact with Serial
void Tact::beginSerial(unsigned int br) {
	// Start up serial communication
  	Serial.begin(br);

  	// flag needed for serial communication
	_runCMD = false;
	
	// run regular startup
	begin();
}


// Serial Event Delegation
void Tact::readSerial() {

	// While there is anything in the 
	// pipe that has not been processed..
	while (Serial.available() > 0) {
		
		// Read incoming serial data
		byte inByte = Serial.read();
		
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
				} else if ( inByte > 64 && inByte < 123 ) {
			        // check if inByte in lowercase range (97-122)
			        if( inByte > 96) {
			          // transform to uppercase
			          inByte -= 32;
			        }

					// Set command key/name.
					_serialCmdKey = inByte;
					// reset the command-index to -1
					_serialCmdIndex = -1;
					// clear list of command parameters
					_serialCmdBuffer[0] = 0;
					_serialCmdBuffer[1] = 0;
					_serialCmdBuffer[2] = 0;
					_serialCmdBuffer[3] = 0;

					// stop cmd if there's more to come
					_runCMD = false;
				} else {
					// ERROR - unexpected token in parameter stream
				}
				break;
		}
	}
	

	// execute completed Serial command
	if( _runCMD ) {

		// Declare sensor value buffer 
	    int results[_serialCmdBuffer[CMD_BUFFER_COUNT]];
	    // Declare peak and bias vars
	    int peak = 0;
	    int bias = 0;
		
		// command is not initial handshake
		if(_serialCmdKey  != 'V') {
			
			// using multiplex sensor?
			if(_useMultiplexer) {
				// Select the sensor-input / bit
		  		// (use this with arduino 0013+)
		    	digitalWrite (MP_4051_S0, bitRead (_serialCmdBuffer[CMD_BUFFER_PIN], 0));
		    	digitalWrite (MP_4051_S1, bitRead (_serialCmdBuffer[CMD_BUFFER_PIN], 1));
		    	digitalWrite (MP_4051_S2, bitRead (_serialCmdBuffer[CMD_BUFFER_PIN], 2));
		    }

		    for (unsigned int d = 0; d < _serialCmdBuffer[CMD_BUFFER_COUNT]; d++) {
		      // Reload new frequency
		      TCNT1 = 0;
		      ICR1 = _serialCmdBuffer[CMD_BUFFER_START] + _serialCmdBuffer[CMD_BUFFER_STEP] * d;
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

			// Announce which of the Tact-inputs that 
	    	// are multiplexed will be transmitted
	    	_sendInt (1024 + _serialCmdBuffer[CMD_BUFFER_PIN]);
		}
		

		// act depending on command key
		switch (_serialCmdKey) {
			
			// transmit spectrum
			case 'S':
				// send data_type for data to be transmitted
	          	_sendInt( 1088 + 0 );
	          	// Tell client how many data values are going to be sent
	         	_sendInt (1098 + _serialCmdBuffer[CMD_BUFFER_COUNT]);
	         	/*
	         	if( _serialCmdBuffer[CMD_BUFFER_COUNT] == 32) {
	         		analogWrite(6, 255);
	         	}
	         	*/
	          	// Go! Send signal spectrum ...
	          	for (int x=0; x < _serialCmdBuffer[CMD_BUFFER_COUNT]; x++) {
	          		_sendInt( results[x] );
	         	}

				break;

			// transmit peak
			case 'P':
				// send data_type for data to be transmitted
	          	_sendInt( 1088 + 1 );
	          	// Tell client how many data values are going to be sent
	          	_sendInt (1098 + 1);
	          	// send data
				_sendInt( peak );
				break;

			case 'B':
				// send data_type for data to be transmitted
	        	_sendInt( 1088 + 2 );
	        	// Tell client how many data values are going to be sent
	        	_sendInt (1098 + 1);
	        	// send data
				_sendInt( bias );
				break;

			case 'V':
				_sendInt (2124 + VERSION);
				break;

			default:
				; // do nothing
		}

		// command is not initial handshake
		if(_serialCmdKey  != 'V') {
			// Confirm that all data 
		    // has been delivered, done!
		    _sendInt (2123);
		}

		_runCMD = false;
	}
}


// Helper to send integers via Serial
/* y = 01010100 11010100    (value is a 2 Byte integer)
 *     yMSB     yLSB        send seperately -> joined by client
 */
void Tact::_sendInt (unsigned int value) {
	Serial.write (byte(lowByte(value)));   // send Low Byte  
	Serial.write (byte(highByte(value)));  // send high Byte   
	// debug: print ints
	// Serial.println(value);
}


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
	// copy required amounts of integers from sensor data array to target array
  	memcpy(targetArray, _sensorList[_sensorID]->data, _sensorList[_sensorID]->cmdBuffer[CMD_BUFFER_COUNT] * sizeof(int) ); 
}


// read Spectrum for single sensor and write values to provided array
void Tact::readSpectrum(int *targetArray) {
	// refresh sensor data for current sensor
	_refresh( _sensorList[0] );
	// work directly with retArray or memcpy into it from elsewhere like
  	memcpy(targetArray, _sensorList[0]->data, _sensorList[0]->cmdBuffer[CMD_BUFFER_COUNT] * sizeof(int) );
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


