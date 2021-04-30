#pragma once
#ifndef JW_COLOR_UTILS_H
#define JW_COLOR_UTILS_H

//************Content************

#include <string>


namespace Utils
{
	void rgb2hsl(unsigned char r, unsigned char g, unsigned char b, double* h, double* s, double* l);
	void hsl2rgb(double h, double s, double l, unsigned char* r, unsigned char* g, unsigned char* b);
}


//*******************************

#endif