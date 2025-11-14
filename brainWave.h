class brainWave
{
  public:
  brainWave() {
    val = 0;
    // prev_val = 0;
  }
  unsigned long get_val() {return val;}
  unsigned long update(unsigned long to_update)
  {
    if(val != to_update) {
      val = to_update;
    }
    return val;
  }

  private:
  unsigned long val;
};