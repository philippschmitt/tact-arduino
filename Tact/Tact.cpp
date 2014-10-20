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
Tact::Tact(bool useMultiplexer) : _useMultiplexer(useMultiplexer) {

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


