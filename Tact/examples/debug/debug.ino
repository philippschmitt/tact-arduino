// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact;


int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;
long interval = 500; 
const int ledPin = A2; 


void setup() {

	pinMode(A2, OUTPUT);

	// Start Serial for logging purposes
	// Serial.begin(9600);

	// Start Tact toolkit
	// Tact.begin();

	Tact.beginSerial();

	// Add new Sensor and config
	Tact.addSensor(48, 32, 2);

	// Add a second sensor
	// Tact.addSensor(60, 30, 2);
}


void loop() {

	// Tact.readBias();

	// used to receive tasks and send data via Serial
	Tact.readSerial();


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

	// TactSerial();

	// flash LED for testing 

	/*
	// Output Bias für einzelnen Sensor
	// target specific sensor: Tact.readBias(int sensorID);
	int bias = Tact.readBias();
	Serial.println(bias);
	*/

	// Output Peak for a single sensor
	// int peak1 = Tact.readBias();

	// Serial.println( Tact.readBias(1) );
	// Serial.println( Tact.readBias(1) );

	// int peak2 = Tact.readBias(1);
	// Serial.println(peak2);

	// Serial.println(" ");

	// delay(1000);

}