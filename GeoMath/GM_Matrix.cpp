/********************************************************************
* File: GM_Matrix.cpp												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_Matrix.h"



/*!
Default constructor
*/
GM_Matrix::GM_Matrix(void) {
	mNumCol = 0;
	mNumRow = 0;
	pMatrix = NULL;
}



/*!
Copy constructor
*/
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



/*!
Constructor from row and column numbers

\param theNumCol
Number of columns
\param theNumRow
Number of rows
*/
GM_Matrix::GM_Matrix(unsigned short theNumCol, unsigned short theNumRow) {
	allocate(theNumCol, theNumRow);
}



/*!
Default destructor
*/
GM_Matrix::~GM_Matrix(void) {
}



/*!
Invalidate the matrix and free the memory
*/
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



/*!
\return
true if the matrix is valid, false otherwise
*/
bool GM_Matrix::isValid() const {
	if (pMatrix && mNumCol > 0 && mNumRow > 0)
		return true;
	else
		return false;
}



/*!
Allocate the matrix with specified rows and columns

\param theNumCol
\param theNumRow
*/
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


const double* GM_Matrix::operator[](unsigned short theRow) const {
	if (theRow < mNumRow)
		return pMatrix[theRow];
	else
		return NULL;
}


GM_Matrix& GM_Matrix::operator=(const GM_Matrix& theMatrix) {
	if (isValid() && theMatrix.isValid() && mNumCol == theMatrix.mNumCol && mNumRow == theMatrix.mNumRow) {
		for (unsigned short i = 0 ; i < mNumRow ; i++) {
			for (unsigned short j = 0 ; j < mNumCol ; j++) {
				pMatrix[i][j] = theMatrix[i][j];
			}
		}
	}
	else {
		invalidate();
	}

	return *this;
}


bool GM_Matrix::operator != (const GM_Matrix& theMatrix) const {
	return !((*this) == theMatrix);
}


bool GM_Matrix::operator == (const GM_Matrix& theMatrix) const {
	if (!isValid() || !theMatrix.isValid() || mNumCol != theMatrix.mNumCol || mNumRow != theMatrix.mNumRow)
		return false;

	bool ret = true;
	for (unsigned short i = 0 ; i < mNumRow && ret ; i++) {
		for (unsigned short j = 0 ; j < mNumCol && ret ; j++) {
			if (fabs(pMatrix[i][j] - theMatrix[i][j]) > GM_NULL_TOLERANCE) {
				ret = false;
			}
		}
	}

	return ret;
}


GM_Matrix GM_Matrix::operator*(const GM_Matrix& theMatrix) const {
	GM_Matrix ret;
	if (!isValid() || !theMatrix.isValid() || mNumCol != theMatrix.mNumRow || mNumRow != theMatrix.mNumCol)
		return ret;

	for (unsigned short i = 0 ; i < mNumRow ; i++) {
		for (unsigned short j = 0 ; j < mNumCol ; j++) {
			for (unsigned short k = 0 ; k < mNumCol ; k++) {
				ret[i][j] += pMatrix[i][k]*theMatrix[k][j];
			}
		}
	}

	return ret;
}




bool GM_Matrix::isQuad() const {
	if (mNumCol == mNumRow)
		return true;
	else
		return false;
}


double GM_Matrix::determinant() const {
	double ret = 0.0;
	if (!isValid() || !isQuad())
		return ret;

	gaussjordanElim(ret);
	return ret;
}



