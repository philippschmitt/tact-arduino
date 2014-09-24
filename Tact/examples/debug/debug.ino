// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact;


void setup() {

	// Start Serial for logging purposes
	Serial.begin(9600);

	// Start Tact toolkit
	Tact.begin();

	// Add new Sensor and config
	Tact.addSensor(60, 30, 1);

	// Add a second sensor
	Tact.addSensor(60, 30, 1);
		
}


void loop() {

	/*
	// Output Bias für einzelnen Sensor
	// target specific sensor: Tact.readBias(int sensorID);
	int bias = Tact.readBias();
	Serial.println(bias);
	*/

	// Output Peak for a single sensor
	// int peak1 = Tact.readBias();

	Serial.println( Tact.readPeak() );
	Serial.println( Tact.readPeak(1) );

	// int peak2 = Tact.readBias(1);
	// Serial.println(peak2);

	Serial.println(" ");

	delay(1000);
	
}