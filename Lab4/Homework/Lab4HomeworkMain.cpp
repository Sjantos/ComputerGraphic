///*************************************************************************************/
////  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
////  uk�adu wsp�rz�dnych dla rzutowania perspektywicznego
///*************************************************************************************/
//#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glut.h>
//#include <iostream>
//
//#define M_PI 3.14159265358979323846
//#define DIVISIONS 100
//
//GLdouble upY = 1.0;
//static GLfloat R = 3.0;
//static GLfloat azymut = 0.0;   // wokol Y
//static GLfloat elewacja = 0.0;
//static GLfloat pix2angle;     // przelicznik pikseli na stopnie
//
//static GLint status = 0;       // stan klawiszy myszy 
//							   // 0 - nie naci�ni�to �adnego klawisza
//							   // 1 - naci�ni�ty zosta� lewy klawisz
//
//static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
//static int y_pos_old = 0;
//static int y_scale_old = 0;
//static int y_scale_new = 1;
//
//static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
//static int delta_y = 0;
//static float delta_scale = 1;
//// i poprzedni� kursora myszy 
//typedef float point3[3];
//point3** pointTable;
//point3** calculateTable;
//static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
//int WeierstrassSum = 100;
//int WeierstrassParameterA = 2;
//bool tableInitialized = false;
//float XRange = 1.0;
//float YScale = 1.0;
//// inicjalizacja po�o�enia obserwatora
///*************************************************************************************/
//// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych
//
///*************************************************************************************/
//// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych
//
//
////Weierstrass function
//double f(double x, int a)
//{
//	double returnValue = 0.0;
//	for (int k = 1; k <= WeierstrassSum; k++)
//	{
//		returnValue += sin(M_PI*pow(k, a)*x) / (M_PI*pow(k, a));
//	}
//	return returnValue;
//}
//
//void initializeTables(int numberOfDivides)
//{
//	float range = 2.0 * XRange;
//	float interval = range / (float)(numberOfDivides - 1);
//
//	int half = numberOfDivides / 2;
//	for (int i = 0; i < numberOfDivides; i++)
//	{
//		float x = i*interval - half*interval;
//
//		for (int j = 0; j < numberOfDivides; j++)
//		{
//			float z = j*interval - half*interval;
//			calculateTable[i][j][0] = x;
//			calculateTable[i][j][2] = z;
//
//			float absX = calculateTable[i][j][0] > 0 ? calculateTable[i][j][0] : -1 * calculateTable[i][j][0];
//			float absZ = calculateTable[i][j][2] > 0 ? calculateTable[i][j][2] : -1 * calculateTable[i][j][2];
//
//			float xVal = (1 + abs(x*sqrt(x*x + z*z - x*x*z*z) / sqrt(x*x + z*z))) / 2;
//			float zVal = (1 + abs(z*sqrt(x*x + z*z - x*x*z*z) / sqrt(x*x + z*z))) / 2;
//
//			float p = sqrt(xVal*zVal + xVal*zVal);
//			float value = f(calculateTable[i][j][0] * calculateTable[i][j][2], WeierstrassParameterA);
//
//			calculateTable[i][j][1] = value;
//		}
//	}
//	pointTable = calculateTable;
//	tableInitialized = true;
//
//	float maxABS = 0.0;
//	float max = 0.0;
//	for (int i = 0; i < numberOfDivides; i++)
//		for (int j = 0; j < numberOfDivides; j++)
//			if (abs(calculateTable[i][j][1]) > maxABS)
//				maxABS = abs(calculateTable[i][j][1]);
//	for (int i = 0; i < numberOfDivides; i++)
//		for (int j = 0; j < numberOfDivides; j++)
//		{
//			calculateTable[i][j][1] += maxABS;
//			if (calculateTable[i][j][1] > max) max = calculateTable[i][j][1];
//		}
//	YScale = max;
//}
//
//void drawMountain(int numberOfDivides)
//{
//	glPointSize(2.0);
//	//glBegin(GL_POINTS);
//	glBegin(GL_LINES);
//	glColor3f(1.0, 1.0, 1.0);
//	float interval = 1.0 / (float)(numberOfDivides - 1);
//	float color = 0.0;
//	for (int a = 1; a < numberOfDivides; a++)
//		for (int b = 1; b < numberOfDivides; b++)
//		{
//			//Help if want to view as 2D (as points)
//			//glColor3f(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
//			//glVertex3fv(pointTable[a][b]);
//
//			glColor3d(pointTable[a][b - 1][1], pointTable[a][b - 1][1], pointTable[a][b - 1][1]);
//			glVertex3fv(pointTable[a][b - 1]);
//			glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
//			glVertex3fv(pointTable[a][b]);
//
//			glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
//			glVertex3fv(pointTable[a][b]);
//			glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
//			glVertex3fv(pointTable[a - 1][b]);
//
//			glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
//			glVertex3fv(pointTable[a - 1][b]);
//			glColor3d(pointTable[a][b - 1][1], pointTable[a][b - 1][1], pointTable[a][b - 1][1]);
//			glVertex3fv(pointTable[a][b - 1]);
//		}
//	glEnd();
//}
//
//void mountain(int numberOfDivides)
//{
//	if (!tableInitialized)
//	{
//		initializeTables(numberOfDivides);
//		tableInitialized = true;
//	}
//
//	drawMountain(numberOfDivides);
//}
//
//void Mouse(int btn, int state, int x, int y)
//{
//	status = 0;          // nie zosta� wci�ni�ty �aden klawisz 
//
//	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
//							   // jako pozycji poprzedniej
//		y_pos_old = y;
//		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
//	}
//	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
//	{
//		y_scale_old = y;
//		status = 2;
//	}
//}
//
///*************************************************************************************/
//// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich 
//// zmiennych globalnych
//
//void Motion(GLsizei x, GLsizei y)
//{
//	if (status == 1)
//	{
//		delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy
//
//		x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie
//
//		delta_y = y - y_pos_old;
//
//		y_pos_old = y;
//	}
//
//
//	if (status == 2)
//	{
//		y_scale_old = y_scale_new;
//		y_scale_new = y;
//		tableInitialized = false;
//		if (y_scale_new != 0 && y_scale_old != 0)
//		{
//			float dif = (float)y_scale_new - (float)y_scale_old;
//			float add = dif / 250;
//			if (abs(add) < 0.5)
//			{
//				XRange += add;
//			}
//		}
//	}
//	glutPostRedisplay();     // przerysowanie obrazu sceny
//}
//
///*************************************************************************************/
//
//void Axes(void)
//{
//	point3  x_min = { -5.0, 0.0, 0.0 };
//	point3  x_max = { 5.0, 0.0, 0.0 };
//	// pocz?tek i koniec obrazu osi x
//
//	point3  y_min = { 0.0, -5.0, 0.0 };
//	point3  y_max = { 0.0, 5.0, 0.0 };
//	// pocz?tek i koniec obrazu osi y
//
//	point3  z_min = { 0.0, 0.0, -5.0 };
//	point3  z_max = { 0.0, 0.0, 5.0 };
//	//  pocz?tek i koniec obrazu osi y
//	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
//	glBegin(GL_LINES); // rysowanie osi x
//	glVertex3fv(x_min);
//	glVertex3fv(x_max);
//	glEnd();
//
//	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
//	glBegin(GL_LINES);  // rysowanie osi y
//	glVertex3fv(y_min);
//	glVertex3fv(y_max);
//	glEnd();
//
//	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
//	glBegin(GL_LINES); // rysowanie osi z
//	glVertex3fv(z_min);
//	glVertex3fv(z_max);
//	glEnd();
//}
///*************************************************************************************/
//// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba 
//// przerysowa� scen�)
//
//void RenderScene(void)
//{
//
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	// Czyszczenie okna aktualnym kolorem czyszcz�cym
//
//	glLoadIdentity();
//	// Czyszczenie macierzy bie??cej
//
//	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, upY, 0.0);
//	// Zdefiniowanie po�o�enia obserwatora
//	Axes();
//	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej
//
//	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
//	{
//		azymut += delta_x*pix2angle / 100;    // modyfikacja k�ta obrotu o kat proporcjonalny
//		elewacja += delta_y*pix2angle / 100;
//	}                                  // do r�nicy po�o�e� kursora myszy
//	if (azymut > 2 * M_PI)
//		azymut -= 2 * M_PI;
//	if (elewacja > 2 * M_PI)
//		elewacja -= 2 * M_PI;
//	if (azymut < 0)
//		azymut += 2 * M_PI;
//	if (elewacja < 0)
//		elewacja += 2 * M_PI;
//
//	if (elewacja > M_PI / 2 && elewacja < 3 * M_PI / 2)
//		upY = -1;
//	else
//		upY = 1;
//
//	viewer[0] = R*cos(azymut)*cos(elewacja);
//	viewer[1] = R*sin(elewacja);
//	viewer[2] = R*sin(azymut)*cos(elewacja);
//
//
//	//glColor3f(1.0f, 1.0f, 1.0f);
//
//	//if (tableInitialized)
//	{
//		double scale = 1.0 / (double)calculateTable[DIVISIONS - 1][DIVISIONS - 1][0];
//		glScaled(scale, scale, scale);
//	}
//	// Ustawienie koloru rysowania na bia�y
//	mountain(DIVISIONS);
//	//glutWireTeapot(3.0);
//	// Narysowanie czajnika
//	glFlush();
//	// Przekazanie polece� rysuj�cych do wykonania
//	glutSwapBuffers();
//
//}
///*************************************************************************************/
//// Funkcja ustalaj�ca stan renderowania
//
//void MyInit(void)
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny
//}
///*************************************************************************************/
//
//// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
//// w przypadku zmiany rozmiar�w okna.
//// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
//// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
//
//void ChangeSize(GLsizei horizontal, GLsizei vertical)
//{
//	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie
//
//	glMatrixMode(GL_PROJECTION);
//	// Prze��czenie macierzy bie��cej na macierz projekcji
//
//	glLoadIdentity();
//	// Czyszcznie macierzy bie��cej 
//
//	gluPerspective(70, 1.0, 1.0, 30.0);
//	// Ustawienie parametr�w dla rzutu perspektywicznego
//
//
//	if (horizontal <= vertical)
//		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
//
//	else
//		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
//	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
//	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna
//
//	glMatrixMode(GL_MODELVIEW);
//	// Prze��czenie macierzy bie��cej na macierz widoku modelu  
//
//	glLoadIdentity();
//	// Czyszczenie macierzy bie��cej 
//
//}
///*************************************************************************************/
//// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
//
//void main(int argc, char* argv[])
//{
//	glutInit(&argc, argv);
//	calculateTable = new point3*[DIVISIONS];
//	for (int i = 0; i < DIVISIONS; i++)
//	{
//		calculateTable[i] = new point3[DIVISIONS];
//	}
//
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//
//	glutInitWindowSize(600, 600);
//
//
//
//	glutCreateWindow("Rzutowanie perspektywiczne");
//
//	glutDisplayFunc(RenderScene);
//	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
//	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem 
//	// gdy zajdzie potrzeba przerysowania okna
//
//	glutReshapeFunc(ChangeSize);
//	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
//	// za zmiany rozmiaru okna                       
//
//	glutMouseFunc(Mouse);
//	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy
//
//	glutMotionFunc(Motion);
//	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy
//
//	MyInit();
//	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
//	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
//	glEnable(GL_DEPTH_TEST);
//	// W��czenie mechanizmu usuwania niewidocznych element�w sceny
//
//	glutMainLoop();
//	// Funkcja uruchamia szkielet biblioteki GLUT
//}
///*************************************************************************************/













