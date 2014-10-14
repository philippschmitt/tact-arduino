# Tact Arduino

Tact is a capacitive touch (or rather: "nearness") sensor for Arduino. It adds sensing ability to any object that has conductive properties.


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