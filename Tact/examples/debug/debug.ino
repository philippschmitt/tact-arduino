// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact(52);

// Add new Tact Sensor
TactSensor sensor1 = Tact.addSensor(40,20,1);


void setup() {
	
	// Start Tact toolkit
	Tact.begin();

	// Start Serial for logging purposes
	Serial.begin(9600);
}


void loop() {

	// Output bias
	// int bias = sensor1.readBias();

	/*
	// read bias from sensor
	int bias =  test.readBias();
	Serial.write(bias);
	*/

	// sensor1.readBias();
	// sensor3.readBias();

	// Tact.readBias();

	// output sensor ID
	// Serial.write( bias );

	delay(1000);
}