// **************************************************************
// *		   
// *  quadtree.h
// *		   
// *  Quadtree class
// *		   
// *  CS 225 Spring 2008
// *		   
// **************************************************************

#ifndef QUADTREE_H
#define QUADTREE_H

#include "png.h"
#include <cmath>
#include <iostream>

class Quadtree
{
	public:
	Quadtree();

	Quadtree(const PNG &source, int resolution);

	Quadtree(Quadtree const &other);

	~Quadtree();

	Quadtree const & operator=(Quadtree const &other);

	void buildTree(PNG const &source, int resolution);

	RGBAPixel getPixel(int x, int y) const;

	PNG decompress() const;

	void clockwiseRotate();
	
	void prune(int tolerance);

	int pruneSize(int tolerance) const;

	int idealPrune(int numLeaves) const;
	
	int countNodes() const;
	
	private:

	// A simple class representing a single node of a Quadtree.
	// You may want to add to this class; in particular, it could probably
	// use a constructor or two...
	class QuadtreeNode
	{
		public:

		QuadtreeNode();

		QuadtreeNode(const RGBAPixel &data);

		QuadtreeNode(QuadtreeNode const &other);
		
		QuadtreeNode(RGBAPixel &data, bool &children, int &dist, QuadtreeNode* &ne, QuadtreeNode* &se, QuadtreeNode* &nw, QuadtreeNode* &sw);

		~QuadtreeNode();

		QuadtreeNode const & operator=(QuadtreeNode const &other);

		QuadtreeNode* nwChild;  // pointer to northwest child
		QuadtreeNode* neChild;  // pointer to northeast child
		QuadtreeNode* swChild;  // pointer to southwest child
		QuadtreeNode* seChild;  // pointer to southeast child

		RGBAPixel element;  // the pixel stored as this node's "data"

		bool hasChildren;
		
		int colorDistance;

		private:

		void _copy(QuadtreeNode const &other);
	};
	
	QuadtreeNode* root;    // pointer to root of quadtree
	int res;		   // make reconstructing PNG less painful
	
	void _copy(Quadtree const &other);

	QuadtreeNode* recursiveBuild(PNG const &source, int resolution, int x, int y);

	RGBAPixel getPixelRecursive(int x, int y, int topX, int topY, QuadtreeNode* top, int resolution) const;

	void recursiveDecompress(PNG &output, QuadtreeNode* top, int resolution, int topX, int topY) const;

	void kill(QuadtreeNode* &node);
	
	int count(QuadtreeNode* &node) const;
	
	void recursiveRotate(QuadtreeNode* current);
	
	RGBAPixel average(RGBAPixel a, RGBAPixel b, RGBAPixel c, RGBAPixel d) const;
	
	int maxDifference(RGBAPixel a, RGBAPixel b, RGBAPixel c, RGBAPixel d) const;
	
	int difference(RGBAPixel a, RGBAPixel b) const;
	
	void recursivePrune(int tolerance, QuadtreeNode* node);
	
	int recursivePruneCheck(int tolerance, QuadtreeNode* node) const;
	
	int recursiveIdealPrune(int numLeaves, int low, int high) const;
	
	int getDifference(QuadtreeNode* &node) const;
	
	RGBAPixel getElement(QuadtreeNode* &node) const;
	
	int maxOverallDifference(QuadtreeNode* &node, RGBAPixel ref) const;
	
	/**** Functions added for testing/grading                ****/
	/**** Do not remove this line or copy its contents here! ****/
	#include "quadtree_given.h"
};

#endif
