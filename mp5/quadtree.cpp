// **************************************************************
// *		   
// *  quadtree.cpp
// *		   
// *  Quadtree class
// *		   
// *  CS 225 Spring 2008
// *		   
// **************************************************************

#include "quadtree.h"
#include <iostream>

using namespace std;

Quadtree::Quadtree()
{
	root = NULL;
}

Quadtree::Quadtree(const PNG & source, int resolution)
{
	res = resolution;
	buildTree(source, resolution);
}

Quadtree::Quadtree(Quadtree const &other)
{
	_copy(other);
}

Quadtree::~Quadtree()
{
	kill(root);
}

void Quadtree::kill(QuadtreeNode* &node)
{
	if(node == NULL)
		return;
		
	kill(node->neChild);
	node->neChild = NULL;
	kill(node->seChild);
	node->seChild = NULL;
	kill(node->swChild);
	node->swChild = NULL;
	kill(node->nwChild);
	node->nwChild = NULL;
	
	delete node;
	node = NULL;
}

int Quadtree::countNodes() const
{
	if(root == NULL)
		return 0;
	if(root->neChild == NULL)
		return 1;
	return count(root->neChild) + count(root->seChild) + count(root->nwChild) + count(root->swChild);
}

int Quadtree::count(QuadtreeNode* &node) const
{
	if(node == NULL)
		return 0;
	if(!node->hasChildren)
		return 1;
	return count(node->neChild) + count(node->seChild) + count(node->nwChild) + count(node->swChild);
}

Quadtree const & Quadtree::operator=(Quadtree const &other)
{
	if(this != &other)
		_copy(other);
	return *this;
}

void Quadtree::buildTree(PNG const &source, int resolution)
{
	res = resolution;
	root = recursiveBuild(source, resolution, 0, 0);
}

Quadtree::QuadtreeNode* Quadtree::recursiveBuild(PNG const &source, int resolution, int x, int y)
{
	QuadtreeNode* output = new QuadtreeNode();

	output->element = RGBAPixel();

	if(resolution != 2)
	{
		output->nwChild = recursiveBuild(source, resolution / 2, x, y);
		output->neChild = recursiveBuild(source, resolution / 2, x + resolution / 2, y);
		output->swChild = recursiveBuild(source, resolution / 2, x, y + resolution / 2);
		output->seChild = recursiveBuild(source, resolution / 2, x + resolution / 2, y + resolution / 2);
	}
	else
	{
		output->nwChild = new QuadtreeNode(*source(x, y));
		output->neChild = new QuadtreeNode(*source(x + 1, y));
		output->swChild = new QuadtreeNode(*source(x, y + 1));
		output->seChild = new QuadtreeNode(*source(x + 1, y + 1));
	}

	return output;
}

RGBAPixel Quadtree::getPixel(int x, int y) const
{
	return getPixelRecursive(x, y, 0, 0, root, res);
}

RGBAPixel Quadtree::getPixelRecursive(int x, int y, int topX, int topY, QuadtreeNode* top, int resolution) const
{
	if(!top->hasChildren)
		return top->element;

	int middleX = topX + (resolution / 2);
	int middleY = topY + (resolution / 2);
	if(x < middleX && y < middleY)
		return getPixelRecursive(x, y, topX, topY, top->nwChild, resolution / 2);
	if(x >= middleX && y < middleY)
		return getPixelRecursive(x, y, middleX , topY, top->neChild, resolution / 2);
	if(x < middleX && y >= middleY)
		return getPixelRecursive(x, y, topX, middleY, top->swChild, resolution / 2);
	if(x >= middleX && y >= middleY)
		return getPixelRecursive(x, y, middleX, middleY, top->seChild, resolution / 2);

	return RGBAPixel();
}

PNG Quadtree::decompress() const
{
	PNG output(res, res);
	recursiveDecompress(output, root, res, 0, 0);
	return output;
}

