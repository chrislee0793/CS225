#include "rgbapixel.h"

RGBAPixel::RGBAPixel()
{
	red = 255;
	green = 255;
	blue = 255;
	alpha = 255;
}

RGBAPixel::RGBAPixel(unsigned char newRed, unsigned char newGreen, unsigned char newBlue)
{
	red = newRed;
	blue = newBlue;
	green = newGreen;
	alpha = 255;
}
