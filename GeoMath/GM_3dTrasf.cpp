/********************************************************************
* File: GM_3dTrasf.cpp												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_3dBasis.h"
#include "GM_3dTrasf.h"



GM_3dTrasf::GM_3dTrasf() : GM_Matrix(3, 3) {
	setIdentity();
}



GM_3dTrasf::~GM_3dTrasf() {
}


GM_3dTrasf::GM_3dTrasf(const GM_3dBasis& startBase, const GM_3dBasis& endBase) {
	if (startBase.isValid() && endBase.isValid() && startBase.isLinearlyInd() && endBase.isLinearlyInd()) {

		// Matrice ottenuta affiancando i vettori di endBase seguiti da quelli di startBase, i vettori
		// rappresentano le colonne della matrice
		GM_Matrix mat(6, 3);
		for (unsigned short i = 0 ; i < 6 ; i++) {
			if (i < 3) {
				mat[0][i] = endBase[i].x();
				mat[1][i] = endBase[i].y();
				mat[2][i] = endBase[i].z();
			}
			else {
				mat[0][i] = startBase[i-3].x();
				mat[1][i] = startBase[i-3].y();
				mat[2][i] = startBase[i-3].z();
			}
		}

		// Riduzione con Gauss-Jordan
		mat.gaussjordanElim();
		
		// La matrice quadrata di destra contiene la matrice di transizione da startBase a endBase, infatti
		// è composta dai vettori di startBase espressi secondo quelli in endBase
		for (unsigned short i = 0 ; i < 3 ; i++) {
			for (unsigned short j = 3 ; j < 6 ; j++) {
				pMatrix[i][j-3] = mat[i][j];
			}
		}
	}
}


GM_3dVector GM_3dTrasf::trasf(const GM_3dVector& theVect) const {
	if (!theVect.isValid())
		return theVect;

	GM_3dVector ret = theVect;
	ret.x(theVect.x()*pMatrix[0][0] + theVect.y()*pMatrix[1][0] + theVect.z()*pMatrix[2][0]);
	ret.y(theVect.x()*pMatrix[0][1] + theVect.y()*pMatrix[1][1] + theVect.z()*pMatrix[2][1]);
	ret.z(theVect.x()*pMatrix[0][2] + theVect.y()*pMatrix[1][2] + theVect.z()*pMatrix[2][0]);

	return ret;
}


GM_3dPoint GM_3dTrasf::trasf(const GM_3dPoint& thePoint) const {
	if (!thePoint.isValid())
		return thePoint;

	GM_3dPoint ret = thePoint;
	ret.x(thePoint.x()*pMatrix[0][0] + thePoint.y()*pMatrix[1][0] + thePoint.z()*pMatrix[2][0]);
	ret.y(thePoint.x()*pMatrix[0][1] + thePoint.y()*pMatrix[1][1] + thePoint.z()*pMatrix[2][1]);
	ret.z(thePoint.x()*pMatrix[0][2] + thePoint.y()*pMatrix[1][2] + thePoint.z()*pMatrix[2][0]);

	return ret;
}
