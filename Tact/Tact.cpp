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
}


// Init new Tact Toolkit
void Tact::begin() {
	
	// Set application state
	int state = STATE_IDLE;

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


// Add Sensor
void Tact::addSensor(int _indexStart, int _indexCount, int _indexStep) {

	// create new sensor object
	// TactSensor sensor(sensors, _indexStart, _indexCount, _indexStep);

	// add new sensor to sensor list
	sensorList[sensors] = new TactSensor(sensors, _indexStart, _indexCount, _indexStep);

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

		int test = sensor.peak;

		// Serial.println( sensor.data[d] > sensor.peak );

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
  	// TOG (PORTB, 0);

}


int Tact::readPeak(int _sensorID) {
	// refresh sensor data for current sensor
	_refresh( (*sensorList[_sensorID]) );
	// return refreshed bias
	return (*sensorList[_sensorID]).peak;
}


// Tact:Read Bias
int Tact::readBias(int _sensorID) {
	// refresh sensor data for current sensor
	_refresh( (*sensorList[_sensorID]) );
	// return refreshed bias
	return (*sensorList[_sensorID]).bias;
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


