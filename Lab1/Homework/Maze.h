#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include "Cell.h"
#include <list>
#include <stack>
#include <iostream>

#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * x)
#endif
class Maze
{
	Cell *table;
	int numberOfCellsX, numberOfCellsY, numberOfCells;
	int visitedCells, realNumberOfCells;

	bool areUnvisited();
public:
	Maze();
	Maze(int width, int height);
	~Maze();

	void ChangeSize(GLsizei horizontal, GLsizei vertical);
	void FrameWithEnterAndExit();
	void Generate();
	void MyInit();
	void RenderScene();
};

