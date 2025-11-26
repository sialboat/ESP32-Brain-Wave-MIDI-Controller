/*
  BrainWave object

  Class that updates, stores, and interpolates a value. Uses the last three samples to compute
  the interpolation algorithms
*/


namespace INTERP
{
  enum class INTERPOLATION
  {
    LINEAR = 1, LAGRANGE = 2, HERMITE = 3,
  };
}

class brainWave
{
public:
  brainWave(size_t i) : fft_index(i) {
    alpha = 0.5f;
    interpolation_mode = INTERP::INTERPOLATION::HERMITE;
    p0 = 0;
    p1 = 0;
    p2 = 0;
    m0 = 0;
    m1 = 0;
    out_val = 0;
  }

  void set_interp(int type)
  {
    switch(type)
    {
      case 1:
        interpolation_mode = INTERP::INTERPOLATION::LINEAR;
        break;
      case 2:
        interpolation_mode = INTERP::INTERPOLATION::LAGRANGE;
        break;
      case 3:
        interpolation_mode = INTERP::INTERPOLATION::HERMITE;
        break;
      default:
        interpolation_mode = INTERP::INTERPOLATION::HERMITE;
        break;
    }
  }
  // updates the value stored in val
  void update(unsigned long new_val)
  {
    // = arr[fft_index];

    // sample data
    p2 = p1;
    p1 = p0;
    p0 = new_val;

    // approximate tangent
    m0 = (p1 - p0) / 2;     // approximate tangent of p0
    m1 = (p2 - p0) / 2;     // approximate tangent of p1

    interp();
  }

  void interp()
  {
    switch(interpolation_mode)
    {
      case INTERP::INTERPOLATION::LINEAR:
        out_val = lerp();
        break;
      case INTERP::INTERPOLATION::LAGRANGE:
        out_val = lagrange();
        break;
      case INTERP::INTERPOLATION::HERMITE:
        out_val = hermite();
        break;
      default:
        out_val = hermite();
        break;
    }
    // return out_val;  
  }

  // simple low computation moving average filter / linear interpolation algorithm
  unsigned long lerp()
  {
    return (unsigned long)(alpha * p0) + ((1 - alpha) * p1);
  }

  /*
   Barycentric Lagrange implementation.
   Lagrange interpolation is the smallest degree polynomial to pass through data points. 
   We can calculate x-coordinate positions by multiplying a their basis value with the y-value and 
   summing each of these components. We use the optimized Barycentric form which can apparently be
   rewritten as

   ( sum ((w_j) / (x - x_j) * p_j)  ) / ( sum( (w_j / (x - x)j) ) )

   where x is the distance from point 1 to point 2
   x_j is the time value of the jth sample
   p_j is the value of the jth sample
   w_j is the jth weight value associated with each sample.

   Since this is done in realtime with the latest 2 samples, we know
   x0 = 0 (index of the sample we just collected)
   x1 = -1 (index of the sample before x0)
   x2 = -2 (index of the sample before x1)

   Similarly, we can compute weights w0, w1, w2 because our sample indices are constant. We use
   Pi_(j != m) 1 / (x_j - x_m)

   to get
   w0 = 1 / (0 - 1)(0 - 2) = 1/2 = 0.5f
   w1 = 1 / (1 - 0)(1 - 2) = -1 = -1.0f
   w2 = 1 / (2 - 0)(2 - 1) = 1/2 = 0.5f

   All of this precomputation is done so we can achieve a performance of O(k) operations for the kth degree polynomial.
   Since we use a cubic polynomial, this is incredibly efficient.

   Referred to the "Barycentric Form" section of the Lagrange Polynomial Wikipedia Page
   https://en.wikipedia.org/wiki/Lagrange_polynomial
  */
  unsigned long lagrange()
  {
    float w0, w1, w2;   // weight values
    int8_t x0, x1, x2;  // sample indices
    float x = alpha;    // distance from point 1 to point 2, adjust it via the alpha variable
    unsigned long c0, c1, c2, num, denom; // values

    // sample indices with respect to p2 (current sample), p1 (one sample ago), p0 (two samples ago)
    x0 = 0;
    x1 = -1;
    x1 = -2;

    // weights w_j
    w0 = 0.5f;
    w1 = -1.0f;
    w2 = 0.5f;

    // compute c1 & c2
    c0 = w0 / (x - x0);
    c1 = w1 / (x - x1);
    c2 = w2 / (x - x2);

    // compute the numerator and denominator
    num = (c0 * p0) + (c1 * p1) + (c2 * p2);
    denom = c0 + c1 + c2;

    return num / denom;
  }

  /*
    Hermite implementation (Cubic Hermite Spline), a generalization of lagrange interpolation
    It generalizes Lagrange by including the derivatives of p0, p1, and p2 alongside the values p0, p1, p2.
    As a result, it makes for a really good real-time interpolation for real-time computation.
    Found initially in a section of TAP's "Beginner Audio Plugin book" or whatever it was called.
    We implement the P(t) interpolation equation across a single unit interval [0, 1] in real time by replacing t for alpha

    P(t) = (2t^3 - 3t^2 + 1)p_0 + (t^3 - 2t^2 + t)m_0 + (-2t^3 + 3t^2)p_1 + (t^3 - t^2)m_1

    Referred to the "Interpolation on a single Unit Interval [0, 1]" section for this implementation on Wikipedia.
    https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Representations
  */
  unsigned long hermite()
  {
    // precompute square and cubic values
    uint16_t a2 = alpha * alpha; 
    uint16_t a3 = alpha * alpha * alpha;

    //calculate each subcomponent
    unsigned long c1 = ((2 * a3) - (3 * a2) + 1) * p0;
    unsigned long c2 = (a3 - (2 * a2) + alpha) * m0;
    unsigned long c3 = ((-2 * a3) + (3 * a2)) * p1;
    unsigned long c4 = (a3 + a2) * m1;
    return c1 + c2 + c3 + c4;
  }

  // fuckshit getters and setters
public:
  void set_fft_index(size_t val) {fft_index = val;}
  size_t get_fft_index() {return fft_index;}
  void set_out_val(unsigned long new_val) {out_val = new_val;}
  void set_p0(unsigned long new_val) {p0 = new_val;}
  unsigned long get_val() {return out_val;}
  void set_alpha(float new_alpha = default_alpha) {alpha = new_alpha;}
  float get_alpha() {return alpha;}

  private:
  size_t fft_index;   // index of the FFT array upon calling "brain->readPowerArray()"
  INTERP::INTERPOLATION interpolation_mode;
  float alpha;		// akin to weight value
  static constexpr float default_alpha = 0.5f;
  unsigned long out_val;
  unsigned long p0;   // current sample
  unsigned long p1;   // previous sample
  unsigned long p2;   // previous previous sample
  unsigned long m0;   // slope of current sample p0
  unsigned long m1;   // slope of previous sample p1
};