/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rz�dnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

#define M_PI 3.14159265358979323846
#define DIVISIONS 10

GLdouble upY = 1.0;
static GLfloat R = 3.0;
static GLfloat azymut = 0.0;   // wokol Y
static GLfloat elewacja = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
							   // 0 - nie naci�ni�to �adnego klawisza
							   // 1 - naci�ni�ty zosta� lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int y_scale_old = 0;
static int y_scale_new = 1;

static int delta_x = 0;        // r�nica pomi�dzy pozycj� bie��c�
static int delta_y = 0;
static float delta_scale = 1;
// i poprzedni� kursora myszy 
typedef float point3[3];
point3** pointTable;
point3** normalsTable;
static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
int WeierstrassSum = 100;
int WeierstrassParameterA = 2;
bool tableInitialized = false;
float XRange = 1.0;
float YScale = 1.0;
// inicjalizacja po�o�enia obserwatora
/*************************************************************************************/
// Funkcja rysuj�ca osie uk�adu wsp�?rz?dnych

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia warto�ci odpowiednich zmiennych globalnych


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

void initializeTables(int numberOfDivides)
{
	float range = 2.0 * XRange;
	float interval = range / (float)(numberOfDivides - 1);

	int half = numberOfDivides / 2;
	for (int i = 0; i < numberOfDivides; i++)
	{
		float u = i*interval - half*interval;

		for (int j = 0; j < numberOfDivides; j++)
		{
			float v = j*interval - half*interval;
			pointTable[i][j][0] = u;
			pointTable[i][j][2] = v;
			pointTable[i][j][1] = f(u*v, WeierstrassParameterA);

			//normals
			float dxdu = 1.0;
			float dxdv = 0.0;
			float dydu = 0.0;
			for (int i = 0; i < WeierstrassSum; i++)
				dydu += v*cos(M_PI*u*v*pow(i, WeierstrassParameterA));
			float dydv = 0.0;
			for (int k = 0; k < WeierstrassSum; k++)
				dydv += u*cos(M_PI*u*v*pow(k, WeierstrassParameterA));
			float dzdu = 0.0;
			float dzdv = 1.0;
			float nx = dydu*dzdv - dzdu*dydv;
			float ny = dzdu*dxdv - dxdu*dzdv;
			float nz = dxdu*dydv - dydu*dxdv;
			float nLength = sqrt(nx*nx + ny*ny + nz*nz);
			nx /= nLength;
			ny /= nLength;
			nz /= nLength;
			normalsTable[i][j][0] = nx;
			normalsTable[i][j][1] = ny;
			normalsTable[i][j][2] = nz;
		}
	}
	pointTable = pointTable;
	tableInitialized = true;

	float maxABS = 0.0;
	float max = 0.0;
	for (int i = 0; i < numberOfDivides; i++)
		for (int j = 0; j < numberOfDivides; j++)
			if (abs(pointTable[i][j][1]) > maxABS)
				maxABS = abs(pointTable[i][j][1]);
	for (int i = 0; i < numberOfDivides; i++)
		for (int j = 0; j < numberOfDivides; j++)
		{
			pointTable[i][j][1] += maxABS;
			if (pointTable[i][j][1] > max) max = pointTable[i][j][1];
		}
	YScale = max;
}

