/********************************************************************
* File: GM_3dPoint.cpp												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/


#include "StdAfx.h"


#include "GM_3dPoint.h"
#include "GM_3dVector.h"


GM_3dPoint::GM_3dPoint(void) {
	invalidate();
}



GM_3dPoint::~GM_3dPoint(void) {
}


GM_3dPoint::GM_3dPoint(const GM_3dPoint& thePoint) {
	mX = thePoint.mX;
	mY = thePoint.mY;
	mZ = thePoint.mZ;
}



GM_3dPoint::GM_3dPoint(double theXCoord, double theYCoord, double theZCoord) {
	mX = theXCoord;
	mY = theYCoord;
	mZ = theZCoord;
}



/*!
Punto giacente sul piano xy e con angolo pari a theXYAngle rispetto all' asse X
(positivo = antiorario)
*/
GM_3dPoint::GM_3dPoint(double theXYAngle) {
	mX = cos(theXYAngle);
	mY = sin(theXYAngle);
	mZ = 0.0;
}



/*!
\param thePoint
Punto di cui misurare la distanza da this

\return
Distanza tra due punti
*/
double GM_3dPoint::distFrom(const GM_3dPoint& thePoint) const {
	double dx = mX - thePoint.x();
	double dy = mY - thePoint.y();
	double dz = mZ - thePoint.z();
	return sqrt(dx*dx + dy*dy + dz*dz);
}








/*!
\return
true se il punto coincide con l' origine false altrimenti
*/
bool GM_3dPoint::isOrigin() const {
	if (fabs(mX) < GM_NULL_TOLERANCE && fabs(mY) < GM_NULL_TOLERANCE && fabs(mZ) < GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}






/*!
Inverte il segno delle coordinate xy, rotazione di 180 gradi sul piano xy
*/
void GM_3dPoint::xyInvert() {
	mX = -mX;
	mY = -mY;
}





/*!
\return
true se le coordinate del punto sono da considerarsi valide, false altrimenti (punto non inizializzato)
*/
bool GM_3dPoint::isValid() const {
	if (mX != DBL_MAX && mX != -DBL_MAX && mY != DBL_MAX && mY != -DBL_MAX && mZ != DBL_MAX && mZ != -DBL_MAX)
		return true;
	else
		return false;
}



/*!
Invalida il punto
*/
void GM_3dPoint::invalidate() {
	mX = mY = mZ = DBL_MAX;
}



GM_3dPoint::operator GM_3dVector() const {
	return GM_3dVector(*this);
}


bool GM_3dPoint::operator == (const GM_3dPoint& thePoint) const {
	return !((*this) != thePoint);
}


bool GM_3dPoint::operator != (const GM_3dPoint& thePoint) const {
	if (fabs(mX - thePoint.mX) > GM_NULL_TOLERANCE || fabs(mY - thePoint.mY) > GM_NULL_TOLERANCE || fabs(mZ - thePoint.mZ) > GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}
