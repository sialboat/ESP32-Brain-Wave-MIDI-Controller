// Arduino Brain Library - Brain SoftSerial Test

// Description: Grabs brain data from software serial on pin 10 and sends CSV out over the hardware serial
// More info: https://github.com/kitschpatrol/Arduino-Brain-Library
// Author: Eric Mika, 2014

// #include <SoftwareSerial.h>
#include <Brain.h>

// Set up the software serial port on pins 10 (RX) and 11 (TX). We'll only actually hook up pin 10.
HardwareSerial brainSerial(2);
// SoftwareSerial brainSerial(7, 42);

// Set up the brain reader, pass it the software serial object you want to listen on.
Brain brain(brainSerial);

void setup() {
    // Start the software serial.
    // softSerial.begin(9600);
    brainSerial.begin(9600, SERIAL_8N1, 7, 42);
    // Start the hardware serial.
    Serial.begin(9600);

}

void loop() {
    // Expect packets about once per second.
    // The .readCSV() function returns a string (well, char*) listing the most recent brain data, in the following format:
    // "signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma"
    // Serial.println(brain.readErrors());
    if (brain.update()) {
        Serial.println("boo");
        Serial.println(brain.readErrors());
        Serial.println(brain.readCSV());
    }
}
