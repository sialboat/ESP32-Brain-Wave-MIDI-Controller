// chaosGenerator.h
// class that abstracts basic chaos generators
#include <iomanip>
#include <functional>
#include <math>
#include <chrono>

#define _USE_MATH_DEFINES

class
chaos_generator
{
public:
	chaos_generator(uint8_t i) : id(i);

	// computes a chaos generator function
	virtual void process();
	
	void set_id(uint8_t new) {id = new;}
	uint8_t get_id() {return id;}
protected:
	uint8_t id;
private:

};

// implementation of fractal_dreams chaos generator
// based off of this website: 
// https://www.softology.pro/tutorials/attractors2d/tutorial.htm
class
fractal_dreams : public chaos_generator
{
private:
	enum class trig_function {sin, cos};
	struct functions{
		trig_function t1;
		trig_function t2;
		trig_function t3;
		trig_function t4;
	}
	void init_permutations()
	{
		trig_function choices[2] = {trig_function::sin,
					trig_function::cos};

		for(auto f1_1 : choices)
			for(auto f1_2 : choices)
				for(auto f2_1 : choices)
					for(auto f2_2 : choices)
		{
			functions f;
			f.t1 = f1_1;
			f.t2 = f1_2;
			f.t3 = f2_1;
			f.t4 = f2_2;
			func_permutations.push_back(f);
		}

	}
public:
	// constructors
	fractal_dreams(uint8_t i, uint8_t m) : chaos_generator(i), mode(m){
		old_x = 0.0f;
		old_y = 0.0f;
		new_x = 0.0f;
		new_y = 0.0f;
		a = b = c = d = 0.0f;
		init_permutations();
	}
	fractal_dreams(uint8_t i) : chaos_generator(i), mode(0){
		old_x = 0.0f;
		old_y = 0.0f;
		new_x = 0.0f;
		new_y = 0.0f;
		a = b = c = d = 0.0f;
		init_permutations();
	}
	fractal_dreams() : chaos_generator(999), mode(0){
		old_x = 0.0f;
		old_y = 0.0f;
		new_x = 0.0f;
		new_y = 0.0f;
		a = b = c = d = 0.0f;
		init_permutations();
	}

	void process() {
		// pass x, y, a, b, c, d
		auto apply = [&](trig_functions t, float arg) {
			return (t == trig_function::sin ? std::sin(arg) : 
				std::cos(arg));
		};
		new_x = apply(func_permutations.at(mode).t1, old_y * b) + 
			c * apply(func_permutations.at(mode).t2, old_x * b);
		new_y = apply(func_permutations.at(mode).t3, old_x * a) +
			d * apply(func_permutations.at(mode).t4, old_y * a);
	}

public:
	uint8_t get_mode() {return mode;}
	void set_mode(uint8_t m) {mode = m;}
	void set_a(float a) {this->a = a;};
	void set_b(float b) {this->b = b;};
	void set_c(float c) {this->c = c;};
	void set_d(float d) {this->d = d;};
	void set_coeffs(float a, float b, float c, float d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
	void get_coeffs(float* a, float* b, float* c, float* d) {
		a = &this->a;
		b = &this->b;
		c = &this->c;
		d = &this->d;
	}
	
private:
// The values for a and b can be any floating point value between -3 and +3.
// The values for c and d can be any floating point value between -0.5 and +1.5.
	uint8_t mode;
	float a, b, c, d;
	float old_x;
	float old_y;
	float new_x;
	float new_y;
	std::vector<functions> func_permutations;
};

// gingerbread man attractor
class
gingerbread_man : public chaos_generator
{
public:
	gingerbread_man(uint8_t i) : chaos_generator(i){
		new_x = 0.0f;
		new_y = 0.0f;
		old_x = -0.1f;
		old_y = 0.0f;
	}
	gingerbread_man(uint8_t i) = default;

	void process() {
		new_x = (1 - old_y) + std::abs(old_x);
		new_y = old_x;
	}

public:
	float get_x() {return new_x;}
	float get_y() {return new_y;}
private:
	float new_x;
	float old_x;
	float new_y;
	float old_y;
};

class
gumoski_mira : public chaos_generator
{
public:
	gumoski_mira(uint8_t i) : chaos_generator(i){
		old_x = 0.0f;
		old_y = 0.0f;
		new_x = 0.0f;
		new_y = 0.0f;
		w = 0.0f;
		t = 0.0f;
		a = 0.0f;
		mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<float> dist(0.0f, 0.36f);
		b = 1.0f - dist(rng);
	}
	gumoski_mira() = default;

	void process() {
		w = 0.0f;
		t = old_x;
		new_x = b * old_y + w;
		w = ( a * old_x + (1 - a) * 2 * (old_x * old_x) ) / ( 1 + (old_x * old_x) );
		new_y = w - t;
	}
public:
	float get_x() {return new_x;}
	float get_y() {return new_y;}
	void get_coeffs(float* a, float* b, float* w, float* t) {
		a = &this->a;
		b = &this->b;
		w = &this->w;
		t = &this->t;
	}

	void set_coeffs(float* a, float* b, float* w, float* t) {
		this->a = a;
		this->b = b;
		this->w = w;
		this->t = t;
	}
private:
	// x and y must be between 20 and -20
	// a must be between 1 and -1
	float a;
	float b;
	float w;
	float t;
	float old_x;
	float old_y;
	float new_x;
	float new_y;
};

class
henon : public chaos_generator
{
public:
	henon(uint8_t i) : chaos_generator(i) {
		old_x = 1.0f;
		old_y = 1.0f;
	}

	void process() {
		new_x = old_y + (1.4f * old_x * old_x);
		new_y = 0.3f * old_x;
	}

public:
	float get_x() {return new_x;}
	float get_y() {return new_y;}
	void set_vals(float* x, float* y)
	{
		old_x = x;
		new_y = y;
	}


private:
	float old_x;
	float old_y;
	float new_x;
	float new_y;
};

class 
hopalong : public chaos_generator
{
public:
	hopalong(uint8_t i) : chaos_generator(i)
	{
		old_x = 0.0f;
		old_y = 0.0f;
		a = 0.0f;
		b = 0.0f;
		c = 0.0f;
	}
	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}
	void process()
	{
		new_x = (old_y - 1) - std::sqrt(std::abs(b * old_x - 1 - c)) * sgn(x - 1);
		new_y = a - old_x - 1;
	}
public:
	void get_vals(float* x, float* y) {
		x = &this->new_x;
		y = &this->new_y;
	}
	void get_coeffs(float* a, float* b, float* c) {
		a = &this->a;
		b = &this->b;
		c = &this->c;
	}
	void set_vals(float* x, float* y) {
		this->old_x = x;
		this->old_y = y;
	}
private:
	// a, b, c can be between 0 and 10 inclusive.
	float a, b, c;
	float old_x, old_y;
	float new_x, new_y;
};

