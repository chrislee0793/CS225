/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

MosaicCanvas * mapTiles(SourceImage const & theSource, vector<TileImage> const & theTiles)
{
	MosaicCanvas* out = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
	vector< Point<3> > points;
	for(int i = 0; i < theTiles.size(); i++) {
		RGBAPixel avg = theTiles[i].getAverageColor();
		Point<3> p(avg.red, avg.green, avg.blue);
		points.push_back(p);
	}
	KDTree<3> colors(points);
	for(int i = 0; i < theSource.getRows(); i++) {
		for(int j = 0; j < theSource.getColumns(); j++) {
			RGBAPixel sc = theSource.getRegionColor(i, j);
			Point<3> nn = colors.findNearestNeighbor(Point<3>(sc.red, sc.green, sc.blue));
			for(int k = 0; k < theTiles.size(); k++) {
				RGBAPixel avg = theTiles[k].getAverageColor();
				if(avg.red == nn[0] && avg.green == nn[1] && avg.blue == nn[2])
					out->setTile(i, j, theTiles[k]);
			}
		}
	}
	return out;
}