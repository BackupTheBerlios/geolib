/***************************************************************************
								 GEOL_Arc.cpp
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

#include <math.h>

#include "GEOL_Context.h"
#include "GEOL_Point.h"
#include "GEOL_Arc.h"




/*!
Default constructor
*/
GEOL_Arc::GEOL_Arc() : GEOL_Entity(){
	mRadius = 0.0;
	mLength = 0.0;
}





/*!
Costructor with begin, end points and radius as parameters

\param theBeginPoint
Begin point
\param theEndPoint
End point
\param theRadius
Radius
*/
GEOL_Arc::GEOL_Arc(GEOL_Point* theBeginPoint, GEOL_Point* theEndPoint, double theRadius, GEOL_ArcVersus theVersus) : GEOL_Entity() {
	setBeginEntity((GEOL_Entity*)theBeginPoint);
	setEndEntity((GEOL_Entity*)theEndPoint);
	
	if (theVersus == GEOL_ArcClockwise) {
		mRadius = fabs(theRadius);
	}
	else {
		mRadius = -fabs(theRadius);
	}
	mRadius = theRadius;
	
	double linearLength = (theBeginPoint) -> pointDistance(theEndPoint);
	if (2.0 * fabs(mRadius) > linearLength) {
		mLength = computeLength();
	}
	else {
		mRadius = linearLength / 2.0;
		mLength = GEOL_PI * mRadius;
	}
}


/*!
Default destructor
*/
GEOL_Arc::~GEOL_Arc() {
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
	mRadius = 0.0;
}



/*!
Set the begin point of the arc

\param theBeginPoint
New begin point of the arc
*/
void GEOL_Arc::begin(const GEOL_Point& theBeginPoint) {
	*((GEOL_Point*)getBeginEntity()) = theBeginPoint;

	double linearLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	if (2.0 * fabs(mRadius) < linearLength) {
		mLength = computeLength();
	}
	else {
		mRadius = linearLength / 2.0;
		mLength = linearLength;
	}
}


/*!
Set the end point of the arc

\param theEndPoint
New end point of the arc
*/
void GEOL_Arc::end(const GEOL_Point& theEndPoint) {
	*((GEOL_Point*)getEndEntity()) = theEndPoint;

	double linearLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	if (2.0 * fabs(mRadius) < linearLength) {
		mLength = computeLength();
	}
	else {
		mRadius = linearLength / 2.0;
		mLength = linearLength;
	}
}


/*!
Set the begin point of the arc

\param theXCoord
X coordinate of new begin point of the arc
\param theYCoord
Y coordinate of new begin point of the arc
*/
void GEOL_Arc::begin(double theXCoord, double theYCoord) {
	((GEOL_Point*)getBeginEntity()) -> x(theXCoord);
	((GEOL_Point*)getBeginEntity()) -> y(theYCoord);

	double linearLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	if (2.0 * fabs(mRadius) < linearLength) {
		mLength = computeLength();
	}
	else {
		mRadius = linearLength / 2.0;
		mLength = linearLength;
	}
}


/*!
Set the end point of the arc

\param theXCoord
X coordinate of new end point of the arc
\param theYCoord
Y coordinate of new end point of the arc
*/
void GEOL_Arc::end(double theXCoord, double theYCoord) {
	((GEOL_Point*)getEndEntity()) -> x(theXCoord);
	((GEOL_Point*)getEndEntity()) -> y(theYCoord);

	double linearLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	if (2.0 * fabs(mRadius) < linearLength) {
		mLength = computeLength();
	}
	else {
		mRadius = linearLength / 2.0;
		mLength = linearLength;
	}
}


/*
Set the radius of the arc without modify its sign

\param theRadius
*/
void GEOL_Arc::radius(double theRadius) {
	theRadius = fabs(theRadius);
	if (mRadius < 0.0) {
		mRadius = -theRadius;
	}
	else {
		mRadius = theRadius;
	}
}


