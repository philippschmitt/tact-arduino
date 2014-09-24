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
	Tact.addSensor(60,30,1);
		
}


void loop() {

	/*
	// Output Bias für einzelnen Sensor
	// target specific sensor: Tact.readBias(int sensorID);
	int bias = Tact.readBias();
	Serial.println(bias);
	*/

	// Output Peak for a single sensor
	int peak = Tact.readPeak();

	Serial.println(peak);
	// Serial.println( Tact.readBias() );


	delay(10);
	
}