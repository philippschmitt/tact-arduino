/*
	Single Sensor

	This sketch shows how to send Tact sensor data to Processing

	Created 16-10-2014
	By Philipp Schmitt

	http:// [github URL]
*/


// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact(TACT_MULTI);


void setup() {

  // Start Tact toolkit with Serial
  Tact.beginSerial();

  // you can define a custom BAUD rate like this:
  // Tact.beginSerial(9600);
}


void loop() {

  // used to receive tasks and send data via Serial
  Tact.readSerial();

  // do other stuff here

}