/*!
Set the versus of the arc modifing its radius sign, clockwise versus has positive radius counterclockwise versus
has negative radius

\param theVersus
Versus of the arc
*/
void GEOL_Arc::versus(GEOL_ArcVersus theVersus) {
	if (theVersus == GEOL_ArcClockwise) {
		mRadius = fabs(mRadius);
	}
	else {
		mRadius = -fabs(mRadius);
	}
}


/*!
\return
The versus of the arc based on the sing of its radius, clockwise versus has positive radius counterclockwise
versus has negative radius
*/
GEOL_ArcVersus GEOL_Arc::versus() const {
	GEOL_ArcVersus ret = GEOL_ArcClockwise;
	
	if (mRadius >= 0.0) {
		ret = GEOL_ArcClockwise;
	}
	else {
		ret = GEOL_ArcCounterClockwise;
	}
	
	return ret;
}


/*!
Compute the angle of the arc from the radius and chord length

\return
The angle of the arc
*/
double GEOL_Arc::angle() const {
	double linearLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	double radius = fabs(mRadius);
	double angle = acos(1.0 - ((linearLength * linearLength) / (2.0 * radius * radius)));
	
	return angle;
}


/*!
*/
void GEOL_Arc::center(GEOL_Point &theCenter) const {
	double xCenter = 0.0;
	double yCenter = 0.0;
	center(xCenter, yCenter);
	theCenter.x(xCenter);
	theCenter.y(yCenter);
}


/*!
*/
void GEOL_Arc::center(double &theXCenter, double &theYCenter) const {
	double deltaX = end() -> x() - begin() -> x();
	double deltaY = end() -> y() - begin() -> y();
	double linearLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	deltaX /= linearLength;
	deltaY /= linearLength;
	double midX = begin() -> x() + ((end() -> x() - begin() -> x()) / 2.0);
	double midY = begin() -> y() + ((end() -> y() - begin() -> y()) / 2.0);
	if (mRadius > 0.0) {
		double tmp = deltaX;
		deltaX = deltaY;
		deltaY = - tmp;
	}
	else {
		double tmp = deltaX;
		deltaX = deltaY;
		deltaY = tmp;
	}
	
	double arcAngle = angle();
	double centerDist = mRadius * cos(arcAngle / 2.0);
	theXCenter = midX + fabs(centerDist) * deltaX;
	theYCenter = midY + fabs(centerDist) * deltaY;
}


/*!
Compute the lenght of the arc

\return
The length of the arc
*/
double GEOL_Arc::computeLength() const {
	double radius = fabs(mRadius);
	double ang = angle();
	return radius * ang;
}



/*!
Assignment operator

\param theArc
Reference to the assigned arc

\return
The reference to this arc
*/
GEOL_Arc& GEOL_Arc::operator=(const GEOL_Arc& theArc) {
	*((GEOL_Point*)getBeginEntity()) = *(theArc.begin());
	*((GEOL_Point*)getEndEntity()) = *(theArc.end());
	mRadius = theArc.radius();
	mLength = theArc.length();
	return *this;
}


/*!
Equality operator, two arcs are equal if their extremities and radius are equal

\param theArc
Arc to compare with this

\return
- true if the arcs are equal
- false otherwise
*/
bool GEOL_Arc::operator==(const GEOL_Arc& theArc) const {
	if (*((GEOL_Point*)getBeginEntity()) == *(theArc.begin()) && *((GEOL_Point*)getEndEntity()) == *(theArc.end()) &&
			fabs(mRadius - theArc.radius()) < GEOL_EQUAL_POINT ) {
		return true;
	}
	else {
		return false;
	}
}


/*!
Inequality operator

\sa operator==(const GEOL_Arc& theArc)
*/
bool GEOL_Arc::operator!=(const GEOL_Arc& theArc) const {
	return !((*this) == theArc);
}

