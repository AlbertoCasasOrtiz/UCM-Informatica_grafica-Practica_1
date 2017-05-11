#ifndef MATRIX_H
#define MATRIX_H

#include <gl/GL.h>
#include <math.h>
#include "PV3D.h"

class SquareMatrix {
private:
	/*Matriz representada como un array de arrays.*/
	GLfloat **matrix;

	/*Tamaño de la matriz.*/
	const GLint TAM = 4;
public:

	/*Crea una matriz a partir de los puntos y vectores dados.*/
	SquareMatrix(PV3D* v1, PV3D* v2, PV3D *v3, PV3D *v4) {
		/*Inicialización de las variables.*/
		this->matrix = new GLfloat*[TAM];
		for (GLint i = 0; i < TAM; i++) {
			this->matrix[i] = new GLfloat[TAM];
		}
		
		/*Inserción de las variables en la matrix.*/
		this->matrix[0][0] = v1->getX();
		this->matrix[1][0] = v1->getY();
		this->matrix[2][0] = v1->getZ();
		this->matrix[3][0] = v1->isVectorOrNormal();
		this->matrix[0][1] = v2->getX();
		this->matrix[1][1] = v2->getY();
		this->matrix[2][1] = v2->getZ();
		this->matrix[3][1] = v2->isVectorOrNormal();
		this->matrix[0][2] = v3->getX();
		this->matrix[1][2] = v3->getY();
		this->matrix[2][2] = v3->getZ();
		this->matrix[3][2] = v3->isVectorOrNormal();
		this->matrix[0][3] = v4->getX();
		this->matrix[1][3] = v4->getY();
		this->matrix[2][3] = v4->getZ();
		this->matrix[3][3] = v4->isVectorOrNormal();
	}

	/*Destructora de la clase SquareMatrix.*/
	~SquareMatrix() {
		for (GLint i = 0; i < this->TAM; i++) {
			delete this->matrix[i];
		}
		delete this->matrix;
	}

	/*Métodos de la clase.*/

	/*Multiplica esta matriz por un punto.*/
	PV3D* multiplica(PV3D* p) {
		PV3D * res = new PV3D(0, 0, 0, 0);
		GLfloat value;
		for (GLint i = 0; i < TAM; i++) {
			value = 0;
			for (GLint j = 0; j < TAM; j++) {
				value += this->matrix[i][j] * p->get(j);
			}
			res->set(i, value);
		}
		return res;
	}

	GLfloat getElem(GLint x, GLint y) {
		return this->matrix[x][y];
	}

};


#endif