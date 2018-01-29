#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <time.h>
#include <iostream>

#define M_PI 3.14159265358979323846

typedef double point3[3];
point3** pointTable;
int WeierstrassSum = 100;
int WeierstrassParameterA = 4;
bool tableInitialized = false;
float XRange = 2.0;

//Weierstrass function
double f(double x, int a)
{
	double returnValue = 0.0;
	for (int k = 1; k <= WeierstrassSum; k++)
	{
		returnValue += sin(M_PI*pow(k, a)*x) / (M_PI*pow(k, a));
	}
	return returnValue;
}

//Initialize table of points to draw, calculates proper values
void initializeTables(int numberOfDivides)
{
	float interval =  XRange / (float)(numberOfDivides - 1);
	float parameter = 1.0 / (float)(numberOfDivides - 1/**numberOfDivides*/);
	pointTable = new point3*[numberOfDivides];
	int half = numberOfDivides / 2;
	for (int i = 0; i < numberOfDivides; i++)
	{
		float x = i*interval -half*interval;
		pointTable [i] = new point3[numberOfDivides];
		for (int j = 0; j < numberOfDivides; j++)
		{
			float y = j*interval -half*interval;
			pointTable[i][j][0] = x*sqrt(x*x + y*y - x*x*y*y) / sqrt(x*x + y*y);
			
			pointTable[i][j][2] = y*sqrt(x*x + y*y - x*x*y*y) / sqrt(x*x + y*y);

			float xVal = (1+abs(pointTable[i][j][0]))/2;
			float zVal = (1+abs(pointTable[i][j][2]))/2;

			float p = sqrt(xVal*zVal+xVal*zVal);
			float value = f(p, WeierstrassParameterA);
			//Randomize values a bit, to make mountain not so perfect
			value = sqrt((rand() / RAND_MAX + 0.5)*value);
			pointTable[i][j][1] = value;
		}
	}
}

//Draw mountain from tables of points
void drawMountain(int numberOfDivides)
{
	glPointSize(2.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	float interval = 1.0 / (float)(numberOfDivides - 1);
	float color = 0.0;
	for (int a = 1; a < numberOfDivides; a++)
		for (int b = 1; b < numberOfDivides; b++)
		{
			glColor3d(pointTable[a][b - 1][1], pointTable[a][b - 1][1], pointTable[a][b - 1][1]);
			glVertex3dv(pointTable[a][b - 1]);
			glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			glVertex3dv(pointTable[a][b]);

			glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			glVertex3dv(pointTable[a][b]);
			glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
			glVertex3dv(pointTable[a - 1][b]);

			glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
			glVertex3dv(pointTable[a-1][b]);
			glColor3d(pointTable[a][b - 1][1], pointTable[a][b - 1][1], pointTable[a][b - 1][1]);
			glVertex3dv(pointTable[a][b-1]);
		}
	glEnd();
}

//main function for rendering mountain (to call in RenderScene)
void mountain(int numberOfDivides)
{
	if (!tableInitialized)
	{
		initializeTables(numberOfDivides);
		tableInitialized = true;
	}

	drawMountain(numberOfDivides);
}

//Called whenever image must be rendered
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotated(-30.0f, 1.0f, 0.0f, 0.0f);
	mountain(100);

	glFlush();
	glutSwapBuffers();
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
	if (vertical == 0)  // don't divide by 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	if (horizontal <= vertical)
		glOrtho(-1.0, 1.0, -1.0 / AspectRatio, 1.0 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-1.0*AspectRatio, 1.0*AspectRatio, -1.0, 1.0, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void main(int argc, char* argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);

	glutCreateWindow("OpenGL Lab2 homework");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}