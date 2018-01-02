/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi  
//  uk³adu wspó³rzêdnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846
#define DIVISIONS 20

GLdouble upY = 1.0;
static GLfloat R = 20.0;
static GLfloat R1 = 10.0;
static GLfloat R2 = 10.0;
static GLfloat azymut = 0.0;   // wokol Y
static GLfloat elewacja = 0.0;
static GLfloat azymut1 = 0.0;   // wokol Y
static GLfloat elewacja1 = 0.0;
static GLfloat azymut2 = 0.0;   // wokol Y
static GLfloat elewacja2 = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naciœniêto ¿adnego klawisza
							   // 1 - naciœniêty zostaæ lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int y_scale_old = 0;
static int y_scale_new = 1;

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
static int delta_y = 0;

static int x_pos_old1 = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old1 = 0;
static int y_scale_old1 = 0;
static int y_scale_new1 = 1;

static int delta_x1 = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
static int delta_y1 = 0;

static int x_pos_old2 = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old2 = 0;
static int y_scale_old2 = 0;
static int y_scale_new2 = 1;

static int delta_x2 = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
static int delta_y2 = 0;


static float delta_scale = 1;
// i poprzedni¹ kursora myszy 
typedef float point3[3];
point3** eggTable;
point3** eggNormals;
static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
static GLfloat light1[] = { 3.0, 0.0, 10.0 };
static GLfloat light2[] = { -3.0, 0.0, 10.0 };
float YScale = 1.0;
bool tablesInitialized = false;
bool model = true;



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
	return M_PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45*u)*sin(M_PI*v);
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
	return -1*M_PI*(90 * pow(u, 5) - 225 * pow(u, 4) + 270 * pow(u, 3) - 180 * pow(u, 2) + 45 * u)*cos(M_PI*v);
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

void initializeTables(int numberOfDivides)
{
	float maxX = 0.0, maxY = 0.0;
	float interval = 1.0 / (float)(numberOfDivides - 1);
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

			
				/*eggNormals[i][j][0] = normX(u, v);
				eggNormals[i][j][1] = normY(u, v);
				eggNormals[i][j][2] = normZ(u, v);*/
			
			//printf("%f  %f  %f\n", normX(u, v), normY(u, v), normZ(u, v));
			if (eggTable[i][j][0] > maxX)
				maxX = eggTable[i][j][0];
			if (eggTable[i][j][1] > maxY)
				maxY = eggTable[i][j][1];
		}
	}
	//printf("%f  %f", maxX, maxY);
}

