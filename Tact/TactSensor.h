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

// forward declarate class Tact
class Tact;

class TactSensor {
	public:
		// Constructor
		TactSensor(Tact *t, int _cmdBuffer[4]);
		// Destructor
		~TactSensor();

		// return current Bias
		int readBias();

		// The config buffer for the current sensor
		int cmdBuffer[4];

	private:
		Tact *_tact; // do nothing
};