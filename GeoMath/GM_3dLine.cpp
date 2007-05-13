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



/*!
Default constructor
*/
GM_3dLine::GM_3dLine(void) {
	invalidate();
}



/*!
Default destructor
*/
GM_3dLine::~GM_3dLine(void) {
}



/*!
Copy constructor
*/
GM_3dLine::GM_3dLine(const GM_3dLine& theLine) {
	mBegin = theLine.mBegin;
	mEnd = theLine.mEnd;
}



/*!
Constructor from two points

\param theBegin
Begin point
\param theEnd
End point
*/
GM_3dLine::GM_3dLine(GM_3dPoint theBegin, GM_3dPoint theEnd) {
	mBegin = theBegin;
	mEnd = theEnd;
}



/*!
Constructor from two points coordinates

\param theBeginXCoord
X coord of begin point
\param theBeginYCoord
Y coord of begin point
\param theBeginZCoord
Z coord of begin point
\param theEndXCoord
X coord of end point
\param theEndYCoord
Y coord of end point
\param theEndZCoord
Z coord of end point
*/
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
Length of the line
*/
double GM_3dLine::length() const {
	if (isValid()) {
		return mBegin.distFrom(mEnd);
	}
	else {
		return -DBL_MAX;
	}
}



/*!
Dot product between two lines, the length of the projection of theLine on this times the
module of this
The lines are intended with start point in the origin

\param theLine
Line for dot product computation with this

\return
Dot product between this and theLine, the length of the projection of theLine on this times the
module of this
Or an invalid line if this ot theLine are not valid
*/
double GM_3dLine::operator*(const GM_3dLine& theLine) const {
	if (isValid() && theLine.isValid()) {
		GM_3dVector v1(*this);
		GM_3dVector v2(theLine);
		return v1 * v2;
	}
	else {
		return DBL_MAX;
	}
}



/*!
Cross product between two lines, the lines are intended with start point in the origin

\param theLine
Line for cross product computation with this

\return
Cross product of theLine and this, is the line normal to the plane defined by this and theLine, with
module equal to the area of the parallelogram defined by this and theLine, and versus defined with the
right hand rule.
Or an invalid line if this ot theLine are not valid
The lines are intended with start point in the origin
*/
GM_3dLine GM_3dLine::operator^(const GM_3dLine& theLine) const {
	if (isValid() && theLine.isValid()) {
		GM_3dVector v1(*this);
		GM_3dVector v2(theLine);
		GM_3dVector externalProd = v1 ^ v2;
		return GM_3dLine(mBegin, (GM_3dVector)mBegin + externalProd);
	}
	else {
		return GM_3dLine();		
	}
}



/*!
Equality operator

\param theLine
Line to compare with this

\return
true if theLine is equal to this, false otherwise
*/
bool GM_3dLine::operator == (const GM_3dLine& theLine) const {
	return !((*this) != theLine);
}




/*!
Inequality operator

\param theLine
Line to compare with this

\return
true if theLine is not equal to this, false otherwise
*/
bool GM_3dLine::operator != (const GM_3dLine& theLine) const {
	if (mBegin != theLine.mBegin || mEnd != theLine.mEnd)
		return true;
	else
		return false;
}



/*!
\return
true if the the line has the projection on the axis less than GM_NULL_TOLERANCE, false otherwise
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
Minimum Z value of the line
*/
double GM_3dLine::minZ() const {
	return mBegin.z() < mEnd.z() ? mBegin.z() : mEnd.z();
}



/*!
\return
Maximum Z value of the line
*/
double GM_3dLine::maxZ() const {
	return mBegin.z() > mEnd.z() ? mBegin.z() : mEnd.z();
}



