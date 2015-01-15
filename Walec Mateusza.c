//////////////////////////////////////////////////////////////////////////////////////////
//
// Program wyswietlajacy szescian w rzucie perspektywicznym. Dlugosc boku szescianu
// moze byc interaktywnie zmieniana za pomoca klwiszy '+' i '-'.
//
//////////////////////////////////////////////////////////////////////////////////////////
//#include <GL/GLU.h>

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "glut.h"
#include "materials.h"





#define DEG2RAD(x)       ((float)(x)*M_PI/180.0)
// Definicja stalych
#define WYSOKOSC				5.0
#define PROMIEN                 2.0
#define OBSERWATOR_ODLEGLOSC    30.0
#define M_PI				3.14159
#define OBSERWATOR_FOV_Y        30.0


// Tablica parametrow swiatla
GLfloat swiatlo[10][4];

// Tablica parametrow materialu z jakiego wykonany jest walec
GLfloat material[6][4];
GLfloat material2[6][4];

// Zmienne globalne
double OBSERWATOR_OBROT_X=195.0;
double OBSERWATOR_OBROT_Y=25.0;
double OBSERWATOR_OBROT_Z=0.0;
double  promien       = PROMIEN;
double  wsp_wielkosci     = 1; // Dlugosc boku szescianu
double wysokosc		  = WYSOKOSC;
int PODZIAL_X=30;
int PODZIAL_Y=30;
int     szerokoscOkna = 1024;
int     wysokoscOkna  = 768;
int kolor=0;
float j = 0, i = 0;
float a,x,y,z;




double R_L2=5, R_L1=5;
double kat_x=45;
double kat_y=-6;
double kat_z=-19;



// Prototypy funkcji
void UstawParametryWidoku(int szer, int wys);
void WyswietlObraz(void);
void ObslugaKlawiatury(unsigned char klawisz, int x, int y);
void UstawDomyslneWartosciParametrow(void);
void DefiniujMaterial(int typ);
//void WlaczOswietlenie(void);

const GLfloat ambient1[4] = { 0.000000, 0.0, 0.074500, 0.000000};		//stopiern odbicia swiatla otoczenia - mat
const GLfloat diffuse1[4] = {0.1, 0.3, 0.9, 1.0};		//matowo??
const GLfloat specular1[4] = {0.597254, 0.598290, 1.0, 0.000000};	//po?ysk - 0 lub 1 wsz?dzie
const GLfloat emission1[4] = { 0.0, 0.0, 0.0, 1.0 };	//emituj?cy 1,1,0,1

void Material1() {
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
	glMaterialf(GL_FRONT, GL_SHININESS, 120);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission1);
}

const GLfloat BronzeEmission[4] = { 0.0, 0.0, 0.0, 0.0 };
void Material2() 
{
	//br?z polerowany
	glMaterialfv(GL_FRONT, GL_AMBIENT, BronzeAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BronzeDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,BronzeSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 126.6);
	glMaterialfv(GL_FRONT, GL_EMISSION, BronzeEmission);
}

GLfloat swiatlo[10][4] = {
	{ 0.0, 0.0, 0.0, 1.0 },  // [0] otoczenie
	{ 1.0, 1.0, 1.0, 1.0 },  // [1] rozproszenie
	{ 1.0, 1.0, 1.0, 1.0 },  // [2] lustrzane
	{ 0.0, 0.0, 1.0, 1.0 },  // [3] polozenie
	{ 0.0, 0.0, -1.0, 0.0 }, // [4] kierunek swiecenia
	{ 0.0, 0.0, 0.0, 0.0 },  // [5] tlumienie katowe swiatla
	{ 180.0, 0.0, 0.0, 0.0 },// [6] kat odciecia swiatla
	{ 1.0, 0.0, 0.0, 0.0 },  // [7] stale tlumienie
	{ 0.0, 0.0, 0.0, 0.0 },  // [8] tlumienie liniowe
	{ 0.0, 0.0, 0.0, 0.0 } }; // [9] tlumienie kwadratowe

