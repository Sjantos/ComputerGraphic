#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

#define M_PI 3.14159265358979323846

GLdouble upY = 1.0;
static GLfloat R = 2.0;
static GLfloat azymut = 0.0;
static GLfloat elewacja = 0.0;
static GLfloat pix2angle;

//mouse key status
static GLint status = 0;

static int x_pos_old = 0;
static int y_pos_old = 0;
static int y_scale_old = 0;
static int y_scale_new = 0;

static int delta_x = 0;
static int delta_y = 0;
static float delta_scale = 1;

typedef float point3[3];
point3** pointTable;
static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
int WeierstrassSum = 100;
int WeierstrassParameterA = 2;
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

//Prepares table of points to draw
void initializeTables(int numberOfDivides)
{
	float interval = XRange / (float)(numberOfDivides - 1);
	float parameter = 1.0 / (float)(numberOfDivides - 1);
	pointTable = new point3*[numberOfDivides];
	int half = numberOfDivides / 2;
	for (int i = 0; i < numberOfDivides; i++)
	{
		float x = i*interval - half*interval;
		pointTable[i] = new point3[numberOfDivides];
		for (int j = 0; j < numberOfDivides; j++)
		{
			float y = j*interval - half*interval;
			pointTable[i][j][0] = x*sqrt(x*x + y*y - x*x*y*y) / sqrt(x*x + y*y);
			pointTable[i][j][2] = y*sqrt(x*x + y*y - x*x*y*y) / sqrt(x*x + y*y);

			float xVal = (1 + abs(pointTable[i][j][0])) / 2;
			float zVal = (1 + abs(pointTable[i][j][2])) / 2;

			float p = sqrt(xVal*zVal + xVal*zVal);
			float value = f(p, WeierstrassParameterA);
			//randomize a little to make mountain not so perfect
			value = sqrt((rand() / RAND_MAX + 0.5)*value);
			pointTable[i][j][1] = value;
		}
	}
}

//Draws points from mountain table
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
			glVertex3fv(pointTable[a][b - 1]);
			glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			glVertex3fv(pointTable[a][b]);

			glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			glVertex3fv(pointTable[a][b]);
			glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
			glVertex3fv(pointTable[a - 1][b]);

			glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
			glVertex3fv(pointTable[a - 1][b]);
			glColor3d(pointTable[a][b - 1][1], pointTable[a][b - 1][1], pointTable[a][b - 1][1]);
			glVertex3fv(pointTable[a][b - 1]);
		}
	glEnd();
}

//Draws mountain (ready to use in RenderScene)
void mountain(int numberOfDivides)
{
	if (!tableInitialized)
	{
		initializeTables(numberOfDivides);
		tableInitialized = true;
	}

	drawMountain(numberOfDivides);
}

//Gets mouse position and prepare them for use
void Mouse(int btn, int state, int x, int y)
{
	status = 0;

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;
		y_pos_old = y;
		status = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y_scale_old = y;
		status = 2;
	}
}

//Calculates move of mouse
void Motion(GLsizei x, GLsizei y)
{
	if (status == 1)
	{
		delta_x = x - x_pos_old;
		x_pos_old = x;

		delta_y = y - y_pos_old;
		y_pos_old = y;
	}

	y_scale_old = y_scale_new;
	y_scale_new = y;
	if (status == 2)
	{
		if (y_scale_new != 0 && y_scale_old != 0)
		{
			float dif = (float)y_scale_new - (float)y_scale_old;
			float add = dif/100;
			printf("%f\n", add);
			if (R+add >XRange/1.1 && R+add < 3*XRange)
				R += add;
		}
	}

	//redraw scene
	glutPostRedisplay();
}

//Draw X, Y, Z axis
void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	
	//Red X axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	//Green Y axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	//Blue Z axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

//Called whenever image must be rendered
void RenderScene(void)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, upY, 0.0);
	Axes();

	if (status == 1)
	{
		azymut += delta_x*pix2angle/100;
		elewacja += delta_y*pix2angle/100;
	}
	//Check for angles domain
	if (azymut > 2 * M_PI)
		azymut -= 2 * M_PI;
	if (elewacja > 2 * M_PI)
		elewacja -= 2 * M_PI;
	if (azymut < 0)
		azymut += 2 * M_PI;
	if (elewacja < 0)
		elewacja += 2 * M_PI;

	if (elewacja > M_PI / 2 && elewacja < 3 * M_PI / 2)
		upY = -1;
	else
		upY = 1;

	//calculate camera position
	viewer[0] = R*cos(azymut)*cos(elewacja);
	viewer[1] = R*sin(elewacja);
	viewer[2] = R*sin(azymut)*cos(elewacja);

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
	pix2angle = 360.0 / (float)horizontal;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.0, 1.0, 30.0);

	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);

	glutCreateWindow("OpenGL Lab3 zad2");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);

	glutMouseFunc(Mouse);

	glutMotionFunc(Motion);

	MyInit();
	
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}