/********************************************************************
* File: GM_3dTriangle.cpp											*
*********************************************************************
* Descrizione:														*
*********************************************************************
* History:															*
* 22.4.2007 Creato da : Cordara Claudio								*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_3dPoint.h"
#include "GM_3dVector.h"
#include "GM_3dLine.h"
#include "GM_3dPlane.h"
#include "GM_3dTriangle.h"



GM_3dTriangle::GM_3dTriangle(void) {
	mSurfAngle = DBL_MAX;
}



GM_3dTriangle::~GM_3dTriangle(void) {
}


GM_3dTriangle::GM_3dTriangle(const GM_3dTriangle& theTriangle) {
	mEdge[0] = theTriangle.mEdge[0];
	mEdge[1] = theTriangle.mEdge[1];
	mEdge[2] = theTriangle.mEdge[2];
	mSurfAngle = theTriangle.mSurfAngle;
}



GM_3dTriangle::GM_3dTriangle(GM_3dPoint theFirstPoint, GM_3dPoint theSecondPoint, GM_3dPoint theThirdPoint) {
	mEdge[0] = GM_3dLine(theFirstPoint, theSecondPoint);
	mEdge[1] = GM_3dLine(theSecondPoint, theThirdPoint);
	mEdge[2] = GM_3dLine(theThirdPoint, theFirstPoint);
	mSurfAngle = DBL_MAX;
}



GM_3dTriangle::GM_3dTriangle(GM_3dLine theFirstEdge, GM_3dLine theSecondEdge, GM_3dLine theThirdEdge) {
	mEdge[0] = theFirstEdge;
	mEdge[1] = theSecondEdge;
	mEdge[2] = theThirdEdge;
	mSurfAngle = DBL_MAX;
}



/*!
\return
false se esistono due lati colineari, true altrimenti
*/
bool GM_3dTriangle::isValid() const {
	if (!mEdge[0].isValid() || !mEdge[1].isValid() || !mEdge[2].isValid())
		return false;

	if ((mEdge[0]^mEdge[1]).isNull() || (mEdge[0]^mEdge[2]).isNull())
		return false;
	else
		return true;
}



/*!
\param theIndex
Indice del lato di cui restituire il riferimento

\return
Riferimento al lato i-esimo del triangolo
*/
GM_3dLine& GM_3dTriangle::operator[](int theIndex) {
	assert(theIndex >= 0 && theIndex <= 2);
	mSurfAngle = DBL_MAX;
	return mEdge[theIndex];
}



/*!
\param theIndex
Indice del lato da restituire

\return
Lato i-esimo del triangolo
*/
GM_3dLine GM_3dTriangle::operator[](int theIndex) const {
	assert(theIndex >= 0 && theIndex <= 2);
	return mEdge[theIndex];
}



/*!
Inverte i tre lati del triangolo
*/
void GM_3dTriangle::invert() {
	for (int i = 0 ; i < 3 ; i++) {
		mEdge[i].invert();	
	}
}



/*!
Ordina i lati del triangolo in modo che la loro proiezione sul piano xy sia oraria/antioraria in
base al valore del parametro.
Se il triangolo è verticale non viene effettuato alcun ordinamento

\param theClockwiseFlag
Se vale true i lati del triangolo vengono ordinati in modo tale che la loro proiezione sul piano xy sia oraria, se
vale false antioraria
*/
void GM_3dTriangle::setXYVersus(bool theClockwiseFlag) {
	if (!isVertical()) {
		GM_3dVector v0(mEdge[0]);
		GM_3dVector v1(mEdge[1]);
		bool isClockwise = v0.isAtLeftOnXY(v1);
		if (isClockwise != theClockwiseFlag) {
			invert();
		}
	}
}



/*!
\return
Z massima del triangolo
*/
double GM_3dTriangle::maxZ() const {
	double ret = -DBL_MAX;
	for (int i = 0 ; i < 3 ; i++) {
		double edgeMaxZ = mEdge[i].maxZ();
		if (edgeMaxZ > ret) {
			ret = edgeMaxZ;
		}
	}
	
	return ret;
}



/*!
\return
Z minima del triangolo
*/
double GM_3dTriangle::minZ() const {
	double ret = DBL_MAX;
	for (int i = 0 ; i < 3 ; i++) {
		double edgeMinZ = mEdge[i].minZ();
		if (edgeMinZ < ret) {
			ret = edgeMinZ;
		}
	}
	
	return ret;
}



/*!
\return
Angolo più piccolo che il piano su cui giace il triangolo forma con il piano xy
*/
double GM_3dTriangle::angle() const {
	if (mSurfAngle == DBL_MAX) {
		const_cast<GM_3dTriangle*>(this) -> computeAngle();
	}
	return mSurfAngle;
}



/*!
Calcola l' angolo più piccolo che il piano su cui giace il triangolo forma con il piano xy
*/
void GM_3dTriangle::computeAngle() {
	GM_3dVector normVector = normalVector();
	GM_3dVector normalVectorOnXY(normVector.x(), normVector.y(), 0.0);
	double dz = normVector.z();
	double dxy = normalVectorOnXY.mod();
	mSurfAngle = (GM_PI / 2.0) - atan2(dz, dxy);
}