//---------------------------------------------------- SWIATLO OTACZAJACE -----------------------------------------------
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
//---------------------------------------------------- REFLEKTOR -----------------------------------------------
void OswietlenieReflektor() {
	const GLfloat ambient_s1[4] = { 1.0, 1.0, 0.0, 1.0 }; 
	const GLfloat diffuse_s1[4] = { 0.3, 0.3, 0.3, 1.0 }; 
	GLfloat specular_s1[4] = { 1.0, 1.0, 1.0, 1.0 };		
	GLfloat position_s1[4] = { 0.0, 0.0, 0.0, 1.0 };	
	GLfloat direction_s1[4] = { -15, -5, 0 };			

	glPushMatrix();
	glRotatef(kat_y, 1, 0, 0);
	glRotatef(kat_x, 0, 1, 0);
	glRotatef(kat_z, 0, 0, 1);
	glTranslatef(R_L2, R_L1, 0);		
	glPushMatrix();
	glutWireSphere(0.25, 30, 30);		
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_s1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular_s1);
	glLightfv(GL_LIGHT1, GL_POSITION, position_s1);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15);			
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction_s1);
	glPopMatrix();
}

//---------------------------------------------------- SWIATLO KIERUNKOWE -----------------------------------------------
void OswietlenieKierunkowe() {
	

	const GLfloat ambient_s2[4] = { 0.0, 1.0, 0.0, 0.8 }; 
	const GLfloat diffuse_s2[4] = { 1.0, 1.0, 0.0, 0.8 }; 
	GLfloat specular_s2[4] = { 1.0, 1.0, 0.0, 1.0 };

	GLfloat position_s2[4] = { 10.0, 10.0, 10.0, 0.0 };	

	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_s2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular_s2);
	glLightfv(GL_LIGHT2, GL_POSITION, position_s2);
}

void RysujTekstRastrowy(void *font, char *tekst)
{
  int i;

  for (i = 0; i < (int)strlen(tekst); i++)
    glutBitmapCharacter(font, tekst[i]);
}
//---------------------------------------------------- NAKLADKA -----------------------------------------------
void RysujNakladke(void)
{
 char buf[255];

  // Zmiana typu rzutu z perspektywicznego na ortogonalny
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, szerokoscOkna,  0.0, wysokoscOkna,-100.0, 100.0); 
 
  // Modelowanie sceny 2D (zawartosci nakladki)
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // Zablokowanie oswietlenia (mialoby ono wplyw na kolor tekstu)
  glDisable(GL_LIGHTING);

  // Okreslenie koloru tekstu
  glColor3f(1.0, 1.0, 1.0);

 // RYSOWANIE MENU PARAMETROW ZRODLA SWIATLA

  sprintf(buf, "Q/q zmiana l.podzialow poziomych (%d)", PODZIAL_X);
  glRasterPos2i(70, 700);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, "W/w zmiana l.podzialow pionowych (%d)",PODZIAL_Y);
  glRasterPos2i(70, 680);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

 sprintf(buf, "a/A owietlenie otaczajace");
  glRasterPos2i(100, 660);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, "s/S oswietlenie reflektor");
  glRasterPos2i(100, 640);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, "d/D oswietlenie kierunkowe");
  glRasterPos2i(100, 620);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

 
  sprintf(buf, "strzalki zmiana polozenia obserwatora");
  glRasterPos2i(70, 600);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, "Parametry reflektora");
  glRasterPos2i(450, 720);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  

  sprintf(buf, "x/X kat nachylenia orbity do osi OX (%.1f)",kat_x);
  glRasterPos2i(400, 680);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, "c/C kat nachylenia orbity do osi OY(%.1f)",kat_y);
  glRasterPos2i(400, 660);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, "z/Z kat nachylenia orbity do osi OZ (%.1f)",kat_z);
  glRasterPos2i(400, 640);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);

  sprintf(buf, "v/V zmiana promienia orbity (%.1f)",     sqrt((R_L1 * R_L1) +(R_L2 * R_L2)));
  glRasterPos2i(400, 620);
  RysujTekstRastrowy(GLUT_BITMAP_8_BY_13, buf);
  

  // Przywrocenie macierzy sprzed wywolania funkcji
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();  

  // Odblokowanie oswietlenia
  glEnable(GL_LIGHTING);
}

