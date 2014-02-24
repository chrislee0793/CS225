#include "scene.h"
#include <iostream>

using namespace std;

//initializes this scene object with a maximum capacity of "max" images
Scene::Scene(int max)
{
	images = new Image[max];
	maximages = max;
	xcoords = new int[max];
	ycoords = new int[max];
	occupied = new bool[max];
	
	for(int i = 0; i < max; i++)
		occupied[i] = false;
}

//frees all space in heap memory allocated by this scene instance
Scene::~Scene()
{
	_clear();
}

//copy constructor to make an independent copy of "source" scene
Scene::Scene(const Scene &source)
{
	_copy(source);
}

//makes this scene an independent copy of the source scene
const Scene & Scene::operator=(const Scene &source)
{
	//fix this shit, check if same address
	//should probably make != operator

	if(&images != &source.images && &xcoords != &source.xcoords && &ycoords != &source.ycoords && &maximages != &source.maximages)
	{
		_clear();
		_copy(source);
	}
	return *this;
}

//increases/decreases maximum number of images in scene
void Scene::changemaxlayers(int newmax)
{
	if(newmax <= 0)
		return;
	Image* newimages = new Image[newmax];
	int* newxcoords = new int[newmax];
	int* newycoords = new int[newmax];
	bool* newoccupied = new bool[newmax];
	
	for(int i = 0; i < maximages; i++)
	{
		newimages[i] = images[i];
		newxcoords[i] = xcoords[i];
		newycoords[i] = ycoords[i];
		newoccupied[i] = occupied[i];
	}

	if(newmax > maximages)
		for(int i = maximages; i < newmax; i++)
		{
			//newimages[i] = new Image();
			//newxcoords[i] = 0;
			//newycoords[i] = 0;
			newoccupied[i] = false;
		}

	maximages = newmax;
	delete[] images;
	delete[] xcoords;
	delete[] ycoords;
	delete[] occupied;
	images = newimages;
	xcoords = newxcoords;
	ycoords = newycoords;
	occupied = newoccupied;
	delete[] newimages;
	delete[] newxcoords;
	delete[] newycoords;
	delete[] newoccupied;
}

//adds a picture the scene at the supplied (x, y) coordinate
void Scene::addpicture(const char *FileName, int index, int x, int y)
{
	if(index >= maximages || index < 0)
		return;
	images[index].readFromFile(FileName);
	xcoords[index] = x;
	ycoords[index] = y;
	occupied[index] = true;
}

//moves an image from one layer to another
void Scene::changelayer(int index, int newindex)
{
	if(index < 0 || index >= maximages || newindex < 0 || newindex >= maximages)
		return;
	images[newindex] = images[index];
	xcoords[newindex] = xcoords[index];
	ycoords[newindex] = ycoords[index];
	occupied[newindex] = true;
	xcoords[index] = 0;
	ycoords[index] = 0;
	occupied[index] = false;
}

//moves image at index to new (x, y) coordinate
void Scene::translate(int index, int xcoord, int ycoord)
{
	if(index < 0 || index >= maximages)
		return;
	xcoords[index] = xcoord;
	ycoords[index] = ycoord;
}

//removes the image at the given index
void Scene::deletepicture(int index)
{
	//images[index] = new Image();
	xcoords[index] = 0;
	ycoords[index] = 0;
	occupied[index] = false;
}

//returns a pointer to the image at specified index
Image* Scene::getpicture(int index) const
{
	if(index >= maximages || index < 0 || !occupied[index])
		return new Image();
	return &images[index];
}

//renders the scene and returns the resulting image by value
Image Scene::drawscene() const
{
	Image output;
	//finding max width
	int width = 0;
	int height = 0;

	for(int i = 0; i < maximages; i++)
		if(occupied[i] && images[i].width() + xcoords[i] > width)
			width = images[i].width() + xcoords[i];

	for(int i = 0; i < maximages; i++)
		if(occupied[i] && images[i].height() + ycoords[i] > height)
			height = images[i].height() + ycoords[i];
	output.resize(width, height);

	for(int i = 0; i < maximages; i++)
		if(occupied[i])
			for(int j = 0; j < images[i].width(); j++)
				for(int k = 0; k < images[i].height(); k++)
					*output(j + xcoords[i], k + ycoords[i]) = RGBAPixel(*images[i](j, k));
	return output;
}

//copies the scene source into this source object
void Scene::_copy(const Scene &source)
{
	maximages = source.maximages;
	images = new Image[maximages];
	xcoords = new int[maximages];
	ycoords = new int[maximages];
	occupied = new bool[maximages];
	for(int i = 0; i < maximages; i++)
	{
		images[i] = source.images[i];
		xcoords[i] = source.xcoords[i];
		ycoords[i] = source.ycoords[i];
		occupied[i] = source.occupied[i];
	}
}

//deletes everything in this instance of Scene
void Scene::_clear()
{
	delete[] images;
	delete[] ycoords;
	delete[] xcoords;
	delete[] occupied;
	images = NULL;
	ycoords = NULL;
	xcoords = NULL;
	occupied = NULL;
}