void drawMountain(int numberOfDivides)
{
	glPointSize(2.0);
	//glBegin(GL_POINTS);
	//glBegin(GL_LINES);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);
	float interval = 1.0 / (float)(numberOfDivides - 1);
	float color = 0.0;
	for (int a = 1; a < numberOfDivides; a++)
		for (int b = 1; b < numberOfDivides; b++)
		{
			//Help if want to view as 2D (as points)
			//glColor3f(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			//glVertex3fv(pointTable[a][b]);

			//glColor3d(pointTable[a][b - 1][1], pointTable[a][b - 1][1], pointTable[a][b - 1][1]);
			//glVertex3fv(pointTable[a][b - 1]);
			//glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			//glVertex3fv(pointTable[a][b]);

			//glColor3d(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			//glVertex3fv(pointTable[a][b]);
			//glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
			//glVertex3fv(pointTable[a - 1][b]);

			//glColor3d(pointTable[a - 1][b][1], pointTable[a - 1][b][1], pointTable[a - 1][b][1]);
			//glVertex3fv(pointTable[a - 1][b]);
			//glColor3d(pointTable[a][b - 1][1], pointTable[a][b - 1][1], pointTable[a][b - 1][1]);
			//glVertex3fv(pointTable[a][b - 1]);

			//lewy dolny trojkat
			glNormal3fv(normalsTable[a][b]);
			glVertex3fv(pointTable[a][b]);
			glNormal3fv(normalsTable[a - 1][b - 1]);
			glVertex3fv(pointTable[a - 1][b - 1]);
			glNormal3fv(normalsTable[a - 1][b]);
			glVertex3fv(pointTable[a - 1][b]);
			//prawy gorny trojkat
			glNormal3fv(normalsTable[a][b]);
			glVertex3fv(pointTable[a][b]);
			glNormal3fv(normalsTable[a - 1][b - 1]);
			glVertex3fv(pointTable[a - 1][b - 1]);
			glNormal3fv(normalsTable[a][b - 1]);
			glVertex3fv(pointTable[a][b - 1]);
		}
	glEnd();

	//for (int i = 0; i < numberOfDivides; i++)
	//{
	//	for (int j = 0; j < numberOfDivides; j++)
	//	{
	//		glBegin(GL_LINES);
	//		glColor3f(1.0, 0.0, 0.0);
	//		glVertex3fv(pointTable[i][j]);
	//		glColor3f(1.0, 0.0, 0.0);
	//		glVertex3f(normalsTable[i][j][0] + pointTable[i][j][0], normalsTable[i][j][1] + pointTable[i][j][1], normalsTable[i][j][2] + pointTable[i][j][2]);
	//		glEnd();
	//	}
	//}
}

