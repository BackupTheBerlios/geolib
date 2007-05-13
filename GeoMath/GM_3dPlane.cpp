/********************************************************************
* File: GM_3dPlane.cpp												*
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
#include "GM_3dTriangle.h"
#include "GM_3dPlane.h"



/*!
Standard constructor
*/
GM_3dPlane::GM_3dPlane(void) {
	invalidate();
}



/*!
Standard destructor
*/
GM_3dPlane::~GM_3dPlane(void) {
	invalidate();
}



/*!
Constructor from coefficients

\param theCoeffArray
Coefficients of the general equation of the plane
*/
GM_3dPlane::GM_3dPlane(double theCoeffArray[4]) {
	for (unsigned int i = 0 ; i < 4 ; i++) {
		mCoeff[i] = theCoeffArray[i];
	}
	normalize();
}



/*!
Constructor from coefficients

\param a
Coefficient of the general equation of the plane
\param b
Coefficient of the general equation of the plane
\param c
Coefficient of the general equation of the plane
\param d
Coefficient of the general equation of the plane
*/
GM_3dPlane::GM_3dPlane(double a, double b, double c, double d) {
	mCoeff[0] = a;
	mCoeff[1] = b;
	mCoeff[2] = c;
	mCoeff[3] = d;
	normalize();
}



/*!
Contructor from three non colinear points, colinearity is not checked

\param thePt1
First non colinear point
\param thePt2
Second non colinear point
\param thePt3
Third non colinear point
*/
GM_3dPlane::GM_3dPlane(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3) {
	if (thePt1.isValid() && thePt2.isValid() && thePt3.isValid()) {
		createFromThreePoints(thePt1, thePt2, thePt3);
		normalize();
	}
	else {
		invalidate();
	}
}



/*!
Constructor from normal vector and a point on the plane

\param theNormal
Normal vector
\param thePoint
Point on plane
*/
GM_3dPlane::GM_3dPlane(GM_3dVector theNormal, GM_3dPoint thePoint) {
	if (theNormal.isValid() && thePoint.isValid()) {
		mCoeff[0] = theNormal.x();
		mCoeff[1] = theNormal.y();
		mCoeff[2] = theNormal.z();
		mCoeff[3] = - (theNormal.x() * thePoint.x() + theNormal.y() * thePoint.y() + theNormal.z() * thePoint.z());
		normalize();
	}
	else {
		invalidate();
	}
}



/*!
Constructor from a triangle

\param theTriangle
Triangle for plane definition
*/
GM_3dPlane::GM_3dPlane(GM_3dTriangle theTriangle) {
	if (theTriangle.isValid()) {
		createFromThreePoints(theTriangle[0].begin(), theTriangle[1].begin(), theTriangle[2].begin());
		normalize();
	}
	else {
		invalidate();
	}
}



/*!
Copy constructor
*/
GM_3dPlane::GM_3dPlane(const GM_3dPlane& thePlane) {
	for (int i = 0 ; i < 4 ; i++) {
		mCoeff[i] = thePlane.mCoeff[i];
	}
}



