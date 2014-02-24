#include "image.h"
#include <stdio.h>

using namespace std;

Image const & Image::operator=(Image const & other)
{
	PNG::operator=(other);
	return *this;
}

//flips the image over the center from left to right
void Image::flipleft()
{
	for(int i = 0; i < (width()) / 2; i++)
	{
		for(int j = 0; j < (height()); j++)
		{
			RGBAPixel* leftPointer = PNG::operator()(i, j);	//pointer to left side pixel
			RGBAPixel leftBackup = *leftPointer;			//backup to keep hold of pixel for when it changes
			RGBAPixel* rightPointer = PNG::operator()(width() - 1 - i, j);	//pointer to right side pixel

			*leftPointer = *rightPointer;
			*rightPointer = leftBackup;
		}
	}
}

//adds r, g, b to the red, green, and blue channels respectively
void Image::adjustbrightness(int r, int g, int b)
{
	for(int i = 0; i < (width()); i++)
	{
		for(int j = 0; j < (height()); j++)
		{
			RGBAPixel *target = PNG::operator()(i, j);
			target->red + r < 0 ? target->red = 0: target->red = min(255, target->red + r);
			target->green + g < 0 ? target->green = 0 : target->green = min(255, target->green + g);
			target->blue + b < 0 ? target->blue = 0 : target->blue = min(255, target->blue + b);
		}
	}
}

//sets color values from x to 255-x, inverting colors
void Image::invertcolors()
{
	for(int i = 0; i < (width()); i++)
	{
		for(int j = 0; j < (height()); j++)
		{
			RGBAPixel *target = PNG::operator()(i, j);
			target->red = 255 - target->red;
			target->blue = 255 - target->blue;
			target->green = 255 - target->green;
		}
	}
}