void Quadtree::recursiveDecompress(PNG &output, QuadtreeNode* top, int resolution, int topX, int topY) const
{
	if(!top->hasChildren)
	{
		for(int i = 0; i < resolution; i++)
			for(int j = 0; j < resolution; j++)
			{
				
				output(topX + i, topY + j)->red = top->element.red;
				output(topX + i, topY + j)->green = top->element.green;
				output(topX + i, topY + j)->blue = top->element.blue;
				output(topX + i, topY + j)->alpha = top->element.alpha;
				
				//output(topX + i, topY + j) = top->element;
			}
		
	}
	else
	{
		recursiveDecompress(output, top->nwChild, resolution / 2, topX, topY);
		recursiveDecompress(output, top->neChild, resolution / 2, topX + (resolution / 2), topY);
		recursiveDecompress(output, top->swChild, resolution / 2, topX, topY + (resolution / 2));
		recursiveDecompress(output, top->seChild, resolution / 2, topX + (resolution / 2), topY + (resolution / 2));
	}
}

void Quadtree::clockwiseRotate()
{
	recursiveRotate(root);
}

void Quadtree::recursiveRotate(QuadtreeNode* current)
{
	if(!current->hasChildren)
		return;
		
	QuadtreeNode* temp = current->nwChild;
	current->nwChild = current->swChild;
	current->swChild = current->seChild;
	current->seChild = current->neChild;
	current->neChild = temp;
	
	recursiveRotate(current->nwChild);
	recursiveRotate(current->neChild);
	recursiveRotate(current->swChild);
	recursiveRotate(current->seChild);
}

void Quadtree::prune(int tolerance)
{
	recursivePrune(tolerance, root);
}

void Quadtree::recursivePrune(int tol, QuadtreeNode* node)
{
	if(!node->hasChildren || node == NULL)
		return;
	
	int diff = getDifference(node);
	
	if(diff != -1 && diff <= tol)
	{
		node->element = getElement(node);
		kill(node->neChild);
		node->neChild = NULL;
		kill(node->nwChild);
		node->nwChild = NULL;
		kill(node->seChild);
		node->seChild = NULL;
		kill(node->swChild);
		node->swChild = NULL;
		
		node->hasChildren = false;
		node->colorDistance = 0;
		return;
	}
	
	recursivePrune(tol, node->neChild);
	recursivePrune(tol, node->nwChild);
	recursivePrune(tol, node->seChild);
	recursivePrune(tol, node->swChild);
	
}

int Quadtree::recursivePruneCheck(int tol, QuadtreeNode* node) const
{
	if(!node->hasChildren || node == NULL)
		return 0;
	
	int diff = getDifference(node);
	
	if(/*diff != -1 &&*/ diff <= tol)
		return count(node) - 1;
	
	
	int out = 0;
	out += recursivePruneCheck(tol, node->neChild);
	out += recursivePruneCheck(tol, node->nwChild);
	out += recursivePruneCheck(tol, node->seChild);
	out += recursivePruneCheck(tol, node->swChild);
	
	return out;	//NODES REMOVED
}

//Gets the difference value of the given node
int Quadtree::getDifference(QuadtreeNode* &node) const
{
	if(!node->hasChildren)
		return 0;
	if(!node->neChild->hasChildren)
		return maxDifference(getElement(node->nwChild), getElement(node->neChild), getElement(node->seChild), getElement(node->swChild));
	return maxOverallDifference(node, getElement(node));
}

//Recurses through entire subtree, comparing each element to the reference element, returning the largest difference
int Quadtree::maxOverallDifference(QuadtreeNode* &node, RGBAPixel ref) const
{
	if(node == NULL)
		return 0;
	if(!node->hasChildren)
		return difference(node->element, ref);
	int val = max(difference(getElement(node), ref), maxOverallDifference(node->neChild, ref));
	val = max(val, maxOverallDifference(node->nwChild, ref));
	val = max(val, maxOverallDifference(node->swChild, ref));
	return max(val, maxOverallDifference(node->seChild, ref));
}

//Computes the color value of the given node, averaging if necessary
RGBAPixel Quadtree::getElement(QuadtreeNode* &node) const
{
	if(node == NULL)
		return RGBAPixel();
	if(!node->hasChildren)
		return node->element;
	return average(getElement(node->neChild), getElement(node->nwChild), getElement(node->seChild), getElement(node->swChild));
}

//Returns the average color of the given pixels
RGBAPixel Quadtree::average(RGBAPixel a, RGBAPixel b, RGBAPixel c, RGBAPixel d) const
{
	int redAvg = (a.red + b.red + c.red + d.red) / 4;
	int grnAvg = (a.green + b.green + c.green + d.green) / 4;
	int bluAvg = (a.blue + b.blue + c.blue + d.blue) / 4;
	
	return RGBAPixel(redAvg, grnAvg, bluAvg);
}

