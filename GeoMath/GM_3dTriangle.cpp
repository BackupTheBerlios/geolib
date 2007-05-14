/********************************************************************
* File: GM_3dTriangle.cpp											*
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
#include "GM_3dPlane.h"
#include "GM_3dTriangle.h"



GM_3dTriangle::GM_3dTriangle(void) {
}



GM_3dTriangle::~GM_3dTriangle(void) {
}


GM_3dTriangle::GM_3dTriangle(const GM_3dTriangle& theTriangle) {
	mEdge[0] = theTriangle.mEdge[0];
	mEdge[1] = theTriangle.mEdge[1];
	mEdge[2] = theTriangle.mEdge[2];
}



GM_3dTriangle::GM_3dTriangle(GM_3dPoint theFirstPoint, GM_3dPoint theSecondPoint, GM_3dPoint theThirdPoint) {
	mEdge[0] = GM_3dLine(theFirstPoint, theSecondPoint);
	mEdge[1] = GM_3dLine(theSecondPoint, theThirdPoint);
	mEdge[2] = GM_3dLine(theThirdPoint, theFirstPoint);
}



GM_3dTriangle::GM_3dTriangle(GM_3dLine theFirstEdge, GM_3dLine theSecondEdge, GM_3dLine theThirdEdge) {
	mEdge[0] = theFirstEdge;
	mEdge[1] = theSecondEdge;
	mEdge[2] = theThirdEdge;
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
double GM_3dTriangle::xyAngle() const {
	GM_3dPlane trPlane(*this);
	return trPlane.xyAngle();
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
Determina se un punto, o la sua proiezione sul piano su cui giace il triangolo, appartiene all' interno
di un triangolo
*/
bool GM_3dTriangle::isInteriorPoint(GM_3dPoint thePoint) const {
	bool ret = false;
	if (!isValid() || !thePoint.isValid())
		return ret;

	GM_3dVector bVect(mEdge[0].begin());
	GM_3dVector E0Vect(mEdge[0].end());
	GM_3dVector E1Vect;
	if (mEdge[1].begin() != mEdge[0].begin() && mEdge[1].begin() != mEdge[0].end()) {
		E1Vect = (GM_3dVector)(mEdge[1].begin());
	}
	else {
		E1Vect = (GM_3dVector)(mEdge[1].end());
	}
	GM_3dVector pointVect = (GM_3dVector)thePoint;

	GM_3dVector dVect = bVect - pointVect;
	double a = E0Vect * E0Vect;
	double b = E0Vect * E1Vect;
	double c = E1Vect * E1Vect;
	double d = E0Vect * dVect;
	double e = E1Vect * dVect;
	//double f = dVect * dVect;

	double det = a*c - b*b;
	double s = b*e - c*d;
	double t = b*d - a*e;

	if (s + t <= det && s >= 0.0 && t >= 0.0) {
		ret = true;
	}
	else {
		ret = false;
	}

	return ret;
}



/*!
Distanza di un punto 3d dal triangolo
*/
double GM_3dTriangle::pointDistance(GM_3dPoint thePoint) const {
	double ret = DBL_MAX;
	if (!isValid() || !thePoint.isValid())
		return ret;

	if (isInteriorPoint(thePoint)) {
		// Punto interno al triangolo, la distanza è pari alla distanza tra il punto e il piano su cui
		// giace il triangolo

		GM_3dPlane triPlane(*this);
		ret = triPlane.pointDistance(thePoint);
	}
	else {
		// Punto esterno al triangolo, prendo la minore tra le distanze del punto con i tre lati

		for (unsigned int i = 0 ; i < 3  ; i++) {
			double d = mEdge[i].pointDistance(thePoint);
			if (d < ret) {
				ret = d;
			}
		}
	}

	return ret;
}
