/*************************************************************************************/
//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi  
//  uk³adu wspó³rzêdnych dla rzutowania perspektywicznego
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>

#define M_PI 3.14159265358979323846

GLdouble upY = 1.0;
static GLfloat R = 2.0;
static GLfloat azymut = 0.0;   // wokol Y
static GLfloat elewacja = 0.0;
static GLfloat pix2angle;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy 
// 0 - nie naciœniêto ¿adnego klawisza
// 1 - naciœniêty zostaæ lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int y_scale_old = 0;
static int y_scale_new = 0;

static int delta_x = 0;        // ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
static int delta_y = 0;
static float delta_scale = 1;
// i poprzedni¹ kursora myszy 
typedef float point3[3];
point3** pointTable;
static GLfloat viewer[] = { 3.0, 3.0, 10.0 };
int WeierstrassSum = 100;
int WeierstrassParameterA = 2;
bool tableInitialized = false;
float XRange = 2.0;
// inicjalizacja po³o¿enia obserwatora
/*************************************************************************************/
// Funkcja rysuj¹ca osie uk³adu wspó?rz?dnych

/*************************************************************************************/
// Funkcja "bada" stan myszy i ustawia wartoœci odpowiednich zmiennych globalnych


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
	float interval = XRange / (float)(numberOfDivides - 1);
	float parameter = 1.0 / (float)(numberOfDivides - 1/**numberOfDivides*/);
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
			float param = x*sqrt(x*x + y*y - x*x*y*y) / sqrt(x*x + y*y);

			//Two another methods for generating (different x parameter equasion)
			//pointTable[i][j][1] = f(abs(pointTable[i][j][0]/2 * (1- pointTable[i][j][2])/2)/*abs(pointTable[i][j][0])*/, WeierstrassParameterA);

			//float xVal = -abs(pointTable[i][j][0]);
			//float zVal = -abs(pointTable[i][j][2]);
			//float p = ( 1+xVal )*( 1+zVal ) -0.1;

			float xVal = (1 + abs(pointTable[i][j][0])) / 2;
			float zVal = (1 + abs(pointTable[i][j][2])) / 2;

			float p = sqrt(xVal*zVal + xVal*zVal);
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
			//glVertex3fv(pointTable[a][b]);

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
	status = 0;          // nie zosta³ wciêniêty ¿aden klawisz 

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;         // przypisanie aktualnie odczytanej pozycji kursora 
		// jako pozycji poprzedniej
		y_pos_old = y;
		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y_scale_old = y;
		status = 2;
	}
}

/*************************************************************************************/
// Funkcja "monitoruje" po³o¿enie kursora myszy i ustawia wartoœci odpowiednich 
// zmiennych globalnych

void Motion(GLsizei x, GLsizei y)
{
	if (status == 1)
	{
		delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
		
		x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie

		delta_y = y - y_pos_old;
		
		y_pos_old = y;
	}

	y_scale_old = y_scale_new;
	y_scale_new = y;
	if (status == 2)
	{
		/*y_scale_old = y_scale_new;
		y_scale_new = y;*/
		if (y_scale_new != 0 && y_scale_old != 0)
		{
			float dif = (float)y_scale_new - (float)y_scale_old;
			float add = dif/100;
			printf("%f\n", add);
			if (R+add >XRange/1.1 && R+add < 3*XRange)
				R += add;
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

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, upY, 0.0);
	// Zdefiniowanie po³o¿enia obserwatora
	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej

	if (status == 1)                     // jeœli lewy klawisz myszy wciêniêty
	{
		azymut += delta_x*pix2angle/100;    // modyfikacja k¹ta obrotu o kat proporcjonalny
		elewacja += delta_y*pix2angle/100;
	}                                  // do ró¿nicy po³o¿eñ kursora myszy
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
	
	//wspó³rzêdna X kamery
	viewer[0] = R*cos(azymut)*cos(elewacja);
	//wspó³rzêdna Y kamery
	viewer[1] = R*sin(elewacja);
	//wspó³rzêdna Z kamery
	viewer[2] = R*sin(azymut)*cos(elewacja);


	//glColor3f(1.0f, 1.0f, 1.0f);
	// Ustawienie koloru rysowania na bia³y
	mountain(100);
	//glutWireTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania
	glutSwapBuffers();

}
/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszcz¹cy (wype³nienia okna) ustawiono na czarny
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

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);

	

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bêdzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzeba przerysowania okna

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