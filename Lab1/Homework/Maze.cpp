#include "Maze.h"



bool Maze::areUnvisited()
{
	for (int i = 0; i < numberOfCells; i++)
	{
		if (!table[i].Visited())
			return true;
	}
	return false;
}

Maze::Maze()
{
	table = nullptr;
	numberOfCells = numberOfCellsX = numberOfCellsY = 0;
	visitedCells = 0;
	realNumberOfCells = 0;
}

Maze::Maze(int width, int height)
{
	visitedCells = 0;
	realNumberOfCells = width * height;
	numberOfCellsX = height + 2;
	numberOfCellsY = width + 2;
	numberOfCells = numberOfCellsX * numberOfCellsY;
	table = new Cell[numberOfCells];

	float x1 = 0, y1 = 0;
	for (int i = 0; i < numberOfCells; i++)
	{
		//Calculating coordinates of cells
		x1 = ((i / numberOfCellsX) - 1)*Cell::GetWidth(); 
		y1 = -((i % numberOfCellsX) - 1)*Cell::GetWidth();
		table[i] = Cell(x1, y1, i);

		//Maze frame made of invisible visited cells - easier to implement
		if ((i < numberOfCellsX) || (i > numberOfCells - numberOfCellsX) ||
			((i % numberOfCellsX) == 0) || ((i % numberOfCellsX) == numberOfCellsY - 1))
		{
			//No matter, won't be on screen, just visiting
			table[i].DeleteBottom();
			table[i].DeleteLeft();
			table[i].DeleteRight();
			table[i].DeleteTop();
		}
	}
}


Maze::~Maze()
{
	delete table;
}

void Maze::ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	if (vertical == 0)	vertical = 1;
	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	if (horizontal <= vertical)
		glOrtho(-1, (numberOfCellsX*Cell::GetWidth()+1)*AspectRatio, -(numberOfCellsY*Cell::GetWidth()-1)/AspectRatio, 1, 1.0, -1.0);
	else
		glOrtho(-1, (numberOfCellsX*Cell::GetWidth()+1)*AspectRatio, -(numberOfCellsY*Cell::GetWidth()-1)/AspectRatio, 1, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
 
	glLoadIdentity();
}

void Maze::FrameWithEnterAndExit()
{
	table[rand() % (numberOfCellsX - 2) + 1].DeleteRight();
	table[numberOfCells - numberOfCellsX + rand() % (numberOfCellsX - 2) + 1].DeleteLeft();
	glFlush();
}

void Maze::Generate()
{
	//The depth - first search algorithm of maze generation is frequently implemented using backtracking:

	//Make the initial cell the current cell and mark it as visited
	int randomStartIndex = rand() % numberOfCellsX + numberOfCellsX;
	std::stack<int> stack;
	Cell currentCell = table[randomStartIndex];
	visitedCells++;
	stack.push(currentCell.GetIndex());

	//	While there are unvisited cells
	while (areUnvisited())
	{
		int topIndex = currentCell.GetIndex() - 1,
			bottomIndex = currentCell.GetIndex() + 1,
			leftIndex = currentCell.GetIndex() - numberOfCellsX,
			rightIndex = currentCell.GetIndex() + numberOfCellsX;
		std::list<int> neighbours;

		if (!table[topIndex].Visited())
			neighbours.push_back(topIndex);
		if (!table[bottomIndex].Visited())
			neighbours.push_back(bottomIndex);
		if (!table[leftIndex].Visited())
			neighbours.push_back(leftIndex);
		if (!table[rightIndex].Visited())
			neighbours.push_back(rightIndex);

		//	If the current cell has any neighbours which have not been visited
		if (neighbours.size() > 0)
		{
			currentCell.Render();
			glFlush();
			//	Choose randomly one of the unvisited neighbours
			std::list<int>::iterator it = neighbours.begin();
			for (int i = 0; i < rand() % neighbours.size(); i++)
				it++;
			//	Push the current cell to the stack
			stack.push(table[currentCell.GetIndex()].GetIndex()/*currentCell*/);
			//	Remove the wall between the current cell and the chosen cell
			if (*it == topIndex)
			{
				table[topIndex].DeleteBottom();
				table[currentCell.GetIndex()].DeleteTop();
			}
			if (*it == bottomIndex) 
			{
				table[bottomIndex].DeleteTop();
				table[currentCell.GetIndex()].DeleteBottom();
			}
			if (*it == leftIndex) 
			{
				table[leftIndex].DeleteRight();
				table[currentCell.GetIndex()].DeleteLeft();
			}
			if (*it == rightIndex) 
			{
				table[rightIndex].DeleteLeft();
				table[currentCell.GetIndex()].DeleteRight();
			}
			//	Make the chosen cell the current cell and mark it as visited
			currentCell = table[*it];
			visitedCells++;
		}
		//	Else if stack is not empty
		else if (!stack.empty())
		{
			//	Pop a cell from the stack
			Cell previousCell = table[stack.top()];
			int previousCellIndex = previousCell.GetIndex();
			currentCell = previousCell;
			//	Make it the current cell
			stack.pop();
		}
	}
}

void Maze::MyInit()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Maze::RenderScene()
{
	for (int i = 0; i < numberOfCells; i++)
		table[i].Render();
	Generate();
	FrameWithEnterAndExit();
}
