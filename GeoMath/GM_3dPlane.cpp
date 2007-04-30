/********************************************************************
* File: GM_3dPlane.cpp												*
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
#include "GM_3dTriangle.h"
#include "GM_3dPlane.h"



GM_3dPlane::GM_3dPlane(void) {
	invalidate();
}



GM_3dPlane::~GM_3dPlane(void) {
	invalidate();
}



GM_3dPlane::GM_3dPlane(double theCoeffArray[4]) {
	for (unsigned int i = 0 ; i < 4 ; i++) {
		mCoeff[i] = theCoeffArray[i];
	}
	normalize();
}



GM_3dPlane::GM_3dPlane(double a, double b, double c, double d) {
	mCoeff[0] = a;
	mCoeff[1] = b;
	mCoeff[2] = c;
	mCoeff[3] = d;
	normalize();
}



GM_3dPlane::GM_3dPlane(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3) {
	createFromThreePoints(thePt1, thePt2, thePt3);
	normalize();
}



GM_3dPlane::GM_3dPlane(GM_3dPoint theNormal, GM_3dPoint thePoint) {
	mCoeff[0] = theNormal.x();
	mCoeff[1] = theNormal.y();
	mCoeff[2] = theNormal.z();
	mCoeff[3] = - (theNormal.x() * thePoint.x() + theNormal.y() * thePoint.y() + theNormal.z() * thePoint.z());
	normalize();
}



GM_3dPlane::GM_3dPlane(GM_3dTriangle theTriangle) {
	createFromThreePoints(theTriangle[0].begin(), theTriangle[1].begin(), theTriangle[2].begin());
	normalize();
}




/*!
Crea un piano a partire da tre punti appartenenti al piano che non giacciano sulla stessa retta

\param thePt1
Primo punto del piano
\param thePt2
Secondo punto del piano
\param thePt3
Terzo punto del piano
*/
void GM_3dPlane::createFromThreePoints(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3) {
	// A = y1 (z2 - z3) + y2 (z3 - z1) + y3 (z1 - z2)
	mCoeff[0] = thePt1.y() * (thePt2.z() - thePt3.z()) +
				thePt2.y() * (thePt3.z() - thePt1.z()) +
				thePt3.y() * (thePt1.z() - thePt2.z());

	// B = z1 (x2 - x3) + z2 (x3 - x1) + z3 (x1 - x2)
	mCoeff[1] = thePt1.z() * (thePt2.x() - thePt3.x()) +
				thePt2.z() * (thePt3.x() - thePt1.x()) +
				thePt3.z() * (thePt1.x() - thePt2.x());

	// C = x1 (y2 - y3) + x2 (y3 - y1) + x3 (y1 - y2)
	mCoeff[2] = thePt1.x() * (thePt2.y() - thePt3.y()) +
				thePt2.x() * (thePt3.y() - thePt1.y()) +
				thePt3.x() * (thePt1.y() - thePt2.y());

	// - D = x1 (y2 z3 - y3 z2) + x2 (y3 z1 - y1 z3) + x3 (y1 z2 - y2 z1)
	mCoeff[3] = - ( thePt1.x() * (thePt2.y() * thePt3.z() - thePt3.y() * thePt2.z()) +
					thePt2.x() * (thePt3.y() * thePt1.z() - thePt1.y() * thePt3.z()) +
					thePt3.x() * (thePt1.y() * thePt2.z() - thePt2.y() * thePt1.z()) );
}



/*!
Normalizza i coefficenti che definiscono il piano in modo da ottenere il piano espresso con coefficenti
in forma Hessiana
*/
void GM_3dPlane::normalize() {
	if (isValid()) {
		double denom = sqrt(mCoeff[0]*mCoeff[0] + mCoeff[1]*mCoeff[1] + mCoeff[2]*mCoeff[2]);
		for (unsigned int i = 0 ; i < 4 ; i++) {
			mCoeff[i] /= denom;
		}
	}
}



/*!
\return
true se i coefficenti del piano sono validi, false altrimenti (piano non inizializzato)
*/
bool GM_3dPlane::isValid() const {
	bool ret = true;
	for (unsigned int i = 0 ; ret && i < 4 ; i++) {
		if (mCoeff[i] == DBL_MAX || mCoeff[i] == -DBL_MAX) {
			ret = false;
		}
	}
	if (ret) {
		ret = false;
		for (unsigned int i = 0 ; !ret && i < 3 ; i++) {
			if (mCoeff[i] > GM_NULL_TOLERANCE) {
				ret = true;
			}
		}
	}
	return ret;
}



/*!
Invalida il piano
*/
void GM_3dPlane::invalidate() {
	for (unsigned int i = 0 ; i < 4 ; i++) {
		mCoeff[i] = DBL_MAX;
	}
}



/*!
Distanza di un punto dal piano

\param thePoint
Punto di cui calcolare la distanza dal piano
\param thePointOnPlane
In uscita contiene il punto sul piano più vicino a thePoint

\return
La distanza di thePoint dal piano
*/
double GM_3dPlane::pointDistance(GM_3dPoint& thePoint, GM_3dPoint& thePointOnPlane) const {
	if (!thePoint.isValid() || isValid())
		return DBL_MAX;

	double ret = pointDistance(thePoint);
	thePointOnPlane = (GM_3dVector)thePoint + (GM_3dVector(mCoeff[0], mCoeff[1], mCoeff[2]) * ret);

	return ret;
}



/*!
Distanza di un punto dal piano

\param thePoint
Punto di cui calcolare la distanza dal piano

\return
La distanza di thePoint dal piano
*/
double GM_3dPlane::pointDistance(GM_3dPoint& thePoint) const {
	if (!thePoint.isValid() || isValid())
		return DBL_MAX;

	double ret = mCoeff[0] * thePoint.x() + mCoeff[1] * thePoint.y() + mCoeff[2] * thePoint.z() + mCoeff[3];
	return ret;
}

