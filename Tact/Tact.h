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


// Prevents problems if someone accidentally #include's library twice
#ifndef Tact_h
#define Tact_h

 	// include Arduino
	#include "Arduino.h"

 	// Sensor Version
	#define VERSION 1

	// Default Serial baud rate
	#define BAUD_RATE 115200

	// Array 
	#define CMD_BUFFER_PIN 0
	#define CMD_BUFFER_START 1
	#define CMD_BUFFER_COUNT 2
	#define CMD_BUFFER_STEP 3

 	// max amount of sensors
 	#define MAX_SENSOR_COUNT 8

 	// use multiplexer or single sensor
 	#define TACT_SINGLE false
 	#define TACT_MULTI true

  	// Max allowed number of meterings
 	// needed to define data array size in class TactSensor
 	#define MAX_BUFFER_SIZE 60

	#define CMD_SEPARATOR ' '

	// Bit set/clear macros
	#define SET(x,y) (x |=(1<<y))
	#define CLR(x,y) (x &= (~(1<<y)))
	#define CHK(x,y) (x & (1<<y))
	#define TOG(x,y) (x^=(1<<y))

 	// Multiplexer 4051' pins
	#define MP_4051_S0 12
	#define MP_4051_S1 11
	#define MP_4051_S2 10

 	// Define class Tact
	class Tact {

		// Tact: List all public vars and functions
		public:
			// Constructor
			Tact(bool useMultiplexer);
			// Init Tact Toolkit
			void begin();
			// Init Tact Toolkit /w Serial
			void beginSerial(unsigned int br = BAUD_RATE);
			// Serial Event Delegation
			void readSerial();
			// Add single sensor with ID 0
			void addSensor(unsigned int _indexStart, unsigned int _indexCount, unsigned int _indexStep);
			// Add Sensor with custom ID
			void addSensor(unsigned int _sensorID, unsigned int _indexStart, unsigned int _indexCount, unsigned int _indexStep);
			// return peak for single sensor
			int readPeak(unsigned int _sensorID = 0);
			// return bias for single sensor
			int readBias(unsigned int _sensorID = 0);
			// read Spectrum and write to provided array
			void readSpectrum(unsigned int _sensorID, int *targetArray);
			// read Spectrum for single sensor and write to provided array
			void readSpectrum(int *targetArray);


		// list all private vars and functions
		// prefix any private vars with _underscore for clarity
		private:

			// Declare private subclass TactSensor that handles... sensors?
			class TactSensor {
				public:
					// constructor
					TactSensor(unsigned int _id, unsigned int _indexStart, unsigned int _indexCount, unsigned int _indexStep);
					// hold sensor id and config
					unsigned int cmdBuffer[4];
					// holds data for current sensor
					unsigned int data[MAX_BUFFER_SIZE];
					// current sensor peak
					unsigned int peak;
					// current sensor bias
					unsigned int bias;
			};

			// Application state
			// int state;

			// Array of pointers to all sensor objects
			TactSensor * _sensorList[MAX_SENSOR_COUNT];

			// read data from sensor and update sensor data array
			void _refresh( TactSensor * sensor );

			// helper to send integers via Serial
			void _sendInt(unsigned int value);

			// helper vars to store Serial commands during retrieval and execution
			char _serialCmdKey;
			unsigned int _serialCmdBuffer[4];
			int _serialCmdIndex;

			// flag: true while Serial command is being executed
			bool _runCMD;

			// flag; set true if multiplexer should be used
			bool _useMultiplexer;
	};


#endif