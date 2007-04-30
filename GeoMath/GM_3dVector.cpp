/********************************************************************
* File: GM_3dVector.cpp												*
*********************************************************************
* Descrizione:														*
*********************************************************************
* History:															*
* 22.4.2007 Creato da : Cordara Claudio								*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_3dVector.h"



GM_3dVector::GM_3dVector(void) : GM_3dPoint() {
}


GM_3dVector::GM_3dVector(const GM_3dVector& theVect) : GM_3dPoint(theVect) {
}



GM_3dVector::GM_3dVector(const GM_3dPoint& thePoint) : GM_3dPoint(thePoint){
}


GM_3dVector::GM_3dVector(double theXCoord, double theYCoord, double theZCoord)  : GM_3dPoint(theXCoord, theYCoord, theZCoord) {
}

GM_3dVector::GM_3dVector(const GM_3dLine& theLine) {
	mX = theLine.end().x() - theLine.begin().x();
	mY = theLine.end().y() - theLine.begin().y();
	mZ = theLine.end().z() - theLine.begin().z();
}


/*!
Vettore giacente sul piano xy e con angolo pari a theXYAngle rispetto all' asse X
(positivo = antiorario)
*/
GM_3dVector::GM_3dVector(double theXYAngle) {
	mX = cos(theXYAngle);
	mY = sin(theXYAngle);
	mZ = 0.0;
}



GM_3dVector::~GM_3dVector(void) {
}


/*!
\param theVect
Vettore con cui calcolare il prodotto scalare rispetto a this

\return
Prodotto scalare dei due vettori, ovvero la lunghezza della proiezione di theVect su this per il modulo
di this
*/
double GM_3dVector::operator*(const GM_3dVector& theVect) const {
	return (mX * theVect.x()) + (mY * theVect.y()) + (mZ * theVect.z());
}



/*!
\param theFactor
Scalare con cui moltiplicare il vettore

\return
Vettore ottenuto moltiplicando this per theFactor
*/
GM_3dVector GM_3dVector::operator*(double theFactor) const {
	return GM_3dVector(mX*theFactor, mY*theFactor, mZ*theFactor);
}



/*!
\param theVect
Vettore con cui calcolare il prodotto vettore rispetto a this

\return
Prodotto vettore dei due vettori, ovvero il vettore perpendicolare al piano individuato da theVect e this,
con modulo pari all' area del parallelogramma definito dai due vettori e verso in accordo alla regola della
terna destrorsa
*/
GM_3dVector GM_3dVector::operator^(const GM_3dVector& theVect) const {
	return GM_3dVector(	mY * theVect.z() - mZ * theVect.y(),
						mZ * theVect.x() - mX * theVect.z(),
						mX * theVect.y() - mY * theVect.x() );
}



/*!
\param theVect
Vettore da sommare a this

\return
Somma di due vettori
*/
GM_3dVector GM_3dVector::operator+(const GM_3dVector& theVect) const {
	return GM_3dVector(mX + theVect.x(), mY + theVect.y(), mZ + theVect.z());
}



/*!
\param theVect
Vettore da sottrarre a this

\return
Differenza di due vettori
*/
GM_3dVector GM_3dVector::operator-(const GM_3dVector& theVect) const {
	return GM_3dVector(mX - theVect.x(), mY - theVect.y(), mZ - theVect.z());
}


/*!
\return
Modulo di un vettore
*/
double GM_3dVector::mod() const {
	return distFrom(GM_3dPoint(0.0, 0.0, 0.0));
}


/*!
Normalizza un vettore, ovvero divide le sue componenti per il suo modulo
*/
void GM_3dVector::normalize() {
	double module = mod();
	if (module > GM_NULL_TOLERANCE) {
		mX /= module;
		mY /= module;
		mZ /= module;
	}
}



/*!
Calcola l' angolo tra il vettore proiettato sul piano xy e l' asse X

\return
Angolo che il vettore proiettato sul piano xy, forma con l' asse X, restituisce un valore compreo tra 0 e 2*PI
*/
double GM_3dVector::xyAngle() const {
	double ret = atan2(mY,mX);
	if (ret < 0.0) {
		ret = 2.0 * GM_PI + ret;
	}

	return ret;
}



/*!
Calcola l' angolo interno tra due vettori, proiettati sul piano xy

\param theVect
Vettore di cui calcolare l' angolo rispetto a this

\return
Angolo interno tra i vettori this e theVect proiettati sul piano xy, restituisce un valore compreo tra 0 e 2*PI
*/
double GM_3dVector::xyAngle(const GM_3dVector& theVect) const {
	double ang1 = atan2(mY,mX);
	if (ang1 < 0.0) {
		ang1 = 2.0 * GM_PI + ang1;
	}
	double ang2 = atan2(theVect.y(),theVect.x());
	if (ang2 < 0.0) {
		ang2 = 2.0 * GM_PI + ang2;
	}
	double ret = ang2 - ang1;
	if (ret < 0.0) {
		ret = 2.0*GM_PI + ret;
	}
	
	return ret;
}


bool GM_3dVector::isAtLeftOnXY(const GM_3dVector& theVect) const {
	return theVect.x()*mY-mX*theVect.y() > 0.0;
}
