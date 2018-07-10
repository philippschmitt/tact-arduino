/*
	Multiple Sensors
 
 	This sketch shows how to read data from multiple sensors on a Tact sensor board
 
 	Created 16-10-2014
 	By Philipp Schmitt
 
 	Modified 16-10-2014
 	By Philipp Schmitt
 
 	http:// [github URL]
 */


// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact(TACT_MULTI);


void setup() {

  // Start Tact toolkit
  Tact.begin();

  // Add new Sensor and config
  Tact.addSensor(0, 48, 32, 2);
  // add another sensor
  Tact.addSensor(1, 60, 20, 1);
  // you can add as many as 8 sensors for any sensor you like:
  Tact.addSensor(7, 40, 40, 1);
}


void loop() {

  // read Bias for sensor
  int bias0 = Tact.readBias(0);
  // read bias for second sensor
  int bias1 = Tact.readBias(1);

  // read Peak for first sensor
  int peak0 = Tact.readPeak();
  // read peak for second sensor
  int peak1 = Tact.readPeak(1);

  // read Spectrum for first sensor
  // pay attention that the spectrum array can hold 
  // as many values as you have defined in Tact.addSensor();
  int spectrum0[32];
  Tact.readSpectrum(0, spectrum0);
  // read spectrum for second sensor
  int spectrum1[20];
  Tact.readSpectrum(1, spectrum1);

}

