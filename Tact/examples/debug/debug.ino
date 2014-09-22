// Include Tact Library
#include <Tact.h>


Tact Tact(52);

TactSensor test = Tact.addSensor();


void setup() {
	
	// Init new Tact toolkit
	Tact.begin();

	// Add new Sensor named test
	// TactSensor test = 

	// Start Serial for logging purposes
	Serial.begin(9600);

}

void loop() {
	// read bias from sensor
	int bias =  test.readBias();

	Serial.write(bias);

	delay(1000);
}