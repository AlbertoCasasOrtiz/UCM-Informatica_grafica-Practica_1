#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include <GL/freeglut.h>
//#include <GL/glut.h>

#include "Quesito.h"
#include "Ficha.h"
#include "MontañaRusa.h"
#include "Coche.h"
#include "Cilindro.h"

#include <iostream>
using namespace std;

// Freeglut parameters
// Flag telling us to keep processing events
// bool continue_in_main_loop= true; //(**)

// Viewport size
int WIDTH= 500, HEIGHT= 500;

// Viewing frustum parameters
GLdouble xRight=10, xLeft=-xRight, yTop=10, yBot=-yTop, N=1, F=1000;

// Camera parameters
GLdouble eyeX=100.0, eyeY=100.0, eyeZ=100.0;
GLdouble lookX=0.0, lookY=0.0, lookZ=0.0;
GLdouble upX=0, upY=1, upZ=0;

/*Variables globales para control por teclado.*/
GLfloat girox = 0.0;
GLfloat giroy = 0.0;
GLfloat giroz = 0.0;
GLfloat gradoGiro = 2.0;

GLfloat cocheMueve = 0.0;
GLfloat avanceCoche = 0.05;

GLboolean muestraNormal = false;
GLboolean dibujaMon = false, dibujaQues = true, dibujaFicha = false, dibujaCoche = false;

/*Definimos los objetos.*/
Quesito *quesito;
Ficha *ficha;
MontañaRusa *mon;
Coche *coche;

GLfloat i = 0, j = 0, k = 0, l = 0;

/*Llamamos a la constructoras de los objetos.*/
void buildSceneObjects() {
	quesito = new Quesito(20, 7, 5, new Color("purple"));
	cout << "Quesito: " << endl;
	cout << "Num vértices: " << quesito->getNumVertices() << endl;
	cout << "Num caras: " << quesito->getNumCaras() << endl;
	cout << "Num normales: " << quesito->getNumNormales() << endl;
	cout << endl;

	ficha = new Ficha(20, 7, 5);
	cout << "Ficha: " << endl;
	cout << "Num vértices: " << ficha->getNumVertices() << endl;
	cout << "Num caras: " << ficha->getNumCaras() << endl;
	cout << "Num normales: " << ficha->getNumNormales() << endl;
	cout << endl;

	mon = new MontañaRusa(100, 20, 1, 3, new Color("white"));
	cout << "Montaña Rusa: " << endl;
	cout << "Num vértices: " << mon->getNumVertices() << endl;
	cout << "Num caras: " << mon->getNumCaras() << endl;
	cout << "Num normales: " << mon->getNumNormales() << endl;
	cout << endl;

	coche = new Coche(0.5, 0.7, 0.7, new Color("purple"), new Color("blue"));
	cout << "Coche: " << endl;
	cout << "Num vértices: " << coche->getNumVertices() << endl;
	cout << "Num caras: " << coche->getNumCaras() << endl;
	cout << "Num normales: " << coche->getNumNormales() << endl;
	cout << endl;
}

