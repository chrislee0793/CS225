#ifndef RGBA_H
#define RGBA_H
class RGBAPixel
{
public:
	RGBAPixel();
	RGBAPixel(unsigned char newRed, unsigned char newGreen, unsigned char newBlue);
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;	
};
#endif
