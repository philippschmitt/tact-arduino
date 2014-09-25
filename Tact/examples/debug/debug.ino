// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact;


void setup() {

	// Start Serial for logging purposes
	// Serial.begin(9600);

	// Start Tact toolkit
	Tact.begin();

	// Tact.beginSerial();

	// Add new Sensor and config
	Tact.addSensor(60, 30, 2);

	// Add a second sensor
	Tact.addSensor(60, 30, 2);
}


void loop() {

	// TactSerial();

	/*
	// Output Bias für einzelnen Sensor
	// target specific sensor: Tact.readBias(int sensorID);
	int bias = Tact.readBias();
	Serial.println(bias);
	*/

	// Output Peak for a single sensor
	// int peak1 = Tact.readBias();

	Serial.println( Tact.readBias(1) );
	// Serial.println( Tact.readBias(1) );

	// int peak2 = Tact.readBias(1);
	// Serial.println(peak2);

	Serial.println(" ");

	delay(100);
	
}