void initGL() {	 		 
	glClearColor(0.6f,0.7f,0.8f,1.0);
    glEnable(GL_LIGHTING);    

	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.1f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	buildSceneObjects();

	// Camera set up
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

	// Frustum set up
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();     
	glOrtho(xLeft, xRight, yBot, yTop, N, F);

	// Viewport set up
    glViewport(0, 0, WIDTH, HEIGHT);  

	 // Light0
    glEnable(GL_LIGHT0);
    GLfloat d[]={1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    GLfloat a[]={0.3f,0.3f,0.3f,1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, a);
	GLfloat p[]={-25.0, 25.0, 25.0, 1.0};	 
	glLightfv(GL_LIGHT0, GL_POSITION, p);
 }


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
	
	// Drawing axes
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0); 
		glVertex3f(0, 0, 0);
		glVertex3f(20, 0, 0);	     
	 
		glColor3f(0.0,1.0,0.0); 
		glVertex3f(0, 0, 0);
		glVertex3f(0, 20, 0);	 
	 
		glColor3f(0.0,0.0,1.0); 
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 20);	     
	glEnd();

	
	/*Espacio de trabajo.*/
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(girox, 1.0, 0.0, 0.0);
	glRotatef(giroy, 0.0, 1.0, 0.0);
	glRotatef(giroz, 0.0, 0.0, 1.0);

		if(dibujaMon)mon->dibuja();
		if(dibujaQues)quesito->dibuja();
		if (dibujaFicha) {
			ficha->dibuja();
		}
		/*Dibuja el coche dentro de la curva.*/
		GLfloat *m;
		if (dibujaCoche) {
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			m = Curva::getFrenetVector(cocheMueve, 3);
			glMultMatrixf(m);

			/*Derrapamos.*/
			if (cocheMueve >= PI/4 - 0.2 && cocheMueve <= PI/4 + 0.2
				|| cocheMueve >= 5 * PI / 4 - 0.2 && cocheMueve <= 5 * PI / 4 + 0.2) {
				glTranslatef(0.0, j, 0.0);
				glRotatef(i, 1.0, 0.0, 0.0);
			}
			/*Corregimos la inclinación tras derrapar.*/
			else if (cocheMueve >= PI / 4 + 0.2 && cocheMueve <= PI / 4 + 0.6
				|| cocheMueve >= 5 * PI / 4 + 0.2 && cocheMueve <= 5 * PI / 4 + 0.6) {
				glTranslatef(0.0, j, 0.0);
				glRotatef(i, 1.0, 0.0, 0.0);
			}
			else {
				i = 0;
				j = 0;
			}


			/*Derrapamos.*/
			if (cocheMueve >= 3 * PI / 4 - 0.2 && cocheMueve <= 3 * PI / 4 + 0.2
				|| cocheMueve >= 7 * PI / 4 - 0.2 && cocheMueve <= 7 * PI / 4 + 0.2
				) {
				glTranslatef(0.0, -l, 0.0);
				glRotatef(-k, 1.0, 0.0, 0.0);
			}
			/*Corregimos la inclinación tras derrapar.*/
			else if (cocheMueve >= 3 * PI / 4 + 0.2 && cocheMueve <= 3 * PI / 4 + 0.6
				|| cocheMueve >= 7 * PI / 4 + 0.2 && cocheMueve <= 7 * PI / 4 + 0.6
				) {
				glTranslatef(0.0, -l, 0.0);
				glRotatef(-k, 1.0, 0.0, 0.0);
			}
			else {
				k = 0;
				l = 0;
			}

			coche->dibuja();
			glPopMatrix();
		}

		
		if (muestraNormal) {
			if(dibujaMon)mon->dibujaNormales();
			if(dibujaQues)quesito->dibujaNormales();
			if(dibujaFicha)ficha->dibujaNormales();

			/*Dibuja las normales del coche en su posición de la curva.*/
			if (dibujaCoche) {
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
				glMultMatrixf(m);

				/*Derrapamos.*/
				if (cocheMueve >= PI / 4 - 0.2 && cocheMueve <= PI / 4 + 0.2
					|| cocheMueve >= 5 * PI / 4 - 0.2 && cocheMueve <= 5 * PI / 4 + 0.2) {
					glTranslatef(0.0, j, 0.0);
					glRotatef(i, 1.0, 0.0, 0.0);
				}
				/*Corregimos la inclinación tras derrapar.*/
				else if (cocheMueve >= PI / 4 + 0.2 && cocheMueve <= PI / 4 + 0.6
					|| cocheMueve >= 5 * PI / 4 + 0.2 && cocheMueve <= 5 * PI / 4 + 0.6) {
					glTranslatef(0.0, j, 0.0);
					glRotatef(i, 1.0, 0.0, 0.0);
				}
				else {
					i = 0;
					j = 0;
				}


				/*Derrapamos.*/
				if (cocheMueve >= 3 * PI / 4 - 0.2 && cocheMueve <= 3 * PI / 4 + 0.2
					|| cocheMueve >= 7 * PI / 4 - 0.2 && cocheMueve <= 7 * PI / 4 + 0.2
					) {
					glTranslatef(0.0, -l, 0.0);
					glRotatef(-k, 1.0, 0.0, 0.0);
				}
				/*Corregimos la inclinación tras derrapar.*/
				else if (cocheMueve >= 3 * PI / 4 + 0.2 && cocheMueve <= 3 * PI / 4 + 0.6
					|| cocheMueve >= 7 * PI / 4 + 0.2 && cocheMueve <= 7 * PI / 4 + 0.6
					) {
					glTranslatef(0.0, -l, 0.0);
					glRotatef(-k, 1.0, 0.0, 0.0);
				}
				if (dibujaCoche)coche->dibujaNormales();
				glPopMatrix();
				glPopMatrix();
			}
		}

	glPopMatrix();
	/*Fin de espacio de trabajo.*/

	glFlush();
	glutSwapBuffers();
}



