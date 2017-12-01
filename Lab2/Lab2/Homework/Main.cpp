/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj� osi  
//  uk�adu wsp�rzednych
/*************************************************************************************/
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

/*************************************************************************************/
// Funkcja rysuj�ca osie uk�adu wsp�rz�dnych

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
			float param = x*sqrt(x*x + y*y - x*x*y*y) / sqrt(x*x + y*y);

			//Two another methods for generating (different x parameter equasion)
			//pointTable[i][j][1] = f(abs(pointTable[i][j][0]/2 * (1- pointTable[i][j][2])/2)/*abs(pointTable[i][j][0])*/, WeierstrassParameterA);

			//float xVal = -abs(pointTable[i][j][0]);
			//float zVal = -abs(pointTable[i][j][2]);
			//float p = ( 1+xVal )*( 1+zVal ) -0.1;

			float xVal = (1+abs(pointTable[i][j][0]))/2;
			float zVal = (1+abs(pointTable[i][j][2]))/2;

			float p = sqrt(xVal*zVal+xVal*zVal);
			float value = f(p, WeierstrassParameterA);
			value = sqrt((rand() / RAND_MAX + 0.5)*value);
			pointTable[i][j][1] = value;
		}
	}
}

void drawMountain(int numberOfDivides)
{
	glPointSize(2.0);
	//glBegin(GL_POINTS);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	float interval = 1.0 / (float)(numberOfDivides - 1);
	float color = 0.0;
	for (int a = 1; a < numberOfDivides; a++)
		for (int b = 1; b < numberOfDivides; b++)
		{
			//Help if want to view as 2D (as points)
			//glColor3f(pointTable[a][b][1], pointTable[a][b][1], pointTable[a][b][1]);
			//glVertex3dv(pointTable[a][b]);

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

void mountain(int numberOfDivides)
{
	if (!tableInitialized)
	{
		initializeTables(numberOfDivides);
		tableInitialized = true;
	}

	drawMountain(numberOfDivides);
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Change -30.0f to -90.f if you want to view as 2D (as points)
	glRotated(-30.0f, 1.0f, 0.0f, 0.0f);
	mountain(100);

	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania

	glutSwapBuffers();
	//
}

/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc�cy (wype�nienia okna) ustawiono na czarny
}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych 
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s� 
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio  okre�laj�cej proporcj�
	// wymiar�w okna 
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;
	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna widoku (viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)  
	glMatrixMode(GL_PROJECTION);
	// Prze��czenie macierzy bie��cej na macierz projekcji 
	glLoadIdentity();
	// Czyszcznie macierzy bie��cej            
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika  proporcji okna
	// Gdy okno nie jest kwadratem wymagane jest okre�lenie tak zwanej
	// przestrzeni ograniczaj�cej pozwalaj�cej zachowa� w�a�ciwe
	// proporcje rysowanego obiektu.
	// Do okreslenia przestrzeni ograniczj�cej s�u�y funkcja
	// glOrtho(...)    
	if (horizontal <= vertical)
		glOrtho(-1.0, 1.0, -1.0 / AspectRatio, 1.0 / AspectRatio, 10.0, -10.0);
	else
		glOrtho(-1.0*AspectRatio, 1.0*AspectRatio, -1.0, 1.0, 10.0, -10.0);

	glMatrixMode(GL_MODELVIEW);
	// Prze��czenie macierzy bie��cej na macierz widoku modelu                                   
	glLoadIdentity();
	// Czyszcenie macierzy bie��cej
}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(int argc, char* argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);

	glutCreateWindow("Uk�ad wsp�rz�dnych 3-D");

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn�
	// (callback function).  Bedzie ona wywo�ywana za ka�dym razem 
	// gdy zajdzie potrzba przeryswania okna 
	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn�
	// zazmiany rozmiaru okna      
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy�ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst�pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W��czenie mechanizmu usuwania powierzchni niewidocznych

	

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

	
}
/*************************************************************************************/