//Returns the largest difference between the given four pixels and their average
int Quadtree::maxDifference(RGBAPixel a, RGBAPixel b, RGBAPixel c, RGBAPixel d) const
{
	RGBAPixel avg = average(a, b, c, d);
	int dist = difference(a, avg);
	dist = max(difference(b, avg), dist);
	dist = max(difference(c, avg), dist);
	dist = max(difference(d, avg), dist);
	return dist;
}

//Returns the difference between the given two pixels.
int Quadtree::difference(RGBAPixel a, RGBAPixel b) const
{
	int redDiff = pow(b.red - a.red, 2);
	int grnDiff = pow(b.green - a.green, 2);
	int bluDiff = pow(b.blue - a.blue, 2);
	return redDiff + grnDiff + bluDiff;
}

int Quadtree::pruneSize(int tolerance) const
{
	return countNodes() - recursivePruneCheck(tolerance, root);	//prunecheck returns number of nodes removed
}

int Quadtree::idealPrune(int numLeaves) const
{	
	int derp = max(getDifference(root->neChild), getDifference(root->nwChild));	//not rewriting getDifference for const nodes, ain't nobody got time for that
	derp = max(derp, getDifference(root->swChild));
	derp = max(derp, getDifference(root->seChild));
	
	//cout << derp << endl;
	
	return recursiveIdealPrune(numLeaves, 0, derp);
}

int Quadtree::recursiveIdealPrune(int numLeaves, int low, int high) const
{
	int num = pruneSize((low + high) / 2);
	//cout << "trying " << (low + high) / 2 << ", got " << num << ", range is " << low << "," << high << endl;
	if(high == low + 1)
		return high;
	if(num <= numLeaves)
		return recursiveIdealPrune(numLeaves, low, (low + high) / 2);
	if((low + high) / 2 - low == 1)
		return low;
	return recursiveIdealPrune(numLeaves, (low + high) / 2, high);
}

void Quadtree::_copy(Quadtree const &other)
{
	root = new QuadtreeNode(other.root->element, other.root->hasChildren, other.root->colorDistance, other.root->neChild, other.root->seChild, other.root->nwChild, other.root->swChild);
	res = other.res;
}


/**
 * begin QuadtreeNode
 */


Quadtree::QuadtreeNode::QuadtreeNode()
{
	hasChildren = true;
	colorDistance = -1;
	element = RGBAPixel();
	neChild = NULL;
	seChild = NULL;
	nwChild = NULL;
	swChild = NULL;
}

Quadtree::QuadtreeNode::QuadtreeNode(const RGBAPixel &data)
{
	element = data;
	hasChildren = false;
	colorDistance = 0;
	neChild = NULL;
	seChild = NULL;
	nwChild = NULL;
	swChild = NULL;
}

Quadtree::QuadtreeNode::QuadtreeNode(QuadtreeNode const &other)
{
	_copy(other);
}

Quadtree::QuadtreeNode::QuadtreeNode(RGBAPixel &data, bool &children, int &dist, QuadtreeNode* &ne, QuadtreeNode* &se, QuadtreeNode* &nw, QuadtreeNode* &sw)
{
	element = data;
	hasChildren = children;
	colorDistance = dist;
	neChild = new QuadtreeNode(*ne);
	seChild = new QuadtreeNode(*se);
	nwChild = new QuadtreeNode(*nw);
	swChild = new QuadtreeNode(*sw);
}

Quadtree::QuadtreeNode::~QuadtreeNode()
{
	//nothing
}

Quadtree::QuadtreeNode const & Quadtree::QuadtreeNode::operator=(QuadtreeNode const &other)
{
	if(this != &other)
		_copy(other);
	return *this;
}

void Quadtree::QuadtreeNode::_copy(QuadtreeNode const &other)
{
	element = other.element;
	hasChildren = other.hasChildren;
	if(hasChildren)
	{
		nwChild = new QuadtreeNode(*other.nwChild);
		neChild = new QuadtreeNode(*other.neChild);
		swChild = new QuadtreeNode(*other.swChild);
		seChild = new QuadtreeNode(*other.seChild);
	}
	else
	{
		nwChild = NULL;
		neChild = NULL;
		swChild = NULL;
		seChild = NULL;
	}
}