class 
ikeda : public chaos_generator
{
public:
	ikeda(uint8_t i) : chaos_generator(i){
		t = 0.0f;
		a = 1.0f;
		// b must be constrained between [0, 1];
		mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<float> dist(0.0f, 1.1f);
		b = dist(rng);
		// c must be between 0.4f plus/minus 2pi;
		std::uniform_real_distribution<float> dist1(0.4f - 2 * M_PI, 0.4f + 2 * M_PI);
		c = dist1(rng);
		// d must be between 5.0f and 8.0f;
		std::uniform_real_distribution<float> dist2(5.0f, 8.0f);
		d = dist2(rng);


	}
	void process() {
		t = c - (d) / (1 + old_x * old_x + old_y * old_y);
		new_x = a + b * (old_x * std::cos(t) - old_y * std::sin(t));
		new_y = b * (old_x * std::sin(t) + old_y * std::cos(t));
	}
public:
	void get_vals(float* x, floay* y) {
		x = &this->new_x;
		y = &this->new_y;
	}
	void set_coeffs(float* a, floay* b, float* c, float* d) {
		this->a = std::clamp(a, 1.0f, 5.0f);
		this->b = std::clamp(b, 0.0f, 1.0f);
		this->c = std::clamp(c, (0.4f - 2 * M_PI), (0.4f + 2 * M_PI));
		this->d = std::clamp(d, 5.0f, 8.0f);
	}
private:
	float old_x, old_y;
	float new_x, new_y;
	float a, b, c, d, t;
};

