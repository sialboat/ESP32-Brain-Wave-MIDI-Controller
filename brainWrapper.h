#include <Brain.h>
// #include <SoftwareSerial.h>
#include "brainWave.h"

/*
  BrainWrapper object

  Abstracts the contents within the Arduino Brain Library into a few method calls.
  Brain Library: https://github.com/kitschpatrol/Arduino-Brain-Library
  Use this object by calling update() within the main loop
*/

typedef struct BRAIN_WAVE
{
  static const int DELTA = 0;
  static const int THETA = 1;
  static const int LOW_ALPHA = 2;
  static const int HIGH_ALPHA = 3;
  static const int LOW_BETA = 4;
  static const int HIGH_BETA = 5;
  static const int LOW_GAMMA = 6;
  static const int HIGH_GAMMA = 7;
}BRAIN_WAVE;

class brainWrapper
{
  public:
  brainWrapper(int RX, unsigned long baud = 9600) {
    pin = RX;
    brainSerial(2);
    brain(brainSerial);

    brainSerial.begin(baud, SERIAL_8N1, pin, 42);
    Serial.println("foo)");
  }
  // brainWrapper(HardwareSerial* h, int RX) : brainSerial(h), brain(b), pin(RX), brain(brainSerial) {
  //   // softSerial.begin(115200);
  //   // softSerial(RX, 11);
    // brainSerial = HardwareSerial(2);
    // brain = Brain(brainSerial);
  //   brain_waves(8, 0);
  //   Serial.println("buh");
  // }

  bool setDebug(bool val) {
    debug = val;
    return debug;
  }

  // void begin(unsigned long baud = 9600)
  // {
  //   brainSerial->begin(baud, SERIAL_8N1, pin, 42);
  // }

  void update()
  {
    Serial.println(brain.readCSV());
    if(brain.update()) {
      unsigned long* waves = brain.readPowerArray();
      attention = brain.readAttention();
      meditation = brain.readMeditation();
      brain_waves.at(BRAIN_WAVE::DELTA) = waves[BRAIN_WAVE::DELTA];
      brain_waves.at(BRAIN_WAVE::THETA) = waves[BRAIN_WAVE::THETA];
      brain_waves.at(BRAIN_WAVE::LOW_ALPHA) = waves[BRAIN_WAVE::LOW_ALPHA];
      brain_waves.at(BRAIN_WAVE::HIGH_ALPHA) = waves[BRAIN_WAVE::HIGH_ALPHA];
      brain_waves.at(BRAIN_WAVE::LOW_BETA) = waves[BRAIN_WAVE::LOW_BETA];
      brain_waves.at(BRAIN_WAVE::HIGH_BETA) = waves[BRAIN_WAVE::HIGH_BETA];
      brain_waves.at(BRAIN_WAVE::LOW_GAMMA) = waves[BRAIN_WAVE::LOW_GAMMA];
      brain_waves.at(BRAIN_WAVE::HIGH_GAMMA) = waves[BRAIN_WAVE::HIGH_GAMMA];
      // brain_waves.at(DELTA) = brain.readDelta();
      // brain_waves.at(THETA) = brain.readTheta();
      // brain_waves.at(LOW_ALPHA) = brain.readLowAlpha();
      // brain_waves.at(HIGH_ALPHA) = brain.readHighAlpha();
      // brain_waves.at(LOW_BETA) = brain.readLowBeta();
      // brain_waves.at(HIGH_BETA) = brain.readHighBeta();
      // brain_waves.at(LOW_GAMMA) = brain.readLowGamma();
      // brain_waves.at(HIGH_GAMMA) = brain.readHighGamma();
    }
    if(debug) {
      Serial.println(brain.readErrors());
      Serial.println(brain.readCSV());
    }
  }

  uint8_t getMeditation() {return meditation;}
  uint8_t getAttention() {return attention;}
  unsigned long getDelta() {return brain_waves.at(BRAIN_WAVE::DELTA);}
  unsigned long getTheta() {return brain_waves.at(BRAIN_WAVE::THETA);}
  unsigned long getLowAlpha() {return brain_waves.at(BRAIN_WAVE::LOW_ALPHA);}
  unsigned long getHighAlpha() {return brain_waves.at(BRAIN_WAVE::HIGH_ALPHA);}
  unsigned long getLowBeta() {return brain_waves.at(BRAIN_WAVE::LOW_BETA);}
  unsigned long getHighBeta() {return brain_waves.at(BRAIN_WAVE::HIGH_BETA);}
  unsigned long getLowGamma() {return brain_waves.at(BRAIN_WAVE::LOW_GAMMA);}
  unsigned long getHighGamma() {return brain_waves.at(BRAIN_WAVE::HIGH_GAMMA);}  
  std::vector<unsigned long> getBrainWaves() {return brain_waves;}

  private:
  bool debug = false;
  HardwareSerial brainSerial;
  // SoftwareSerial* brainSerial;
  Brain brain;
  int pin;
  std::vector<unsigned long> brain_waves;
  uint8_t attention;
  uint8_t meditation;
};