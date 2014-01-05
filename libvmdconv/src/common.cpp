
#define _USE_MATH_DEFINES 
#include <cmath> 

#include "common.hpp"

float ToRadian(float x)
{
	return x*M_PI / 180.0f;
}
float ToDegree(float x)
{
	return x/M_PI * 180.0f;
}
