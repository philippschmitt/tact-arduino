// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact;


void setup() {

	pinMode(A2, OUTPUT);

	// Start Serial for logging purposes
	Serial.begin(115200);

	// Start Tact toolkit
	// Tact.begin();
	Tact.beginSerial();

	// Add new Sensor and config
	/*
	Tact.addSensor(0, 68, 32, 2);
	Tact.addSensor(1, 48, 32, 1);
	Tact.addSensor(2, 30, 32, 2);
	Tact.addSensor(3, 50, 48, 1);
	Tact.addSensor(4, 68, 32, 2);
	Tact.addSensor(5, 48, 32, 1);
	Tact.addSensor(6, 30, 32, 2);
	Tact.addSensor(7, 50, 48, 1);
	*/

	// Add a second sensor
	// Tact.addSensor(60, 30, 2);
}


void loop() {

	// used to receive tasks and send data via Serial
	Tact.readSerial();

	/*
	unsigned long currentMillis = millis();
 
	if(currentMillis - previousMillis > interval) {
	// save the last time you blinked the LED 
	previousMillis = currentMillis;   

	// if the LED is off turn it on and vice-versa:
	if (ledState == LOW)
	  ledState = HIGH;
	else
	  ledState = LOW;

	// set the LED with the ledState of the variable:
	digitalWrite(ledPin, ledState);
	}
	*/

	// TactSerial();

	// flash LED for testing 

	
	// Output Bias für einzelnen Sensor
	// target specific sensor: Tact.readBias(int sensorID);
	// int bias = Tact.readBias();
	// Serial.println(bias);

	/*
	// Output Peak for a single sensor
	Serial.println( Tact.readPeak() );
	Serial.println( Tact.readPeak(1) );
	Serial.println( Tact.readPeak(2) );
	Serial.println( Tact.readPeak(3) );
	Serial.println( Tact.readPeak(4) );
	Serial.println( Tact.readPeak(5) );
	Serial.println( Tact.readPeak(6) );
	Serial.println( Tact.readPeak(7) );

	Serial.println( );
	*/
	
	// Serial.println( 1 );
	// Serial.println( Tact.readBias(1) );

	// int peak2 = Tact.readBias(1);
	// Serial.println(peak2);

	// Serial.println(" ");

	// delay(1000);

}