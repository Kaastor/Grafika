//////////////////////////////////////////////////////////////////////////////////////////
//
// Program wyswietlajacy szescian w rzucie perspektywicznym. Dlugosc boku szescianu
// moze byc interaktywnie zmieniana za pomoca klwiszy '+' i '-'.
// KOLORY:
// Biały 1,1,1
// Purpurowy 1,0,1
// Żółty 1.1.0
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include "materials.h"
// Definicja stalych
double PROMIEN = 5.0;
double OBSERWATOR_ODLEGLOSC = 60.0;
double OBSERWATOR_OBROT_X = 20.0;
double OBSERWATOR_OBROT_Y = 20.0;

#define OBSERWATOR_FOV_Y        30.0
#define PODZIAL_X			    80.0
#define PODZIAL_Y				80.0
double EPS = PODZIAL_X, EPS2 = PODZIAL_Y;

// Zmienne globalne

int     szerokoscOkna = 1024;
int     wysokoscOkna = 768;
float j = 0, i = 0;
float a, x, y, z;
int q = 20;
int p = 20;
//
double R_L2=10, R_L1=10;
double kat_x=10;
double kat_y=10;
double kat_z=10;
//
// Prototypy funkcji
void UstawParametryWidoku(int szer, int wys);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);
#define M_PI 3.1415926535
#define DEG2RAD(x) ((double)(x)*PI/180.0)
const long double PI = 3.1415926535897932384626433832795028841971693993751058209;
GLfloat swiatlo[10][4] = {
    {0.0, 0.0, 0.0, 1.0},  // [0] otoczenie
    {1.0, 1.0, 1.0, 1.0},  // [1] rozproszenie
    {1.0, 1.0, 1.0, 1.0},  // [2] lustrzane
    {0.0, 0.0, 1.0, 1.0},  // [3] polozenie
    {0.0, 0.0, -1.0, 0.0}, // [4] kierunek swiecenia
    {0.0, 0.0, 0.0, 0.0},  // [5] tlumienie katowe swiatla
    {180.0, 0.0, 0.0, 0.0},// [6] kat odciecia swiatla
    {1.0, 0.0, 0.0, 0.0},  // [7] stale tlumienie
    {0.0, 0.0, 0.0, 0.0},  // [8] tlumienie liniowe
    {0.0, 0.0, 0.0, 0.0}}; // [9] tlumienie kwadratowe
//**************** MATERIAŁY - PARAMETRY *********************************************
//w odpowiednich miejscach zmieniam rgba kolorów	//jeśli ma być emision to wszystkie alfa na 1 oraz w emission dany kolor, reszta zero
const GLfloat ambient1[4] = { 1.0, 1.0, 0.0, 1.0 };		//stopiern odbicia swiatla otoczenia - mat
const GLfloat diffuse1[4] = { 0.5, 0.5, 0.0, 1.0 };		//matowość
const GLfloat specular1[4] = { 0.0, 0.0, 0.0, 1.0 };	//połysk - 0 lub 1 wszędzie
const GLfloat emission1[4] = { 0.0, 0.0, 0.0, 0.0 };	//emitujący 1,1,0,1
void Material1() {
	//żółty matowy
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
}

const GLfloat PolishedBronzeEmission[4] = { 0.0, 0.0, 0.0, 0.0 };
void Material2() {
	//brąz polerowany
	glMaterialfv(GL_FRONT, GL_AMBIENT, PolishedBronzeAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, PolishedBronzeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, PolishedBronzeSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 0);
	glMaterialfv(GL_FRONT, GL_EMISSION, PolishedBronzeEmission);
}	


//**************** OŚWIETLENIE - PARAMETRY *********************************************

void OswietlenieReflektor() {
	const GLfloat ambient_s1[4] = { 0.0, 1.0, 0.0, 1.0 }; //ostatnia wartosc to natezenie swiatla = tutaj 1
	const GLfloat diffuse_s1[4] = { 1.0, 1.0, 0.0, 1.0 }; // natezenie 1, kolor
	GLfloat specular_s1[4] = { 1.0, 1.0, 0.0, 1.0 };		//kolor
	GLfloat position_s1[4] = { 0.0, 0.0, 0.0, 1.0 };	//polozenie zrodla swiatla
	GLfloat direction_s1[4] = { -45, 0, 0 };

	glPushMatrix();
	glRotatef(kat_y, 1, 0, 0);
	glRotatef(kat_x, 0, 1, 0);
	glRotatef(kat_z, 0, 0, 1);
	glTranslatef(R_L2, R_L1, 0);		//modyfikacja oddalenia reflektora
	glPushMatrix();
	glutWireSphere(0.25, 30, 30);		//utworzenie reflektora ktorym jest sfera
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_s1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular_s1);
	glLightfv(GL_LIGHT1, GL_POSITION, position_s1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45);			//KAT ODCIECIA
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction_s1);
	glPopMatrix();
}



