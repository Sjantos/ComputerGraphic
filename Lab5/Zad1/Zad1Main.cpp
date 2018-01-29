#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846
#define DIVISIONS 256

GLdouble upY = 1.0;
//camera variables
static GLfloat R = 20.0;
static GLfloat azymut = 0.0;
static GLfloat elewacja = 0.0;

static int x_pos_old = 0;
static int y_pos_old = 0;
static int y_scale_old = 0;
static int y_scale_new = 1;
static int delta_x = 0;
static int delta_y = 0;
static float delta_scale = 1;

//light 1 variables
static GLfloat R1 = 10.0;
static GLfloat azymut1 = 0.0;
static GLfloat elewacja1 = 0.0;

static int x_pos_old1 = 0;
static int y_pos_old1 = 0;
static int y_scale_old1 = 0;
static int y_scale_new1 = 1;
static int delta_x1 = 0;
static int delta_y1 = 0;

//light 2 variables
static GLfloat R2 = 10.0;
static GLfloat azymut2 = 0.0;
static GLfloat elewacja2 = 0.0;

static int x_pos_old2 = 0;
static int y_pos_old2 = 0;
static int y_scale_old2 = 0;
static int y_scale_new2 = 1;
static int delta_x2 = 0;
static int delta_y2 = 0;

static GLfloat pix2angle;
static GLint status = 0;

typedef float point3[3];
point3** eggTable;
point3** eggNormals;
static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
static GLfloat light1[] = { 3.0, 0.0, 10.0 };
static GLfloat light2[] = { -3.0, 0.0, 10.0 };
float YScale = 1.0;
bool tablesInitialized = false;
bool model = true;

//Functions used in calculating normals
float x(float u, float v)
{
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI*v);
}

float y(float u, float v)
{
	return (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
}

float z(float u, float v)
{
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI*v);
}

float xu(float u, float v)
{
	return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*cos(M_PI*v);
}

float xv(float u, float v)
{
	return M_PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*sin(M_PI*v);
}

float yu(float u, float v)
{
	return (640 * pow(u, 3) - 960 * pow(u, 2) + 320 * u);
}

float yv(float u, float v)
{
	return 0.0;
}

float zu(float u, float v)
{
	return (-450 * pow(u, 4) + 900 * pow(u, 3) - 810 * pow(u, 2) + 360 * u - 45)*sin(M_PI*v);
}

float zv(float u, float v)
{
	return -1 * M_PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(M_PI*v);
}

float normalX(float u, float v)
{
	return (yu(u, v)*zv(u, v) - zu(u, v)*yv(u, v));
}

float normalY(float u, float v)
{
	return (zu(u, v)*xv(u, v) - xu(u, v)*zv(u, v));
}

float normalZ(float u, float v)
{
	return (xu(u, v)*yv(u, v) - yu(u, v)*xv(u, v));
}

float normLength(float u, float v)
{
	return sqrt(normalX(u, v)*normalX(u, v) + normalY(u, v)*normalY(u, v) + normalZ(u, v)*normalZ(u, v));
}

float normX(float u, float v)
{
	return normalX(u, v) / normLength(u, v);
}

float normY(float u, float v)
{
	return normalY(u, v) / normLength(u, v);
}

float normZ(float u, float v)
{
	return normalZ(u, v) / normLength(u, v);
}

//Reads .tga texture and put it into right structure
GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
#pragma pack(1)            
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);
	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;
	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}
	switch (sDepth)
	{
	case 3:
		*ImFormat = GL_BGR_EXT;
		*ImComponents = GL_RGB8;
		break;
	case 4:
		*ImFormat = GL_BGRA_EXT;
		*ImComponents = GL_RGBA8;
		break;
	case 1:
		*ImFormat = GL_LUMINANCE;
		*ImComponents = GL_LUMINANCE8;
		break;
	};
	fclose(pFile);
	return pbitsperpixel;
}

