#ifndef FICHA_H
#define FICHA_H

#include "Quesito.h"
#include "Malla.h"

class Ficha : public Malla{
private:
	/*Número de vértces en el perfil.*/
	const GLint numVerticesPerfil = 3;

	/*Perfil que generará el tubo.*/
	PV3D** perfil;

	/*Crea el perfil.*/
	void creaPerfil(GLfloat radio, GLfloat altura) {
		this->perfil[0] = new PV3D(radio, 0.0, 0.0, 1);
		this->perfil[1] = new PV3D(radio, altura, 0.0, 1);
		this->perfil[2] = new PV3D(0.0, altura, 0.0, 1);
	}

	/*Número de fichas que forman el quesito.*/
	const GLint NUM_FICHAS = 6;
public:

	/*
	Constructora de la clase Ficha.
	nQ = número de lados de la parte curva de la ficha.
	radio = radio de la ficha.
	altura = altura de la ficha.
	*/
	Ficha(GLint nQ, GLint radio, GLint altura) {
		GLfloat incr = 0;

		/*Inicializamos malla y perfil.*/
		Malla();
		this->perfil = new PV3D*[this->numVerticesPerfil];

		/*Creamos el perfil.*/
		this->creaPerfil(radio, altura);


		for (GLint i = 0; i < NUM_FICHAS; i++) {
			/*Obtiene el color de la ficha*/
			Color *color = this->getColorFicha(i);

			/*Llamamos al método revolución, que generará los vértices y las caras.*/
			this->revolución(this->perfil, this->numVerticesPerfil, nQ, radio, altura, incr, PI / 3 - 0.03, color);

			/*Guardamos los últimos vértices generados.*/
			GLint verticeUltimaArriba = this->getNumVertices() - 3;
			GLint verticeUltimaAbajo = this->getNumVertices() - 2;

			/*Ahora creamos las caras laterales.*/

			/*Generamos los vértices centraes.*/
			this->insertaVertice(0.0, altura, 0.0);
			this->insertaVertice(0.0, 0.0, 0.0);

			/*Cara lateral izquierda.*/
			this->creaCara(4);

			/*Se multiplica el número de vértices por nQ para no sobreescribir caras anteriores, y se resta 2 por las 2 que hemos añadido de vértices centrales.*/
			this->insertaVerticeEnCara(this->getNumCaras() - 1, this->getNumVertices() - (nQ+1)*this->numVerticesPerfil - 2 + 1, this->getNumNormales());
			this->insertaVerticeEnCara(this->getNumCaras() - 1, this->getNumVertices() - 2, this->getNumNormales());
			this->insertaVerticeEnCara(this->getNumCaras() - 1, this->getNumVertices() - 1, this->getNumNormales());
			this->insertaVerticeEnCara(this->getNumCaras() - 1, this->getNumVertices() - (nQ + 1)*this->numVerticesPerfil - 2, this->getNumNormales());

			PV3D* v = this->CalculoVectorNormalPorNewell(this->getCara(this->getNumCaras() - 1));

			this->insertaNormal(v->getX(), v->getY(), v->getZ());
			this->insertaColor(color->getR(), color->getG(), color->getB());

			/*Cara lateral derecha.*/
			this->creaCara(4);

			this->insertaVerticeEnCara(this->getNumCaras() - 1, verticeUltimaAbajo, this->getNumNormales());
			this->insertaVerticeEnCara(this->getNumCaras() - 1, verticeUltimaArriba, this->getNumNormales());
			this->insertaVerticeEnCara(this->getNumCaras() - 1, this->getNumVertices() - 1, this->getNumNormales());
			this->insertaVerticeEnCara(this->getNumCaras() - 1, this->getNumVertices() - 2, this->getNumNormales());

			v = this->CalculoVectorNormalPorNewell(this->getCara(this->getNumCaras() - 1));

			this->insertaNormal(v->getX(), v->getY(), v->getZ());
			this->insertaColor(color->getR(), color->getG(), color->getB());

			/*El incremento para la posición del siguiente quesito.*/
			incr += 2 * PI / 6;
		}
	}

	/*Devuelve el color de cada una de las fichas.*/
	Color* getColorFicha(GLint i) {
		switch (i) {
		case 0:
			return new Color("cyan");
			break;
		case 1:
			return new Color("brown");
			break;
		case 2:
			return new Color("green");
			break;
		case 3:
			return new Color("magenta");
			break;
		case 4:
			return new Color("yellow");
			break;
		case 5:
			return new Color("orange");
			break;
		}
	}
};

#endif