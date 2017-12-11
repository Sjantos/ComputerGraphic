/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rz�dnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <cmath>

#define M_PI 3.14159265358979323846
#define DIVISIONS 100

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
point3** eggTable;
static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
float YScale = 1.0;
bool tablesInitialized = false;

float x(float u, float v)
{
	float returnValue = 0.0;
	returnValue = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*cos(M_PI*v);
	return returnValue;
}

float y(float u, float v)
{
	float returnValue = 0.0;
	returnValue = (160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2));
	return returnValue;
}

float z(float u, float v)
{
	float returnValue = 0.0;
	returnValue = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u)*sin(M_PI*v);
	return returnValue;
}

void initializeTables(int numberOfDivides)
{
	float interval = 1.0 / (float)(numberOfDivides - 1);
	eggTable = new point3*[numberOfDivides];
	for (int i = 0; i < numberOfDivides; i++)
	{
		eggTable[i] = new point3[numberOfDivides];
		for (int j = 0; j < numberOfDivides; j++)
		{
			eggTable[i][j][0] = x((float)i*interval, (float)j*interval);
			eggTable[i][j][1] = y((float)i*interval, (float)j*interval);
			eggTable[i][j][2] = z((float)i*interval, (float)j*interval);
		}
	}
}

void drawSolidEgg(int numberOfDivides)
{
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);
	for (int a = 1; a < numberOfDivides; a++)
		for (int b = 1; b < numberOfDivides; b++)
		{
			//lewy dolny trojkat
			glVertex3fv(eggTable[a][b]);
			glVertex3fv(eggTable[a - 1][b - 1]);
			glVertex3fv(eggTable[a - 1][b]);
			//prawy gorny trojkat
			glVertex3fv(eggTable[a][b]);
			glVertex3fv(eggTable[a - 1][b - 1]);
			glVertex3fv(eggTable[a][b - 1]);
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
		tablesInitialized = false;
		if (y_scale_new != 0 && y_scale_old != 0)
		{
			float dif = (float)y_scale_new - (float)y_scale_old;
			float add = dif / 250;
			if (abs(add) < 0.5)
			{
				//XRange += add;
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

	// Ustawienie koloru rysowania na bia�y
	glScalef(0.1, 0.1, 0.1);
	egg(DIVISIONS);
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