//Prepare table of points to render
void initializeTables(int numberOfDivides)
{
	float maxX = 0.0, maxY = 0.0;
	float interval = 1.0 / (float)(numberOfDivides-1);
	eggTable = new point3*[numberOfDivides];
	eggNormals = new point3*[numberOfDivides];
	for (int i = 0; i < numberOfDivides; i++)
	{
		eggTable[i] = new point3[numberOfDivides];
		eggNormals[i] = new point3[numberOfDivides];
		for (int j = 0; j < numberOfDivides; j++)
		{
			float u = (float)i*interval, v = (float)j*interval;
			eggTable[i][j][0] = x(u, v);
			eggTable[i][j][1] = y(u, v);
			eggTable[i][j][2] = z(u, v);
			if (eggTable[i][j][0] == 0.0f && eggTable[i][j][2] == 0.0f)
			{
				if (eggTable[i][j][1] > 0)
				{
					eggNormals[i][j][0] = 0.0;
					eggNormals[i][j][1] = 1.0;
					eggNormals[i][j][2] = 0.0;
				}
				else
				{
					eggNormals[i][j][0] = 0.0;
					eggNormals[i][j][1] = -1.0;
					eggNormals[i][j][2] = 0.0;
				}
			}
			else
			{
				if (i >= numberOfDivides / 2)
				{
					eggNormals[i][j][0] = -1 * normX(u, v);
					eggNormals[i][j][1] = -1 * normY(u, v);
					eggNormals[i][j][2] = -1 * normZ(u, v);
				}
				else
				{
					eggNormals[i][j][0] = normX(u, v);
					eggNormals[i][j][1] = normY(u, v);
					eggNormals[i][j][2] = normZ(u, v);
				}
			}
			if (eggTable[i][j][0] > maxX)
				maxX = eggTable[i][j][0];
			if (eggTable[i][j][1] > maxY)
				maxY = eggTable[i][j][1];
		}
	}
}

//Draw polygons, takes care of vertices order
//CullFace not used due to bugs
void drawSolidEgg(int numberOfDivides)
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 0.5);
	float interval = 1.0 / (float)(numberOfDivides - 1);

	for (int i = 0; i < DIVISIONS - 1; i++) {
		for (int j = 0; j < DIVISIONS - 1; j++) {
			//if (i < (DIVISIONS / 2)) 
			//{
				glTexCoord2f((double)(i + 1)*interval, (double)j*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);

				glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);

				glTexCoord2f((double)(i)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i][j][0], eggNormals[i][j][1], eggNormals[i][j][2]);
				glVertex3f(eggTable[i][j][0], eggTable[i][j][1], eggTable[i][j][2]);

				glTexCoord2f((double)(i + 1)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);

				glTexCoord2f((double)(i + 1)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i + 1][j + 1][0], eggNormals[i + 1][j + 1][1], eggNormals[i + 1][j + 1][2]);
				glVertex3f(eggTable[i + 1][j + 1][0], eggTable[i + 1][j + 1][1], eggTable[i + 1][j + 1][2]);

				glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);
			//}
			//else 
			//{
				glTexCoord2f((double)(i)*interval, (double)j*interval);
				glNormal3f(eggNormals[i][j][0], eggNormals[i][j][1], eggNormals[i][j][2]);
				glVertex3f(eggTable[i][j][0], eggTable[i][j][1], eggTable[i][j][2]);

				glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);

				glTexCoord2f((double)(i + 1)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);

				glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);

				glTexCoord2f((double)(i + 1)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i + 1][j + 1][0], eggNormals[i + 1][j + 1][1], eggNormals[i + 1][j + 1][2]);
				glVertex3f(eggTable[i + 1][j + 1][0], eggTable[i + 1][j + 1][1], eggTable[i + 1][j + 1][2]);

				glTexCoord2f((double)(i + 1)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);
			//}

		}
	}
	glEnd();
}

//Draw egg model on screen (ready to use in RenderScene)
void egg(int numberOfDivides)
{
	if (!tablesInitialized)
	{
		initializeTables(numberOfDivides);
		tablesInitialized = true;
	}
	drawSolidEgg(numberOfDivides);
}

//Capture mouse clicks and movement, prepare some data
void Mouse(int btn, int state, int x, int y)
{
	status = 0;

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old1 = x;
		y_pos_old1 = y;
		status = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old2 = x;
		y_pos_old2 = y;
		status = 2;
	}
}

