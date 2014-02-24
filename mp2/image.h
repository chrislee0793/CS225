#ifndef IMAGE_H
#define IMAGE_H

#include "png.h"

class Image : public PNG
{
public:
	//assignment operator is needed
	Image const & operator=(Image const & other);
	//flips the image over the center from left to right
	void flipleft();
	//adds r, g, b to the red, green, and blue channels respectively
	void adjustbrightness(int r, int g, int b);
	//sets color values from x to 255-x, inverting colors
	void invertcolors();
};

#endif