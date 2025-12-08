/*
  BRAINWAVES SOUND CONTROLLER
  
  contains software used for the ESP32-based MEAP (Mason's ESP32 Audio Prototyping board) prototyping system developed by Mason Mann
  for MPATE-GE 2047 Advanced Computer Music. This .ino file interfaces with a MindFlex EEG sensor with the Brain library from Frontiernerds
  and uses tinyUSB for USB usbc_serial and MIDI communication.

  Brain Library: https://github.com/kitschpatrol/Arduino-Brain-Library
*/

#include <Arduino.h>
#include "brainWrapper.h"
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <Brain.h>
#include <Adafruit_NeoPixel.h>

// add a neopixel for visual feedback / indicator of other changes that will happen.
// Consult ~/Documents/Arduino/neopixel_demo/neopixel_demo.ino for what to do.
#define NEOPIXEL_PIN 999  // SILAS PLEASE CHANGE THIS LATER THANKS
#define BUILTIN_LED 42
#define BRAIN_PIN 7

// MEAP STUFF
#define CONTROL_RATE 128  // Hz, powers of 2 are most reliable
#include <Meap.h>         // MEAP library, includes all dependent libraries, including all Mozzi modules

// usbc_serial
Adafruit_USBD_CDC usbc_serial;
Adafruit_USBD_MIDI usb_midi;

// MIDI_CREATE_INSTANCE(Hardwareusbc_serial, Serial1, MIDI); // defines MIDI in/out ports
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);  // defines MIDI in/out ports
Meap meap;                                                 // creates MEAP object to handle inputs and other MEAP library functions

HardwareSerial brainSerial(2);
// SoftwareSerial brainSerial(BRAIN_PIN, BUILTIN_LED);
Brain brain(brainSerial);
brainWrapper bryan(&brain, &usbc_serial, &usb_midi);

Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(4, NEOPIXEL_PIN, NEO_RGB);

// #if ARDUINO_USB_MODE
// #warning This sketch must be used when USB is in OTG mode
// void setup() {}
// void loop() {}
// #else

void setup() {
  if (!TinyUSBDevice.isInitialized()) {
    TinyUSBDevice.begin(0);
  }
  usbc_serial.begin(115200);
  usb_midi.setStringDescriptor("TinyUSB MIDI");
  brainSerial.begin(9600, SERIAL_8N1, BRAIN_PIN, BUILTIN_LED);
  // softSerial.begin();
  // initialize MIDI, listen to all MIDI Channels.
  // This will also call usb_midi's begin()
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // failsafe-if already enumerated, additional class driver begin() for stuff like
  // msc, hid, midi won't kick in until re-enumeration.
  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach();
    delay(10);
    TinyUSBDevice.attach();
  }

  // Attach MIDI Functions
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  // MIDI.setProgramChange(handleProgramChange);

  //  MIDI.setHandleControlChange(handleControlChange);
  //  MIDI.setHandlePitchBend(handlePitchBend);
  //  MIDI.setHandleProgramChange(handleProgramChange);
  //  MIDI.set whatever the fuck else I decide to do

  //  consult MIDI.h (~/Documents/Arduino/libraries/MIDI_Library/src/MIDI.h and
  //  ~/Documents/Arduino/libraries/MIDI_Library/src/MIDI.cpp

  meap.begin();
  startMozzi(CONTROL_RATE);
  bryan.setDebug(true);
  // USB.onEvent(usbEventCallback);
  // tinyusb_enable_interface(USB_INTERFACE_MIDI, TUD_MIDI_DESC_LEN,
  //                         tusb_midi_load_descriptor);
  // USB.begin();
  while (!TinyUSBDevice.mounted()) delay(10);
  usbc_serial.println("morning!");
}

