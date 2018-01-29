#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

#define M_PI 3.14159265358979323846
#define DIVISIONS 100

GLdouble upY = 1.0;
static GLfloat R = 3.0;
static GLfloat azymut = 0.0;
static GLfloat elewacja = 0.0;
static GLfloat pix2angle;

//Mouse key status
static GLint status = 0;

static int x_pos_old = 0;
static int y_pos_old = 0;
static int y_scale_old = 0;
static int y_scale_new = 1;

static int delta_x = 0;
static int delta_y = 0;
static float delta_scale = 1;

typedef float point3[3];
point3** pointTable;
point3** calculateTable;
static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
int WeierstrassSum = 100;
int WeierstrassParameterA = 2;
bool tableInitialized = false;
float XRange = 1.0;
float YScale = 1.0;

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

//Prepare table of points
void initializeTables(int numberOfDivides)
{
	float range = 2.0 * XRange;
	float interval = range / (float)(numberOfDivides - 1);
	
	int half = numberOfDivides / 2;
	for (int i = 0; i < numberOfDivides; i++)
	{
		float x = i*interval - half*interval;

		for (int j = 0; j < numberOfDivides; j++)
		{
			float z = j*interval - half*interval;
			calculateTable[i][j][0] = x;
			calculateTable[i][j][2] = z;

			float absX = calculateTable[i][j][0] > 0 ? calculateTable[i][j][0] : -1 * calculateTable[i][j][0];
			float absZ = calculateTable[i][j][2] > 0 ? calculateTable[i][j][2] : -1 * calculateTable[i][j][2];

			float xVal = (1 + abs(x*sqrt(x*x + z*z - x*x*z*z) / sqrt(x*x + z*z))) / 2;
			float zVal = (1 + abs(z*sqrt(x*x + z*z - x*x*z*z) / sqrt(x*x + z*z))) / 2;

			float p = sqrt(xVal*zVal + xVal*zVal);
			float value = f(calculateTable[i][j][0]*calculateTable[i][j][2], WeierstrassParameterA);

			calculateTable[i][j][1] = value;
		}
	}
	pointTable = calculateTable;
	tableInitialized = true;

	//prepare for some scaling
	float maxABS = 0.0;
	float max = 0.0;
	for (int i = 0; i < numberOfDivides; i++)
		for (int j = 0; j < numberOfDivides; j++)
			if (abs(calculateTable[i][j][1]) > maxABS)
				maxABS = abs(calculateTable[i][j][1]);
	for (int i = 0; i < numberOfDivides; i++)
		for (int j = 0; j < numberOfDivides; j++)
		{
			calculateTable[i][j][1] += maxABS;
			if (calculateTable[i][j][1] > max) max = calculateTable[i][j][1];
		}
	YScale = max;
}

//Draw mountain points from table
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

//Draw mountain (ready to use in RenderScene)
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
	
	if (status == 2)
	{
		y_scale_old = y_scale_new;
		y_scale_new = y;
		tableInitialized = false;
		if (y_scale_new != 0 && y_scale_old != 0)
		{
			float dif = (float)y_scale_new - (float)y_scale_old;
			float add = dif / 250;
			if (abs(add) < 0.5)
			{
				XRange += add;
			}
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
		azymut += delta_x*pix2angle / 100;
		elewacja += delta_y*pix2angle / 100;
	}
	//check for angles domain
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

	//camera position
	viewer[0] = R*cos(azymut)*cos(elewacja);
	viewer[1] = R*sin(elewacja);
	viewer[2] = R*sin(azymut)*cos(elewacja);
	{
		double scale = 1.0 / (double)calculateTable[DIVISIONS-1][DIVISIONS-1][0];
		glScaled(scale, scale, scale);
	}
	mountain(DIVISIONS);
	
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
	calculateTable = new point3*[DIVISIONS];
	for (int i = 0; i < DIVISIONS; i++)
	{
		calculateTable[i] = new point3[DIVISIONS];
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Lab3 homework");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}