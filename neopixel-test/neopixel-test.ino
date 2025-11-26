// neopixel_demo.ino
#include <Adafruit_NeoPixel.h>

// replace the 32 below with whatever pin your Neopixel is connected to
Adafruit_NeoPixel neopixel = Adafruit_NeoPixel(4, 32, NEO_RGB);

int tempo = 1000;

void setup() {
  neopixel.begin();
  neopixel.clear();
  neopixel.show();
}

void loop() {
  // the Neopixel setPixel function works like...
  // setPixelColor(int address, int red, int green, int blue);
  // the address is 0 by default until we start adding more neopixels, just set it to 0 and don't worry about it
  // red, green, and blue can be numbers from 0 to 255, with zero being off, and 255 being all the way up
  // turn neopixel red
  for(int i = 0; i < 4; i++) {
    neopixel.setPixelColor(i, 255, 0, 0);
  }
  neopixel.show();
  delay(tempo);

  // turn neopixel green
  for(int i = 0; i < 4; i++) {
    neopixel.setPixelColor(i, 0, 255, 0);
  }
  neopixel.show();
  delay(tempo);

  // turn neopixel blue
  for(int i = 0; i < 4; i++) {
    neopixel.setPixelColor(i, 0, 0, 255);
  }
  neopixel.show();
  delay(tempo);

  // turn neopixel white - by setting same amount of red, green, and blue
  // setting all to 255 is blindingly bright so I used a lower number
  for(int i = 0; i < 4; i++) {
    neopixel.setPixelColor(i, 100, 100, 100);
  }
  neopixel.show();
  delay(tempo);  

  // turn neopixel yellow - by mixing some yellow and some green
  for(int i = 0; i < 4; i++) {
    neopixel.setPixelColor(i, 127, 127, 0);
  }
  neopixel.show();
  delay(tempo);

  // turn neopixel pink - by mixing some red and some blue
  for(int i = 0; i < 4; i++) {
    neopixel.setPixelColor(i, 127, 0, 127);
  }
  neopixel.show();
  delay(tempo);

  // turn neopixel off - by not lighting up any colors
  for(int i = 0; i < 4; i++) {
    neopixel.setPixelColor(i, 0, 0, 0);
  }
  neopixel.show();
  delay(tempo);
}
