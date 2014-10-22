# Tact Arduino

Tact is a capacitive touch (or rather: "nearness") sensor for Arduino. It adds sensing ability to any object that has conductive properties.

* [Processing Library](https://github.com/StudioNAND/tact-processing)
* [Tact Arduino Sketches](https://github.com/StudioNAND/tact-arduino-sketch) (using the circuit without this library)
* Hardware repository: tbd


## How does it work?
Tact is


## Public Functions


### Setup
	// Create new Tact instance
    Tact Tact;
    
    void setup() {
    	// Start Tact 
    	Tact.begin();
    }
    
    
### Add sensor
If you are not working with Serial, Tact sensors have to be configured before they can be used. Do this in setup.

<add: Tact configuration description: start/count/step>
	
	void setup() {
	  // Add single sensor
      Tact.addSensor(int start, int count, int stepSize);
    
      // add multiplexed sensor
      Tact.addSensor(int sensorID, int start, int count, int stepSize);
	}	


### Read spectrum
    int spectrum[size];
    Tact.readSpectrum(sensorID, spectrum);

    
### Read peak
    int peak = Tact.readPeak(sensorID = 0);

   
### Read bias
    int bias = Tact.readBias(sensorID = 0);
    
### Working with Serial
You can also control your Tact sensor from Processing on your PC via serial communiation. It works like this:
    
    #include <Tact.h>
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
    
