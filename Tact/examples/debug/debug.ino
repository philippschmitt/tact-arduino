// Include Tact Library
#include <Tact.h>

// Init new Tact Toolkit
Tact Tact;


void setup() {

	pinMode(A2, OUTPUT);

	// Start Serial for logging purposes
	Serial.begin(115200);

	// Start Tact toolkit
	Tact.begin();
	// Tact.beginSerial();

	// Add new Sensor and config
	Tact.addSensor(0, 68, 32, 2);
	
	// Add a second sensor
	//Tact.addSensor(0, 48, 32, 2);
}


void loop() {

	// used to receive tasks and send data via Serial
	// Tact.readSerial();
	
	// read Bias
	// Tact.readBias( sensorID );

	// read Peak
	// Tact.readPeak( sensorID );
	
	// read Spectrum
	int spectrum[32];
	Tact.readSpectrum(0, spectrum);	

	Serial.println( spectrum[15] );
	Serial.println( );

	// delay(200);

}
