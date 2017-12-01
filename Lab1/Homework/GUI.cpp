#include "GUI.h"

Maze *GUI::m = new Maze(25, 25);

void GUI::MyInit(void)
{
	m->MyInit();
}

void GUI::RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	m->RenderScene();

	glFlush();
}

void GUI::ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	m->ChangeSize(horizontal, vertical);
}

GUI::GUI()
{
	
}


GUI::~GUI()
{
	delete m;
}

void GUI::Run()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1920, 1000);
	glutCreateWindow("Lab1 Homework");
	glutDisplayFunc(GUI::RenderScene);
	glutReshapeFunc(GUI::ChangeSize);
	MyInit();
	glutMainLoop();
}