//----------------------------------------------------RYSOWANIE FIGURY -----------------------------------------------
void RysujWalec(double R, double H)
{
	
	
	
    float x, y, z, fi, teta,pr, OD, DO, OD1, DO1;
     const long double PI = 3.1415926535897932384626433832795028841971693993751058209;//, EPS = PODZIAL_X, EPS2 = PODZIAL_Y;
	 long double EPS=PODZIAL_X, EPS2=PODZIAL_Y;
	 int kolor=0;
	 OD1 = 0.0;
	 DO1 = 2.0001*PI;
	 OD = 0.0;
	 DO = 2.0001*PI;
	 Material1();

	  //glBegin(GL_LINES);
 glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_FLAT);
	 
  //uklad wspol
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(-20.0, 0.0, 0.0);
  glVertex3f(20.0, 0.0, 0.0);
 
  
  glColor3f(0.0,1.0,0.0);
  glVertex3f(0.0, -20.0, 0.0);
  glVertex3f(0.0, 20.0, 0.0);
 
 
  glColor3f(0.0,0.5,1.0);
  glVertex3f(0.0, 0.0, -20.0);
  glVertex3f(0.0, 0.0, 20.0);
 
  
  glEnd();
// glColor3f(0.5,1.5,1.0);
  

  //---------------------------------------------------- POW BOCZNA -----------------------------------------------
	
     for(y = 0; y <= H; y += H/EPS)
    { 
		 
          for(fi = OD1; fi < 0.75 * DO1; fi += PI/EPS2)
          {
			 //if(fi>= 0.75 * DO1 - fi) break;
			   glBegin(GL_POLYGON);
			  
			   if(kolor%2==0)
				 {
					 Material1();
				 }
				 else
				{
					  Material2();
				}
			   glNormal3f(cos(fi), 0.0, sin(fi));
				glBegin(GL_LINES);
				//glVertex3f(cos(fi), 0.0, sin(fi));
			   x = R * cos(fi);
               z = R * sin(fi);
			   glVertex3f(x, y, z);
			   if(y+H/EPS>5)
			   {
				   glVertex3f(x,5,z);
			   }
			   else
			   {
					glVertex3f(x,y+H/EPS,z);
			   }
			 if(fi + PI/EPS2 >= 0.75 * DO1)
			   {
				   float fi2= 2.000000000001 * 0.75 * PI;
				   float tmp_fi=fi;
				   fi=fi2;
					 x = R * cos(fi);
					 z = R * sin(fi);
					 if(y+H/EPS>5)
					 {
						  glVertex3f(x,5,z);
					 }
					 else
					 {
						glVertex3f(x,y+H/EPS,z);
					}
					
					 x = R * cos(fi);
					 z = R * sin(fi);

					glVertex3f(x, y, z);
					fi=tmp_fi;
					glEnd();
					break;
				}
			  else
			  {
				  fi+=PI/EPS2;
			  }

			   x = R * cos(fi);
               z = R * sin(fi);
			   if(y+H/EPS>5)
			   {
				   glVertex3f(x,5,z);
			   }
			   else
			   {
					glVertex3f(x,y+H/EPS,z);
			   }
			  glVertex3f(x, y, z);
			  fi-=PI/EPS2;
			  x = R * cos(fi);
              z = R * sin(fi);
			  glVertex3f(x, y, z);
			   glEnd();
			   kolor+=1;
			   
          }
		  kolor=0;
		  //
	//	  glBegin(GL_POLYGON);
	//	  glVertex3f(x, y, -2);
	//	 glEnd();
     }
	 
	 glBegin(GL_TRIANGLE_FAN);
	
				 glNormal3f(0.0, -1.0, 0.0);
//---------------------------------------------------- PODSTAWA GORNA -----------------------------------------------
		 // glVertex3f(0.0, 0.0, 0.0);
				 kolor=0;
		for(fi = OD; fi <= 0.75 * DO; fi += PI/EPS2)
          {
			  

			  if(kolor%2==1)
				 {
					 Material1();
				 }
				 else
				{
					  Material2();
				}
			  glVertex3f(0, 0, 0);
			  //glNormal3f(0.0, -1.0, 0.0);
			  if((fi-PI/EPS2) >=-0.001)
			   {
					x = R * cos(fi-PI/EPS2);
					z = R * sin(fi-PI/EPS2);
					glVertex3f(x, 0, z);
			   }
			   x = R * cos(fi);
               z = R * sin(fi);
               glVertex3f(x, 0, z);
			   kolor+=1;

          }
     glVertex3f(0, 0, 0);
	 glVertex3f(0, 0, -2);
	 glVertex3f(x, 0, z);
     glEnd();
	 kolor=0;
	//---------------------------------------------------- PODSTAWA DOLNA -----------------------------------------------
	 
     glBegin(GL_TRIANGLE_FAN);
	 glNormal3f(0.0, 1.0, 0.0);
		  //glVertex3f(0, H, 0);
          for(fi = OD; fi <= 0.75 * DO; fi += PI/EPS2)
          {
			    if(kolor%2==1)
				 {
					 Material1();
				 }
				 else
				{
					  Material2();
				}

			  glVertex3f(0, H, 0);
			  if((fi-PI/EPS2) >=-0.001)
			   {
			   x = R * cos(fi-PI/EPS2);
               z = R * sin(fi-PI/EPS2);
			   glVertex3f(x, H, z);
			   }
			  




               x = R * cos(fi);
               z = R * sin(fi);
               glVertex3f(x, H, z);
			   kolor+=1;

          }
     glVertex3f(0, H, 0);
	 glVertex3f(0, H, -2);
	 glVertex3f(x, H, z);
	 
     glEnd();
	 
	
	 
	glBegin(GL_POLYGON);

	EPS2/=2;
