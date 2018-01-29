#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

GLfloat deform = 5;
GLfloat width = 300;
GLint level = 3;

//Sets random color for next vertice
void randColor()
{
	glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
}

void fractal(GLfloat x, GLfloat y, GLfloat width, GLint level)
{
	if (level > 0)
	{
		//recursive calls
		width /= 3;
		//Top row
		fractal(x, y, width, level - 1);
		fractal(x, y - width, width, level - 1);
		fractal(x, y - 2 * width, width, level - 1);

		//Middle row
		fractal(x - width, y, width, level - 1);
		fractal(x - width, y - 2 * width, width, level - 1);

		//Bottom row
		fractal(x - 2 * width, y, width, level - 1);
		fractal(x - 2 * width, y - width, width, level - 1);
		fractal(x - 2 * width, y - 2 * width, width, level - 1);
	}
	else
	{
		//Draw basic square
		GLfloat r = ((double)rand() / RAND_MAX) * deform;
		glBegin(GL_POLYGON);
		randColor();
		glVertex2f(x + r, y + r);
		randColor();
		glVertex2f(x + r - width, y + r);
		randColor();
		glVertex2f(x + r - width, y + r - width);
		randColor();
		glVertex2f(x + r, y + r - width);
		glEnd();
	}
}

//Called whenever image must be rendered
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	fractal(width / 2, width / 2, width, level);
	glFlush();
}

//Initialize OpenGL window
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//Recalculates images to have correct proportion after window resizing
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;

	if (vertical == 0)
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	if (horizontal <= vertical)
		glOrtho(-width/2, width/2, -width / 2*AspectRatio, width / 2*AspectRatio, 1.0, -1.0);
	else
		glOrtho(-width*AspectRatio/2, width*AspectRatio/2, -width/2, width/2, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();
}

void main(int argc, char* argv[])
{
	std::cout << "Podaj stopien rozjechania (im wiecej tym gorzej)\n";
	std::cin >> deform;
	std::cout << "Podaj stopien fraktala (im wiecej tym drobniej)\n";
	std::cin >> level;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

	glutCreateWindow("OpenGL Lab1");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);

	MyInit();

	glutMainLoop();
}