/*!
\return
true if the line is vertical, that is component on Z axes > of GM_NULL_TOLERANCE and the components on X and Y
axes < of GM_NULL_TOLERANCE, false otherwise
*/
bool GM_3dLine::isVertical() const {
	if (fabs(dx()) < GM_NULL_TOLERANCE && fabs(dy()) < GM_NULL_TOLERANCE && fabs(dz()) > GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
\return
true if the line is horizontal, that is component on Z axes < GM_NULL_TOLERANCE, false otherwise
*/
bool GM_3dLine::isHorizontal() const {
	if (fabs(dz()) < GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}



/*!
Compute the point of the line with a given Z component

\param theZLevel
Z component to use in point computation

\return
Point on the line with Z component equal to theZLevel, if theZLevel is less then minim Z of the line return
a point with -DBL_MAX in its components, if it is greater then maximum Z return a point with DBL_MAX in its
components
If the line is not valid return an invalid point
If the line is horizontal with Z component equal to theZLevel return the begin point of the line
*/
GM_3dPoint GM_3dLine::pointAtZ(double theZLevel) const {
	if (!isValid()) {
		return GM_3dPoint();
	}
	if (theZLevel < minZ() - GM_DIFF_TOLERANCE) {
		return GM_3dPoint(-DBL_MAX, -DBL_MAX, -DBL_MAX);
	}
	if (theZLevel > maxZ() + GM_DIFF_TOLERANCE) {
		return GM_3dPoint(DBL_MAX, DBL_MAX, DBL_MAX);
	}

	if (dz() < GM_NULL_TOLERANCE && fabs(theZLevel - mBegin.z()) < GM_DIFF_TOLERANCE) {
		return mBegin;
	}
	else {
		double ratio = fabs(theZLevel - begin().z()) / fabs(dz());
		return GM_3dPoint(begin().x() + dx()*ratio, begin().y() + dy()*ratio, theZLevel);
	}
}



/*!
Invert the direction of the line swapping its begin and end point, nothing happens if the line
is invalid
*/
void GM_3dLine::invert() {
	if (isValid()) {
		GM_3dPoint swap = mBegin;
		mBegin = mEnd;
		mEnd = swap;
	}
}



/*!
Compute the vector normal to the projection of the line on xy plane, with direction on the right/left
side of the line accordingly to the parameter

\param theRightSideFlag
If true the normal vector on the right side of the line will be computated, otherwise the vector on the
left side

\return
The vector normal to the projection of the line on xy plane, with direction on the right/left side of the
line accordingly the value of theRightSideFlag
The returned vector is normalized
If the line is invalid an invalid vector is returned, if the line ha null length the null vector is returned
*/
GM_3dVector GM_3dLine::normalXYVector(bool theRightSideFlag) const {
	if (!isValid()) {
		return GM_3dVector();
	}

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
Compute the angle that the projection of the line on xy plane forms with the X axes

\return
The angle that the projection of the line on the xy plane forms with X axes in the range [0 , 2*PI]
If the line is invalid return DBL_MAX
*/
double GM_3dLine::xyAngle() const {
	if (isValid()) {
		GM_3dVector lineDirOnXY(dx(), dy(), 0.0);
		return lineDirOnXY.xyAngle();
	}
	else {
		return DBL_MAX;
	}
}



/*!
Compute the interior angle between two lines projecten on xy plane

\param theLine
Line used for angle computation

\return
Interior angle between this and theLine projected on xy plane , return a value in the interval [0 ; 2*PI],
positive angles is counterclockwise, or DBL_MAX if this or theLine are not valid
*/
double GM_3dLine::xyAngle(GM_3dLine& theLine) const {
	if (isValid() && theLine.isValid()) {
		GM_3dVector lineDirOnXY(dx(), dy(), 0.0);
		GM_3dVector theLineDirOnXY(theLine.dx(), theLine.dy(), 0.0);
		return lineDirOnXY.xyAngle(theLineDirOnXY);
	}
	else {
		return DBL_MAX;
	}
}



/*!
\return
true if the line coordinates are valid points, false otherwise
*/
bool GM_3dLine::isValid() const {
	return mBegin.isValid() && mEnd.isValid();
}



/*!
Line invalidation
*/
void GM_3dLine::invalidate() {
	mBegin.invalidate();
	mEnd.invalidate();
}



/*!
Compute the center poit of the line

\return
The center point of the line, or an invalid point if the line is not valid
*/
GM_3dPoint GM_3dLine::center() const {
	if (isValid()) {
		double x = mBegin.x() + ((mEnd.x() - mBegin.x()) / 2.0);
		double y = mBegin.y() + ((mEnd.y() - mBegin.y()) / 2.0);
		double z = mBegin.z() + ((mEnd.z() - mBegin.z()) / 2.0);
		return GM_3dPoint(x, y, z);
	}
	else {
		return GM_3dPoint();
	}
}



/*!
Compute a point on the line from a parameter in [0,1] interval, where 0 is the begin and 1 is the end
of the line

\param theSection
Line section to use for point computation

\return
Point on the line at the section theSectio, return an invalid point if the line is invalid or theSection is
out of range
*/
GM_3dPoint GM_3dLine::pointFromSection(double theSection) const {
	GM_3dPoint ret;
	if (!isValid() || theSection < -GM_DIFF_TOLERANCE || theSection > 1.0 + GM_DIFF_TOLERANCE) {
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
From a point on the line compute the section on the line corrisponding to the specified point, the section
is in [0,1] interval, where 0 is the begin and 1 is the end of the line.

\param thePoint
Point on the line to use for section computation

\return
Section of the line computed from thePoint, 0 = begin 1 = end, return -DBL_MAX if the line or thePoinr are
not valid or if thePoint is not on the line
*/
double GM_3dLine::sectionFromPoint(GM_3dPoint thePoint) const {
	double ret = -DBL_MAX;
	if (!isValid() || !thePoint.isValid())
		return ret;

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
Distance between the line and a specified point

\param thePoint
Point to use for distance computation
Punto di cui calcolare la distanza dalla linea
\param thePointOnLine
On output contains the point on the line nearest to thePoint, is invalid if the line thePoint are also invalid

\return
The distance of thePoint from this, or -DBL_MAX if this or thePoint are not valid
*/
double GM_3dLine::pointDistance(GM_3dPoint thePoint, GM_3dPoint& thePointOnLine) const {
	double ret = -DBL_MAX;
	thePointOnLine.invalidate();
	if (!isValid() || !thePoint.isValid())
		return ret;

	GM_3dVector v = (GM_3dVector)mEnd - (GM_3dVector)mBegin;
	GM_3dVector w = (GM_3dVector)thePoint - (GM_3dVector)mBegin;

	double c1 = w * v;
	double c2 = v * v;

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



/*!
Distance between the line and a specified point

\param thePoint
Point to use for distance computation
Punto di cui calcolare la distanza dalla linea

\return
The distance of thePoint from this, or -DBL_MAX if this or thePoint are not valid
*/
double GM_3dLine::pointDistance(GM_3dPoint thePoint) const {
	GM_3dPoint dummyPoint;
	return pointDistance(thePoint, dummyPoint);
}




/*!
\return
Length of the line projection on x axis, or -DBL_MAX if this isn't valid
*/
double GM_3dLine::dx() const {
	if (isValid()) {
		return mEnd.x() - mBegin.x();
	}
	else {
		return -DBL_MAX;
	}
}



/*!
\return
Length of the line projection on y axis, or -DBL_MAX if this isn't valid
*/
double GM_3dLine::dy() const {
	if (isValid()) {
		return mEnd.y() - mBegin.y();
	}
	else {
		return -DBL_MAX;
	}
}



/*!
\return
Length of the line projection on z axis, or -DBL_MAX if this isn't valid
*/
double GM_3dLine::dz() const {
	if (isValid()) {
		return mEnd.z() - mBegin.z();
	}
	else {
		return -DBL_MAX;
	}
}



/*!
\return
The begin point of the line
*/
GM_3dPoint GM_3dLine::begin() const {
	return mBegin;
}



/*!
\return
The end point of the line
*/
GM_3dPoint GM_3dLine::end() const {
	return mEnd;
}



/*!
\return
The begin point of the line by reference
*/
GM_3dPoint& GM_3dLine::begin() {
	return mBegin;
}



/*!
\return
The end point of the line by reference
*/
GM_3dPoint& GM_3dLine::end() {
	return mEnd;
}


/*!
Set the begin point of the line

\param theBegin
Point to assign at the begin of the line
*/
void GM_3dLine::begin(const GM_3dPoint& theBegin) {
	mBegin = theBegin;
}



/*!
Set the end point of the line

\param theEnd
Point to assign at the end of the line
*/
void GM_3dLine::end(const GM_3dPoint& theEnd) {
	mEnd = theEnd;
}
