/*
	Single Sensor

	This sketch shows how to read data from a single Tact sensor

	Created 16-10-2014
	By Philipp Schmitt

	Modified 16-10-2014
	By Philipp Schmitt

	http:// [github URL]
*/


// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact(TACT_SINGLE);


void setup() {

	// Start Tact toolkit
	Tact.begin();

	// Add new Sensor and config
	Tact.addSensor(48, 32, 2);
}


void loop() {
	
	// read Bias
	int bias = Tact.readBias();

	// read Peak
	int peak = Tact.readPeak();
	
	// read Spectrum
	int spectrum[32];
	Tact.readSpectrum(spectrum);

}
