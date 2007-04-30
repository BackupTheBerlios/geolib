/********************************************************************
* File: GM_Matrix.cpp												*
*********************************************************************
* Descrizione:														*
*********************************************************************
* History:															*
* 22.4.2007 Creato da : Cordara Claudio								*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_Matrix.h"



GM_Matrix::GM_Matrix(void) {
	mNumCol = 0;
	mNumRow = 0;
	pMatrix = NULL;
}


GM_Matrix::GM_Matrix(const GM_Matrix& theMatrix) {
	if (theMatrix.isValid()) {
		allocate(theMatrix.mNumCol, theMatrix.mNumRow);
		if (isValid()) {
			for (unsigned short i = 0 ; i < mNumRow ; i++) {
				for (unsigned short j = 0 ; i < mNumCol ; j++) {
					pMatrix[i][j] = (theMatrix.pMatrix)[i][j];
				}
			}
		}
	}
}


GM_Matrix::GM_Matrix(unsigned short theNumCol, unsigned short theNumRow) {
	allocate(theNumCol, theNumRow);
}



GM_Matrix::~GM_Matrix(void) {
}


void GM_Matrix::invalidate() {
	if (pMatrix) {
		for (unsigned short i = 0 ; i < mNumRow ; i++) {
			free(pMatrix[i]);
		}
		free(pMatrix);
	}
	mNumCol = 0;
	mNumRow = 0;
	pMatrix = NULL;
}


bool GM_Matrix::isValid() const {
	if (pMatrix && mNumCol > 0 && mNumRow > 0)
		return true;
	else
		return false;
}


void GM_Matrix::allocate(unsigned short theNumCol, unsigned short theNumRow) {
	invalidate();
	if (theNumCol == 0 || theNumRow == 0) {
		return;
	}

	pMatrix = (double**) calloc(sizeof(double*), theNumRow);
	if (!pMatrix) {
		invalidate();
		return;
	}
	for (unsigned short i = 0 ; i < mNumRow ; i++) {
		pMatrix[i] = (double*) calloc(sizeof(double), theNumCol);
		if (!pMatrix[i]) {
			invalidate();
			return;
		}
	}

	mNumCol = theNumCol;
	mNumRow = theNumRow;
}


double* GM_Matrix::operator[](unsigned short theRow) {
	if (theRow < mNumRow)
		return pMatrix[theRow];
	else
		return NULL;
}
