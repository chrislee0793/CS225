/**
 * @file testsquaremaze.cpp
 * Performs basic tests of SquareMaze.
 * @date April 2007
 * @author Jonathan Ray
 */

#include <iostream>
#include <vector>
#include <ctime>
#include "dsets.h"
#include "maze.h"
#include "png.h"

int main()
{ 
   SquareMaze small;
   small.makeMaze(4,4);
   PNG* out1 = small.drawMaze();
   out1->writeToFile("smallUnsolved.png");
   PNG* out2 = small.drawMazeWithSolution();
   out2->writeToFile("smallSolved.png");
   std::vector<int> solution = small.solveMaze();

   delete out1;
   delete out2;


   SquareMaze m;
   const clock_t begin = clock();
   m.makeMaze(800,800);
   std::cout << float(clock() - begin) / CLOCKS_PER_SEC << endl;
   std::cout << "MakeMaze complete" << std::endl;

   PNG* unsolved = m.drawMaze();
   unsolved->writeToFile("unsolved.png");
   delete unsolved;
   std::cout << "drawMaze complete" << std::endl;
   
   std::vector<int> sol = m.solveMaze();
   std::cout << "solveMaze complete" << std::endl;
   
   PNG* solved = m.drawMazeWithSolution();
   solved->writeToFile("solved.png");
   delete solved;
   std::cout << "drawMazeWithSolution complete" << std::endl;
   
   return 0;
}