//pow wycieta w plaszczyznie x,y
	//+++++++++++++++++++++++++++++++++++++++ PLASZCZYZNA WYCIETA X,Y+++++++++++++++++++++++++++++++++++++
	 kolor=0;
	for(x = 0; x <= 2; x += 2/EPS2)
	{
		if(kolor%2==0)
				 {
					 Material1();
				 }
				 else
				{
					  Material2();
				}
          for(y=0 ; y <= 5; y += 5/EPS)
		  {
			  
			  float x2,y2;
			  glBegin(GL_POLYGON);
			  glNormal3f(0.0, 0.0, -1.0);
			
			  

			  if(y+5/EPS>5)
			  {
				  y2=5;
			  }
			  else
			  {
				  y2=y+5/EPS;
			  }

			  if(x+2/EPS2>2)
			  {
				  x2=2;
			  }
			  else
			  {
				  x2=x+2/EPS2;
			  }

			glVertex3f(x, y2, 0);
			glVertex3f(x2, y2, 0);
			glVertex3f(x2, y, 0);
			
			glVertex3f(x, y, 0);
			
             glEnd();
          }
		  kolor+=1;
	}
	kolor=0;
	//placzyzna wycieta w plaszczyznie z,y
	//+++++++++++++++++++++++++++++++++++++++ PLASZCZYZNA WYCIETA Z,Y+++++++++++++++++++++++++++++++++++++
	for(z =0; z <= 2; z += 2/EPS2)
	{
		if(kolor%2==0)
				 {
					 Material1();
				 }
				 else
				{
					  Material2();
				}
          for(y=0 ; y <= 5; y+= 5/EPS)
		  {
			  float z2,y2;
			glBegin(GL_POLYGON);
			  glNormal3f(1.0, 0.0, 0.0);
			  if(y+5/EPS>5)
			  {
				  y2=5;
			  }
			  else
			  {
				  y2=y+5/EPS;
			  }

			  if(z+2/EPS2>2)
			  {
				  z2=2;
			  }
			  else
			  {
				  z2=z+2/EPS2;
			  }
            glVertex3f(0, y2, -z);
			glVertex3f(0, y2, -z2);
			glVertex3f(0, y, -z2);
			glVertex3f(0,y, -z);
			
			
			
			glEnd();
          }
		  kolor+=1;
	}
   


	OswietlenieReflektor(); 
	OswietlenieKierunkowe();
	

     
	
}

void UstawParametryWidoku(int szer, int wys)
{
  // Zapamietanie wielkosci widoku
  szerokoscOkna = szer;
  wysokoscOkna  = wys;

  // Ustawienie parametrow viewportu
  glViewport(0, 0, szerokoscOkna, wysokoscOkna);

  // Przejscie w tryb modyfikacji macierzy rzutowania
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(OBSERWATOR_FOV_Y, (float)szerokoscOkna/(float)wysokoscOkna, 1.0, 1000.0);
}



