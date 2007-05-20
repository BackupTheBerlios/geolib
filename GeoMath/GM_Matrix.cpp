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
Number of columns
\param theNumRow
Number of rows
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



/*!
Get a pointer to a specified row of the matrix

\param theRow
Row to get

\return
A pointer to the specified row, or NULL if theRow is out of range
*/
double* GM_Matrix::operator[](unsigned short theRow) {
	if (theRow < mNumRow)
		return pMatrix[theRow];
	else
		return NULL;
}



/*!
Get a const pointer to a specified row of the matrix

\param theRow
Row to get

\return
A const pointer to the specified row, or NULL if theRow is out of range
*/
const double* GM_Matrix::operator[](unsigned short theRow) const {
	if (theRow < mNumRow)
		return pMatrix[theRow];
	else
		return NULL;
}

/*!
Assignment operator

\param theMatrix
Matrix to assign to this

\return
A refence to this overwritten by theMatrix, if theMatrix is invalid this is invalidated and
returned
*/
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



/*!
Equality operator

\param theMatrix
Matrix to compare with this

\return
true if theMatrix is not equal to this within tolerance GM_NULL_TOLERANCE, true otherwise, return true if
this or theMatrix are invalid
*/
bool GM_Matrix::operator != (const GM_Matrix& theMatrix) const {
	return !((*this) == theMatrix);
}



/*!
Equality operator

\param theMatrix
Matrix to compare with this

\return
true if theMatrix is equal to this within tolerance GM_NULL_TOLERANCE, false otherwise or if this or
theMatrix are invalid
*/
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



/*!
Row by columns product between two matrix

\param
Matrix to use with this in row by column product

\return
The matrix obtained from the row by column product between this and theMatrix, if this or theMatrix are invalid
or incompatible
*/
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



/*!
\return
true if this is a square matrix, false otherwise
*/
bool GM_Matrix::isQuad() const {
	if (isValid() && mNumCol == mNumRow)
		return true;
	else
		return false;
}



/*!
Compute the determinant of this using the Gauss-Jordan elimination algorithm

\return
The determinant of this, or DBL_MAX if this is invalid or is not a square matrix
*/
double GM_Matrix::determinant() const {
	double ret = DBL_MAX;
	if (!isValid() || !isQuad())
		return ret;

	gaussjordanElim(ret);
	return ret;
}



/*!
Compute the matrix obtained from the application of the Gauss-Jordan elimination algorithm to this and in the
same time compute the determinant if is possible.

\param theDet
On output contains the determinat of this, if this is valid and is a square matrix, otherwise contains DBL_MAX

\return
The matrix obtained from the Gauss-Jordan elimination algorithm on this, return an invalid matrix if this is
not valid
*/
GM_Matrix GM_Matrix::gaussjordanElim(double& theDet) const {
	GM_Matrix ret;
	theDet = DBL_MAX;

	if (isValid()) {
		ret = (*this);
		double detMult = 1.0;

		for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
			// If in column colNdx there is at least an element != 0, move the biggest (in module) un the
			// colNdx-th row
			
			// Find in colNdx-th column the biggest (in module) element, the pivot
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
				// Swap the row containing the pivot element with the colNdx-th row
				ret.rowSwap(rowToSwap, colNdx);
				if (rowToSwap != colNdx) {
					detMult *= -1.0;
				}
				
				// Set the pivot element to 1, multiply its row by 1/pivot, keep trace of the
				// determinant value
				ret.rowMult(colNdx, 1.0 / ret[colNdx][colNdx]);
				detMult *= 1.0 / ret[colNdx][colNdx];

				// Reduce to 0 the elements above and below the pivot with the necessary
				// multiplication and addition of rows
				for (unsigned short rowNdx = 0 ; rowNdx < mNumRow ; rowNdx++) {
					if (rowNdx != colNdx && ret[rowNdx][colNdx] != 0) {
						ret.rowSum(rowNdx, colNdx, -(1.0 / ret[rowNdx][colNdx]));
					}
				}
			}
		}
	
		if (isQuad()) {
			// This is a square matrix, compute the determinant

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



/*!
Compute the matrix obtained from the application of the Gauss-Jordan elimination algorithm to this without
determinant computation

\return
The matrix obtained from the Gauss-Jordan elimination algorithm on this, return an invalid matrix if this is
not valid
*/
GM_Matrix GM_Matrix::gaussjordanElim() const {
	double dummyDouble = 0.0;
	return gaussjordanElim(dummyDouble);
}



/*!
Swap two rows of the matrix by its index, nothing happens if the index are out of range

\param theRow1
Index of the first row
\param theRow2
Index of the second row
*/
void GM_Matrix::rowSwap(unsigned short theRow1, unsigned short theRow2) {
	if (theRow1 >= mNumRow || theRow2 >= mNumRow || theRow1 == theRow2)
		return;

	for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
		double swap = pMatrix[theRow1][colNdx];
		pMatrix[theRow1][colNdx] = pMatrix[theRow2][colNdx];
		pMatrix[theRow2][colNdx] = swap;
	}
}


/*!
Sum up a row with another row multiplied with a constant != 0, nothing happens if the index are out of range
or theMult is zero

\param theRow
Base row
\param theRowToAdd
Row to add
\param theMult
Multiplication constant of theRowToAdd
*/
void GM_Matrix::rowSum(unsigned short theRow, unsigned short theRowToAdd, double theMult) {
	if (theRow >= mNumRow || theRowToAdd >= mNumRow || theRow == theRowToAdd || fabs(theMult) < GM_NULL_TOLERANCE)
		return;

	for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
		pMatrix[theRow][colNdx] += (pMatrix[theRowToAdd][colNdx] * theMult);
	}
}



/*!
Muliply a row by a constant != 0, nothing happens if the index are out of range, or theMult is zero

\param theRow
Row to multiply
\param theMult
Multiplier
*/
void GM_Matrix::rowMult(unsigned short theRow, double theMult) {
	if (theRow >= mNumRow || fabs(theMult) < GM_NULL_TOLERANCE)
		return;

	for (unsigned short colNdx = 0 ; colNdx < mNumCol ; colNdx++) {
		pMatrix[theRow][colNdx] *= theMult;
	}
}



/*!
Compute the inverse of this, if it is possible, using the Gauss-Jordan elimination algorithm

\return
The inverse of this if this is a valid square matrix with determinand != 0, an invalid matrix otherwise
*/
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



/*!
Compute the transposed matrix of this

\return
The transposed matrix of this, or an invalid matrix if this is invalid
*/
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



/*!
Set this to the identity matrix
*/
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