void mountain(int numberOfDivides)
{
	if (!tableInitialized)
	{
		initializeTables(numberOfDivides);
		tableInitialized = true;
	}

	drawMountain(numberOfDivides);
}

void Mouse(int btn, int state, int x, int y)
{
	status = 0;          // nie zosta� wci�ni�ty �aden klawisz 

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
							   // jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;          // wci�ni�ty zosta� lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y_scale_old = y;
		status = 2;
	}
}

/*************************************************************************************/
// Funkcja "monitoruje" po�o�enie kursora myszy i ustawia warto�ci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{
	if (status == 1)
	{
		delta_x = x - x_pos_old;     // obliczenie r�nicy po�o�enia kursora myszy

		x_pos_old = x;            // podstawienie bie��cego po�o�enia jako poprzednie

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
// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana, gdy trzeba 
// przerysowa� scen�)

void RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, upY, 0.0);
	// Zdefiniowanie po�o�enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy�ej

	if (status == 1)                     // je�li lewy klawisz myszy wci�ni�ty
	{
		azymut += delta_x*pix2angle / 100;    // modyfikacja k�ta obrotu o kat proporcjonalny
		elewacja += delta_y*pix2angle / 100;
	}                                  // do r�nicy po�o�e� kursora myszy
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


	//glColor3f(1.0f, 1.0f, 1.0f);

	//if (tableInitialized)
	{
		double scale = 1.0 / (double)pointTable[DIVISIONS - 1][DIVISIONS - 1][0];
		glScaled(scale, scale, scale);
	}
	// Ustawienie koloru rysowania na bia�y
	mountain(DIVISIONS);
	//glutWireTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
	glutSwapBuffers();

}
/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz�cy (wype�nienia okna) ustawiono na czarny
	// Definicja materia�u z jakiego zrobiony jest czajnik 

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ka =[kar,kag,kab] dla �wiat�a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki kd =[kdr,kdg,kdb] �wiat�a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wsp�czynniki ks =[ksr,ksg,ksb] dla �wiat�a odbitego                

	GLfloat mat_shininess = { 20.0 };
	// wsp�czynnik n opisuj�cy po�ysk powierzchni


	/*************************************************************************************/
	// Definicja �r�d�a �wiat�a


	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	// po�o�enie �r�d�a


	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk�adowe intensywno�ci �wiecenia �r�d�a �wiat�a powoduj�cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk�adowa sta�a ds dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_linear = { 0.05f };
	// sk�adowa liniowa dl dla modelu zmian o�wietlenia w funkcji 
	// odleg�o�ci od �r�d�a

	GLfloat att_quadratic = { 0.001f };
	// sk�adowa kwadratowa dq dla modelu zmian o�wietlenia w funkcji
	// odleg�o�ci od �r�d�a

	/*************************************************************************************/
	// Ustawienie parametr�w materia�u i �r�d�a �wiat�a

	/*************************************************************************************/
	// Ustawienie patrametr�w materia�u


	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);


	/*************************************************************************************/
	// Ustawienie parametr�w �r�d�a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


	/*************************************************************************************/
	// Ustawienie opcji systemu o�wietlania sceny 

	glShadeModel(GL_SMOOTH); // w�aczenie �agodnego cieniowania
	glEnable(GL_LIGHTING);   // w�aczenie systemu o�wietlenia sceny 
	glEnable(GL_LIGHT0);     // w��czenie �r�d�a o numerze 0
	glEnable(GL_DEPTH_TEST); // w��czenie mechanizmu z-bufora 

}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;  // przeliczenie pikseli na stopnie

	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie��cej 

	gluPerspective(70, 1.0, 1.0, 30.0);
	// Ustawienie parametr�w dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko�ci okna okna widoku (viewport) w zale�no�ci
	// relacji pomi�dzy wysoko�ci� i szeroko�ci� okna

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej 

}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	pointTable = new point3*[DIVISIONS];
	normalsTable = new point3*[DIVISIONS];
	for (int i = 0; i < DIVISIONS; i++)
	{
		pointTable[i] = new point3[DIVISIONS];
		normalsTable[i] = new point3[DIVISIONS];
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);



	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  B�dzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// za zmiany rozmiaru okna                       

	glutMouseFunc(Mouse);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj� zwrotn� odpowiedzialn� za badanie ruchu myszy

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania niewidocznych element�w sceny

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/