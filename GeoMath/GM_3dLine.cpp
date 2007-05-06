/********************************************************************
* File: GM_3dLine.cpp												*
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
#include "GM_3dLine.h"



GM_3dLine::GM_3dLine(void) {
	invalidate();
}


GM_3dLine::~GM_3dLine(void) {
}


GM_3dLine::GM_3dLine(const GM_3dLine& theLine) {
	mBegin = theLine.mBegin;
	mEnd = theLine.mEnd;
}



GM_3dLine::GM_3dLine(GM_3dPoint theBegin, GM_3dPoint theEnd) {
	mBegin = theBegin;
	mEnd = theEnd;
}



GM_3dLine::GM_3dLine(double theBeginXCoord, double theBeginYCoord, double theBeginZCoord, double theEndXCoord, double theEndYCoord, double theEndZCoord) {
	mBegin.x(theBeginXCoord);
	mBegin.y(theBeginYCoord);
	mBegin.z(theBeginZCoord);
	mEnd.x(theEndXCoord);
	mEnd.y(theEndYCoord);
	mEnd.z(theEndZCoord);
}



/*!
\return
Lunghezza della linea
*/
double GM_3dLine::length() const {
	return mBegin.distFrom(mEnd);
}



/*!
\return
Prodotto scalare delle due linee considerate come vettori, ovvero la lunghezza della
proiezione di theLine su this per la lunghezza di this
Le due linee si suppongono entrambe con punto iniziale nell' origine
*/
double GM_3dLine::operator*(const GM_3dLine& theLine) const {
	GM_3dVector v1(*this);
	GM_3dVector v2(theLine);
	return v1 * v2;
}



/*!
\return
Prodotto vettore delle due linee considerate come vettori, ovvero il vettore perpendicolare
al piano individuato da theLine e this, con modulo pari all' area del parallelogramma definito dai due
vettori e verso in accordo alla regola della terna destrorsa
Le due linee si suppongono entrambe con punto iniziale nell' origine
*/
GM_3dLine GM_3dLine::operator^(const GM_3dLine& theLine) const {
	GM_3dVector v1(*this);
	GM_3dVector v2(theLine);
	GM_3dVector externalProd = v1 ^ v2;
	return GM_3dLine(mBegin, (GM_3dVector)mBegin + externalProd);
}


bool GM_3dLine::operator == (const GM_3dLine& theLine) const {
	return !((*this) != theLine);
}


bool GM_3dLine::operator != (const GM_3dLine& theLine) const {
	if (mBegin != theLine.mBegin || mEnd != theLine.mEnd)
		return true;
	else
		return false;
}