void drawSolidEgg(int numberOfDivides)
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.5, 0.5, 0.5);
	float interval = 1.0 / (float)(numberOfDivides - 1);

	for (int i = 0; i < DIVISIONS - 1; i++) {
		for (int j = 0; j < DIVISIONS - 1; j++) {
			if (i < (DIVISIONS - 1) / 2) {
				//glTexCoord2f((double)(i + 1)*interval, (double)j*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);

				//glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);

				//glTexCoord2f((double)(i)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i][j][0], eggNormals[i][j][1], eggNormals[i][j][2]);
				glVertex3f(eggTable[i][j][0], eggTable[i][j][1], eggTable[i][j][2]);

				//glTexCoord2f((double)(i + 1)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);

				//glTexCoord2f((double)(i + 1)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i + 1][j + 1][0], eggNormals[i + 1][j + 1][1], eggNormals[i + 1][j + 1][2]);
				glVertex3f(eggTable[i + 1][j + 1][0], eggTable[i + 1][j + 1][1], eggTable[i + 1][j + 1][2]);

				//glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);

			}
			else {
				//glTexCoord2f((double)(i)*interval, (double)j*interval);
				glNormal3f(eggNormals[i][j][0], eggNormals[i][j][1], eggNormals[i][j][2]);
				glVertex3f(eggTable[i][j][0], eggTable[i][j][1], eggTable[i][j][2]);

				//glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);

				//glTexCoord2f((double)(i + 1)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);

				//glTexCoord2f((double)(i)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i][j + 1][0], eggNormals[i][j + 1][1], eggNormals[i][j + 1][2]);
				glVertex3f(eggTable[i][j + 1][0], eggTable[i][j + 1][1], eggTable[i][j + 1][2]);

				//glTexCoord2f((double)(i + 1)*interval, (double)(j + 1)*interval);
				glNormal3f(eggNormals[i + 1][j + 1][0], eggNormals[i + 1][j + 1][1], eggNormals[i + 1][j + 1][2]);
				glVertex3f(eggTable[i + 1][j + 1][0], eggTable[i + 1][j + 1][1], eggTable[i + 1][j + 1][2]);

				//glTexCoord2f((double)(i + 1)*interval, (double)(j)*interval);
				glNormal3f(eggNormals[i + 1][j][0], eggNormals[i + 1][j][1], eggNormals[i + 1][j][2]);
				glVertex3f(eggTable[i + 1][j][0], eggTable[i + 1][j][1], eggTable[i + 1][j][2]);
			}

		}
	}

	//for (int a = 1; a < numberOfDivides; a++)
	//	for (int b = 1; b < numberOfDivides; b++)
	//	{
	//		//lewy dolny trojkat
	//			glNormal3fv(eggNormals[a][b]);
	//		glVertex3fv(eggTable[a][b]);
	//			glNormal3fv(eggNormals[a-1][b-1]);
	//		glVertex3fv(eggTable[a - 1][b - 1]);
	//			glNormal3fv(eggNormals[a-1][b]);
	//		glVertex3fv(eggTable[a - 1][b]);
	//		//prawy gorny trojkat
	//			glNormal3fv(eggNormals[a][b]);
	//		glVertex3fv(eggTable[a][b]);
	//			glNormal3fv(eggNormals[a-1][b-1]);
	//		glVertex3fv(eggTable[a - 1][b - 1]);
	//			glNormal3fv(eggNormals[a][b-1]);
	//		glVertex3fv(eggTable[a][b - 1]);
	//	}
	glEnd();

	glBegin(GL_LINES);
	for (int i = 0; i < numberOfDivides; i++)
	{
		for (int j = 0; j < numberOfDivides; j++)
		{
			glColor3f(1.0, 0.0, 0.0);
			glVertex3fv(eggTable[i][j]);
			glVertex3f(eggTable[i][j][0] +  eggNormals[i][j][0], eggTable[i][j][1] + eggNormals[i][j][1], eggTable[i][j][2] + eggNormals[i][j][2]);
		}
	}
	glEnd();
}

void egg(int numberOfDivides)
{
	if (!tablesInitialized)
	{
		initializeTables(numberOfDivides);
		tablesInitialized = true;
	}
	drawSolidEgg(numberOfDivides);
}


void Mouse(int btn, int state, int x, int y)
{
	status = 0;          // nie zosta³ wciêniêty ¿aden klawisz 

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old1 = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		y_pos_old1 = y;
		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old2 = x;         // przypisanie aktualnie odczytanej pozycji kursora 
								// jako pozycji poprzedniej
		y_pos_old2 = y;
		status = 2;
	}
}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{
	if (model)
	{
		if (status == 1)
		{
			delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy

			x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

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
		if (status == 1)
		{
			delta_x1 = x - x_pos_old1;     // obliczenie ró¿nicy po³o¿enia kursora myszy

			x_pos_old1 = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

			delta_y1 = y - y_pos_old1;

			y_pos_old1 = y;




			/*************************************************************************************/
			// Definicja Ÿród³a œwiat³a

			GLfloat light_position[] = { 6.0, 6.0, 10.0, 1.0 };

			GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 }; //zerowac na laboratorium (otoczenie swieci na jajko)
			GLfloat light_diffuse[] = { 0.9, 0.9, 0.0, 1.0 }; //swiatlo rozproszone
			GLfloat light_specular[] = { 1.0, 1.0, 0.0, 1.0 }; //swiatlo odbite
			GLfloat att_constant = { 1.0 };
			GLfloat att_linear = { (float)0.05 };
			GLfloat att_quadratic = { (float)0.001 };
			/*************************************************************************************/
			// Ustawienie parametrów Ÿród³a
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
			delta_x2 = x - x_pos_old2;     // obliczenie ró¿nicy po³o¿enia kursora myszy

			x_pos_old2 = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

			delta_y2 = y - y_pos_old2;

			y_pos_old2 = y;




			GLfloat light_position1[] = { -3.0, -3.0, 10.0, 1.0 };

			GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 }; //zerowac na laboratorium (otoczenie swieci na jajko)
			GLfloat light_diffuse1[] = { 0.9, 0.0, 0.0, 1.0 }; //swiatlo rozproszone
			GLfloat light_specular1[] = { 1.0, 0.0, 0.0, 1.0 }; //swiatlo odbite
			GLfloat att_constant1 = { 1.0 };
			GLfloat att_linear1 = { (float)0.05 };
			GLfloat att_quadratic1 = { (float)0.001 };
			/*************************************************************************************/
			// Ustawienie parametrów Ÿród³a

			glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
			glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
			glLightfv(GL_LIGHT1, GL_POSITION, light2);

			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant1);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear1);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic1);





		}
	}




	
	glutPostRedisplay();     // przerysowanie obrazu sceny
}

