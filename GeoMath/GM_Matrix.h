/********************************************************************
* File: GM_Matrix.h													*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once



/*!
Row by column matrix, that is matrix[r][c] is the element of the matrix at row r and column c
*/
class GM_Matrix {
public:
	GM_Matrix();
	GM_Matrix(const GM_Matrix& theMatrix);
	GM_Matrix(unsigned short theNumCol, unsigned short theNumRow);
	virtual ~GM_Matrix();

	double* operator[](unsigned short theRow);
	const double* operator[](unsigned short theRow) const;
	GM_Matrix& operator=(const GM_Matrix& theMatrix);
	bool operator == (const GM_Matrix& theMatrix) const;
	bool operator != (const GM_Matrix& theMatrix) const;
	GM_Matrix operator*(const GM_Matrix& theMatrix) const;

	bool isQuad() const;

	GM_Matrix gaussjordanElim() const;
	GM_Matrix gaussjordanElim(double& theDet) const;
	double determinant() const;
	GM_Matrix inverse() const;
	GM_Matrix transpose() const;

	void setIdentity();

	void invalidate();
	bool isValid() const;

protected:
	void allocate(unsigned short theNumCol, unsigned short theNumRow);

	void rowSwap(unsigned short theRow1, unsigned short theRow2);
	void rowSum(unsigned short theRow, unsigned short theRowToAdd, double theMult);
	void rowMult(unsigned short theRow, double theMult);

	/*!
	Number of columns
	*/
	unsigned short mNumCol;
	/*!
	Number of rows
	*/
	unsigned short mNumRow;
	/*!
	Pointer to the matrix head, is an array of arrays of double
	*/
	double **pMatrix;
};
