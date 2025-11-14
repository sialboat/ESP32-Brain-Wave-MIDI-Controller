/*
 * SPDX-FileCopyrightText: 2019 Ha Thach (tinyusb.org)
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-FileContributor: 2023 esp32beans@gmail.com
 */

// This program is based on an ESP-IDF USB MIDI TinyUSB example with minimal
// changes so it works on Arduino-esp32.

#define CONTROL_RATE 128 // Hz, powers of 2 are most reliable
#include <Meap.h>        // MEAP library, includes all dependent libraries, including all Mozzi modules

Meap meap;                                           // creates MEAP object to handle inputs and other MEAP library functions
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI); // defines MIDI in/out ports


#if ARDUINO_USB_MODE
#warning This sketch must be used when USB is in OTG mode
void setup() {}
void loop() {}
#else
#include "USB.h"

// Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:


#include "esp32-hal-tinyusb.h"

static const char *TAG = "usbdmidi";
/** TinyUSB descriptors **/

extern "C" uint16_t tusb_midi_load_descriptor(uint8_t *dst, uint8_t *itf) {
  uint8_t str_index = tinyusb_add_string_descriptor("TinyUSB MIDI");
  uint8_t ep_num = tinyusb_get_free_duplex_endpoint();
  TU_VERIFY(ep_num != 0);
  uint8_t descriptor[TUD_MIDI_DESC_LEN] = {
    // Interface number, string index, EP Out & EP In address, EP size
    TUD_MIDI_DESCRIPTOR(*itf, str_index, ep_num, (uint8_t)(0x80 | ep_num),
                        64)
  };
  *itf += 1;
  memcpy(dst, descriptor, TUD_MIDI_DESC_LEN);
  return TUD_MIDI_DESC_LEN;
}

// From usb.org MIDI 1.0 specification. This 4 byte structure is the unit
// of transfer for MIDI data over USB.
typedef struct __attribute__((__packed__)) {
  uint8_t code_index_number : 4;
  uint8_t cable_number : 4;
  uint8_t MIDI_0;
  uint8_t MIDI_1;
  uint8_t MIDI_2;
} USB_MIDI_t;

// Basic MIDI Messages
// midi spec talks ab this
#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define CONTROL_CHANGE 0xB0

static void usbEventCallback(void *arg, esp_event_base_t event_base,
                             int32_t event_id, void *event_data) {
  if (event_base == ARDUINO_USB_EVENTS) {
    arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
    switch (event_id) {
      case ARDUINO_USB_STARTED_EVENT:
        Serial.println("USB PLUGGED");
        break;
      case ARDUINO_USB_STOPPED_EVENT:
        Serial.println("USB UNPLUGGED");
        break;
      case ARDUINO_USB_SUSPEND_EVENT:
        Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n",
                      data->suspend.remote_wakeup_en);
        break;
      case ARDUINO_USB_RESUME_EVENT:
        Serial.println("USB RESUMED");
        break;

      default:
        break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  meap.begin();
  startMozzi(CONTROL_RATE);   

  USB.onEvent(usbEventCallback);
  tinyusb_enable_interface(USB_INTERFACE_MIDI, TUD_MIDI_DESC_LEN,
                           tusb_midi_load_descriptor);
  USB.begin();
  while (!Serial && millis() < 5000)
    delay(10);
}

void loop() {
  audioHook(); // handles Mozzi audio generation behind the scenes
}


/** Called automatically at rate specified by CONTROL_RATE macro, most of your code should live in here
 */
void updateControl()
{
  meap.readInputs();
  // ---------- YOUR updateControl CODE BELOW ----------
}

/** Called automatically at rate specified by AUDIO_RATE macro, for calculating samples sent to DAC, too much code in here can disrupt your output
 */
AudioOutput_t updateAudio()
{
  int64_t out_sample = 0;

  return StereoOutput::fromNBit(8, (out_sample * meap.volume_val) >> 12, (out_sample * meap.volume_val) >> 12);
}

/**
 * Runs whenever a touch pad is pressed or released
 *
 * int number: the number (0-7) of the pad that was pressed
 * bool pressed: true indicates pad was pressed, false indicates it was released
 */
void updateTouch(int number, bool pressed)
{
  if (pressed)
  { // Any pad pressed
  }
  else
  { // Any pad released
  }
  switch (number)
  {
  case 0:
    if (pressed)
    { // Pad 0 pressed
      Serial.println("t0 pressed ");
    }
    else
    { // Pad 0 released
      Serial.println("t0 released");
    }
    break;
  case 1:
    if (pressed)
    { // Pad 1 pressed
      Serial.println("t1 pressed");
    }
    else
    { // Pad 1 released
      Serial.println("t1 released");
    }
    break;
  case 2:
    if (pressed)
    { // Pad 2 pressed
      Serial.println("t2 pressed");
    }
    else
    { // Pad 2 released
      Serial.println("t2 released");
    }
    break;
  case 3:
    if (pressed)
    { // Pad 3 pressed
      Serial.println("t3 pressed");
    }
    else
    { // Pad 3 released
      Serial.println("t3 released");
    }
    break;
  case 4:
    if (pressed)
    { // Pad 4 pressed
      Serial.println("t4 pressed");
    }
    else
    { // Pad 4 released
      Serial.println("t4 released");
    }
    break;
  case 5:
    if (pressed)
    { // Pad 5 pressed
      Serial.println("t5 pressed");
    }
    else
    { // Pad 5 released
      Serial.println("t5 released");
    }
    break;
  case 6:
    if (pressed)
    { // Pad 6 pressed
      Serial.println("t6 pressed");
    }
    else
    { // Pad 6 released
      Serial.println("t6 released");
    }
    break;
  case 7:
    if (pressed)
    { // Pad 7 pressed
      Serial.println("t7 pressed");
    }
    else
    { // Pad 7 released
      Serial.println("t7 released");
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
void updateDip(int number, bool up)
{
  if (up)
  { // Any DIP toggled up
  }
  else
  { // Any DIP toggled down
  }
  switch (number)
  {
  case 0:
    if (up)
    { // DIP 0 up
      Serial.println("d0 up");
    }
    else
    { // DIP 0 down
      Serial.println("d0 down");
    }
    break;
  case 1:
    if (up)
    { // DIP 1 up
      Serial.println("d1 up");
    }
    else
    { // DIP 1 down
      Serial.println("d1 down");
    }
    break;
  case 2:
    if (up)
    { // DIP 2 up
      Serial.println("d2 up");
    }
    else
    { // DIP 2 down
      Serial.println("d2 down");
    }
    break;
  case 3:
    if (up)
    { // DIP 3 up
      Serial.println("d3 up");
    }
    else
    { // DIP 3 down
      Serial.println("d3 down");
    }
    break;
  case 4:
    if (up)
    { // DIP 4 up
      Serial.println("d4 up");
    }
    else
    { // DIP 4 down
      Serial.println("d4 down");
    }
    break;
  case 5:
    if (up)
    { // DIP 5 up
      Serial.println("d5 up");
    }
    else
    { // DIP 5 down
      Serial.println("d5 down");
    }
    break;
  case 6:
    if (up)
    { // DIP 6 up
      Serial.println("d6 up");
    }
    else
    { // DIP 6 down
      Serial.println("d6 down");
    }
    break;
  case 7:
    if (up)
    { // DIP 7 up
      Serial.println("d7 up");
    }
    else
    { // DIP 7 down
      Serial.println("d7 down");
    }
    break;
  }
}

#endif /* ARDUINO_USB_MODE */
