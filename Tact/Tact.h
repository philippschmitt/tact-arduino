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
	#define CMD_BUFFER_INDEX_PIN 0
	#define CMD_BUFFER_INDEX_START 1
	#define CMD_BUFFER_INDEX_COUNT 2
	#define CMD_BUFFER_INDEX_STEP 3

 	// max amount of sensors
 	#define MAX_SENSOR_COUNT 8

  	// Max allowed number of meterings
 	// needed to define data array size in class TactSensor
 	#define MAX_BUFFER_SIZE 60

	#define CMD_SEPARATOR ' '

	// Bit set/clear macros
	#define SET(x,y) (x |=(1<<y))
	#define CLR(x,y) (x &= (~(1<<y)))
	#define CHK(x,y) (x & (1<<y))
	#define TOG(x,y) (x^=(1<<y))

 	/*
	#define STATE_IDLE 0
	#define STATE_RECEIVE_CMD 1
	#define STATE_TRANSMIT_SENSOR 2
	#define STATE_TRANSMIT_PEAK 3
	#define STATE_TRANSMIT_BIAS 4
	*/

 	// Multiplexer 4051' pins
	#define MP_4051_S0 12
	#define MP_4051_S1 11
	#define MP_4051_S2 10

 	// Define class Tact
	class Tact {

		// Tact: List all public vars and functions
		public:
			// Constructor
			Tact();
			// Init Tact Toolkit
			void begin();
			// Init Tact Toolkit /w Serial
			void beginSerial();
			// Serial Event Delegation
			void readSerial();
			// Add Sensor
			void addSensor(int _indexStart, int _indexCount, int _indexStep);
			// return peak for single sensor
			int readPeak(int _sensorID = 0);
			// return bias for single sensor
			int readBias(int _sensorID = 0);
			// Count registered sensors
			int sensors;


		// list all private vars and functions
		// prefix any private vars with _underscore for clarity
		private:

			// Declare private subclass TactSensor that handles... sensors?
			class TactSensor {
				public:
					// constructor
					TactSensor(int _id, int _indexStart, int _indexCount, int _indexStep);
					// hold sensor id and config
					int cmdBuffer[4];
					// holds data for current sensor
					int data[MAX_BUFFER_SIZE];
					// current sensor peak
					int peak;
					// current sensor bias
					int bias;
			};

			// Application state
			// int state;

			// Array with pointers to all sensor objects
			TactSensor * _sensorList[MAX_SENSOR_COUNT];

			// read data from sensor and update sensor data array
			void _refresh( TactSensor & sensor );

			// handle Serial events
			void _serialEvent(const byte inByte);

			// execute Serial CMD
			void _executeSerialCommand();

			// helper to send integers via Serial
			void _sendInt(int value);

			// Process incoming Serial data
			// void serialEvent(const byte inByte);
			// void sendInt(int value);
			//execute current set command
			// void execute();

			char _serialCmdKey;
			int _serialCmdBuffer[4];
			int _serialCmdIndex;
			int _serialState;
			bool _runCMD;

	};


#endif