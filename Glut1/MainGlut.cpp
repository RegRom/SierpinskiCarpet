/*************************************************************************************/

// Program rysuj�cy losowo kolorowany dywan Sierpi�skiego z perturbacjami

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>

/*************************************************************************************/

//G��boko�� fraktala
int depth = 3;
//Zmienna okre�laj�ca perturbacje w po�o�eniu kwadrat�w
float randomness = 0.02;
//Zmienna okre�laj�ca perturbacje w rozmiarze kwadrat�w
float size_randomness = 0.95;


//Funkcja losuj�ca liczb� float do losowego okre�lenia koloru
float myRand()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}


//Funkcja rysuj�ca pojedynczy kwadrat w losowym kolorze
void drawSquare(float x, float y, float size)
{
	//U�ywamy prymitywu do rysowania wielok�tu, w naszym przypadku
	//jest to czworok�t, wi�c tworzymy cztery wierzcho�ki,
	//kolory losujemy funkcj� myRand()
	glBegin(GL_POLYGON);

	glColor3f(myRand(), myRand(), myRand());
	glVertex2f(x, y);
	glColor3f(myRand(), myRand(), myRand());
	glVertex2f(x + size, y);
	glColor3f(myRand(), myRand(), myRand());
	glVertex2f(x + size, y - size);
	glColor3f(myRand(), myRand(), myRand());
	glVertex2f(x, y - size);

	glEnd();
}

//Funkcja rysuj�ca dywan Sierpi�skiego
void drawCarpet(float x, float y, float size, int currentDepth)
{
	//U�ywamy zagnie�dzonej p�tli, jedna z nich odpowiada
	//za rysowanie kwadrat�w w pionie, a druga w poziomie
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			//Je�li "i" oraz "j" maj� warto�� jeden, to mamy �rodkowy kwadrat, jego nie rysujemy
			if (i != 1 || j != 1)
			{
				if (currentDepth > depth - 2) drawSquare(
					(x + j * size / 3) + size * myRand() * randomness,
					(y - i * size / 3) + size * myRand() * randomness,
					(size + myRand() * size_randomness) / 3);
				// Wywo�anie funkcji w celu narysowania kolejnych 8 kwadrat�w
				else drawCarpet(
					(x + j * size / 3) + size * myRand() * randomness,
					(y - i * size / 3) + size * myRand() * randomness,
					(size + myRand() + size_randomness) / 3,
					currentDepth + 1);
			}
		}
	}
}


void RenderScene(void)
{
	// Czyszczenie okna aktualnym kolorem czyszcz�cym
	glClear(GL_COLOR_BUFFER_BIT);

	//Rysowanie dywanu
	drawCarpet(-90, 90, 180.0f, 0);

	// Przekazanie polece� rysuj�cych do wykonania
	glFlush();

}

/*************************************************************************************/

// Funkcja ustalaj�ca stan renderowania



void MyInit(void)

{

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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
	// Ustawienie wielko�ci okna okna urz�dzenia (Viewport)
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
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okre�lenie uk�adu wsp�rz�dnych     

	glLoadIdentity();
}

/*************************************************************************************/

// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
int main(int argc, char** argv)

{
	//Inicjowanie generatora liczb losowych
	srand(time(NULL));
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wy�wietlania
	// GLUT_SINGLE - pojedynczy bufor wy�wietlania
	// GLUT_RGBA - model kolor�w RGB

	glutCreateWindow("Dywan Sierpinskiego");
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