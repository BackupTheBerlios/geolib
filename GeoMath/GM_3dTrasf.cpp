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



/*!
Default constructor
*/
GM_3dTrasf::GM_3dTrasf() : GM_Matrix(3, 3) {
	setIdentity();
}



/*!
Default destructor
*/
GM_3dTrasf::~GM_3dTrasf() {
}



/*!
Constructor from two basis

\param theStartBase
Source base
\param theEndBase
Destination base
*/
GM_3dTrasf::GM_3dTrasf(const GM_3dBasis& theStartBase, const GM_3dBasis& theEndBase) {
	if (theStartBase.isValid() && theEndBase.isValid() && theStartBase.isLinearlyInd() && theEndBase.isLinearlyInd()) {

		// Matrix obtained placing side by side the generators of theEndBase followd by the generatirs
		// in theStartBase, the generators will be the columns of the matrix
		GM_Matrix mat(6, 3);
		for (unsigned short i = 0 ; i < 6 ; i++) {
			if (i < 3) {
				mat[0][i] = theEndBase[i].x();
				mat[1][i] = theEndBase[i].y();
				mat[2][i] = theEndBase[i].z();
			}
			else {
				mat[0][i] = theStartBase[i-3].x();
				mat[1][i] = theStartBase[i-3].y();
				mat[2][i] = theStartBase[i-3].z();
			}
		}

		// Gauss-Jordan elimination
		mat.gaussjordanElim();
		
		// The square matrix in the last three columms is the tranformetion matrix from theStartBase to
		// theEndBase, it is formed by the generators of theStartBase expressed with a linear combination
		// of vectors in theEndBase
		for (unsigned short i = 0 ; i < 3 ; i++) {
			for (unsigned short j = 3 ; j < 6 ; j++) {
				pMatrix[i][j-3] = mat[i][j];
			}
		}
	}
}


/*!
Transform e vector in the start base of the transformation in one expressed with a linear combination of
the generators of the destination base

\param theVect
Vector to transform

\return
The vector theVect expressed with a linear combination of the generators of the destination base, or an
invalid vector if theVect is invalid
*/
GM_3dVector GM_3dTrasf::trasf(const GM_3dVector& theVect) const {
	if (!theVect.isValid())
		return theVect;

	GM_3dVector ret = theVect;
	ret.x(theVect.x()*pMatrix[0][0] + theVect.y()*pMatrix[1][0] + theVect.z()*pMatrix[2][0]);
	ret.y(theVect.x()*pMatrix[0][1] + theVect.y()*pMatrix[1][1] + theVect.z()*pMatrix[2][1]);
	ret.z(theVect.x()*pMatrix[0][2] + theVect.y()*pMatrix[1][2] + theVect.z()*pMatrix[2][0]);

	return ret;
}


/*!
Transform e point in the start base of the transformation in one expressed with a linear combination of
the generators of the destination base

\param thePoint
Point to transform

\return
The point thePoint expressed with a linear combination of the generators of the destination base, or an
invalid point if thePoint is invalid
*/
GM_3dPoint GM_3dTrasf::trasf(const GM_3dPoint& thePoint) const {
	if (!thePoint.isValid())
		return thePoint;

	GM_3dPoint ret = thePoint;
	ret.x(thePoint.x()*pMatrix[0][0] + thePoint.y()*pMatrix[1][0] + thePoint.z()*pMatrix[2][0]);
	ret.y(thePoint.x()*pMatrix[0][1] + thePoint.y()*pMatrix[1][1] + thePoint.z()*pMatrix[2][1]);
	ret.z(thePoint.x()*pMatrix[0][2] + thePoint.y()*pMatrix[1][2] + thePoint.z()*pMatrix[2][0]);

	return ret;
}