void loop() {
// usbc_serial.println("a");
#ifdef TINYUSB_NEED_POLLING_TASK
  // Manual call tud_task since it isn't called by Core's background
  TinyUSBDevice.task();
#endif

  // shit the bed; failsafe.
  if (!TinyUSBDevice.mounted()) {
    return;
  }
  // delay(100);
  bryan.update();
  // usbc_serial.println(bryan.getTheta());
  // usbc_serial.println(bryan.getDelta());
  // usbc_serial.println(bryan.getLowAlpha());
  // usbc_serial.println(bryan.getHighAlpha());
  // usbc_serial.println(bryan.getLowBeta());
  // usbc_serial.println(bryan.getHighBeta());
  // usbc_serial.println(bryan.getLowGamma());
  // usbc_serial.println(bryan.getHighGamma());
  // cc1 = down[0] ? (int)mapf(chaos1.get_x() + meap.pot_vals[0], 0.0f, 1047.0f, 0, 127) : map(bryan.getTheta() + meap.pot_vals[0], 0, 2501045, 0, 127);
  // cc2 = down[1] ? (int)mapf(chaos1.get_y() + meap.pot_vals[0], 0.0f, 1046.0f, 0, 127) : map(bryan.getDelta() + meap.pot_vals[0], 0, 2501045, 0, 127);
  // cc3 = down[2] ? (int)mapf(chaos2.get_x() + meap.pot_vals[0], -20.0f, 1065.0f, 0, 127) : map(bryan.getLowAlpha() + meap.pot_vals[0], 0.0f, 2501045.0f, 0, 127);
  // cc4 = down[3] ? (int)mapf(chaos2.get_y() + meap.pot_vals[0], -20.0f, 1065.0f, 0, 127) : map(bryan.getHighAlpha() + meap.pot_vals[0], 0, 2501045, 0, 127);
  // cc5 = down[4] ? (int)mapf(chaos3.get_x() + meap.pot_vals[0], -2.0f, 1048.0f, 0, 127) : map(bryan.getLowBeta() + meap.pot_vals[0], 0, 1601045, 0, 127);
  // cc6 = down[5] ? (int)mapf(chaos3.get_x() + meap.pot_vals[0], -2.0f, 1047.0f, 0, 127) : map(bryan.getHighBeta() + meap.pot_vals[0], 0, 601045, 0, 127);
  // cc7 = down[6] ? (int)mapf(chaos4.get_y() + meap.pot_vals[0], -4.0f, 1049.0f, 0, 127) : map(bryan.getLowGamma() + meap.pot_vals[0], 0, 501045, 0, 127);
  // cc8 = down[7] ? (int)mapf(chaos4.get_x() + meap.pot_vals[0], -4.0f, 1049.0f, 0, 127) : map(bryan.getHighGamma() + meap.pot_vals[0], 0, 501045, 0, 127);

  // MIDI.sendControlChange(0, cc1, 1);
  // MIDI.sendControlChange(1, cc2, 1);
  // MIDI.sendControlChange(2, cc3, 1);
  // MIDI.sendControlChange(3, cc4, 1);
  // MIDI.sendControlChange(4, cc5, 1);
  // MIDI.sendControlChange(5, cc6, 1);
  // MIDI.sendControlChange(6, cc7, 1);
  // MIDI.sendControlChange(7, cc8, 1);
  MIDI.sendControlChange(0, map(bryan.getTheta(), 0, 2500000, 0, 127), 1);
  MIDI.sendControlChange(1, map(bryan.getDelta(), 0, 2500000, 0, 127), 1);
  MIDI.sendControlChange(2, map(bryan.getLowAlpha(), 0, 2500000, 0, 127), 1);
  MIDI.sendControlChange(3, map(bryan.getHighAlpha(), 0, 2500000, 0, 127), 1);
  MIDI.sendControlChange(4, map(bryan.getLowBeta(), 0, 1600000, 0, 127), 1);
  MIDI.sendControlChange(5, map(bryan.getHighBeta(), 0, 600000, 0, 127), 1);
  MIDI.sendControlChange(6, map(bryan.getLowGamma(), 0, 500000, 0, 127), 1);
  MIDI.sendControlChange(7, map(bryan.getHighGamma(), 0, 500000, 0, 127), 1);
  MIDI.sendControlChange(8, constrain(map(meap.pot_vals[0], 0, 1046, 0, 127), 0, 127), 1);
  MIDI.sendControlChange(9, constrain(map(meap.pot_vals[1], 0, 1046, 0, 127), 0, 127), 1);
  MIDI.read();
  audioHook();  // handles Mozzi audio generation behind the scenes
}


/** Called automatically at rate specified by CONTROL_RATE macro, most of your code should live in here
*/
void updateControl() {
  meap.readInputs();
  // bryan.update();
  // usbc_serial.println(brain.readErrors());
  // usbc_serial.println(brain.readCSV());
  // ---------- YOUR updateControl CODE BELOW ----------
}

/** Called automatically at rate specified by AUDIO_RATE macro, for calculating samples sent to DAC, too much code in here can disrupt your output
 */
AudioOutput_t updateAudio() {
  int64_t out_sample = 0;

  return StereoOutput::fromNBit(8, (out_sample * meap.volume_val) >> 12, (out_sample * meap.volume_val) >> 12);
}

/**
 * Runs whenever a touch pad is pressed or released
 *
 * int number: the number (0-7) of the pad that was pressed
 * bool pressed: true indicates pad was pressed, false indicates it was released
 */
