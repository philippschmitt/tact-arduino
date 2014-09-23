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

#include "Arduino.h"
#include "TactSensor.h"


TactSensor::TactSensor (Tact *t, int _cmdBuffer[4]) {
	// Serial.write(5);

	_tact = t;
	// write cmdBuffer for TactSensor object
	// you can't easily copy arrays in c++
	// while probably not the most elegant, this solution is very readable
	cmdBuffer[0] = _cmdBuffer[0];
	cmdBuffer[1] = _cmdBuffer[1];
	cmdBuffer[2] = _cmdBuffer[2];
	cmdBuffer[3] = _cmdBuffer[3];

}


TactSensor::~TactSensor() {
	// do nothing
}


int TactSensor::readBias() {
	
	// Ask Tact for latest sensor readings
	// Tact.readSensor(cmdBuffer);
}

