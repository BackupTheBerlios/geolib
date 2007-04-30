/********************************************************************
* File: GM_Matrix.h													*
*********************************************************************
* Descrizione:														*
*********************************************************************
* History:															*
* 22.4.2007 Creato da : Cordara Claudio								*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once


class GM_Matrix {
public:
	GM_Matrix();
	GM_Matrix(const GM_Matrix& theMatrix);
	GM_Matrix(unsigned short theNumCol, unsigned short theNumRow);
	~GM_Matrix();

	void invalidate();
	bool isValid() const;

	double* operator[](unsigned short theRow);
private:
	void allocate(unsigned short theNumCol, unsigned short theNumRow);

	unsigned short mNumCol;
	unsigned short mNumRow;
	double **pMatrix;
};