GM_Matrix GM_Matrix::gaussjordanElim(double& theDet) const {
	GM_Matrix ret;
	theDet = 0.0;

	if (isValid()) {
		ret = (*this);
		double detMult = 1.0;

		for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
			// Se esiste nella colonna colNdx un elemento != 0 si posta quello con modulo maggior sulla riga
			// con lo stesso indice (colNdx) scambiando le righe
			
			// Cerca nella colonna colNdx la riga con l' elemento di modulo maggiore e la memorizza in
			// rowToSwap (pivoting)
			bool rowFound = false;
			unsigned short rowToSwap = 0;
			for (unsigned short rowNdx = 0 ; rowNdx < mNumRow ; rowNdx++) {
				if (ret[rowNdx][colNdx] != 0.0) {
					if (rowFound) {
						if (fabs(ret[rowNdx][colNdx]) > fabs(ret[rowToSwap][colNdx])) {
							rowToSwap = rowNdx;
						}
					}
					else {
						rowToSwap = rowNdx;
					}
					rowFound = true;
				}
			}

			if (rowFound) {
				// Scambia la riga col pivot con quella di indice colNdx
				ret.rowSwap(rowToSwap, colNdx);
				if (rowToSwap != colNdx) {
					detMult *= -1.0;
				}
				
				// Si porta a 1 il pivot con un opportuna moltiplicazione della sua riga
				ret.rowMult(colNdx, 1.0 / ret[colNdx][colNdx]);
				detMult *= 1.0 / ret[colNdx][colNdx];
				
				// Poi con moltiplicazioni e somme si azzerano tutti gli elementi sopra e sotto
				for (unsigned short rowNdx = 0 ; rowNdx < mNumRow ; rowNdx++) {
					if (rowNdx != colNdx && ret[rowNdx][colNdx] != 0) {
						ret.rowSum(rowNdx, colNdx, -(1.0 / ret[rowNdx][colNdx]));
					}
				}
			}
		}
	
		if (isQuad()) {
			for (unsigned short i = 0 ; i < mNumCol ; i++) {
				detMult *= ret[i][i];
			}
			if (fabs(detMult) > GM_NULL_TOLERANCE) {
				theDet = 1.0 / detMult;
			}
			else {
				theDet = 0.0;
			}
		}
	}

	return ret;
}



GM_Matrix GM_Matrix::gaussjordanElim() const {
	double dummyDouble = 0.0;
	return gaussjordanElim(dummyDouble);
}



// Scambio di righe
void GM_Matrix::rowSwap(unsigned short theRow1, unsigned short theRow2) {
	if (theRow1 >= mNumRow || theRow2 >= mNumRow || theRow1 == theRow2)
		return;

	for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
		double swap = pMatrix[theRow1][colNdx];
		pMatrix[theRow1][colNdx] = pMatrix[theRow2][colNdx];
		pMatrix[theRow2][colNdx] = swap;
	}
}

// Somma di una righa per un altra moltiplicata per una costante != 0
void GM_Matrix::rowSum(unsigned short theRow, unsigned short theRowToAdd, double theMult) {
	if (theRow >= mNumRow || theRowToAdd >= mNumRow || theRow == theRowToAdd || fabs(theMult) < GM_NULL_TOLERANCE)
		return;

	for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
		pMatrix[theRow][colNdx] += (pMatrix[theRowToAdd][colNdx] * theMult);
	}
}

// Moltiplicazione di una riga per una costante != 0
void GM_Matrix::rowMult(unsigned short theRow, double theMult) {
	if (theRow >= mNumRow || fabs(theMult) < GM_NULL_TOLERANCE)
		return;

	for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
		pMatrix[theRow][colNdx] *= theMult;
	}
}


GM_Matrix GM_Matrix::inverse() const {
	GM_Matrix ret;
	if (!isValid() || !isQuad() || determinant() == 0.0)
		return ret;

	GM_Matrix compMat(2 * mNumCol, mNumRow);
	for (unsigned short i = 0 ; i < mNumRow ; i++) {
		for (unsigned short j = 0 ; j < 2 * mNumCol ; j++) {
			if (j < mNumCol) {
				compMat[i][j] = pMatrix[i][j];
			}
			else {
				if (j - mNumCol == i) {
					compMat[i][j] = 1.0;
				}
				else {
					compMat[i][j] = 0.0;
				}
			}
		}
	}

	GM_Matrix compMatRed = gaussjordanElim();

	for (unsigned short i = 0 ; i < mNumRow ; i++) {
		for (unsigned short j = 0 ; j < mNumCol ; j++) {
			ret[i][j] = compMatRed[i][j];
		}
	}

	return ret;
}



	
GM_Matrix GM_Matrix::transpose() const {
	if (!isValid())
		return (*this);

	GM_Matrix ret(mNumRow, mNumCol);
	for (unsigned short i = 0 ; i < mNumRow ; i++) {
		for (unsigned short j = 0 ; j < mNumCol ; j++) {
			ret[j][i] = pMatrix[i][j];
		}
	}

	return ret;
}


void GM_Matrix::setIdentity() {
	if (!isValid() || !isQuad())
		return;

	for (unsigned short i = 0 ; i < mNumRow ; i++) {
		for (unsigned short j = 0 ; j < mNumCol ; j++) {
			if (i == j) {
				pMatrix[i][j] = 1.0;
			}
			else {
				pMatrix[i][j] = 0.0;
			}
		}
	}
}
