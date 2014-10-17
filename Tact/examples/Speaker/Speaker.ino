#include <Tact.h>

// Create new Tact toolkit
Tact Tact(TACT_SINGLE);

// variable to store the servo position
int counter = 0;
int value = 0;

// connect speaker to pwm pin 5 or 6
int speakerPin = 5;

void setup() {
    // start Tact toolkit
    Tact.begin();
    // add new sensor
    Tact.addSensor(48, 32, 1);
    // define output pin for speaker
    pinMode(speakerPin, OUTPUT);
} 

void loop() {
    // increase counter by 1
    counter++;

    // sum up 10 peak readings
    if(counter < 10) {
        // read peak and add to sum stored in "value"
        value += Tact.readPeak();
    // calculate average and set tone
    } else {
        // calculate peak average
        value = map(value/10, 300, 500, 400, 800);
        // set tone on
        tone(speakerPin, value);
        // reset helper variables
        value = 0;
        counter = 0;
    }
}