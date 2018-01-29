#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

static GLfloat thetaX = 0.0;
static GLfloat thetaY = 0.0;
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
//viewer position
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };

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
		if (y_scale_new != 0 && y_scale_old != 0)
		{
			float dif = (float)y_scale_new / (float)y_scale_old;
			float add = dif - 1.0;
			delta_scale += add;
		}

		y_scale_old = y_scale_new;
		y_scale_new = y;
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
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	Axes();

	//if key down, prepare object rotation angles
	if (status == 1)
	{
		thetaY += delta_x*pix2angle;
		thetaX += delta_y*pix2angle;
	}

	glRotatef(thetaY, 0.0, 1.0, 0.0);
	glRotatef(thetaX, 1.0, 0.0, 0.0);
	glScalef(delta_scale, delta_scale, delta_scale);

	glColor3f(1.0f, 1.0f, 1.0f);
	glutWireTeapot(3.0);
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

void main(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Lab3 zad1");

	glutDisplayFunc(RenderScene);

	glutReshapeFunc(ChangeSize);

	glutMouseFunc(Mouse);

	glutMotionFunc(Motion);

	MyInit();
	
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}