void updateTouch(int number, bool pressed) {
  if (pressed) {  // Any pad pressed
  } else {        // Any pad released
  }
  switch (number) {
    case 0:
      if (pressed) {  // Pad 0 pressed
        usbc_serial.println("t0 pressed ");
      } else {  // Pad 0 released
        usbc_serial.println("t0 released");
      }
      break;
    case 1:
      if (pressed) {  // Pad 1 pressed
        usbc_serial.println("t1 pressed");
      } else {  // Pad 1 released
        usbc_serial.println("t1 released");
      }
      break;
    case 2:
      if (pressed) {  // Pad 2 pressed
        usbc_serial.println("t2 pressed");
      } else {  // Pad 2 released
        usbc_serial.println("t2 released");
      }
      break;
    case 3:
      if (pressed) {  // Pad 3 pressed
        usbc_serial.println("t3 pressed");
      } else {  // Pad 3 released
        usbc_serial.println("t3 released");
      }
      break;
    case 4:
      if (pressed) {  // Pad 4 pressed
        usbc_serial.println("t4 pressed");
      } else {  // Pad 4 released
        usbc_serial.println("t4 released");
      }
      break;
    case 5:
      if (pressed) {  // Pad 5 pressed
        usbc_serial.println("t5 pressed");
      } else {  // Pad 5 released
        usbc_serial.println("t5 released");
      }
      break;
    case 6:
      if (pressed) {  // Pad 6 pressed
        usbc_serial.println("t6 pressed");
      } else {  // Pad 6 released
        usbc_serial.println("t6 released");
      }
      break;
    case 7:
      if (pressed) {  // Pad 7 pressed
        usbc_serial.println("t7 pressed");
      } else {  // Pad 7 released
        usbc_serial.println("t7 released");
      }
      break;
  }
}

/**
 * Runs whenever a DIP switch is toggled
 *
 * int number: the number (0-7) of the switch that was toggled
 * bool up: true indicated switch was toggled up, false indicates switch was toggled
 */
void updateDip(int number, bool up) {
  if (up) {  // Any DIP toggled up
  } else {   // Any DIP toggled down
  }
  switch (number) {
    case 0:
      if (up) {  // DIP 0 up
        usbc_serial.println("d0 up");
      } else {  // DIP 0 down
        usbc_serial.println("d0 down");
      }
      break;
    case 1:
      if (up) {  // DIP 1 up
        usbc_serial.println("d1 up");
      } else {  // DIP 1 down
        usbc_serial.println("d1 down");
      }
      break;
    case 2:
      if (up) {  // DIP 2 up
        usbc_serial.println("d2 up");
      } else {  // DIP 2 down
        usbc_serial.println("d2 down");
      }
      break;
    case 3:
      if (up) {  // DIP 3 up
        usbc_serial.println("d3 up");
      } else {  // DIP 3 down
        usbc_serial.println("d3 down");
      }
      break;
    case 4:
      if (up) {  // DIP 4 up
        usbc_serial.println("d4 up");
      } else {  // DIP 4 down
        usbc_serial.println("d4 down");
      }
      break;
    case 5:
      if (up) {  // DIP 5 up
        usbc_serial.println("d5 up");
      } else {  // DIP 5 down
        usbc_serial.println("d5 down");
      }
      break;
    case 6:
      if (up) {  // DIP 6 up
        usbc_serial.println("d6 up");
      } else {  // DIP 6 down
        usbc_serial.println("d6 down");
      }
      break;
    case 7:
      if (up) {  // DIP 7 up
        usbc_serial.println("d7 up");
      } else {  // DIP 7 down
        usbc_serial.println("d7 down");
      }
      break;
  }
}

void handleNoteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  // do something here
  usbc_serial.print("note on @");
  usbc_serial.print(channel);
  usbc_serial.print(", ");
  usbc_serial.print(pitch);
  usbc_serial.print(", ");
  usbc_serial.print(velocity);
  usbc_serial.print(" (chan / pitch / velocity)\n");
}
void handleNoteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  // do something here
  usbc_serial.print("note off @");
  usbc_serial.print(channel);
  usbc_serial.print(", ");
  usbc_serial.print(pitch);
  usbc_serial.print(", ");
  usbc_serial.print(velocity);
  usbc_serial.print(" (chan / pitch / velocity)\n");
}
void handleControlChange(uint8_t channel, uint8_t value, uint8_t cc_num) {
  // do something here
  usbc_serial.print("CC @ ");
  usbc_serial.print(channel);
  usbc_serial.print(", ");
  usbc_serial.print(value);
  usbc_serial.print(", ");
  usbc_serial.print(cc_num);
  usbc_serial.print(" (chan / val / cc_num)\n");
}
void handleProgramChange(uint8_t prog_num, uint8_t channel) {
  // do something here
}

// #endif /* ARDUINO_USB_MODE */