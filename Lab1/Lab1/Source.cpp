/*************************************************************************************/
// Troch� bardziej skomplikowany program wykorzystuj�cy funkcje biblioteki OpenGL
/*************************************************************************************/
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
/*************************************************************************************/

GLfloat deform = 5;
GLfloat width = 300;
GLint level = 3;

void randColor()
{
	glColor3f((double)rand() / RAND_MAX, (double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
}

void fractal(GLfloat x, GLfloat y, GLfloat width, GLint level)
{
	if (level > 0)
	{
		width /= 3;
		//G�rny rz�d
		fractal(x, y, width, level - 1);
		fractal(x, y - width, width, level - 1);
		fractal(x, y - 2 * width, width, level - 1);

		//Srodkowy rz�d
		fractal(x - width, y, width, level - 1);
		fractal(x - width, y - 2 * width, width, level - 1);

		//Dolny rz�d
		fractal(x - 2 * width, y, width, level - 1);
		fractal(x - 2 * width, y - width, width, level - 1);
		fractal(x - 2 * width, y - 2 * width, width, level - 1);
	}
	else
	{
		GLfloat r = ((double)rand() / RAND_MAX) * deform;
		glBegin(GL_POLYGON);
		randColor();
		glVertex2f(x + r, y + r);
		randColor();
		glVertex2f(x + r - width, y + r);
		randColor();
		glVertex2f(x + r - width, y + r - width);
		randColor();
		glVertex2f(x + r, y + r - width);
		glEnd();
	}
}


// Funkcaja okre�laj�ca, co ma by� rysowane 
// (zawsze wywo�ywana, gdy trzeba przerysowa� scen�)

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	fractal(width / 2, width / 2, width, level);
	glFlush();
	// Przekazanie polece� rysuj�cych do wykonania
}
/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor okna wn�trza okna - ustawiono na szary
}
/*************************************************************************************/
// Funkcja s�u��ca do kontroli zachowania proporcji rysowanych obiekt�w
// niezale�nie od rozmiar�w okna graficznego


void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szeroko�� i wysoko�� okna) s�
// przekazywane do funkcji za ka�dym razem, gdy zmieni si� rozmiar okna
{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio okre�laj�cej proporcj� wymiar�w okna

	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielko�ciokna okna urz�dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okre�lenie uk�adu wsp�rz�dnych obserwatora
	glLoadIdentity();
	// Okre�lenie przestrzeni ograniczaj�cej
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wsp�czynnika proporcji okna
	// Gdy okno na ekranie nie jest kwadratem wymagane jest 
	// okre�lenie okna obserwatora. 
	// Pozwala to zachowa� w�a�ciwe proporcje rysowanego obiektu
	// Do okre�lenia okna obserwatora s�u�y funkcja glOrtho(...)

	if (horizontal <= vertical)
		glOrtho(-width/2, width/2, -width / 2*AspectRatio, width / 2*AspectRatio, 1.0, -1.0);
	else
		glOrtho(-width*AspectRatio/2, width*AspectRatio/2, -width/2, width/2, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych    
	glLoadIdentity();
}
/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli

void main(int argc, char* argv[])
{
	std::cout << "Podaj stopien rozjechania (im wiecej tym gorzej)\n";
	std::cin >> deform;
	std::cout << "Podaj stopien fraktala (im wiecej tym drobniej)\n";
	std::cin >> level;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB

	glutCreateWindow("Drugi program w OpenGL");
	// Utworzenie okna i okre�lenie tre�ci napisu w nag��wku okna

	glutDisplayFunc(RenderScene);
	// Okre�lenie, �e funkcja RenderScene b�dzie funkcj� zwrotn� (callback)
	// Biblioteka GLUT b�dzie wywo�ywa�a t� funkcj� za ka�dym razem, gdy
	// trzeba b�dzie przerysowa� okno


	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj� zwrotn� odpowiedzialn� za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy�ej) wykonuje wszelkie  
	// inicjalizacje konieczneprzed przyst�pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/