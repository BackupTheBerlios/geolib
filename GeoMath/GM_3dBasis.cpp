/********************************************************************
* File: GM_3dBasis.cpp												*
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



/*!
Default constructor
*/
GM_3dBasis::GM_3dBasis() {
}



/*!
Copy constructor
*/
GM_3dBasis::GM_3dBasis(const GM_3dBasis& theBasis) {
	mGen[0] = theBasis.mGen[0];
	mGen[1] = theBasis.mGen[1];
	mGen[2] = theBasis.mGen[2];
}



/*!
Constructor from three vectors

\param theGen1
First generator
\param theGen1
Second generator
\param theGen1
Third generator
*/
GM_3dBasis::GM_3dBasis(GM_3dVector theGen1, GM_3dVector theGen2, GM_3dVector theGen3) {
	mGen[0] = theGen1;
	mGen[1] = theGen2;
	mGen[2] = theGen3;
}



/*!
Default destructor
*/
GM_3dBasis::~GM_3dBasis() {
}



/*!
false if the generators are not valid, true otherwise
*/
bool GM_3dBasis::isValid() const {
	if (mGen[0].isValid() && mGen[1].isValid() && mGen[2].isValid())
		return true;
	else
		return false;
}



/*!
Invalidate the generators
*/
void GM_3dBasis::invalidate() {
	mGen[0].invalidate();
	mGen[1].invalidate();
	mGen[2].invalidate();
}



/*!
Normalize the generators
*/
void GM_3dBasis::normalize() {
	mGen[0].normalize();
	mGen[1].normalize();
	mGen[2].normalize();
}



/*!
\return
true if the generators is linearly independent, false otherwise
*/
bool GM_3dBasis::isLinearlyInd() const {
	if ((mGen[0]^mGen[1]).mod() > GM_NULL_TOLERANCE && (mGen[0]^mGen[2]).mod() > GM_NULL_TOLERANCE && (mGen[1]^mGen[2]).mod() > GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
Get a generator from its index

\param theIndex
Index of the generator to return

\return
Th generator with index theIndex, or an invalid vector if theIndex is out of range or the basis is not valid
*/
GM_3dVector GM_3dBasis::operator[](unsigned short theIndex) const {
	if (isValid() && theIndex < 3)
		return mGen[theIndex];
	else
		return GM_3dVector();
}