void OswietlenieOgolne() {
	// Odblokowanie oswietlenia
  glEnable(GL_LIGHTING);

  // Odblokowanie zerowego zrodla swiatla
 

  // Inicjowanie zrodla swiatla
  glLightfv(GL_LIGHT3, GL_AMBIENT, swiatlo[0]);
  glLightfv(GL_LIGHT3, GL_DIFFUSE, swiatlo[1]);
  glLightfv(GL_LIGHT3, GL_SPECULAR, swiatlo[2]);
  glLightfv(GL_LIGHT3, GL_POSITION, swiatlo[3]);  
  glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, swiatlo[4]);
  glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, swiatlo[5][0]);
  glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, swiatlo[6][0]);
  glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, swiatlo[7][0]);
  glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, swiatlo[8][0]);
  glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, swiatlo[9][0]);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}
void OswietlenieKierunkowe() {
	const GLfloat ambient_s2[4] = { 1.0, 0.5, 0.0, 0.8 };
	const GLfloat diffuse_s2[4] = { 0.3, 0.3, 0.3, 0.8 }; 
	GLfloat specular_s2[4] = { 1.0, 0.5, 0.0, 0.0 };
	GLfloat position_s2[4] = { 10.0, 10.0, 10.0, 0.0 };	//polozenie

	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_s2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular_s2);
	glLightfv(GL_LIGHT2, GL_POSITION, position_s2);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja rysujaca szescian o boku "a" w trybie GL_QUAD_STRIP.