/*!
\return
true se la linea ha tutte e tre le proiezioni sugli assi minori di GM_NULL_TOLERANCE, false altrimenti
*/
bool GM_3dLine::isNull() const {
	if (fabs(mBegin.x() - mEnd.x()) < GM_NULL_TOLERANCE &&
		fabs(mBegin.y() - mEnd.y()) < GM_NULL_TOLERANCE &&
		fabs(mBegin.z() - mEnd.z()) < GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
\return
Z minima della linea
*/
double GM_3dLine::minZ() const {
	return mBegin.z() < mEnd.z() ? mBegin.z() : mEnd.z();
}



/*!
\return
Z massima della linea
*/
double GM_3dLine::maxZ() const {
	return mBegin.z() > mEnd.z() ? mBegin.z() : mEnd.z();
}



/*!
\return
true se la linea è verticale, ovvero ha componente z > di GM_NULL_TOLERANCE e componeni in x e in y < di
GM_NULL_TOLERANCE, false altrimenti
*/
bool GM_3dLine::isVertical() const {
	if (fabs(dx()) < GM_NULL_TOLERANCE && fabs(dy()) < GM_NULL_TOLERANCE && fabs(dz()) > GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
\return
true se la linea è orizzontale, ovvero ha componente z < di GM_NULL_TOLERANCE, false altrimenti
*/
bool GM_3dLine::isHorizontal() const {
	if (fabs(dz()) < GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
\return
Punto sulla lina con coordinata Z pari a theZLevel, se theZLevel è fuori dal range della linea restituisce
un punto con -DBL_MAX o DBL_MAX in tutte le componenti se theZLevel è rispettivamente sotto la z minima o
sopra la z massima.
Se la linea è orizzontale con z pari a theZLevel restituisce mBegin
*/
GM_3dPoint GM_3dLine::pointAtZ(double theZLevel) const {
	if (theZLevel < minZ() - GM_DIFF_TOLERANCE)
		return GM_3dPoint(-DBL_MAX, -DBL_MAX, -DBL_MAX);
	if (theZLevel > maxZ() + GM_DIFF_TOLERANCE)
		return GM_3dPoint(DBL_MAX, DBL_MAX, DBL_MAX);

	if (dz() < GM_NULL_TOLERANCE && fabs(theZLevel - mBegin.z()) < GM_DIFF_TOLERANCE) {
		return mBegin;
	}
	else {
		double ratio = fabs(theZLevel - begin().z()) / fabs(dz());
		return GM_3dPoint(begin().x() + dx()*ratio, begin().y() + dy()*ratio, theZLevel);
	}
}



/*!
Inverte la direzione della linea scambiano mBegin e mEnd
*/
void GM_3dLine::invert() {
	GM_3dPoint swap = mBegin;
	mBegin = mEnd;
	mEnd = swap;
}



/*!
Restituisce il vettore normale alla proiezione della linea sul piano xy, con direzione sul lato destro/sinistro
della linea rispetta alla direzione della linea stessa, in base al valore del parametro

\param theRightSideFlag
Se vale true viene restituito il vettore perpendicolare alla linea proiettata sul piano xy a destra rispetto
alla direzione della linea, se vale false viene restiuito il vettore a sinistra

\return
Il vettore perpendicolare alla linea proiettata sul piano xy a sinistra/destra rispetto alla direzione della linea
*/
GM_3dVector GM_3dLine::normalXYVector(bool theRightSideFlag) const {
	GM_3dVector ret(mEnd.x() - mBegin.x(), mEnd.y() - mBegin.y(), 0.0); 

	if (theRightSideFlag) {
		double tmp = ret.x();
		ret.x(ret.y());
		ret.y(-tmp);
	}
	else {
		double tmp = ret.x();
		ret.x(-ret.y());
		ret.y(tmp);
	}

	double module = ret.mod();
	if (module > GM_NULL_TOLERANCE) {
		ret.x(ret.x() / module);
		ret.y(ret.y() / module);
		ret.z(ret.z() / module);
	}
	else {
		ret.x(0.0);
		ret.y(0.0);
		ret.z(0.0);
	}

	return ret;
}



/*!
Calcola l' angolo che la proiezione della linea sul piano xy forma con l' asse X

\return
Angolo che la proiezione della linea sul piano xy, forma con l' asse X, restituisce
un valore compreo tra 0 e 2*PI
*/
double GM_3dLine::xyAngle() const {
	GM_3dVector lineDirOnXY(dx(), dy(), 0.0);
	return lineDirOnXY.xyAngle();
}



/*!
Calcola l' angolo interno tra due linee (this e la linea passata come parametro) entrambe proiettate sul piano xy

\param theLine
Linea di cui calclare l' angolo rispetto a this

\return
Angolo interno compreso tra la proiezione delle due linee sul piano xy, restituisce un valore compreo tra 0 e 2*PI
*/
double GM_3dLine::xyAngle(GM_3dLine& theLine) const {
	GM_3dVector lineDirOnXY(dx(), dy(), 0.0);
	GM_3dVector theLineDirOnXY(theLine.dx(), theLine.dy(), 0.0);
	return lineDirOnXY.xyAngle(theLineDirOnXY);
}



/*!
\return
true se le coordinate della linea sono valide, false altrimenti (linea non inizializzata)
*/
bool GM_3dLine::isValid() const {
	return mBegin.isValid() && mEnd.isValid();
}



/*!
Invalida la linea
*/
void GM_3dLine::invalidate() {
	mBegin.invalidate();
	mEnd.invalidate();
}



/*!
Calcola il centro di una linea
*/
GM_3dPoint GM_3dLine::center() const {
	double x = mBegin.x() + ((mEnd.x() - mBegin.x()) / 2.0);
	double y = mBegin.y() + ((mEnd.y() - mBegin.y()) / 2.0);
	double z = mBegin.z() + ((mEnd.z() - mBegin.z()) / 2.0);
	return GM_3dPoint(x, y, z);
}



/*!
Calcola un punto sulla linea dato un parametro tra 0 e 1 dove 0 corrisponde a mBegin e 1 a mEnd

\param theSection
Sezione della linea di cui determinare il punto, 0 = begin 1 = end

\return
Punto sulla linea corrispondente alla sezione specificata, restituisce un punto non valido se theSection
è fuori range
*/
GM_3dPoint GM_3dLine::pointFromSection(double theSection) const {
	GM_3dPoint ret;
	if (theSection < -GM_DIFF_TOLERANCE || theSection > 1.0 + GM_DIFF_TOLERANCE) {
		return ret;
	}
	if (theSection < 0.0) {
		theSection = 0.0;
	}
	if (theSection > 1.0) {
		theSection = 1.0;
	}

	theSection *= length();

	GM_3dVector lineDir(dx(), dy(), dz());
	lineDir.normalize();
	ret = (GM_3dVector)mBegin + (lineDir * theSection);

	return ret;
}



/*!
Dato un punto appartenente alla linea calcola la sua sezione ovvero la posizione sulla linea con un numero tra
0 e 1 in cui 0 corrisponde a mBegin e 1 a mEnd

\param thePoint
Punto appartenente alla linea di cui calcolare la sezione

\return
Sezione della linea individuata da thePoint, 0 = begin 1 = end, restituisce -DBL_MAX se thePoint
non giace sulla linea
*/
double GM_3dLine::sectionFromPoint(GM_3dPoint thePoint) const {
	double ret = -DBL_MAX;
	GM_3dVector lineDir(dx(), dy(), dz());
	lineDir.normalize();
	GM_3dVector pointDir(thePoint.x() - mBegin.x(), thePoint.y() - mBegin.y(), thePoint.z() - mBegin.z());
	pointDir.normalize();

	if (fabs((lineDir * pointDir) - 1.0) < GM_DIFF_TOLERANCE) {
		double distFromBegin = thePoint.distFrom(mBegin);
		double distFromEnd = thePoint.distFrom(mEnd);
		double lineLen = length();
		if (fabs(distFromBegin + distFromEnd - lineLen) < GM_DIFF_TOLERANCE) {
			ret = distFromBegin / lineLen;
		}
	}

	return ret;
}



/*!
Distanza tra un punto e una linea

\param thePoint
Punto di cui calcolare la distanza dalla linea
\param thePointOnLine
In uscita contiene il punto sulla linea più vicino a thePoint

\return
La distanza di thePoint dalla linea
*/
double GM_3dLine::pointDistance(GM_3dPoint thePoint, GM_3dPoint& thePointOnLine) const {
	GM_3dVector v = (GM_3dVector)mEnd - (GM_3dVector)mBegin;
	GM_3dVector w = (GM_3dVector)thePoint - (GM_3dVector)mBegin;

	double c1 = w * v;
	double c2 = v * v;

	double ret = -DBL_MAX;
	if (c1 < GM_NULL_TOLERANCE) {
		ret = thePoint.distFrom(mBegin);
		thePointOnLine = mBegin;
	}
	else if (c2 < c1 +  GM_NULL_TOLERANCE) {
		ret = thePoint.distFrom(mEnd);
		thePointOnLine = mEnd;
	}
	else {
		double b = c1 / c2;
		thePointOnLine = (GM_3dVector)mBegin + (v * b);
		ret = thePoint.distFrom(thePointOnLine);
	}

	return ret;
}


double GM_3dLine::pointDistance(GM_3dPoint thePoint) const {
	GM_3dPoint dummyPoint;
	return pointDistance(thePoint, dummyPoint);
}
