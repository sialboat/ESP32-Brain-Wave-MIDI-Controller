/*
  bufferedBrainWave class

  Class that updates, stores, and interpolates a value. Uses a bounded buffer to
  interpolate between different values.
*/

namespace INTERP
{
  enum class INTERPOLATION
  {
    NEAREST_NEIGHBORS = 0, LINEAR = 1, CUBIC = 2, LAGRANGE = 3, HERMITE = 4;
  };
}

template<typename E>
class buffer
{
  public:
  buffer(int t = 4) {
    read_index = 0;
    write_index = 0;
    alpha = 0.2f;
    out_val = 0;
    set_interp_mode(t);
  }

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
    switch (interpolation_mode)
    {
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

  E hermite()
  {
    // calculate power values of alpha

    // calculate indices and m0 values

    // compute subcomponents of p(t)

    // compute p(t)
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