// Srodek szescianu znajduje się w punkcie (0,0,0).
void RysujKule(double R)
{

	double dAlfa = 360.0 / EPS;
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_DEPTH_TEST);

	/*glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(-50.0, 0.0, 0.0);
	glVertex3f(50.0, 0.0, 0.0);
	
	glVertex3f(0.0, -20.0, 0.0);
	glVertex3f(0.0, 20.0, 0.0);
	
	glVertex3f(0.0, 0.0, -20.0);
	glVertex3f(0.0, 0.0, 20.0);
	glEnd();*/
	///^^ uklad wsporzednych
	Material2();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 0.0);
	for (i = 0.0; i*dAlfa <= 90.0; i++)
	{
		glVertex3f(R*cos(DEG2RAD(i*dAlfa)), R*sin(DEG2RAD(i*dAlfa)), 0.0);	
		if ((int)i % 2 == 0) Material2();
		else Material1();
	}
	glNormal3f(0.0, 0.0, -1.0);
	glEnd();
	Material2();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 0.0);
	for (i = 0.0; i*dAlfa <= 90.0; i++)
	{

		glVertex3f(0.0, R*cos(DEG2RAD(i*dAlfa)), R*sin(DEG2RAD(i*dAlfa)));
		if ((int)i % 2 == 0) Material2();
		else Material1();
	}
	glNormal3f(-1.0, 0.0, 0.0);
	glEnd();
	Material2();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 0.0);
	for (i = 0.0; i*dAlfa <= 90.0; i++)
	{
		glVertex3f(R*cos(DEG2RAD(i*dAlfa)), 0.0, R*sin(DEG2RAD(i*dAlfa)));
		if ((int)i % 2 == 0) Material1();
		else Material2();
	}
	glNormal3f(0.0, -1.0, 0.0);
	glEnd();
	
	//***************************************//	SFERA //*********
	
	for (j = 0; j < q; j++)		
	{
		Material1();
		glBegin(GL_TRIANGLE_STRIP);
		for (i = 0; i <= p / 4; i++)
		{
			
			if ((int)i % 2 == 0) Material2();		//<----pionowe
			else Material1();

			glNormal3f(
				R * cos((float)(j + 1) / q*PI / 2.0) *
				cos(2.0* (float)i / p * PI),
				R * sin((float)(j + 1) / q * PI / 2.0),
				R * cos((float)(j + 1) / q * PI / 2.0) *
				sin(2.0 * (float)i / p*PI));
			glVertex3f(
				R * cos((float)(j + 1) / q*PI / 2.0) *
				cos(2.0* (float)i / p * PI),
				R * sin((float)(j + 1) / q * PI / 2.0),
				R * cos((float)(j + 1) / q * PI / 2.0) *
				sin(2.0 * (float)i / p*PI));
			
			if ((int)i % 2 == 0) Material2();		//<----pionowe
			else Material1();
			glNormal3f(
				R * cos((float)j / q* PI / 2.0) *
				cos(2.0* (float)i / p * PI),
				R * sin((float)j / q * PI / 2.0),
				R * cos((float)j / q * PI / 2.0) *
				sin(2.0 * (float)i / p*PI));
			glVertex3f(
				R * cos((float)j / q* PI / 2.0) *
				cos(2.0* (float)i / p * PI),
				R * sin((float)j / q * PI / 2.0),
				R * cos((float)j / q * PI / 2.0) *
				sin(2.0 * (float)i / p*PI));

		
		}
		glEnd();
	}
	Material2();   //materiał dla reflektora
	OswietlenieReflektor(); 
	OswietlenieKierunkowe(); 
	//jak wrzuce to do funkcji WyswietlObraz to oswietli cala scene
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja ustawiajaca parametry rzutu perspektywicznego i rozmiary viewportu. Powinna
// być wywolywana kazdorazowo po zmianie rozmiarow okna programu.
void UstawParametryWidoku(int szer, int wys)
{
	// Zapamietanie wielkosci widoku
	szerokoscOkna = szer;
	wysokoscOkna = wys;

	// Ustawienie parametrow viewportu
	glViewport(0, 0, szerokoscOkna, wysokoscOkna);

	// Przejscie w tryb modyfikacji macierzy rzutowania
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(OBSERWATOR_FOV_Y, (float)szerokoscOkna / (float)wysokoscOkna, 1.0, 1000.0);
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja wyswietlajaca pojedyncza klatke animacji
void WyswietlObraz(void)
{
	// Wyczyszczenie bufora koloru i bufora glebokosci
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
	glMatrixMode(GL_MODELVIEW);
	
	// Zastapienie aktywnej macierzy macierza jednostkowa
	glLoadIdentity();
	OswietlenieOgolne();
	// Ustalenie polozenia obserwatora
	glTranslatef(0, 0, -OBSERWATOR_ODLEGLOSC);
	glRotatef(OBSERWATOR_OBROT_X, 1, 0, 0);
	glRotatef(OBSERWATOR_OBROT_Y, 0, 1, 0);
		
	// Narysowanie szescianu
	RysujKule(PROMIEN);

	// Przelaczenie buforow ramki
	glutSwapBuffers();
}


//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
	
	if (klawisz == '1')
		OBSERWATOR_OBROT_X += 1.01;
	else if (klawisz == '2')
		OBSERWATOR_OBROT_X -= 1.01;
	else if (klawisz == '3')
		OBSERWATOR_OBROT_Y += 1.01;
	else if (klawisz == '4')
		OBSERWATOR_OBROT_Y -= 1.01;
	else if (klawisz == 'p' )
	{
		p += 4;
		EPS += 4.00;
	}
	else if (klawisz == 'P' )
	{
		p -= 4;
		EPS -= 4.00;
	}
	else if (klawisz == 'q')
		q += 4;
	else if (klawisz == 'Q')
		q -= 4;
	else if (klawisz == 'r')
		PROMIEN += 4.0;
	else if (klawisz == 'R')
		PROMIEN -= 4.0;
	else if (klawisz == '8')
		glEnable(GL_LIGHT1);
	else if (klawisz == '*')
		glDisable(GL_LIGHT1);
	else if (klawisz == '7')
		glEnable(GL_LIGHT2);
	else if (klawisz == '&')
		glDisable(GL_LIGHT2);
	else if (klawisz == '6')
		glEnable(GL_LIGHT3);
	else if (klawisz == '^')
		glDisable(GL_LIGHT3);
	else if (klawisz == '9')
		glShadeModel(GL_FLAT);
	else if (klawisz == '(')
		glShadeModel(GL_SMOOTH);
	
	else if (klawisz == 'x')
		kat_x += 4.0;
	else if (klawisz == 'X')
		kat_x -= 4.0;
	else if (klawisz == 'c')
		kat_y += 4.0;
	else if (klawisz == 'C')
		kat_y -= 4.0;
	else if (klawisz == 'z')
		kat_z += 4.0;
	else if (klawisz == 'Z')
		kat_z -= 4.0;
	else if (klawisz == 'v')
		R_L1 += 4.0;
	else if (klawisz == 'V')
		R_L1 -= 4.0;
	else if (klawisz == 'b')
		R_L2 += 4.0;
	else if (klawisz == 'B')
		R_L2 -= 4.0;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char **argv)
{
	// Zainicjowanie biblioteki GLUT
	glutInit(&argc, argv);

	// Ustawienie trybu wyswietlania
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Ustawienie polozenia dolenego lewego rogu okna
	glutInitWindowPosition(100, 100);

	// Ustawienie rozmiarow okna
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);

	// Utworzenie okna
	glutCreateWindow("1/4 sfery - GL_TRIANGLE_STRIP");

	// Odblokowanie bufora glebokosci
	glEnable(GL_DEPTH_TEST);

	// Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
	glClearDepth(1000.0);

	// Ustawienie koloru czyszczenia bufora ramki
	glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

	// Wlaczenie wyswietlania wielokatow w postaci obrysow (przydatne w celach diagnostycznych).
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Zarejestrowanie funkcji (callback) odpowiedzialnej za 
	glutDisplayFunc(WyswietlObraz);

	// Zarejestrowanie funkcji (callback) wywolywanej za kazdym razem kiedy
	// zmieniane sa rozmiary okna
	glutReshapeFunc(UstawParametryWidoku);

	// Zarejestrowanie funkcji wykonywanej gdy okno nie obsluguje
	// zadnych zadan
	glutIdleFunc(WyswietlObraz);

	// Zarejestrowanie funkcji obslugi klawiatury
	glutKeyboardFunc(ObslugaKlawiatury);

	// Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
	// w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
	glutMainLoop();

	return 0;
}
