/***************************************************************************
								GEOL_Segment.cpp
                             -------------------
    begin                : 25.06.2004
    copyright            : (C) 2004 by Claudio Cordara
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

#include "GEOL_Context.h"
#include "GEOL_Point.h"
#include "GEOL_Segment.h"


/*!
Default constructor
*/
GEOL_Segment::GEOL_Segment() : GEOL_Entity() {
	mLength = 0.0;
}


/*!
Costructor with begin and end points as parameters

\param theBeginPoint
Begin point
\param theEndPoint
End point
*/
GEOL_Segment::GEOL_Segment(GEOL_Point* theBeginPoint, GEOL_Point* theEndPoint) : GEOL_Entity() {
	setBeginEntity((GEOL_Entity*)theBeginPoint);
	setEndEntity((GEOL_Entity*)theEndPoint);
	mLength = theBeginPoint -> pointDistance(theEndPoint);
}


/*!
Default destructor
*/
GEOL_Segment::~GEOL_Segment() {
	if (mBegin) {
		GEOL_Point *beginPoint = (GEOL_Point*)(getBeginEntity());		
		getContext() -> deletePoint(beginPoint);
		mBegin = 0.0;
	}
	if (mEnd) {
		GEOL_Point *endPoint = (GEOL_Point*)(getEndEntity());
		getContext() -> deletePoint(endPoint);
		mEnd = 0.0;
	}
	mLength = 0.0;
}


/*!
Set the begin point of the segment, and update the segment length

\param theBeginPoint
New begin point of the segment
*/
void GEOL_Segment::begin(const GEOL_Point& theBeginPoint) {
	GEOL_Point *pointPtr = (GEOL_Point*)getBeginEntity();
	*(pointPtr) = theBeginPoint;
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
}


/*!
Set the end point of the segment, and update the segment length

\param theEndPoint
New end point of the segment
*/
void GEOL_Segment::end(const GEOL_Point& theEndPoint) {
	GEOL_Point *pointPtr = (GEOL_Point*)getEndEntity();
	*(pointPtr) = theEndPoint;
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
}


/*!
Set the begin point of the segment, and update the segment length

\param theXCoord
X coordinate of new begin point of the segment
\param theYCoord
Y coordinate of new begin point of the segment
*/
void GEOL_Segment::begin(double theXCoord, double theYCoord) {
	((GEOL_Point*)getBeginEntity()) -> x(theXCoord);
	((GEOL_Point*)getBeginEntity()) -> y(theYCoord);
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
}


/*!
Set the end point of the segment, and update the segment length

\param theXCoord
X coordinate of new end point of the segment
\param theYCoord
Y coordinate of new end point of the segment
*/
void GEOL_Segment::end(double theXCoord, double theYCoord) {
	((GEOL_Point*)getEndEntity()) -> x(theXCoord);
	((GEOL_Point*)getEndEntity()) -> y(theYCoord);
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
}


/*!
\return
The angle between the segment and the x axis
*/
double GEOL_Segment::angle() const {
	double dx = ((GEOL_Point*)getEndEntity()) -> x() - ((GEOL_Point*)getBeginEntity()) -> x();
	double dy = ((GEOL_Point*)getEndEntity()) -> y() - ((GEOL_Point*)getBeginEntity()) -> y();
	return atan2(dx, dy);
}


/*!
\return
The angle between this segment and the segment passed
*/
double GEOL_Segment::angleWith(const GEOL_Segment& theSegment) const {
	double thisAngle = angle();
	double segmentAngle = theSegment.angle();
	double angleDiff = thisAngle - segmentAngle;
	if (angleDiff < - M_PI) {
		angleDiff += 2.0 * M_PI;
	}
	else if (angleDiff > M_PI) {
		angleDiff -= 2.0 * M_PI;
	}
	
	return angleDiff;
}


/*!
Check if the entity passed is one of the end points of the segment

\param theEntity
Entity to check

\return
- true if the entity passed is an end point of the segment
- false otherwise
*/
bool GEOL_Segment::isEndPoint(const GEOL_Entity *theEntity) {
	bool ret = false;
	if (theEntity == getBeginEntity() || theEntity == getEndEntity()) {
		ret = true;
	}
	
	return ret;
}


/*!
Assignment operator

\param theSegment
Reference to the assigned segment

\return
The reference to this segment
*/
GEOL_Segment& GEOL_Segment::operator=(const GEOL_Segment& theSegment) {
	*((GEOL_Point*)getBeginEntity()) = *(theSegment.begin());
	*((GEOL_Point*)getEndEntity()) = *(theSegment.end());
	mLength = theSegment.length();
	return *this;
}


/*!
Equality operator, two segments are equal if their extremities are equal

\param theSegment
Segment to compare with this

\return
- true if the segments are equal
- false otherwise
*/
bool GEOL_Segment::operator==(const GEOL_Segment& theSegment) const {	
	if (*((GEOL_Point*)getBeginEntity()) == *(theSegment.begin()) && *((GEOL_Point*)getEndEntity()) == *(theSegment.end())) {
		return true;
	}
	else {
		return false;
	}
}


/*!
Inequality operator

\sa operator==(const GEOL_Segment& theSegment)
*/
bool GEOL_Segment::operator!=(const GEOL_Segment& theSegment) const {
	return !((*this) == theSegment);
}




