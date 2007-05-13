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



/*!
Default constructor
*/
GM_3dPoint::GM_3dPoint(void) {
	invalidate();
}



/*!
Default destructor
*/
GM_3dPoint::~GM_3dPoint(void) {
}



/*!
Copy constructor
*/
GM_3dPoint::GM_3dPoint(const GM_3dPoint& thePoint) {
	mX = thePoint.mX;
	mY = thePoint.mY;
	mZ = thePoint.mZ;
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
GM_3dPoint::GM_3dPoint(double theXCoord, double theYCoord, double theZCoord) {
	mX = theXCoord;
	mY = theYCoord;
	mZ = theZCoord;
}



/*!
Construct a point on xy plane with the given angle respect to X axes (positive = counterclockwise)

\param theXYAngle
Angle respect to X axes
*/
GM_3dPoint::GM_3dPoint(double theXYAngle) {
	mX = cos(theXYAngle);
	mY = sin(theXYAngle);
	mZ = 0.0;
}



/*!
Compute the distance between this and a given point

\param thePoint
Point for distance computation

\return
The distance between this an thePoint, or -DBL_MAX if points are not valid
*/
double GM_3dPoint::distFrom(const GM_3dPoint& thePoint) const {
	if (!isValid() || !thePoint.isValid())
		return -DBL_MAX;
	double dx = mX - thePoint.x();
	double dy = mY - thePoint.y();
	double dz = mZ - thePoint.z();
	return sqrt(dx*dx + dy*dy + dz*dz);
}



/*!
Check if this is the origin of axes

\return
true if this is the axes origin, false otherwise or if this is not valid
*/
bool GM_3dPoint::isOrigin() const {
	if (fabs(mX) < GM_NULL_TOLERANCE && fabs(mY) < GM_NULL_TOLERANCE && fabs(mZ) < GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
Revert the sign of xy coordinates, 180 deg. rotation on xy plane
*/
void GM_3dPoint::xyInvert() {
	mX = -mX;
	mY = -mY;
}



/*!
\return
true if the point coordinates are valid, false otherwise (not initialized point)
*/
bool GM_3dPoint::isValid() const {
	if (mX != DBL_MAX && mX != -DBL_MAX && mY != DBL_MAX && mY != -DBL_MAX && mZ != DBL_MAX && mZ != -DBL_MAX)
		return true;
	else
		return false;
}



/*!
Invalidate point coordinates
*/
void GM_3dPoint::invalidate() {
	mX = mY = mZ = DBL_MAX;
}



/*!
Casting operator with 3D Vectors
*/
GM_3dPoint::operator GM_3dVector() const {
	return GM_3dVector(*this);
}



/*!
Equality operator

\param thePoint
Point to compare with this

\return
true if thePoint is equal to this within tolerance (GM_NULL_TOLERANCE), false otherwise
*/
bool GM_3dPoint::operator == (const GM_3dPoint& thePoint) const {
	return !((*this) != thePoint);
}



/*!
Inequality operator

\param thePoint
Point to compare with this

\return
true if thePoint is not equal to this within tolerance (GM_NULL_TOLERANCE), false otherwise
*/
bool GM_3dPoint::operator != (const GM_3dPoint& thePoint) const {
	if (fabs(mX - thePoint.mX) > GM_NULL_TOLERANCE || fabs(mY - thePoint.mY) > GM_NULL_TOLERANCE || fabs(mZ - thePoint.mZ) > GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}
