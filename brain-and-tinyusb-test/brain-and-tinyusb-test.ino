// Define pins for UART2 (Serial2)
// Replace with valid UART2-capable pins for your board
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <Brain.h>
#include <SoftwareSerial.h>
#define RXD2 7
#define TXD2 42

Adafruit_USBD_CDC usbc_serial;     // USB Serial (CDC)
Adafruit_USBD_MIDI usb_midi;       // USB MIDI
// SoftwareSerial brainSerial(RXD2, TXD2);
HardwareSerial brainSerial(2);     // Hardware UART2 for Brain

Brain brain(brainSerial);

void setup() {
  TinyUSBDevice.begin(0);           // Start TinyUSB stack
  usbc_serial.begin(115200);       // USB Serial
  usb_midi.setStringDescriptor("TinyUSB MIDI");

  // brainSerial.begin(9600);
  brainSerial.begin(9600, SERIAL_8N1, RXD2, TXD2); // HardwareSerial for Brain

  usbc_serial.println("USB Serial (CDC) initialized!");
}

void loop() {
  // Use usbc_serial for USB logging
  // usbc_serial.println(brain.readCSV());
  if(brain.update()) {
    usbc_serial.println("Loop running!");
    usbc_serial.println(brain.readCSV());
    usbc_serial.println(brain.readErrors());
  }
  // Use brainSerial/brain object for comms with the Brain
  // e.g., if (brain.available()) ...

  // Use usb_midi for MIDI comms as needed
}