class
lorenz : public chaos_generator
{
public:
	lorenz(uint8_t i) : chaos_generator(i) {
		old_x = old_y = old_z = 1.0f;
		new_x = new_y = new_z = 0.0f;
		dt = 0.015f;
		a = 10.0f; // must be positive
		b = 24.74f; // must be positive, becomes unstable for b > 1;
		c = 8.0f/3.0f; // must be positive, keep this value though.
	}
	void process() {
		new_x = old_x + dt * a * (old_y - old_x);
		new_y = old_y + dt * (x * (b - old_z) - old_y);
		new_z = old_z +d t * (old_x * old_y - c * old_z);
		}

public:
	void get_vals(float* x, float* y, float* z) {
		x = &this->new_x;
		y = &this->new_y;
		z = &this->new_z;
	}
	void get_coeffs(float* a, float* b, float* c, float* dt) {
		a = &this->a;
		b = &this->b;
		c = &this->c;
		dt = &this->dt;
	}
	void set_coeffs(float* a, float* b, float* c, float* dt) {
		this->a = std::max(a, 0.0f);
		this->b = std::max(b, 0.0f);
		this->c = std::max(c, 0.0f);
		this->dt = dt;
	}
private:
	float old_x, old_y, old_z;
	float new_x, new_y, new_z;
	float dt, a, b, c;
};

class
popcorn : public chaos_generator
{
public:
	// h should be between 0.001 and 0.1
	// k should be greater than 1
	popcorn(uint8_t i) : chaos_generator(i) {
		old_x = old_y = 0.0f;
		new_x = new_y = 0.0f;
		h = 0.35f;
		k = 3.0f;
	}
	void process() {
		new_x = old_x - h * std::sin(y + std::tan(k * old_y));
		new_y = old_y - h * std::sin(x + std::tan(h * old_x));
	}
public:
	float get_vals(float* x, float* y) {
		x = &this->new_x;
		y = &this->new_y;
	}
	float get_coefs(float* h, float* k) {
		h = &this->h;
		k = &this->k;
	}
	void set_coefs(float* h, float* k) {
		this->h = std::clamp(h, 0.001f, 0.1f);
		this->k = std::max(1.0f, k);
	}
private:
	float old_x, old_y;
	float new_x, new_y;
	float h, k;
};

class
quadrup_two : public chaos_generator
{
public:
	quadrup_two(uint8_t i) : chaos_generator(i) {
		new_y = new_x = 0.0f;
		old_x = old_y = 0.0f;
		a = 34.0f;
		b = 1.0f;
		c = 5.0f;

	}
	void process() {
		int sgn = (x>0)?1:(x<0)?-1:0;
		float t = std::log(std::abs(c * old_x - b));
		new_x = old_y - sgn * std::sin(std::ln(std::abs(b * old_x - c))) * std::arctan(t * t);
	}

public:
	void get_coeffs(float* a, float* b, float* c) {
		a = &this->a;
		b = &this->b;
		c = &this->c;
	}
	void get_vals(float* x, float* y) {
		x = &this->new_x;
		y = &this->new_y;
	}

private:
	float a, b, c;
	float old_x, old_y;
	float new_x, new_y;

};

