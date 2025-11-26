#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <Brain.h>
#include <Adafruit_NeoPixel.h>
#include "brainWave.h"

/*
  BrainWrapper object

  Abstracts the contents within the Arduino Brain Library into a few method calls.
  Brain Library: https://github.com/kitschpatrol/Arduino-Brain-Library
  Use this object by calling update() within the main loop.
  Assumes the Brain object has already been connected via a Hardware Serial or a Software Serial
*/

namespace BRAIN_WAVE
{
  enum class FFT_BAND {
    DELTA = 0,
    THETA = 1,
    LOW_ALPHA = 2,
    HIGH_ALPHA = 3,
    LOW_BETA = 4,
    HIGH_BETA = 5,
    LOW_GAMMA = 6,
    HIGH_GAMMA = 7,
  };
}
class brainWrapper
{
public:
  brainWrapper(Brain* b, Adafruit_USBD_CDC* s, Adafruit_USBD_MIDI* m) : brain(b), adafruit_serial(s), adafruit_midi(m) {
    for(int i = 0; i < 8; i++) {
      brainWave wave(i);
      brain_waves.push_back(wave);
    }
  }

  bool setDebug(bool val) {
    debug = val;
    return debug;
  }

  void update()
  {
    if(brain->update()) {
      waves = brain->readPowerArray();
      attention = brain->readAttention();
      meditation = brain->readMeditation();
     if(debug) {
        adafruit_serial->println(brain->readErrors());
        adafruit_serial->println(brain->readCSV());
      }
      interp();
    }
  }

  // update brain wave stuff
  void interp()
  {
    for(size_t i = 0; i < brain_waves.size(); i++) {
      unsigned long new_value = waves[i];
      brain_waves.at(i).update(new_value);
    }
  }

  // getters and setters
 public:
  uint8_t getMeditation() {return meditation;}
  uint8_t getAttention() {return attention;}
  unsigned long getDelta() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::DELTA)).get_val();}
  unsigned long getTheta() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::THETA)).get_val();}
  unsigned long getLowAlpha() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::LOW_ALPHA)).get_val();}
  unsigned long getHighAlpha() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::HIGH_ALPHA)).get_val();}
  unsigned long getLowBeta() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::LOW_BETA)).get_val();}
  unsigned long getHighBeta() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::HIGH_BETA)).get_val();}
  unsigned long getLowGamma() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::LOW_GAMMA)).get_val();}
  unsigned long getHighGamma() {return brain_waves.at(static_cast<size_t>(BRAIN_WAVE::FFT_BAND::HIGH_GAMMA)).get_val();}  
  std::vector<brainWave>* getBrainWaves() {return &brain_waves;}

  // the fuckshit that makes this happen
private:
  Brain* brain;
  Adafruit_USBD_MIDI* adafruit_midi;
  Adafruit_USBD_CDC* adafruit_serial;
  unsigned long* waves;
  bool debug = false;
  int pin;
  std::vector<brainWave> brain_waves;
  uint8_t attention;
  uint8_t meditation;
};
