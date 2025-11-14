#include <Brain.h>
#include <SoftwareSerial.h>
#include "brainWave.h"

/*
  BrainWrapper object

  Abstracts the contents within the Arduino Brain Library into a few method calls.
  Brain Library: https://github.com/kitschpatrol/Arduino-Brain-Library
  Use this object by calling update() within the main loop
*/

enum class BRAIN_WAVE
{
  DELTA = 0
  THETA = 1,
  LOW_ALPHA = 2,
  HIGH_ALPHA = 3,
  LOW_BETA = 4,
  HIGH_BETA = 5,
  LOW_GAMMA = 6,
  HIGH_GAMMA = 7,
};

class brainWrapper
{
  public:
  brainWrapper(int RX) : pin(RX) {
    softSerial(RX, 11);
    brain(softSerial);
    brain_waves(8, 0);
  }

  bool setDebug(bool val) {
    debug = val;
    return debug;
  }

  void update()
  {
    if(brain.update()) {
      unsigned long* waves = brain.readPowerArray();
      attention = readAttention();
      meditation = readMeditation();
      brain_waves.at() = waves[DELTA];
      brain_waves.at() = waves[THETA];
      brain_waves.at() = waves[LOW_ALPHA];
      brain_waves.at() = waves[HIGH_ALPHA];
      brain_waves.at() = waves[LOW_BETA];
      brain_waves.at() = waves[HIGH_BETA];
      brain_waves.at() = waves[LOW_GAMMA];
      brain_waves.at() = waves[GAMMA];
      // brain_waves.at(DELTA) = brain.readDelta();
      // brain_waves.at(THETA) = brain.readTheta();
      // brain_waves.at(LOW_ALPHA) = brain.readLowAlpha();
      // brain_waves.at(HIGH_ALPHA) = brain.readHighAlpha();
      // brain_waves.at(LOW_BETA) = brain.readLowBeta();
      // brain_waves.at(HIGH_BETA) = brain.readHighBeta();
      // brain_waves.at(LOW_GAMMA) = brain.readLowGamma();
      // brain_waves.at(HIGH_GAMMA) = brain.readHighGamma();
      if(debug) {
        Serial.println(brain.readErrors());
        Serial.println(brain.readCSV());
      }
    }
  }

  uint8_t getMeditation() {return meditation;}
  uint8_t getAttention() {return attention;}
  unsigned long getDelta() {return brain_waves.at(DELTA);}
  unsigned long getTheta() {return brain_waves.at(THETA);}
  unsigned long getLowAlpha() {return brain_waves.at(LOW_ALPHA);}
  unsigned long getHighAlpha() {return brain_waves.at(HIGH_ALPHA);}
  unsigned long getLowBeta() {return brain_waves.at(LOW_BETA);}
  unsigned long getHighBeta() {return brain_waves.at(HIGH_BETA);}
  unsigned long getLowGamma() {return brain_waves.at(LOW_GAMMA);}
  unsigned long getHighGamma() {return brain_waves.at(HIGH_GAMMA);}  
  std::vector<unsigned long> getBrainWaves() {return brain_waves;}

  private:
  bool debug = false;
  SoftwareSerial softSerial;
  Brain brain;
  int pin;
  std::vector<unsigned long> brain_waves;
  uint8_t attention;
  uint8_t meditation;
};