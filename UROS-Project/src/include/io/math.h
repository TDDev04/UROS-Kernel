#pragma once
#define PI 3.141592653589793238462
#define min(a, b) ( a > b ? b : a)
#define max(a, b) ( a < b ? b : a )
#define abs(a) (a < 0 ? -a : a)

// pseudo-randomizer
static unsigned long ux=123456789, uy=362436069, uz=521288629;
void seedX(int seed);
unsigned long xorshf96(void);
static unsigned long int next = 1;  // NB: "unsigned long int" is assumed to be 32 bits wide
int rand(void);
void srand();

typedef struct NumberAscii
{
    int num, ascii;
}NumberAscii;

int convertNumberToASCII(int num);
#define SQRT_MAGIC_F 0x5f3759df 
float sqrt(const float x);  
int clamp(int x, int min, int max);
double fabs(double x);
#define SIN_TABLE_LENGTH 4096
double sin_table[SIN_TABLE_LENGTH];
void InitMath(void);
double sin(double x);
double cos(double x);
double tan(double x);