/*!
\return
Vettore normale al piano su cui gace il triangolo
*/
GM_3dPoint GM_3dTriangle::normalVector() const {
	GM_3dLine vecProd = mEdge[0]^mEdge[1];
	GM_3dVector ret(vecProd.dx(), vecProd.dy(), vecProd.dz());
	double module = ret.mod();
	ret.x(ret.x() / module);
	ret.y(ret.y() / module);
	ret.z(ret.z() / module);

	return ret;
}



/*!
\param theEdgeIndex
Indice che identifica il lato del triangolo di cui calcolare la normale

\return
Vettore normale a un lato del triangolo proiettato sul piano xy e diretto dall' interno all' esterno del triangolo
*/
GM_3dPoint GM_3dTriangle::normalVectorOnEdge(int theEdgeIndex) const {
	int precEdgeIndex = theEdgeIndex - 1;
	if (precEdgeIndex == -1)
		precEdgeIndex = 2;

	GM_3dLine precEdge = mEdge[precEdgeIndex];
	precEdge.invert();

	// Prodotto vettore del lato con il lato precedente
	GM_3dLine v1 = mEdge[theEdgeIndex]^precEdge;
	// Prodotto vettore del lato col risultato dell' operazione precedente
	GM_3dLine v2 = mEdge[theEdgeIndex]^v1;
	

	// Bisogna azzerare la componente in Z (proiezione sul piano xy)
	GM_3dVector ret(v2.dx(), v2.dy(), 0.0);
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
\return
true se il triangolo è orizzontale cioè se la sua normale ha solo la componente z > GM_NULL_TOLERANCE, false
altrimenti
*/
bool GM_3dTriangle::isHorizontal() const {
	GM_3dPoint normVect = normalVector();
	if (fabs(normVect.x()) < GM_NULL_TOLERANCE && fabs(normVect.y()) < GM_NULL_TOLERANCE && fabs(normVect.z()) > GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
\return
true se il triangolo è verticale cioè se la sua normale ha la componente z < GM_NULL_TOLERANCE, false
altrimenti
*/
bool GM_3dTriangle::isVertical() const {
	GM_3dPoint normVect = normalVector();
	if ((fabs(normVect.x()) > GM_NULL_TOLERANCE || fabs(normVect.y()) > GM_NULL_TOLERANCE) && fabs(normVect.z()) < GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
\return
true se i tre lati del triangolo sono consecutivi e connessi, false altrimenti
*/
bool GM_3dTriangle::isConnected() const {
	if (mEdge[0].end().distFrom(mEdge[1].begin()) < GM_DIFF_TOLERANCE &&
		mEdge[1].end().distFrom(mEdge[2].begin()) < GM_DIFF_TOLERANCE &&
		mEdge[2].end().distFrom(mEdge[0].begin()) < GM_DIFF_TOLERANCE)
			return true;
	else
			return false;
}



/*!
Calcola una linea sul triangolo alla Z specificata, se la Z è fuori dalla bbox del triangolo restituisce una
linea non valida

\param theZLevel
Z di cui calcolare la linea sulla superfice del triangolo

\return
Linea sul triangolo alla Z specificata se è compresa nella bbox del triangolo, altrimenti restituisce
una linea non valida
*/
GM_3dLine GM_3dTriangle::zSection(double theZLevel) const {
	GM_3dLine ret;
	double _minZ = minZ();
	double _maxZ = maxZ();
	assert(theZLevel > _minZ + GM_DIFF_TOLERANCE && theZLevel < _maxZ - GM_DIFF_TOLERANCE);

	if (theZLevel > _minZ + GM_DIFF_TOLERANCE && theZLevel < _maxZ - GM_DIFF_TOLERANCE) {
		bool startPointFound = false;
		bool endPointFound = false;
		for (unsigned int i = 0 ; !endPointFound && i < 3 ; i++) {
			GM_3dPoint edgePoint = mEdge[i].pointAtZ(theZLevel);
			if (edgePoint.isValid()) {
				if (startPointFound) {
					ret.end(edgePoint);
					endPointFound = true;
				}
				else {
					ret.begin(edgePoint);
					startPointFound = true;
				}
			}
		}
	}

	return ret;
}



/*!
Invalida il triangolo
*/
void GM_3dTriangle::invalidate() {
	for (int i = 0 ; i < 3 ; i++) {
		mEdge[i].invalidate();
	}
}


/*!
Determina se un punto appartiene all' interno di un triangolo
*/
bool GM_3dTriangle::isInteriorPoint(GM_3dPoint thePoint) const {
	return false;
}



/*!
Distanza di un punto 3d dal triangolo
DA FINIRE
*/
double GM_3dTriangle::pointDistance(GM_3dPoint thePoint) const {
	// Calcolo la distanza tra il punto e il piano su cui giace il triangolo
	// Il punto più vicino sul piano appartiene al triangolo
		// Il punto e la distanza costituiscono il risultato
	// Il punto più vicino sul piano non appartiene al triangolo
		// Calcolo la distanza tra il punto e i tre lati del triangolo prendendo
		// la più piccola

	return DBL_MAX;
}