/*************************************************************************************/

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  pocz?tek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}
/*************************************************************************************/
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana, gdy trzeba 
// przerysowaæ scenê)

void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], -viewer[2], 0.0, 0.0, 0.0, 0.0, upY, 0.0);
	// Zdefiniowanie po³o¿enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej
	glTranslatef(light1[0], light1[1], light1[2]);
	glutWireCube(0.5);
	glTranslatef(-1 * light1[0], -1 * light1[1], -1*light1[2]);
	glTranslatef(light2[0], light2[1], light2[2]);
	glutWireCube(0.5);
	glTranslatef(-1 * light2[0], -1 * light2[1], -1 * light2[2]);
	//glTranslatef(0.0, 0.0, 0.0);

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


	if (status == 1 && !model)                     // jeœli lewy klawisz myszy wciêniêty
	{
		azymut1 += delta_x1*pix2angle / 100;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		elewacja1 += delta_y1*pix2angle / 100;
	}                                  // do ró¿nicy po³o¿eñ kursora myszy
	if (status == 2 && !model)
	{
		azymut2 += delta_x2*pix2angle / 100;    // modyfikacja k¹ta obrotu o kat proporcjonalny
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
	printf("%f  %f  %f\n", light1[0], light1[1], light1[2]);

	light2[0] = R2*cos(azymut2)*cos(elewacja2);
	light2[1] = R2*sin(elewacja2);
	light2[2] = R2*sin(azymut2)*cos(elewacja2);

	// Ustawienie koloru rysowania na bia³y
	//glScalef(0.2, 0.2, 0.2);
	//glutSolidTeapot(4);
	//glTranslatef(0.0, -5.0, 0.0);
	egg(DIVISIONS);
	//glutWireTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
	glutSwapBuffers();

}
/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania

void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = true;
	if (key == 'l') model = false;

	RenderScene(); // przerysowanie obrazu sceny
}


void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny


	// Definicja materia³u z jakiego zrobiony jest czajnik 

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego                

	GLfloat mat_shininess = { 20.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni

	/*************************************************************************************/
	// Definicja Ÿród³a œwiat³a

	GLfloat light_position[] = { 0.0, 0.0, 30.0, 1.0 };
	// po³o¿enie Ÿród³a

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 }; //zerowac na laboratorium (otoczenie swieci na jajko)
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 0.0, 0.0, 0.0, 1.0 }; //swiatlo rozproszone
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 0.0, 1.0 }; //swiatlo odbite
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLfloat att_linear = { (float)0.05 };
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLfloat att_quadratic = { (float)0.001 };
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	/*************************************************************************************/
	// Ustawienie parametrów materia³u i Ÿród³a œwiat³a
	/*************************************************************************************/
	// Ustawienie patrametrów materia³u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light1);// light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light1);// light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny 





	/*************************************************************************************/
	// Definicja Ÿród³a œwiat³a 2

	GLfloat light_position1[] = { 10.0, 0.0, 30.0, 1.0 };

	GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 }; //zerowac na laboratorium (otoczenie swieci na jajko)
	GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 }; //swiatlo rozproszone
	GLfloat light_specular1[] = { 1.0, 0.0, 0.0, 1.0 }; //swiatlo odbite
	GLfloat att_constant1 = { 1.0 };
	GLfloat att_linear1 = { (float)0.05 };
	GLfloat att_quadratic1 = { (float)0.001 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a

	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light2);// light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant1);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear1);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic1);






	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny 
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora 
}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej 

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej 

}
/*************************************************************************************/
// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void main(/*int argc, char* argv[]*/)
{
	//glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);



	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutKeyboardFunc(keys);

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// za zmiany rozmiaru okna                       

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania niewidocznych elementów sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/