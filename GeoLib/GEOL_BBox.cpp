/***************************************************************************
								GEOL_BBox.cpp
                             -------------------
    begin                : 25.6.2006
    copyright            : (C) 2006 by Claudio Cordara
    email                : claudiocordara@libero.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "GEOL_Prefix.h"

#include "GEOL_Point.h"
#include "GEOL_BBox.h"


/*!
Default constructor
*/
GEOL_BBox::GEOL_BBox() {
	mValid = false;
	mMinX = DBL_MAX;
	mMaxX = -DBL_MAX;
	mMinY = DBL_MAX;
	mMaxY = -DBL_MAX;
}


/*!
*/
GEOL_BBox::GEOL_BBox(double theMinX, double theMinY, double theMaxX, double theMaxY) {
	mMinX = theMinX;
	mMaxX = theMaxX;
	mMinY = theMinY;
	mMaxY = theMaxY;
	checkValidity();
}


/*!
Copy constructor
*/
GEOL_BBox::GEOL_BBox(const GEOL_BBox& theBBox) {
	mValid = theBBox.mValid;
	mMinX = theBBox.mMinX;
	mMaxX = theBBox.mMaxX;
	mMinY = theBBox.mMinY;
	mMaxY = theBBox.mMaxY;	
}


/*!
Default destructor
*/
GEOL_BBox::~GEOL_BBox() {
	mValid = false;
	mMinX = DBL_MAX;
	mMaxX = -DBL_MAX;
	mMinY = DBL_MAX;
	mMaxY = -DBL_MAX;
}


/*!
Set the maximum x limit of the bounding box and check its validity

\param theMaxX
The new maximum x limit of the bounding box
*/
void GEOL_BBox::setMaxX(double theMaxX) {
	mMaxX = theMaxX;
	checkValidity();
}


/*!
Set the maximum y limit of the bounding box and check its validity

\param theMaxY
The new maximum y limit of the bounding box
*/
void GEOL_BBox::setMaxY(double theMaxY) {
	mMaxY = theMaxY;
	checkValidity();
}


/*!
Set the minimum x limit of the bounding box and check its validity

\param theMinX
The new minimum x limit of the bounding box
*/
void GEOL_BBox::setMinX(double theMinX) {
	mMinX = theMinX;
	checkValidity();
}


/*!
Set the minimum y limit of the bounding box and check its validity

\param theMinY
The new minimum y limit of the bounding box
*/
void GEOL_BBox::setMinY(double theMinY) {
	mMinY = theMinY;
	checkValidity();
}


/*!
Check the validity of the boundig box limits and set the mValid flag
*/
void GEOL_BBox::checkValidity() {
	if (mMinX <= mMaxX && mMinY <= mMaxY) {
		mValid = true;
	}
	else {
		mValid = false;
	}
}


/*!
Invalidate the bounding box
*/
void GEOL_BBox::invalidate() {
	mValid = false;
	mMinX = mMinY = DBL_MAX;
	mMaxX = mMaxY = -DBL_MAX;
}


/*!
\return
The area of the bounding box, 0 if the boundig box is invalid
*/
double GEOL_BBox::area() const {
	double ret = 0.0;
	if (mValid) {
		ret = (mMaxX - mMinX) * (mMaxY - mMinY);
	}
	
	return ret;
}


/*!
Assignment operator
*/
GEOL_BBox& GEOL_BBox::operator=(const GEOL_BBox& theBBox) {
	mValid = theBBox.mValid;
	mMinX = theBBox.mMinX;
	mMaxX = theBBox.mMaxX;
	mMinY = theBBox.mMinY;
	mMaxY = theBBox.mMaxY;	

	return *this;
}


/*!
Equality operator
*/
bool GEOL_BBox::operator==(const GEOL_BBox& theBBox) const {
	if (fabs(mMinX < theBBox.getMinX()) < GEOL_EQUAL_DIST &&
		fabs(mMaxX < theBBox.getMaxX()) < GEOL_EQUAL_DIST &&
		fabs(mMinY < theBBox.getMinY()) < GEOL_EQUAL_DIST &&
		fabs(mMaxY < theBBox.getMaxY()) < GEOL_EQUAL_DIST)
		
			return true;
	else
			return false;
}


/*!
Compute the union of this bounding box and the bounding box specified

\param theBBox
Bounding box to unify with this

\return
The union of this and theBBox
*/
const GEOL_BBox GEOL_BBox::operator+(const GEOL_BBox& theBBox) const {
	GEOL_BBox ret;
	
	if (mMinX < theBBox.getMinX()) {
		ret.setMinX(mMinX);
	}
	else {
		ret.setMinX(theBBox.getMinX());
	}
	if (mMaxX > theBBox.getMaxX()) {
		ret.setMaxX(mMaxX);
	}
	else {
		ret.setMaxX(theBBox.getMaxX());
	}
	if (mMinY < theBBox.getMinY()) {
		ret.setMinY(mMinY);
	}
	else {
		ret.setMinY(theBBox.getMinY());
	}
	if (mMaxY > theBBox.getMaxY()) {
		ret.setMaxY(mMaxY);
	}
	else {
		ret.setMaxY(theBBox.getMaxY());
	}
	
	return ret;
}


/*!
Compute the intersection of this bounding box against the bounding box specified

\param theBBox
Bounding box to intersect

\return
The intersection of this and theBBox
*/
GEOL_BBox GEOL_BBox::intersect(const GEOL_BBox& theBBox) const {
	GEOL_BBox ret;
	if (isIntersected(theBBox)) {
		ret.setMinX(theBBox.getMinX() > mMinX ? theBBox.getMinX() : mMinX);
		ret.setMaxX(theBBox.getMaxX() < mMaxX ? theBBox.getMaxX() : mMaxX);
		ret.setMinY(theBBox.getMinY() > mMinY ? theBBox.getMinY() : mMinY);
		ret.setMaxY(theBBox.getMaxY() < mMaxY ? theBBox.getMaxY() : mMaxY);
	}
	
	return ret;
}


/*!
Intersection check between this and another bounding box

\param theBBox
Bounding box to check

\return
- true if this and theBBox intersects
- false otherwise
*/
bool GEOL_BBox::isIntersected(const GEOL_BBox& theBBox) const {
	bool ret = true;
	if (theBBox.getMinX() > mMaxX || theBBox.getMaxX() < mMinX || theBBox.getMinY() > mMaxY || theBBox.getMaxY() < mMinY) {
		ret = false;
	}
	
	return ret;
}


/*!
Containment check of a point in this bounding box

\param theXCoord
X coord of the point to check
\param theYCoord
Y coord of the point to check

\return
- true if this contains thePoint
- false otherwise
*/
bool GEOL_BBox::isPointInBBox(double theXCoord, double theYCoord) const {
	if (theXCoord >= mMinX && theXCoord <= mMaxX && theYCoord >= mMinY && theYCoord <= mMaxY)
		return true;
	else
		return false;
}


/*!
Containment check of a point in this bounding box

\param thePoint
Point to check

\return
- true if this contains thePoint
- false otherwise
*/
bool GEOL_BBox::isPointInBBox(const GEOL_Point& thePoint) const {
	return isPointInBBox(thePoint.x(), thePoint.y());
}