class
rossler : public chaos_generator
{
public:
	rossler(uint8_t i) : chaos_generator(i) {
		delta_x = delta_y = delta_z = 0.0f;
		old_x = old_y = old_z = 0.1f;
		new_x = new_y = new_z = 0.0f;
		a = b = 0.2f;
		c = 5.7f;
	}
	// a [.1, .5]; b [.1, .4];
	// c [4.0 6.0]
	void process() {
		delta_x = ((-1 * old_x) - old_z);
		new_x = old_x + delta_x;
		delta_y = old_x + (a * old_y);
		new_y = old_y + delta_y;
		delta_z = (b + old_z * (old_x - c));
		new_z = old_z + delta_z;
	}
public:
	void get_vals(float* x, float* dx, float y, float* dy, float* z, float* dz)
	{
		x = &this->new_x;
		y = &this->new_y;
		z = &this->new_z;
		dx = &this->delta_x;
		dy = &this->delta_y;
		dz = &this->delta_z;
	}
	void set_coefs(float* a, float* b, float* c) {
		this->a = std::clamp(a, 0.1f, 0.5f);
		this->b = std::clamp(b, 0.1f, 0.4f);
		this->c = std::clamp(c, 4.0f, 6.0f);
	}

	void get_coefs(float* a, float* b, float* c) {
		a = &this->a;
		b = &this->b;
		c = &this->c;
	}
private:
	float delta_x, delta_y, delta_z;
	float a, b, c;
	float old_x, old_y, old_z;
	float new_x, new_y, new_z;

};

class 
symmetric_icons : public chaos_generator
{
public:
	symmetric_icons(uint8_t i) : chaos_generator(i) {
		old_x = 0.1f;
		old_y = 0.1f;
		new_x = new_y = 0.0f;
		n = 3;

	}
	void process() {
		float r = std::sqrt(old_x * old_x + old_y * old_y);
		float theta = std::atan2(old_y, old_x);

		float rn = std::pow(r, n);
		float u = rn * std::cos(n * theta);
		float v = rn * std::sin(n * theta);

		new_x = a * old_x + (b + c) * u - (b + 2 * d) * v + e;
		new_y = a * old_y + (c - b) * v;
	}

public:
	void get_vals(float* x, float* y) {
		x = &this->x;
		y = &this->y;
	}
	void get_coefs(float* a, float* b, float* c, float* d, float* e, float* n) {
		a = &this->a;
		b = &this->b;
		c = &this->c;
		d = &this->d;
		e = &this->e;
		n = &this->n;
	}
	void set_coefs(float* a, float* c, float* d, float* e, float* n) {
		this->a = std::clamp(a, -3.0f, 3.0f);
		this->b = -1 * a;
		this->c = c;
		this->d = std::clamp(d, -1.0f, 1.0f);
		this->e = std::clamp(e, -1.0f, 1.0f);
		this->n = std::max(2.0f, n);
	}

private:
	// n >= 2
	// a [-3, 3]; anything around [-1, 1] is not interesting
	// b is opposite of a
	// c can be any value
	// d is between [-1, 1]. 0 is mirror symmetry
	// e is between [-1, 1]. not close to zero.
	float old_x, old_y;
	float new_x, new_y;
	uint8_t n;
	float a, b, c, d, e;
};

class
tinkerbell : public chaos_generator;
{
public:
	tinkerbell(uint8_t i) : chaos_generator(i) {
		new_x = new_y = 0.0f;
		old_x = old_y = 0.1f;
		a = b = c = d = 1.0f;
	}
	void process() {
		new_x = std::sqrt(old_x) - std::sqrt(old_y) + (a * old_x) + (b * old_y);
		new_y = (2 * old_x * old_y) + (c * old_x) + (d * old_y);
	}

public:
	void get_vals(float* x, float* y) {
		x = &this->new_x;
		y = &this->new_y;
	}
	void set_vals(float* x, float* y) {
		this->x = x;
		this->y = y;
	}
	void get_coefs(float* a, float* b, float* c, float* d) {
		a = &this->a;
		b = &this->b;
		c = &this->c;
		d = &this->d;
	}
	void set_coefs(float* a, float* b, float* c, float* d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}
private:
	float a, b, c, d;
	float new_x, new_y;
	float old_x, old_y;
};
