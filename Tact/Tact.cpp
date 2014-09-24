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

}


// Add Sensor
void Tact::addSensor(int _indexStart, int _indexCount, int _indexStep) {

	// create new sensor object
	// TactSensor sensor(sensors, _indexStart, _indexCount, _indexStep);

	// add new sensor to sensor list
	sensorList[sensors] = new TactSensor(sensors, _indexStart, _indexCount, _indexStep);;

	// increase sensor count by 1
	sensors++;
}

void Tact::_refresh( TactSensor & sensor ) {
	
	// Declare sensor value buffer 
    int results[sensor.cmdBuffer[CMD_BUFFER_INDEX_COUNT]];
    // Declare peak and bias vars
    int peak = 0;
    int bias = 0;

    for (unsigned int d = 0; d < sensor.cmdBuffer[CMD_BUFFER_INDEX_COUNT]; d++) {
      // Reload new frequency
      TCNT1 = 0;
      ICR1 = sensor.cmdBuffer[CMD_BUFFER_INDEX_START] + sensor.cmdBuffer[CMD_BUFFER_INDEX_STEP] * d;
      OCR1A = ICR1 / 2;

      // Restart generator
      SET (TCCR1B, 0);
      // Read response signal
      results[d] = (float) analogRead(0);

      // copy data to object data array
      sensor.data[d] = results[d];

      // Stop generator
      CLR (TCCR1B, 0);

      // Check if current result is higher than previously stored peak
      // if true, overwrite peak and bias
      if( results[d] > peak ) {
        peak = results[d];
        bias = d;
      }
    }

    // Update object peak and bias vars
    sensor.peak = peak;
    sensor.bias = bias;
    
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
	// data = new int[CMD_BUFFER_INDEX_START];
	peak = 0;
	bias = 10;
}


void Tact::TactSensor::test() {
	// Serial.write( cmdBuffer[1] );
}


