/*
  bufferedBrainWave class

  Class that updates, stores, and interpolates a value. Uses a bounded buffer to
  interpolate between different values. Allows for customization of interpolation 
  algorithms.
*/

namespace INTERP
{
  enum class INTERPOLATION
  {
    NEAREST_NEIGHBORS = 0, LINEAR = 1, CUBIC = 2, LAGRANGE = 3, HERMITE = 4,
  };
}

template<typename E>
class buffer
{
  public:
  buffer(int t = 4) {
    memset(0, buffer, buffer_length);
    read_index = 0;
    write_index = 0;
    alpha = 0.2f;
    out_val = 0;
    set_interp_mode(t);
  }

  // random fuckshit oneliner methods
  void set_buffer_len(size_t val) {buffer_length = val;}
  size_t get_buffer_len() {return buffer_length;}
  void get_val() {return out_val;}
  void set_val_at(int index, E new_val) {buffer.at(index) = new_val;}
  unsigned long get_val_at(int index) {return buffer.at(index);}
  std::vector<E>* get_buffer() {return &buffer;}
  void set_alpha(float new_alpha) {alpha = new_alpha;}
  float get_alpha() {return alpha;}

  void set_interp_mode(int type)
  {
    switch(type)
    {
      case 0:
        interpolation_mode = INTERP::INTERPOLATION::NEAREST_NEIGHBORS;
        break;
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

  long read()
  {
    // read_index is updated in respective functions
    switch (interpolation_mode) {
      case INTERP::INTERPOLATION::NEAREST_NEIGHBORS:
        out_val = nearest_neighbors();
        break;
      case INTERP::INTERPOLATION::LINEAR:
        out_val = linear();
        break;
      case INTERP::INTERPOLATION::CUBIC:
        out_val = cubic();
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
    return out_val;
  }

  void write(long input)
  {
    write_index += (write_index + 1) & (buffer_length - 1);
    buffer[write_index] = input;
  }

  void reset()
  {
    memset(buffer, 0, buffer_length);
  }

  E nearest_neighbors()
  {
    int index = int(std::round(write_index - buffer[write_index]));
    read_index = (index < 0) ? (index + buffer_length) : (index);
    read_index++;
    return buffer[index];
  }

  E linear()
  {
    int index1 = read_index;
    int index2 = (read_index + 1) & (buffer_length - 1);
    read_index++;
    return alpha * buffer[index1] + (1 - alpha) * buffer[index2];
  }

  E cubic()
  {
    int indices[4];
    for(size_t i = 0; i < 4; i++) {
      read_index index = write_index - buffer_length[write_index] + (i - 1);
      indices[i] = (read_index < 0) ? (read_index + buffer_length) : (read_index);
    }

    E sample[4];
    for(size_t i = 0; i < 4; i++) {
      sample[i] = buffer[static_cast<size_t>(indices[i])];
    }

    E a0 = sample[3] - sample[2] - sample[0] + sample[1];
    E a1 = sample[0] - sample[1] - a0;
    E a2 = sample[2] - sample[0];
    E a3 = sample[1];

    return a0 * (alpha * alpha * alpha) + a1 * (alpha * alpha) + a2 * alpha + a3;
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
  E lagrange()
  {
    E c0, c1, c2;
    float w0, w1, w2;
    float x = alpha;
    int x0, x1, x2;
    
    // calculate weights
    w0 = 0.5f;
    w1 = -1f;
    w2 = 0.5f;

    // calculate indices
    x0 = read_index;
    x1 = ( (read_index - 1) & (buffer_length - 1) );
    x2 = ( (read_index - 2) & (buffer_length - 1) );

    // compute c1 and c2
    c0 = w0 / (x - x0);
    c1 = w1 / (x - x1);
    c2 = w2 / (x - x2);

    // compute subcomponents
    E numerator = (c0 * buffer[x0]) + (c1 * buffer[x1]) + (c2 * buffer[x2]);
    E denominator = c0 + c1 + c2;

    // update read_index
    read_index++;

    // return values
    return numerator / denominator;
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
  E hermite()
  {
    // calculate power values of alpha
    float a2 = alpha * alpha;
    float a3 = alpha * alpha * alpha;

    // precompute index values
    size_t i0 = (read_index) & (buffer_length - 1);
    size_t i1 = (read_index - 1) & (buffer_length - 1);
    size_t i2 = (read_index - 2) & (buffer_length - 1);
    size_t i3 = (read_index - 3) & (buffer_length - 1);
    // calculate indices and m0 values
    // m0 = (3b[0] - 4b[-1] + b[-2]) / (2)
    // m1 = (3b[1] - 4b[0] + b[-1]) / (2)
    E m0 = ( (3 * i0) - (4 * i1) + i2) / 2;
    E m1 = ( (3 * i1) - (4 * i2) + i3 ) / 2;
    // compute subcomponents of p(t)
    E c1 = ((2 * a3) - (3 * a2) + 1) * p0;
    E c2 = (a3 - (2 * a2) + a) * m0;
    E c3 = ((-2 * a3) + (3 * a2)) * p1;
    E c4 = (a3 + a2) * m1;

    // adjust read_index
    read_index = (read_index + 1) & (buffer_length - 1);

    // compute p(t)
    return c1 + c2 + c3 + c4;
  }

  private:
  const INTERP::INTERPOLATION interpolation_mode;
  float alpha;
  size_t buffer_length = 5;
  E buffer[buffer_length];
  E out_val;
  int read_index;
  size_t write_index;
  
  // unsigned long p0;   // current sample
  // unsigned long p1;   // previous sample
  // unsigned long p2;   // previous previous sample
};