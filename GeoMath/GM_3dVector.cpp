/********************************************************************
* File: GM_3dVector.cpp												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_3dVector.h"



/*!
Default constructor
*/
GM_3dVector::GM_3dVector(void) : GM_3dPoint() {
}



/*!
Copy constructor
*/
GM_3dVector::GM_3dVector(const GM_3dVector& theVect) : GM_3dPoint(theVect) {
}


/*!
Constructor from 3D point

\param thePoint
Point to use for vector construction
*/
GM_3dVector::GM_3dVector(const GM_3dPoint& thePoint) : GM_3dPoint(thePoint){
}



/*!
Constructor from coordinates values

\param theXCoord
X coord
\param theYCoord
Y coord
\param theZCoord
Z coord
*/
GM_3dVector::GM_3dVector(double theXCoord, double theYCoord, double theZCoord)  : GM_3dPoint(theXCoord, theYCoord, theZCoord) {
}



/*!
Constructor from a 3D line

\param theLine
Line to use for vector construction
*/
GM_3dVector::GM_3dVector(const GM_3dLine& theLine) : GM_3dPoint() {
	if (theLine.isValid()) {
		mX = theLine.end().x() - theLine.begin().x();
		mY = theLine.end().y() - theLine.begin().y();
		mZ = theLine.end().z() - theLine.begin().z();
	}
}



/*!
Construct a vector on xy plane with the given angle respect to X axes (positive = counterclockwise)

\param theXYAngle
Angle respect to X axes
*/
GM_3dVector::GM_3dVector(double theXYAngle) : GM_3dPoint() {
	mX = cos(theXYAngle);
	mY = sin(theXYAngle);
	mZ = 0.0;
}



/*!
Default destructor
*/
GM_3dVector::~GM_3dVector(void) {
}



/*!
Dot product between vectors

\param theVect
Vector for dot product computation with this

\return
Dot product between this and theVect, the length of projection of theVect on this times the module of
this, or DBL_MAX if the vectors are not valid
*/
double GM_3dVector::operator*(const GM_3dVector& theVect) const {
	if (isValid() && theVect.isValid()) {
		return (mX * theVect.x()) + (mY * theVect.y()) + (mZ * theVect.z());
	}
	else {
		return DBL_MAX;
	}
}



/*!
Scalar product

\param theFactor
Scalar multiplier

\return
The vector obtained multiplying this with theFactor, or this if is not a valid vector
*/
GM_3dVector GM_3dVector::operator*(double theFactor) const {
	if (isValid()) {
		return GM_3dVector(mX*theFactor, mY*theFactor, mZ*theFactor);
	}
	else {
		return *this;
	}
}



/*!
Cross product between two vectors

\param theVect
Vector for cross product computation with this

\return
Cross product of theVect and this, is the vector normal to the plane defined by this and theVect, with
module equal to the area of the parallelogram defined by this and theVect, and versus defined with the
right hand rule.
Or an invalid vector if this ot theVect are not valid
*/
GM_3dVector GM_3dVector::operator^(const GM_3dVector& theVect) const {
	if (isValid() && theVect.isValid()) {
		return GM_3dVector(	mY * theVect.z() - mZ * theVect.y(),
							mZ * theVect.x() - mX * theVect.z(),
							mX * theVect.y() - mY * theVect.x() );
	}
	else {
		return GM_3dVector();
	}
}



/*!
Sum of two vectors

\param theVect
Vector to sum with this

\return
Vector obtained summing this and theVect, or an invalid vector if this ot theVect are not valid
*/
GM_3dVector GM_3dVector::operator+(const GM_3dVector& theVect) const {
	if (isValid() && theVect.isValid()) {
		return GM_3dVector(mX + theVect.x(), mY + theVect.y(), mZ + theVect.z());
	}
	else {
		return GM_3dVector();
	}
}



/*!
Difference of two vectors

\param theVect
Vettore da subtract from this

\return
Difference between this and theVect, or an invalid vector if this ot theVect are not valid
*/
GM_3dVector GM_3dVector::operator-(const GM_3dVector& theVect) const {
	if (isValid() && theVect.isValid()) {
		return GM_3dVector(mX - theVect.x(), mY - theVect.y(), mZ - theVect.z());
	}
	else {
		return GM_3dVector();
	}
}



/*!
Module of a vector, its length

\return
Module of this, or -DBL_MAX is this isn't valid
*/
double GM_3dVector::mod() const {
	if (isValid()) {
		return distFrom(GM_3dPoint(0.0, 0.0, 0.0));
	}
	else {
		return -DBL_MAX;
	}
}



/*!
Vector normalization, normalize its module to 1, nothing happens if this isn't valid
*/
void GM_3dVector::normalize() {
	if (isValid()) {
		double module = mod();
		if (module > GM_NULL_TOLERANCE) {
			mX /= module;
			mY /= module;
			mZ /= module;
		}
	}
}



/*!
Compute the angle between the vector pojectod on xy plane an the x axes (positive = counterclockwise)

\return
Angle between this projecten on xy plane and the x axes, return a value in the interval [0 ; 2*PI], positive
angles is counterclockwise, or -DBL_MAX if this isn't valid
*/
double GM_3dVector::xyAngle() const {
	double ret = -DBL_MAX;
	if (isValid()) {
		ret = atan2(mY,mX);
		if (ret < 0.0) {
			ret = 2.0 * GM_PI + ret;
		}
	}

	return ret;
}



/*!
Compute the interior angle between two vectors projected un xy plane

\param theVect
Vector used for angle computation

\return
Interior angle between this and theVect projected on xy plane , return a value in the interval [0 ; 2*PI],
positive angles is counterclockwise
*/
double GM_3dVector::xyAngle(const GM_3dVector& theVect) const {
	double ret = -DBL_MAX;
	if (isValid() && theVect.isValid()) {
		double ang1 = atan2(mY,mX);
		if (ang1 < 0.0) {
			ang1 = 2.0 * GM_PI + ang1;
		}
		double ang2 = atan2(theVect.y(),theVect.x());
		if (ang2 < 0.0) {
			ang2 = 2.0 * GM_PI + ang2;
		}
		ret = ang2 - ang1;
		if (ret < 0.0) {
			ret = 2.0*GM_PI + ret;
		}
	}
	
	return ret;
}



/*!
Determine the relative position of two vectors projected on xy plane

\return
true if this is at left of theVect, false otherwise or if the vectors are not valid, the two vectors is
projected on xy plane
*/
bool GM_3dVector::isAtLeftOnXY(const GM_3dVector& theVect) const {
	if (isValid() && theVect.isValid()) {
		return theVect.x()*mY-mX*theVect.y() > 0.0;
	}
	else {
		return false;
	}
}



/*!
Determine if this and the vector passed are linearly dependent or not

\param theVect
Vector to check against this

\return
true if this and theVect are linearly dependent, false otherwise or if the vectors are not valid
*/
bool GM_3dVector::isLinearlyDependent(const GM_3dVector& theVect) const {
	bool ret = false;
	if (isValid() || theVect.isValid()) {
		double dot00 = (*this) * (*this);
		double dot01 = (*this) * theVect;
		double dot10 = theVect * (*this);
		double dot11 = theVect * theVect;
		double dotDet = (dot00 * dot11) - (dot01 * dot10);
		if (dotDet < GM_NULL_TOLERANCE) {
			ret = true;
		}
		else {
			ret = false;
		}
	}

	return ret;
}