//Calculates things after mouse move
void Motion(GLsizei x, GLsizei y)
{
	if (model)
	{
		//camera
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
			if (y_scale_new != 0 && y_scale_old != 0)
			{
				float dif = (float)y_scale_new - (float)y_scale_old;
				float add = dif / 250;
				if (abs(add) < 0.5)
				{
					R += add;
				}
			}
		}
	}
	else
	{
		//lights
		if (status == 1)
		{
			delta_x1 = x - x_pos_old1;
			x_pos_old1 = x;
			delta_y1 = y - y_pos_old1;
			y_pos_old1 = y;

			GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
			GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat att_constant = { 1.0 };
			GLfloat att_linear = { (float)0.05 };
			GLfloat att_quadratic = { (float)0.001 };
			
			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light1);

			glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
			glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
			glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
		}

		if (status == 2)
		{
			delta_x2 = x - x_pos_old2;
			x_pos_old2 = x;
			delta_y2 = y - y_pos_old2;
			y_pos_old2 = y;

			GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
			GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
			GLfloat att_constant1 = { 1.0 };
			GLfloat att_linear1 = { (float)0.05 };
			GLfloat att_quadratic1 = { (float)0.001 };
			
			glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
			glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
			glLightfv(GL_LIGHT1, GL_POSITION, light2);

			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant1);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear1);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic1);
		}
	}
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

//Render image to screen
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], -viewer[2], 0.0, 0.0, 0.0, 0.0, upY, 0.0);
	Axes();
	
	//draw small cubes in light position (for check)
	glTranslatef(light1[0], light1[1], light1[2]);
	glutWireCube(0.5);
	glTranslatef(-1 * light1[0], -1 * light1[1], -1 * light1[2]);
	glTranslatef(light2[0], light2[1], light2[2]);
	glutWireCube(0.5);
	glTranslatef(-1 * light2[0], -1 * light2[1], -1 * light2[2]);

	//camera
	if (status == 1 && model)
	{
		azymut += delta_x*pix2angle / 100;
		elewacja += delta_y*pix2angle / 100;
	}
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
	viewer[0] = R*cos(azymut)*cos(elewacja);
	viewer[1] = R*sin(elewacja);
	viewer[2] = R*sin(azymut)*cos(elewacja);

	//lights
	if (status == 1 && !model)
	{
		azymut1 += delta_x1*pix2angle / 100;
		elewacja1 += delta_y1*pix2angle / 100;
	}
	if (status == 2 && !model)
	{
		azymut2 += delta_x2*pix2angle / 100;
		elewacja2 += delta_y2*pix2angle / 100;
	}
	if (azymut1 > 2 * M_PI)
		azymut1 -= 2 * M_PI;
	if (elewacja1 > 2 * M_PI)
		elewacja1 -= 2 * M_PI;
	if (azymut1 < 0)
		azymut1 += 2 * M_PI;
	if (elewacja1 < 0)
		elewacja1 += 2 * M_PI;

	if (azymut2 > 2 * M_PI)
		azymut2 -= 2 * M_PI;
	if (elewacja2 > 2 * M_PI)
		elewacja2 -= 2 * M_PI;
	if (azymut2 < 0)
		azymut2 += 2 * M_PI;
	if (elewacja2 < 0)
		elewacja2 += 2 * M_PI;

	light1[0] = R1*cos(azymut1)*cos(elewacja1);
	light1[1] = R1*sin(elewacja1);
	light1[2] = R1*sin(azymut1)*cos(elewacja1);

	light2[0] = R2*cos(azymut2)*cos(elewacja2);
	light2[1] = R2*sin(elewacja2);
	light2[2] = R2*sin(azymut2)*cos(elewacja2);

	glTranslatef(0.0, -5.0, 0.0);
	egg(DIVISIONS);
	glFlush();
	glutSwapBuffers();

}

//Executed when key pressed
void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = true;
	if (key == 'l') model = false;

	RenderScene();
}

//Initialize OpenGL window
void MyInit(void)
{
	GLbyte *pBytes = nullptr;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;
	glEnable(GL_CULL_FACE);
	try
	{
		//Enter here texture file name to load and use
		pBytes = LoadTGAImage("fur2048.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);
	}
	catch (const std::exception&)
	{
		printf("failed opening file\n");
	}
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);
	free(pBytes);
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 20.0 };

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat att_constant = { 1.0 };
	GLfloat att_linear = { (float)0.05 };
	GLfloat att_quadratic = { (float)0.001 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light1);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat att_constant1 = { 1.0 };
	GLfloat att_linear1 = { (float)0.05 };
	GLfloat att_quadratic1 = { (float)0.001 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light2);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic1);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
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

void main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Lab5 zad1");
	glutDisplayFunc(RenderScene);
	glutKeyboardFunc(keys);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	MyInit();
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}