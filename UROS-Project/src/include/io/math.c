#define PI 3.141592653589793238462
#define min(a, b) ( a > b ? b : a)
#define max(a, b) ( a < b ? b : a )
#define abs(a) (a < 0 ? -a : a)

// pseudo-randomizer
static unsigned long ux=123456789, uy=362436069, uz=521288629;
void seedX(int seed){
    ux = seed;
}
unsigned long xorshf96(void) {          //period 2^96-1
    ux ^= ux << 16;
    ux ^= ux >> 5;
    ux ^= ux << 1;

   unsigned t = ux;
   ux = uy;
   uy = uz;
   uz = t ^ ux ^ uy;

  return uz;
}

static unsigned long int next = 1;  // NB: "unsigned long int" is assumed to be 32 bits wide
int rand(void)  // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int) (next / 65536) % 32768;
}
 
void srand()
{
    next = xorshf96();
}

typedef struct NumberAscii{
	int num, ascii;
}NumberAscii;

int convertNumberToASCII(int num){
	NumberAscii number[10] = {
		{0, 48},
		{1, 49},
		{2, 50},
		{3, 51},
		{4, 52},
		{5, 53},
		{6, 54},
		{7, 55},
		{8, 56},
		{9, 57},
	};

	for(int i = 0; i< 10; i++)
		if(number[i].num == num)
			return number[i].ascii;
	return -1;
}

// fast isqrt method

#define SQRT_MAGIC_F 0x5f3759df 
float sqrt(const float x)  
{
  const float xhalf = 0.5f*x;
 
  union // get bits for floating value
  {
    float x;
    int i;
  } u;
  u.x = x;
  u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
  return x*u.x*(1.5f - xhalf*u.x*u.x);// Newton step, repeating increases accuracy
}

int clamp(int x, int min, int max)
{
    // https://news.ycombinator.com/item?id=16410339
    return x < min ? min : x > max ? max : x; 
} 

// sin, cos, tan

double fabs(double x)
{
	return x < 0 ? -x : x;
}

#define SIN_TABLE_LENGTH 4096

const double RAD_TO_INDEX = SIN_TABLE_LENGTH / (2 * PI);
double sin_table[SIN_TABLE_LENGTH];

void InitMath(void)
{
    for (int n = 0; n < SIN_TABLE_LENGTH; n++) {
        double x = n / RAD_TO_INDEX - PI;
        double result = x -
            x * x * x / 6.0 +
            x * x * x * x * x / 120.0 -
            x * x * x * x * x * x * x / 5040.0 +
            x * x * x * x * x * x * x * x * x / 362880;
        sin_table[n] = -result;
    }
}

double sin(double x)
{
	return sin_table[((int)(x * RAD_TO_INDEX)) % SIN_TABLE_LENGTH];
}

double cos(double x)
{
	return sin(x + PI/2);
}

double tan(double x)
{
	return sin(x) / cos(x);
}