void resize(int newWidth, int newHeight) {
	WIDTH= newWidth;
	HEIGHT= newHeight;
	GLdouble RatioViewPort= (float)WIDTH/(float)HEIGHT;
	glViewport (0, 0, WIDTH, HEIGHT) ;
   
	GLdouble SVAWidth= xRight-xLeft;
	GLdouble SVAHeight= yTop-yBot;
	GLdouble SVARatio= SVAWidth/SVAHeight;
	if (SVARatio >= RatioViewPort) {		 
		GLdouble newHeight= SVAWidth/RatioViewPort;
		GLdouble yMiddle= ( yBot+yTop )/2.0;
		yTop= yMiddle + newHeight/2.0;
		yBot= yMiddle - newHeight/2.0;
    }
	else {      
		GLdouble newWidth= SVAHeight*RatioViewPort;
		GLdouble xMiddle= ( xLeft+xRight )/2.0;
		xRight= xMiddle + newWidth/2.0;
		xLeft=  xMiddle - newWidth/2.0;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();   
	glOrtho(xLeft, xRight, yBot, yTop, N, F);
}


/*Método key para teclas normales.*/
void key(unsigned char key, int x, int y) {
	bool need_redisplay = true;
	switch (key) {
	case 27:  /* Escape key */
			  //continue_in_main_loop = false; // (**)
			  //Freeglut's sentence for stopping glut's main loop (*)
		glutLeaveMainLoop();
		break;
	case 'a':
		giroy -= gradoGiro;
		break;
	case 'z':
		giroy += gradoGiro;
		break;
	case 'g':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'h':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'j':
		muestraNormal = true;
		break;
	case 'k':
		muestraNormal = false;
		break;
	case 'n':
		glEnable(GL_CULL_FACE);
		break;
	case 'm':
		glDisable(GL_CULL_FACE);
		break;
	case 'q':
		cocheMueve += avanceCoche;
		if (cocheMueve >= 2 * PI)
			cocheMueve = 0;
		if (cocheMueve >= PI / 4 - 0.2 && cocheMueve <= PI / 4 + 0.2
			|| cocheMueve >= 5 * PI / 4 - 0.2 && cocheMueve <= 5 * PI / 4 + 0.2) {
			i += 7.0;
			j += 0.15;
		}
		else if (cocheMueve >= PI / 4 + 0.2 && cocheMueve <= PI / 4 + 0.6
			|| cocheMueve >= 5 * PI / 4 + 0.2 && cocheMueve <= 5 * PI / 4 + 0.6) {
			i -= 7;
			j -= 0.15;
		}
		if (cocheMueve >= 3 * PI / 4 - 0.2 && cocheMueve <= 3 * PI / 4 + 0.2
			|| cocheMueve >= 7 * PI / 4 - 0.2 && cocheMueve <= 7 * PI / 4 + 0.2
			) {
			k += 7.0;
			l += 0.15;
		}
		else if (cocheMueve >= 3 * PI / 4 + 0.2 && cocheMueve <= 3 * PI / 4 + 0.6
			|| cocheMueve >= 7 * PI / 4 + 0.2 && cocheMueve <= 7 * PI / 4 + 0.6
			) {
			k -= 7.0;
			l -= 0.15;
		}
		break;
	case 'w':
		cocheMueve -= avanceCoche/2;
		if (cocheMueve <= -2 * PI)
			cocheMueve = 0;
		break;
	case '1':
		girox = giroy = giroz = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		muestraNormal = false;
		dibujaQues = true;
		dibujaFicha = false;
		dibujaMon = false;
		dibujaCoche = false;
		break;
	case '2':
		girox = giroy = giroz = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		muestraNormal = false;
		dibujaQues = false;
		dibujaFicha = true;
		dibujaMon = false;
		dibujaCoche = false;
		break;
	case '3':
		girox = giroy = giroz = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		muestraNormal = false;
		dibujaQues = false;
		dibujaFicha = false;
		dibujaMon = true;
		dibujaCoche = false;
		break;
	case '4':
		girox = giroy = giroz = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		cocheMueve = 0;
		muestraNormal = false;
		dibujaQues = false;
		dibujaFicha = false;
		dibujaMon = false;
		dibujaCoche = true;
		break;
	case '5':
		girox = giroy = giroz = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		cocheMueve = 0;
		muestraNormal = false;
		dibujaQues = false;
		dibujaFicha = false;
		dibujaMon = true;
		dibujaCoche = true;
		break;
	case '0':
		girox = giroy = giroz = 0;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
		muestraNormal = false;
		dibujaQues = false;
		dibujaFicha = false;
		dibujaMon = false;
		break;
	default:
		need_redisplay = false;
		break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}


/*Metodo key para teclas especiales.*/
void key(int key, int x, int y){
	bool need_redisplay = true;
	switch (key) {
		case 27:  /* Escape key */
			//continue_in_main_loop = false; // (**)
			//Freeglut's sentence for stopping glut's main loop (*)
			glutLeaveMainLoop (); 
			break;
		case GLUT_KEY_UP:
			girox-= gradoGiro;
			break;
		case GLUT_KEY_DOWN:
			girox += gradoGiro;
			break;
		case GLUT_KEY_LEFT:
			giroz -= gradoGiro;
			break;
		case GLUT_KEY_RIGHT:
			giroz += gradoGiro;
			break;
		default:
			need_redisplay = false;
			break;
	}

	if (need_redisplay)
		glutPostRedisplay();
}


int main(int argc, char *argv[]){
	cout<< "Starting console..." << endl;

	int my_window; // my window's identifier

	// Initialization
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition (140, 140);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	// Window construction
	my_window = glutCreateWindow("Freeglut 3D-project");
    
	// Callback registration
	glutReshapeFunc(resize);
	glutSpecialFunc(key);
	glutKeyboardFunc(key);
	glutDisplayFunc(display);

	// OpenGL basic setting
	initGL();

	// Freeglut's main loop can be stopped executing (**)
	// while (continue_in_main_loop) glutMainLoopEvent();

	// Classic glut's main loop can be stopped after X-closing the window,
	// using the following freeglut's setting (*)
	glutSetOption (GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION) ;

	// Classic glut's main loop can be stopped in freeglut using (*)
	glutMainLoop(); 

	// We would never reach this point using classic glut
	system("PAUSE"); 
   
	return 0;
}
