// Adafruit TinyUSB Test
// Serial and MIDI Communication ino test file using Adafruit TinyUSB abstraction
// of the TinyUSB library. Tested with the MEAP, an ESP32-based audio prototyping board
// using "USB CDC on Boot Enabled" "TinyUSB USB Mode" and "UART/CDC Upload Mode"
#include <Arduino.h>
#include <MIDI.h>
#include "Adafruit_TinyUSB.h"
Adafruit_USBD_CDC usbc_serial;
Adafruit_USBD_MIDI usb_midi;

// them globals
#define BUILTIN_LED 42

int channel = 1;
int pitch = 48;
int velocity = 25;

MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  
  if(!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }

  usbc_serial.begin(115200);
  usb_midi.setStringDescriptor("TinyUSB MIDI");

  MIDI.begin(MIDI_CHANNEL_OMNI);

  if(TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  // TinyUSB_Device_Init(0);

  while(!TinyUSBDevice.mounted() ) delay(10);

  usbc_serial.println("morning!");
}

void loop() {
  #ifdef TINYUSB_NEED_POLLING_TASK
    TinyUSBDevice.task();
  #endif

  // shit the bed; failsafe
  if (!TinyUSBDevice.mounted()) {
    return;
  }

  MIDI.sendNoteOn(48, 127, 1);
  MIDI.sendNoteOff(48, 127, 1);

  usbc_serial.println("abcdefg");
  usbc_serial.print("Note on: channel = ");
  usbc_serial.print(channel);

  usbc_serial.print(" pitch: = ");
  usbc_serial.print(pitch);

  usbc_serial.print(" velocity = ");
  usbc_serial.print(velocity);

  digitalWrite(BUILTIN_LED, HIGH);
  delay(1000);
  digitalWrite(BUILTIN_LED, LOW);
  channel += (1 % (9));
  pitch += (1 % (63));
  velocity += (1 % (147));
  MIDI.read();
  delay(1000);
}

void handleNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
  usbc_serial.print("Note on: channel = ");
  usbc_serial.print(channel);

  usbc_serial.print(" pitch: = ");
  usbc_serial.print(pitch);

  usbc_serial.print(" velocity = ");
  usbc_serial.print(velocity);
}
void handleNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity)
{
  usbc_serial.print("Note off: channel = ");
  usbc_serial.print(channel);

  usbc_serial.print(" pitch: = ");
  usbc_serial.print(pitch);

  usbc_serial.print(" velocity = ");
  usbc_serial.print(velocity);
}
