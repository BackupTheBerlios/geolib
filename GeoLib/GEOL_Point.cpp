/***************************************************************************
								GEOL_Point.cpp
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

#include "GEOL_Point.h"


/*!
Default constructor
*/
GEOL_Point::GEOL_Point() : GEOL_Entity() {
	mBegin = 0.0;
	mEnd = 0.0;
}


/*!
Default destructor
*/
GEOL_Point::~GEOL_Point() {
	mBegin = 0.0;
	mEnd = 0.0;
}


/*!
Constructor wit X,Y coordinate as parameters

\param theXCoord
The X coordinate
\param theYCoord
The Y coordinate
*/
GEOL_Point::GEOL_Point(double theXCoord, double theYCoord) : GEOL_Entity() {	
	mBegin = theXCoord;
	mEnd = theYCoord;
}


/*!
Copy constructor
*/
GEOL_Point::GEOL_Point(const GEOL_Point& thePoint) : GEOL_Entity() {
	mBegin = thePoint.x();
	mEnd = thePoint.y();
}


/*!
Compute the distance between this point and the point passed as parameter

\param thePoint
The point to compute the distance with

\return
The distance between this point and thePoint
*/
double GEOL_Point::pointDistance(const GEOL_Point& thePoint) const {
	double deltaX = fabs(mBegin - thePoint.x());
	double deltaY = fabs(mEnd - thePoint.y());
	
	return sqrt(deltaX*deltaX + deltaY*deltaY);
}



/*!
\sa pointDistance(const GEOL_Point& thePoint)
*/
double GEOL_Point::pointDistance(const GEOL_Point* thePoint) const {
	return pointDistance(*thePoint);
}


/*!
Compute the distance between this point and the axis origin

\return
The distance between this point and the axis origin
*/
double GEOL_Point::originDistance() {
	return sqrt(mBegin * mBegin + mEnd * mEnd);	
}





/*!
Assignment operator

\param thePoint
Reference to the assigned point

\return
The reference to this point
*/
GEOL_Point& GEOL_Point::operator=(const GEOL_Point& thePoint) {
	mBegin = thePoint.x();
	mEnd = thePoint.y();
	return *this;
}



/*!
Equality operator, two points are equal if their distance is less then GEOL_EQUAL_POINT constant, that represent
the tolerance for points equality

\param thePoint
Points to compare with this

\return
- true if the points are equal within the tolerance
- false otherwise
*/
bool GEOL_Point::operator==(const GEOL_Point& thePoint) const {
	if (pointDistance(thePoint) < GEOL_EQUAL_POINT) {
		return true;
	}
	else {
		return false;
	}
}


/*!
Inequality operator

\sa operator==(const GEOL_Point& thePoint)
*/
bool GEOL_Point::operator!=(const GEOL_Point& thePoint) const {
	return !((*this) == thePoint);
}



