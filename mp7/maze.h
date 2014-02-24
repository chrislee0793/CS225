#ifndef MAZE_H
#define MAZE_H

#include "png.h"
#include "dsets.h"
#include <vector>

class SquareMaze {

public:
	SquareMaze();

	~SquareMaze();

	void makeMaze(int width, int height);

	bool canTravel(int x, int y, int dir) const;

	void setWall(int x, int y, int dir, bool exits);

	std::vector< int > solveMaze();

	PNG* drawMaze() const;

	PNG* drawMazeWithSolution();

private:
	int w;
	int h;

	int index(int x, int y) const;

	int xcoord(int indx) const;

	int ycoord(int indx) const;

	int allowableWalls(int indx, DisjointSets & ds) const;

	std::vector<int> recursiveSolve(std::vector<int> working, int x, int y);

	bool lengthConflict(std::vector<int> a, std::vector<int> b) const;

	class Cell {
	public:
		Cell();

		bool right;
		bool down;
	};

	Cell* maze;

	//DisjointSets dset;
};

#endif