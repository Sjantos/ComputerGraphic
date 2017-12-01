# include "Maze.h"
//#include "GUI.h"
#include <iostream>
#include <time.h>

using namespace std;
int width, height;
int mazeWidth, mazeHeight;
Maze *m;

void MyInit(void)
{
	m->MyInit();
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	m->RenderScene();

	glFlush();
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	m->ChangeSize(horizontal, vertical);
}

void main(int argc, char* argv[])
{
	int tmp;
	std::cout << "Podaj poczatkowa szerokosc i wysokosc okna" << endl;
	std::cin >> width >> height;
	if (width < height)
	{
		tmp = width;
		width = height;
		height = tmp;
	}
	std::cout << "Podaj szerokosc i wysokosc labiryntu (lepiej nie przekraczac 192x100)" << endl;
	std::cin >> mazeWidth >> mazeHeight;
	if (mazeWidth < mazeHeight)
	{
		tmp = mazeWidth;
		mazeWidth = mazeHeight;
		mazeHeight = tmp;
	}
	m = new Maze(mazeWidth, mazeHeight);
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Lab1 Homework");
	glutDisplayFunc((RenderScene));
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMainLoop();
}