void WyswietlObraz(void)
{
  // Wyczyszczenie bufora koloru i bufora glebokosci
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  // Przejscie w tryb modyfikacji macierzy przeksztalcen geometrycznych
  glMatrixMode(GL_MODELVIEW);

  // Zastapienie aktywnej macierzy macierza jednostkowa
  glLoadIdentity();

  // Ustalenie polozenia obserwatora
  glTranslatef(0, 0, -OBSERWATOR_ODLEGLOSC);
  glRotatef(OBSERWATOR_OBROT_X, 1, 0, 0);
  glRotatef(OBSERWATOR_OBROT_Y, 0, 1, 0);
  glRotatef(OBSERWATOR_OBROT_Z, 0, 0, 1);
  OswietlenieOgolne();
  //UstawDomyslneWartosciParametrow();
  //DefiniujMaterial(0);
  Material1();
  // Narysowanie szescianu
  RysujWalec(promien, wysokosc);
  
  RysujNakladke();
  // Przelaczenie buforow ramki
  glutSwapBuffers();
 
  
}

void ObslugaKlawiszySpecjalnych(int klawisz, int x, int y)
{
  switch(klawisz)
    {
    case GLUT_KEY_UP:
      OBSERWATOR_OBROT_X += 3.0;
      break;
 
    case GLUT_KEY_DOWN:
      OBSERWATOR_OBROT_X -=3.0;
      break;
 
    case GLUT_KEY_LEFT:
      OBSERWATOR_OBROT_Y +=3.0;
	  break;
	    
 
    case GLUT_KEY_RIGHT:
      OBSERWATOR_OBROT_Y -=3.0;
      break;
	
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// Funkcja obslugi klawiatury
void ObslugaKlawiatury(unsigned char klawisz, int x, int y)
{
  if(klawisz == '+')
  {
      promien *= 1.01;
	  wsp_wielkosci*=1.01;
  }
  else if (klawisz == '-')
  {
      promien /= 1.01;
	  wsp_wielkosci/=1.01;
  }
  else if(klawisz == 'q')
	  PODZIAL_X +=1;
  else if(klawisz == 'Q')
	  PODZIAL_X -=1;
  else if (klawisz == 'a')
	  glEnable(GL_LIGHT0);
  else if (klawisz == 'A')
	  glDisable(GL_LIGHT0);

   else if (klawisz == 's')
	  glEnable(GL_LIGHT1);
  else if (klawisz == 'S')
	  glDisable(GL_LIGHT1);
    else if (klawisz == 'd')
	  glEnable(GL_LIGHT2);
  else if (klawisz == 'D')
	  glDisable(GL_LIGHT2);

  else if(klawisz == 'w')
	  PODZIAL_Y +=1;
  else if(klawisz == 'W')
	  PODZIAL_Y -=1;
  else if (klawisz == 27)
      exit(0);      

  else if (klawisz == 'x')
		kat_x -= 4.0;
	else if (klawisz == 'X')
		kat_x += 4.0;
	else if (klawisz == 'c')
		kat_y -= 4.0;
	else if (klawisz == 'C')
		kat_y += 4.0;
	else if (klawisz == 'z')
		kat_z -= 4.0;
	else if (klawisz == 'Z')
		kat_z += 4.0;
	else if (klawisz == 'v')
	{
		R_L1 -= 4.0;
		R_L2-=4.0;
	}
	else if (klawisz == 'V')
	{
		R_L1 += 4.0;
		R_L2+=4.0;
	}
	
}


//////////////////////////////////////////////////////////////////////////////////////////
// Glowna funkcja programu
int  main(int argc, char **argv)
{
  // Zainicjowanie biblioteki GLUT
  glutInit(&argc, argv);

  // Ustawienie trybu wyswietlania
  glutInitDisplayMode (GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

  // Ustawienie polozenia dolenego lewego rogu okna
  glutInitWindowPosition(100, 100);

  // Ustawienie rozmiarow okna
  glutInitWindowSize(szerokoscOkna, wysokoscOkna);

  // Utworzenie okna
  glutCreateWindow("Walec - GL_POLYGON");

  // Odblokowanie bufora glebokosci
  glEnable(GL_DEPTH_TEST);

  // Ustawienie wartosci czyszczacej zawartosc bufora glebokosci
  glClearDepth(1000.0);

  // Ustawienie koloru czyszczenia bufora ramki
  glClearColor (0.0f, 0.0f, 0.3f, 0.0f);

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

  glutSpecialFunc(ObslugaKlawiszySpecjalnych);

  //UstawDomyslneWartosciParametrow();
  // Obsluga glownej petli programu (wywolywanie zarejestrowanych callbackow
  // w odpowiedzi na odbierane zdarzenia lub obsluga stanu bezczynnosci)
  glutMainLoop();

  return 0;
}
