/*
  BrainWave object

  Class that updates, stores, and interpolates a value. Uses the last three samples to compute
  the interpolation algorithms
*/

// namespace alphas {
//   const float delta = 0.9999f;
//   const float theta = 0.0f;
//   const float low_alpha = 0.0f;
//   const float high_alpha = 0.9995f;
//   const float low_beta = 0.0f;
//   const float high_beta = 0.0f;
//   const float low_gamma = 0.0f;
//   const float high_gamma = 0.0f;
//   const float alphas[8] = {delta, theta, low_alpha, high_alpha, low_beta, high_beta, low_gamma, high_gamma};
//   /*(
//       DELTA = 0,
//   THETA = 1,
//   LOW_ALPHA = 2,
//   HIGH_ALPHA = 3,
//   LOW_BETA = 4,
//   HIGH_BETA = 5,
//   LOW_GAMMA = 6,
//   HIGH_GAMMA = 7,
//   )*/
// };

class brainWave {
public:
  brainWave(size_t i)
    : fft_index(i) {
    // alpha = alphas::alphas[fft_index];
    alpha = 0.99995f;
    // alpha = 0.9995f;
    p0 = 0;
    p1 = 0;
    p2 = 0;
    m0 = 0;
    m1 = 0;
    out_val = 0;
  }
  // updates the value stored in val
  void update(unsigned long new_val) {
    // = arr[fft_index];

    // sample data
    p2 = p1;
    p1 = p0;
    p0 = new_val;

    // approximate tangent
    m0 = ( p0 - p1) * 0.5f;  // approximate tangent of p0
    m1 = ( p1 - p2) * 0.5f;  // approximate tangent of p1
    // out = interp();
    float itrp = interp();
    out_val = (itrp < 0) ? 0 : (unsigned long) itrp;
  }

  float interp() {
    out = leaky_integrator();
    return out;
  }

  void update_alpha(float tau, float dt=1.0f) {
    alpha = exp(-dt / tau);
  }

  // simple low computation moving average filter / linear interpolation algorithm
  float lerp() {
    return (alpha * moving_average()) + ((1.0f - alpha) * p0);
  }

  float leaky_integrator() {
    l_out = (alpha)*l_out + (1.f - alpha) * p0;
    return l_out;
  }

  float moving_average() { 
    return (p0 + p1 + p2) / 3.0f;
  }

  // fuckshit getters and setters
public:
  void set_fft_index(size_t val) {
    fft_index = val;
  }
  size_t get_fft_index() {
    return fft_index;
  }
  void set_out_val(unsigned long new_val) {
    out_val = new_val;
  }
  void set_p0(unsigned long new_val) {
    p0 = new_val;
  }
  unsigned long get_val() {
    return out_val;
  }
  float get_valf() {
    return out;
  }
  void set_alpha(float new_alpha = default_alpha) {
    alpha = new_alpha;
  }
  float get_alpha() {
    return alpha;
  }

private:
  size_t fft_index;  // index of the FFT array upon calling "brain->readPowerArray()"
  float alpha;  // akin to weight value
  static constexpr float default_alpha = 0.5f;
  unsigned long out_val;
  float out = 0.0f;
  float p0;  // current sample
  float p1;  // previous sample
  float p2;  // previous previous sample
  float m0;  // slope of current sample p0
  float m1;  // slope of previous sample p1

  float l_out;
  float l_in;
  float alp = 0.999;
};