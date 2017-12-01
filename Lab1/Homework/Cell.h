#pragma once
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>

class Cell
{
	static const float width; //Square size
	float x, y; //Upper left side vertex
	bool visited;
	int index;
	bool top, bottom, left, right;
public:
	Cell();
	//x1, y1 coordinates of upper left vertex
	Cell(float x1, float y1, int i);
	~Cell();

	//Deletes bottom wall of cell
	void DeleteBottom();
	//Deletes left wall of cell
	void DeleteLeft();
	//Deletes right wall of cell
	void DeleteRight();
	//Deletes top wall of cell
	void DeleteTop();
	//Return table index
	int GetIndex();
	//Render one cell
	void Render();
	//Draw bottom wall of cell
	void SetBottom();
	//Draw left wall of cell
	void SetLeft();
	//Draw right wall of cell
	void SetRight();
	//Draw top wall of cell
	void SetTop();
	//Returns true if cell was visited
	bool Visited();

	//Return cell width
	static float GetWidth();
};

