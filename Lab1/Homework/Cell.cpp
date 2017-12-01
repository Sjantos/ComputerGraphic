#include "Cell.h"

const float Cell::width = 10.0;

Cell::Cell()
{
	x = y = 0.0;
	visited = false;
	index = 0;
	top = bottom = left = right = true;
}

Cell::Cell(float x1, float y1, int i)
{
	x = x1;
	y = y1;
	visited = false;
	index = i;
	top = bottom = left = right = true;
}


Cell::~Cell()
{
}

void Cell::DeleteBottom()
{
	visited = true;
	bottom = false;
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x, y - width);
	glVertex2f(x + width, y - width);
	glEnd();
}

void Cell::DeleteLeft()
{
	visited = true;
	left = false;
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x, y - width);
	glEnd();
}

void Cell::DeleteRight()
{
	visited = true;
	right = false;
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y - width);
	glEnd();
}

void Cell::DeleteTop()
{
	visited = true;
	top = false;
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glEnd();
}

int Cell::GetIndex()
{
	return index;
}


void Cell::Render()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(4);
	if (top)
	{
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(x + width, y);
		glEnd();
	}
	if (bottom)
	{
		glBegin(GL_LINES);
		glVertex2f(x, y - width);
		glVertex2f(x + width, y - width);
		glEnd();
	}
	if (left)
	{
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(x, y - width);
		glEnd();
	}
	if (right)
	{
		glBegin(GL_LINES);
		glVertex2f(x + width, y);
		glVertex2f(x + width, y - width);
		glEnd();
	}
}

void Cell::SetBottom()
{
	bottom = true;
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x, y - width);
	glVertex2f(x + width, y - width);
	glEnd();
}

void Cell::SetLeft()
{
	left = true;
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x, y - width);
	glEnd();
}

void Cell::SetRight()
{
	right = true;
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y - width);
	glEnd();
}

void Cell::SetTop()
{
	top = true;
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glEnd();
}

bool Cell::Visited()
{
	return visited;
}

float Cell::GetWidth()
{
	return width;
}