#include <stdlib.h>
#include "gradientColorPicker.h"

/**
 * Constructs a new gradientColorPicker.
 *
 * @param fadeColor1 The first color to start the gradient at.
 * @param fadeColor2 The second color to end the gradient with.
 * @param radius How quickly to transition to fadeColor2.
 * @param centerX X coordinate for the center of the gradient.
 * @param centerY Y coordinate for the center of the gradient.
 */
gradientColorPicker::gradientColorPicker( RGBAPixel fadeColor1, 
		RGBAPixel fadeColor2, int radius, int centerX, int centerY ) {
	/** 
	 * @todo Construct your gradientColorPicker here! You may find it
	 *	helpful to create additional member variables to store things.
	 */
	 start = fadeColor1;
	 end = fadeColor2;
	 rad = radius;
	 centX = centerX;
	 centY = centerY;
}

/**
 * Picks the color for pixel (x, y).
 *
 * The first color fades into the second color as you move from the initial
 * fill point, the center, to the radius. Beyond the radius, all pixels
 * should be just color2. 
 *
 * You should calculate the distance between two points using the standard
 * Manhattan distance formula, 
 * 
 * \f$d = |center\_x - given\_x| + |center\_y - given\_y|\f$
 *
 * Then, scale each of the three channels (R, G, and B) from fadeColor1 to
 * fadeColor2 linearly from d = 0 to d = radius. 
 *
 * For example, the red color at distance d where d is less than the radius
 * must be
 *
 * \f$ redFill = fadeColor.red - \left\lfloor
   \frac{d*fadeColor1.red}{radius}\right\rfloor +
   \left\lfloor\frac{d*fadeColor2.red}{radius}\right\rfloor\f$
 *
 * Note that all values are integers. If you do not follow this formula,
 * your colors may be very close but round differently than ours.
 *
 * @param x The x coordinate to pick a color for.
 * @param y The y coordinate to pick a color for.
 * @return The color selected for (x, y).
 */
RGBAPixel gradientColorPicker::operator()(int x, int y)
{
	RGBAPixel color;
	/**
	 * @todo Return the correct color here!
	 */
	/*
	int dist = min(rad, abs(centX - x) + abs(centY - y));
	
	color.red = start.red - floor((dist * start.red) / rad) + floor((dist * end.red) / rad);
	color.green = start.green - floor((dist * start.green) / rad) + floor((dist * end.green) / rad);
	color.blue = start.blue - floor((dist * start.green) / rad) + floor((dist * end.blue) / rad);
	*/

	int dx = abs(centX - x) + abs(centY - y);
	int fr = start.red - floor((dx*start.red)/rad) + floor((dx*end.red)/rad);
	int fg = start.green - floor((dx*start.green)/rad) + floor((dx*end.green)/rad);
	int fb = start.blue - floor((dx*start.blue)/rad) + floor((dx*end.blue)/rad);
	if(fr > 255) fr = 255;
	if(fg > 255) fg = 255;
	if(fb > 255) fb = 255;
	if(fr < 0) fr = 0;
	if(fg < 0) fg = 0;
	if(fb < 0) fb = 0;
	if(dx > rad) return end;
	color.red = fr;
	color.green = fg;
	color.blue = fb;

	/**
	 * @todo Return the correct color here!
	 */
	return color;
	return color;
}
