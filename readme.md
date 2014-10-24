# Tact Arduino

Tact is a capacitive touch (or rather: "nearness") sensor for Arduino. It adds sensing ability to any object that has conductive properties.

* [Processing Library](https://github.com/StudioNAND/tact-processing): Work with sensor data on your computer
* [Tact Arduino Sketches](https://github.com/StudioNAND/tact-arduino-sketch): Use the sensor without this library
* [Tact Hardware repository](https://github.com/StudioNAND/tact-hardware): Build your own Tact sensor. It's easy.


## Working with Tact
Connect the sensor wire to a conductive object. If you plan to sense distance to this object, large surfaces (like a copper plate) or big volumes (like a bowl of water) work best.  
The sensor output is a graph that changes as you approach or touch the object. Here's a little demo:

![image](https://s3-eu-west-1.amazonaws.com/tact/assets/tactA-demo01/tactA-demo01_728.gif)


### The Tact Graph
The most interesting parts about the Tact graph are its maxima: **Peak** and **Bias**.  
Bias is best at sensing distance to an object, while peak mostly senses differences in touches.

![image](https://s3-eu-west-1.amazonaws.com/tact/assets/diagram/tact-spectrum-peak-bias.png)


## Installation
Download the [latest release](http://s3-eu-west-1.amazonaws.com/tact/arduino-library/Tact.zip) and follow the instructions on the Arduino website: [Installing Additional Arduino Libraries](http://arduino.cc/en/pmwiki.php?n=Guide/Libraries).

Should you choose to download the repository directly from GitHub, please copy *only the Tact folder* into your Arduino libraries folder.


## Usage
The library comes with various example sketches that should help you getting started. Additionally, here's a documentation of all available commands:


### Setup
````Arduino
// Create new Tact instance
// Use TACT_MULTI for a multiplexer sensor and TACT_SINGLE for a single sensor.
Tact Tact(TACT_SINGLE);
    
void setup() {
  // Start Tact 
  Tact.begin();
}
````
    
    
### Adding sensors
Tact sensors have to be configured in setup.

````Arduino
void setup() {
  // Add a single sensor
  Tact.addSensor(48, 32, 2);      

  // Add multiple sensors:
  // (only possible if you use the multiplexer circuit)
  Tact.addSensor(1, 40, 48 , 1);
  Tact.addSensor(6, 18, 32, 6);
  Tact.addSensor(7, 88, 22, 1);
}
````

But what's that?! While the sensor spectrum is theoretically infinite in size, we usually only need a portion of it. That's why we have to define:

* start at position **48** of the spectrum
* perform **32** sensor readings
* in increments of **2**


This becomes `Tact.addSensor(48, 32, 2);`.  

Your individual configuration depends mostly on the object you're using. And of course on what you're trying to measure and how detailed your spectrum has to be in order to measure that. Here's an illustrated explanation:

![image](http://s3-eu-west-1.amazonaws.com/tact/assets/diagram/tact-addSensor-config.png)


### Read spectrum

````Arduino	
// Read 32 values from sensor 0 and store them in array `spectrum`:
int spectrum[32];
Tact.readSpectrum(0, spectrum);
````

    
### Read peak

````Arduino
// read current peak for sensor 0
int peak = Tact.readPeak();

// read current peak for sensor 3
int peak = Tact.readPeak(3);
````

   
### Read bias
````Arduino
// read current peak for sensor 0
int bias = Tact.readBias();

// read current peak for sensor 3
int bias = Tact.readBias(3);
````

    
### Working with Serial
You can also control your Tact sensor from Processing on your PC via serial communiation. It works like this:

````Arduino  
#include <Tact.h>
Tact Tact(TACT_MULTI);

void setup() {
  // Start Tact toolkit with Serial
  // default baud rate: 115200
  Tact.beginSerial();

  // you can define a custom baud rate like this:
  // Tact.beginSerial(9600);
}

void loop() {
  / used to receive tasks and send data via Serial
  // make sure to call this in every loop
  Tact.readSerial();

  // do other stuff here
}
````   
