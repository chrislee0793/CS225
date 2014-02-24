#include "maze.h"
#define UP 3
#define DOWN 1
#define LEFT 2
#define RIGHT 0

using namespace std;

SquareMaze::SquareMaze() {
	//nothin'
}

SquareMaze::~SquareMaze() {
	delete[] maze;
}

void SquareMaze::makeMaze(int width, int height) {
	if(width <= 0 || height <= 0)
		return;

	//seed the random generator
	srand(time(NULL));

	w = width;
	h = height;

	maze = new Cell[w * h];

	DisjointSets dset;
	dset.addelements(w * h);
	
	for(int i = 0; i < w * h; i++) {

		int dirs = allowableWalls(i, dset);

		int totalDirs = 0;

		int left = 1;
		int up = 2;
		int right = 4;
		int down = 8;

		if(dirs == 15) {	//all directions available
			totalDirs = 4;
		}
		else if(dirs == 0) {
			//cout << "uhoh" << endl;
			continue;
		}
		else {
			left = dirs & 1;	//unpacking
			up = dirs & 2;
			right = dirs & 4;
			down = dirs & 8;
		}

		bool done = false;
		while(!done) {
			int random = rand() % 4 + 1;
			
			if(random == 1 && left != 0)	{	//left
				setWall(xcoord(i) - 1, ycoord(i), 0, false);
				dset.setunion(i, i - 1);
				done = true;
			}
			else if(random == 2 && up != 0)	{	//up
				setWall(xcoord(i), ycoord(i) - 1, 1, false);
				dset.setunion(i, i - w);
				done = true;
			}
			else if(random == 3 && right != 0) {	//right
				setWall(xcoord(i), ycoord(i), 0, false);
				dset.setunion(i, i + 1);
				done = true;
			}
			else if(random == 4 && down != 0) {	//down
				setWall(xcoord(i), ycoord(i), 1, false);
				dset.setunion(i, i + w);
				done = true;
			}
		}
	}
	//finger crossing time
	//cout << "roots: " << dset.numroots() << endl;
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
	if(dir == RIGHT)
		return !maze[index(x, y)].right;
	if(dir == DOWN)
		return !maze[index(x, y)].down;
	if(dir == LEFT) {
		if(x == 0)
			return false;
		return !maze[index(x - 1, y)].right;
	}
	if(dir == UP) {
		if(y == 0)
			return false;
		return !maze[index(x, y - 1)].down;
	}
	return true;
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
	if(dir == 0)
		maze[index(x, y)].right = exists;
	else
		maze[index(x, y)].down = exists;
}

vector< int > SquareMaze::solveMaze() {
	return recursiveSolve(vector<int>(), 0, 0);
}

vector<int> SquareMaze::recursiveSolve(vector<int> working, int x, int y) {

	int back = (working.size() != 0) ? working.back() : DOWN;	//don't want to turn pi/2 and go backwards, that would be rubbish

	//need to check if it's coming from the opposite direction
	bool left = canTravel(x, y, LEFT) && back != RIGHT;
	bool right = canTravel(x, y, RIGHT) && back != LEFT;
	bool up = canTravel(x, y, UP) && back != DOWN;
	bool down = canTravel(x, y, DOWN) && back != UP;
	bool bottomRow = false;

	if(y == h - 1)
		bottomRow = true;	//potentially an end

	//check to see if there are no more new directions to head in
	if(!left && !right && !up && !down) {
		if(!bottomRow)
			working.push_back(-1);	//this is not a valid ending, so terminating with -1 to signify
		return working;		//if we're on the bottom row, not ending in -1 to signify potential ending
	}

	//otherwise, recurse in all other directions

	vector<int> leftPath;
	vector<int> rightPath;
	vector<int> upPath;
	vector<int> downPath;

	if(left) {
		leftPath = working;
		leftPath.push_back(LEFT);
		leftPath = recursiveSolve(leftPath, x - 1, y);
	}

	if(right) {
		rightPath = working;
		rightPath.push_back(RIGHT);
		rightPath = recursiveSolve(rightPath, x + 1, y);
	}

	if(up) {
		upPath = working;
		upPath.push_back(UP);
		upPath = recursiveSolve(upPath, x, y - 1);
	}

	if(down) {
		downPath = working;
		downPath.push_back(DOWN);
		downPath = recursiveSolve(downPath, x, y + 1);
	}

	vector<int> out;

	bool leftEnd = false, rightEnd = false, upEnd = false, downEnd = false;
	if(leftPath.size() != 0 && leftPath.back() != -1)
		leftEnd = true;
	if(rightPath.size() != 0 && rightPath.back() != -1)
		rightEnd = true;
	if(upPath.size() != 0 && upPath.back() != -1)
		upEnd = true;
	if(downPath.size() != 0 && downPath.back() != -1)
		downEnd = true;

	if(bottomRow && !leftEnd && !rightEnd && !upEnd && !downEnd)	//return the current path if this path is the bottom row and no other paths are
		return working;

	if(leftEnd && leftPath.size() > out.size())
		out = leftPath;

	if(rightEnd && (rightPath.size() > out.size() || (rightPath.size() == out.size() && lengthConflict(rightPath, out))))
		out = rightPath;

	if(upEnd && (upPath.size() > out.size() || (upPath.size() == out.size() && lengthConflict(upPath, out))))
		out = upPath;

	if(downEnd && (downPath.size() > out.size() || (downPath.size() == out.size() && lengthConflict(downPath, out))))
		out = downPath;

	return out;
}