/*!
Set up the plane from three non colinear points, colinearity is not checked, the plane is invalidate
if the points are not valid

\param thePt1
First point on plane
\param thePt2
Second point on plane
\param thePt3
Third point on plane
*/
void GM_3dPlane::createFromThreePoints(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3) {
	invalidate();
	if (!thePt1.isValid() || !thePt2.isValid() || !thePt3.isValid())
		return;

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
Normalize the plane coefficients, to obtain the plane in its Hessian form
*/
void GM_3dPlane::normalize() {
	if (isValid()) {
		double denom = sqrt(mCoeff[0]*mCoeff[0] + mCoeff[1]*mCoeff[1] + mCoeff[2]*mCoeff[2]);
		if (denom < GM_NULL_TOLERANCE) {
			invalidate();
		}
		else {
			for (unsigned int i = 0 ; i < 4 ; i++) {
				mCoeff[i] /= denom;
			}
		}
	}
}



/*!
\return
true if the plane coefficients are valid  and define a valid plane, false otherwise
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
			if (fabs(mCoeff[i]) > GM_NULL_TOLERANCE) {
				ret = true;
			}
		}
	}
	return ret;
}



/*!
Plane invalidation
*/
void GM_3dPlane::invalidate() {
	for (unsigned int i = 0 ; i < 4 ; i++) {
		mCoeff[i] = DBL_MAX;
	}
}



/*!
Distance between the plane and a given point, or DBL_MAX if the plane or the point are not valid

\param thePoint
Point to use for distance computation
\param thePointOnPlane
On output contains the point on the plane nearest to thePoint, its invalid if this or thePoint are also
invalid

\return
Distance between this and thePoint, or DBL_MAX if this or thePoint are not valid
*/
double GM_3dPlane::pointDistance(const GM_3dPoint& thePoint, GM_3dPoint& thePointOnPlane) const {
	if (!thePoint.isValid() || !isValid())
		return -DBL_MAX;

	return fabs(pointDistanceSgn(thePoint, thePointOnPlane));
}



/*!
Signed distance between the plane and a given point, or DBL_MAX if the plane or the point are not valid.
The distance is positive if the point is in the positive direction of the vector normal to the plane, is
negative otherwise

\param thePoint
Point to use for distance computation
\param thePointOnPlane
On output contains the point on the plane nearest to thePoint, its invalid if this or thePoint are also
invalid

\return
Signed distance between this and thePoint, or DBL_MAX if this or thePoint are not valid
*/
double GM_3dPlane::pointDistanceSgn(const GM_3dPoint& thePoint, GM_3dPoint& thePointOnPlane) const {
	if (!thePoint.isValid() || !isValid())
		return DBL_MAX;

	double ret = pointDistanceSgn(thePoint);
	thePointOnPlane = (GM_3dVector)thePoint - (GM_3dVector(mCoeff[0], mCoeff[1], mCoeff[2]) * ret);

	return ret;
}



/*!
Distance between the plane and a given point, or DBL_MAX if the plane or the point are not valid

\param thePoint
Point to use for distance computation

\return
Distance between this and thePoint, or DBL_MAX if this or thePoint are not valid
*/
double GM_3dPlane::pointDistance(const GM_3dPoint& thePoint) const {
	if (!thePoint.isValid() || !isValid())
		return DBL_MAX;

	return fabs(pointDistanceSgn(thePoint));
}



/*!
Signed distance between the plane and a given point, or DBL_MAX if the plane or the point are not valid.
The distance is positive if the point is in the positive direction of the vector normal to the plane, is
negative otherwise

\param thePoint
Point to use for distance computation

\return
Signed distance between this and thePoint, or DBL_MAX if this or thePoint are not valid
*/
double GM_3dPlane::pointDistanceSgn(const GM_3dPoint& thePoint) const {
	if (!thePoint.isValid() || !isValid())
		return DBL_MAX;

	return (mCoeff[0] * thePoint.x() + mCoeff[1] * thePoint.y() + mCoeff[2] * thePoint.z() + mCoeff[3]);
}



/*!
Equality operator

\param thePlane
Plane to compare with this

\return
true if thePlane is equal to this, false otherwise
*/
bool GM_3dPlane::operator == (const GM_3dPlane& thePlane) const {
	return !((*this) != thePlane);
}



/*!
Inequality operator

\param thePlane
Plane to compare with this

\return
true if thePlane is not equal to this, false otherwise
*/
bool GM_3dPlane::operator != (const GM_3dPlane& thePlane) const {
	if (fabs(mCoeff[0]-thePlane.mCoeff[0]) > GM_NULL_TOLERANCE || fabs(mCoeff[1]-thePlane.mCoeff[1]) > GM_NULL_TOLERANCE || fabs(mCoeff[2]-thePlane.mCoeff[2]) > GM_NULL_TOLERANCE || fabs(mCoeff[3]-thePlane.mCoeff[3]) > GM_NULL_TOLERANCE) {
		return true;
	}
	else {
		return false;
	}
}



/*!
Get a plane coefficient by its index, or DBL_MAX if the index is out of range

\param theIndex
Index of the coefficient to return

\return
The plane coefficient with index theIndex, or DBL_MAX if theIndex is out of range
*/
double GM_3dPlane::operator [](unsigned int theIndex) const {
	if (theIndex < 4) {
		return mCoeff[theIndex];
	}
	else {
		return DBL_MAX;
	}
}



/*!
Get the normal vector of the plane using the plane coefficients

\return
The vector normal to this
*/
GM_3dVector GM_3dPlane::normalVector() const {
	return GM_3dVector(mCoeff[0], mCoeff[1], mCoeff[2]);
}



/*!
Compute the angle that the vector normal to the plane forms with xy plane

\return
The angle that the vector normal to this forms with the xy plane, or DBL_MAX if this is not valid
*/
double GM_3dPlane::xyAngle() const {
	double ret = DBL_MAX;
	if (!isValid())
		return ret;

	GM_3dVector normVector = normalVector();
	double dz = fabs(normVector.z());
	double dxy = sqrt(normVector.x()*normVector.x() + normVector.y()*normVector.y());
	ret = (GM_PI / 2.0) - atan2(dz, dxy);
	
	return ret;
}
