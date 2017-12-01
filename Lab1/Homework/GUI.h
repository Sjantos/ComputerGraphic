#pragma once
#include "Maze.h"
class GUI
{
	static GUI::Maze *m;

	void static MyInit(void);
	void static RenderScene(void);
	void static ChangeSize(GLsizei horizontal, GLsizei vertical);
public:
	GUI();
	~GUI();

	void Run();
};

