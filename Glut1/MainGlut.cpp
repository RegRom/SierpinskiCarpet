/*************************************************************************************/

// Program rysuj¹cy losowo kolorowany dywan Sierpiñskiego z perturbacjami

/*************************************************************************************/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <math.h>

/*************************************************************************************/

//G³êbokoœæ fraktala
int depth = 3;
//Zmienna okreœlaj¹ca perturbacje w po³o¿eniu kwadratów
float randomness = 0.02;
//Zmienna okreœlaj¹ca perturbacje w rozmiarze kwadratów
float size_randomness = 0.95;


//Funkcja losuj¹ca liczbê float do losowego okreœlenia koloru
float myRand()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}


//Funkcja rysuj¹ca pojedynczy kwadrat w losowym kolorze
void drawSquare(float x, float y, float size)
{
	//U¿ywamy prymitywu do rysowania wielok¹tu, w naszym przypadku
	//jest to czworok¹t, wiêc tworzymy cztery wierzcho³ki,
	//kolory losujemy funkcj¹ myRand()
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

//Funkcja rysuj¹ca dywan Sierpiñskiego
void drawCarpet(float x, float y, float size, int currentDepth)
{
	//U¿ywamy zagnie¿dzonej pêtli, jedna z nich odpowiada
	//za rysowanie kwadratów w pionie, a druga w poziomie
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			//Jeœli "i" oraz "j" maj¹ wartoœæ jeden, to mamy œrodkowy kwadrat, jego nie rysujemy
			if (i != 1 || j != 1)
			{
				if (currentDepth > depth - 2) drawSquare(
					(x + j * size / 3) + size * myRand() * randomness,
					(y - i * size / 3) + size * myRand() * randomness,
					(size + myRand() * size_randomness) / 3);
				// Wywo³anie funkcji w celu narysowania kolejnych 8 kwadratów
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
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym
	glClear(GL_COLOR_BUFFER_BIT);

	//Rysowanie dywanu
	drawCarpet(-90, 90, 180.0f, 0);

	// Przekazanie poleceñ rysuj¹cych do wykonania
	glFlush();

}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania



void MyInit(void)

{

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wnêtrza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja s³u¿¹ca do kontroli zachowania proporcji rysowanych obiektów
// niezale¿nie od rozmiarów okna graficznego
void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna

{
	GLfloat AspectRatio;
	// Deklaracja zmiennej AspectRatio okreœlaj¹cej proporcjê wymiarów okna

	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;

	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkoœci okna okna urz¹dzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)

	glMatrixMode(GL_PROJECTION);
	// Okreœlenie uk³adu wspó³rzêdnych obserwatora

	glLoadIdentity();
	// Okreœlenie przestrzeni ograniczaj¹cej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie wspó³czynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest
	// okreœlenie okna obserwatora.
	// Pozwala to zachowaæ w³aœciwe proporcje rysowanego obiektu
	// Do okreœlenia okna obserwatora s³u¿y funkcja glOrtho(...)

	if (horizontal <= vertical)
		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else
		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych     

	glLoadIdentity();
}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli
int main(int argc, char** argv)

{
	//Inicjowanie generatora liczb losowych
	srand(time(NULL));
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyœwietlania
	// GLUT_SINGLE - pojedynczy bufor wyœwietlania
	// GLUT_RGBA - model kolorów RGB

	glutCreateWindow("Dywan Sierpinskiego");
	// Utworzenie okna i okreœlenie treœci napisu w nag³ówku okna

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹ (callback)
	// Biblioteka GLUT bêdzie wywo³ywa³a t¹ funkcjê za ka¿dym razem, gdy
	// trzeba bêdzie przerysowaæ okno

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹ za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powy¿ej) wykonuje wszelkie 
	// inicjalizacje konieczneprzed przyst¹pieniem do renderowania

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}