bool SquareMaze::lengthConflict(vector<int> a, vector<int> b) const {		//true if a has a lower x value and should be used
	int ax = 0, bx = 0;
	for(int i = 0; i < a.size(); i++) {		//an instance of RIGHT increases x value, LEFT lowers it
		if(a[i] == RIGHT)
			ax++;
		else if(a[i] == LEFT)
			ax--;
	}
	for(int i = 0; i < b.size(); i++) {
		if(b[i] == RIGHT)
			bx++;
		else if(a[i] == LEFT)
			bx--;
	}
	return ax < bx;
}

PNG* SquareMaze::drawMaze() const {
	PNG* derp = new PNG(w * 10 + 1, h * 10 + 1);

	for(int i = 10; i < w * 10 + 1; i++) {	//top row
		(*derp)(i, 0)->red = 0;
		(*derp)(i, 0)->green = 0;
		(*derp)(i, 0)->blue = 0;
	}

	for(int i = 0; i < h * 10 + 1; i++) {	//left row
		(*derp)(0, i)->red = 0;
		(*derp)(0, i)->green = 0;
		(*derp)(0, i)->blue = 0;
	}

	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++) {
			int ind = index(i, j);
			if(maze[ind].right) {
				for(int k = 0; k <= 10; k++) {
					(*derp)((i + 1) * 10, j * 10 + k)->red = 0;
					(*derp)((i + 1) * 10, j * 10 + k)->green = 0;
					(*derp)((i + 1) * 10, j * 10 + k)->blue = 0;
				}
			}
			if(maze[ind].down) {
				for(int k = 0; k <= 10; k++) {
					(*derp)(i * 10 + k, (j + 1) * 10)->red = 0;
					(*derp)(i * 10 + k, (j + 1) * 10)->green = 0;
					(*derp)(i * 10 + k, (j + 1) * 10)->blue = 0;
				}
			}
		}
	}
	return derp;
}

PNG* SquareMaze::drawMazeWithSolution() {
	PNG* result = drawMaze();
	vector<int> solution = solveMaze();
	int x = 5;
	int y = 5;
	int xc = 0;
	int yc = 0;
	//cout << solution.size() << endl;

	for(int i = 0; i < solution.size(); i++) {
		//cout << " " << solution[i] << " ";
		if(solution[i] == DOWN) {
			for(int j = 0; j < 11; j++) {
				(*result)(x, y + j)->green = 0;
				(*result)(x, y + j)->blue = 0;
			}
			y += 10;
			yc++;
		}
		if(solution[i] == UP) {
			for(int j = 0; j < 11; j++) {
				(*result)(x, y - j)->green = 0;
				(*result)(x, y - j)->blue = 0;
			}
			y -= 10;
			yc--;
		}
		if(solution[i] == RIGHT) {
			for(int j = 0; j < 11; j++) {
				(*result)(x + j, y)->green = 0;
				(*result)(x + j, y)->blue = 0;
			}
			x += 10;
			xc++;
		}
		if(solution[i] == LEFT) {
			for(int j = 0; j < 11; j++) {
				(*result)(x - j, y)->green = 0;
				(*result)(x - j, y)->blue = 0;
			}
			x -= 10;
			xc--;
		}
	}

	for(int i = 1; i < 10; i++) {
		(*result)(xc * 10 + i, (yc + 1) * 10)->red = 255;
		(*result)(xc * 10 + i, (yc + 1) * 10)->green = 255;
		(*result)(xc * 10 + i, (yc + 1) * 10)->blue = 255;
	}

	return result;
}

int SquareMaze::index(int x, int y) const {
	return x + y * w;
}

int SquareMaze::xcoord(int indx) const {
	return indx % w;
}

int SquareMaze::ycoord(int indx) const {
	return (indx - xcoord(indx)) / h;
}

int SquareMaze::allowableWalls(int indx, DisjointSets & ds) const {
	
	int out = 0;
	int x = xcoord(indx);
	int y = ycoord(indx);
	//left
	if(x != 0 && ds.find(indx) != ds.find(indx - 1))
		out += 1;
	//up
	if(y != 0 && ds.find(indx) != ds.find(indx - h))
		out += 2;
	//right
	if(x != w - 1 && ds.find(indx) != ds.find(indx + 1))
		out += 4;
	//down
	if(y != h - 1 && ds.find(indx) != ds.find(indx + h))
		out += 8;

	return out;
}

SquareMaze::Cell::Cell() {
	right = true;	//true if wall is present, false otherwise
	down = true;
}