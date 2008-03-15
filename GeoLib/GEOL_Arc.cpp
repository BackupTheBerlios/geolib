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

#include "GEOL_Context.h"
#include "GEOL_Point.h"
#include "GEOL_Arc.h"


/*!
Default constructor
*/
GEOL_Arc::GEOL_Arc() : GEOL_Entity() {
	mRadius = 0.0;
	mLength = 0.0;
	mObjType = geol_Arc;
}


/*!
Costructor with begin, end points and radius as parameters

\param theBeginPoint
Begin point
\param theEndPoint
End point
\param theRadius
Radius
\param theVersus
Versus of the arc
*/
GEOL_Arc::GEOL_Arc(GEOL_Point* theBeginPoint, GEOL_Point* theEndPoint, double theRadius, GEOL_ArcVersus theVersus) : GEOL_Entity() {
	setBeginEntity((GEOL_Entity*)theBeginPoint);
	setEndEntity((GEOL_Entity*)theEndPoint);
	
	if (theVersus == geol_ArcClockwise) {
		mRadius = fabs(theRadius);
	}
	else {
		mRadius = -fabs(theRadius);
	}
	
	double linearLength = (theBeginPoint) -> pointDistance(theEndPoint);
	if (2.0 * fabs(mRadius) > linearLength) {
		mLength = computeLength();
	}
	else {
		mRadius = versus() == geol_ArcClockwise ? linearLength / 2.0 : -linearLength / 2.0;
		mLength = GEOL_PI * mRadius;
	}
	mObjType = geol_Arc;
}


/*!
Default destructor, delete extremity points if its reference counter vanishes
*/
GEOL_Arc::~GEOL_Arc() {
	GEOL_Point *beginPoint = (GEOL_Point*)(getBeginEntity());
	GEOL_Point *endPoint = (GEOL_Point*)(getEndEntity());
	
	if (beginPoint) {
		if (!beginPoint -> decRefCount()) {
			getContext() -> deleteObject(beginPoint, true);
		}
	}
	if (endPoint) {
		if (!endPoint -> decRefCount()) {
			getContext() -> deleteObject(endPoint, true);
		}
	}
	mBegin = 0.0;
	mEnd = 0.0;
	mRadius = 0.0;
}


/*!
Set the begin point of the arc, update the arc length and invalidate the bbox

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
		mRadius = versus() == geol_ArcClockwise ? linearLength / 2.0 : -linearLength / 2.0;
		mLength = linearLength;
	}
	invalidateBBox();
}


/*!
Set the end point of the arc, update the arc length and invalidate the bbox

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
		mRadius = versus() == geol_ArcClockwise ? linearLength / 2.0 : -linearLength / 2.0;
		mLength = linearLength;
	}
	invalidateBBox();
}


/*!
Set the begin point of the arc, update the arc length and invalidate the bbox

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
		mRadius = versus() == geol_ArcClockwise ? linearLength / 2.0 : -linearLength / 2.0;
		mLength = linearLength;
	}
	invalidateBBox();
}


/*!
Set the end point of the arc, update the arc length and invalidate the bbox

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
		mRadius = versus() == geol_ArcClockwise ? linearLength / 2.0 : -linearLength / 2.0;
		mLength = linearLength;
	}
	invalidateBBox();
}


/*
Set the radius of the arc without modify its sign, invalidate the bbox

\param theRadius
The radius
*/
void GEOL_Arc::radius(double theRadius) {
	theRadius = fabs(theRadius);
	if (mRadius < 0.0) {
		mRadius = -theRadius;
	}
	else {
		mRadius = theRadius;
	}
	invalidateBBox();
}


/*!
Set the versus of the arc modifing its radius sign, clockwise versus has positive radius counterclockwise versus
has negative radius, invalidate the bbox

\param theVersus
Versus of the arc
*/
void GEOL_Arc::versus(GEOL_ArcVersus theVersus) {
	if (theVersus == geol_ArcClockwise) {
		mRadius = fabs(mRadius);
	}
	else {
		mRadius = -fabs(mRadius);
	}
	invalidateBBox();
}


/*!
\return
The versus of the arc based on the sing of its radius, clockwise versus has positive radius counterclockwise
versus has negative radius
*/
GEOL_Arc::GEOL_ArcVersus GEOL_Arc::versus() const {
	GEOL_ArcVersus ret = geol_ArcClockwise;
	
	if (mRadius >= 0.0) {
		ret = geol_ArcClockwise;
	}
	else {
		ret = geol_ArcCounterClockwise;
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
Return the center coordinates of the arc in the parameter

\param theCenter
On exit contains the coordinates of the center
*/
void GEOL_Arc::center(GEOL_Point &theCenter) const {
	double xCenter = 0.0;
	double yCenter = 0.0;
	center(xCenter, yCenter);
	theCenter.x(xCenter);
	theCenter.y(yCenter);
}


/*!
Return the center coordinates of the arc in the two parameters, center calculation is carried out
measuring the distance between the center of the arc and the mid point of the begin-end segment using only
the radius and the arc angle.
With this distance and the direction of the normal to the begin-end segment is easy to compute the center
coordinates

\param theXCenter
On exit contains the x coordinate of the center
\param theYCenter
On exit contains the y coordinate of the center
*/
void GEOL_Arc::center(double &theXCenter, double &theYCenter) const {
	// Normalized vector of begin-end direction
	double deltaX = end() -> x() - begin() -> x();
	double deltaY = end() -> y() - begin() -> y();
	double linearLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	deltaX /= linearLength;
	deltaY /= linearLength;
	
	// Mid point on the segment from begin to end points
	double midX = begin() -> x() + ((end() -> x() - begin() -> x()) / 2.0);
	double midY = begin() -> y() + ((end() -> y() - begin() -> y()) / 2.0);
	
	// Direction of the normal to the segment from begin to end, at the right of the segment if mRadius > 0 and
	// arc is clockwise, at the left otherwise
	if (mRadius > 0.0) {
		double tmp = deltaX;
		deltaX = deltaY;
		deltaY = - tmp;
	}
	else {
		double tmp = deltaX;
		deltaX = - deltaY;
		deltaY = tmp;
	}

	// Distance from the center of the arc and the center of the begin-end segment
	double arcAngle = angle();
	assert(arcAngle < GEOL_PI);
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
Check if an entity is one of the end points of the arc

\param theEntity
Entity to check

\return
- true if the entity is an end point of the segment
- false otherwise
*/
bool GEOL_Arc::isEndPoint(const GEOL_Entity *theEntity) {
	bool ret = false;
	if (theEntity == getBeginEntity() || theEntity == getEndEntity()) {
		ret = true;
	}
	
	return ret;
}


/*!
Notification on object destruction, if the object destroied is an end point of the arc, the arc itself has
to be destroyed

\param theObject
Object destroyed
\param theDestroyFlag
On exit its true if the arc itself has to be destroyed, false otherwise

\return
- true if the notification succeeds
- false otherwise
*/
bool GEOL_Arc::notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag) {
	theDestroyFlag = false;
	if (!theObject)
		return false;
		
	bool ret = true;
	if (isEndPoint((GEOL_Entity*)theObject)) {
		theDestroyFlag = true;
	}
	
	return ret;
}



/*!
\return
The area between the arc and the x axis (integral), is a signed value that is if the arc is counterclockwise the area
will be negative
Computation can be divided in two cases :
- The arc is all in the semicircle above or below the circle horizontal axis
  The area is A = Q + S - T, where Q is the trapezoid formed by the extremities of the arc and its projection on x axes,
  S is the area of the circle sector below the arc and T is the triangle defined by the extremities and the center of the
  arc
- The arc is not all in the semicircle above or below the circle horizontal axis
  Dividing the arc in two pieces where the horizontal axis is intersected, the computation can be reduced at the previous case
*/
double GEOL_Arc::area() const {
	double ret = -1.0;
	double xCenter = 0.0;
	double yCenter = 0.0;
	center(xCenter, yCenter);

	int beginQuad = getBeginQuad(xCenter, yCenter);
	int endQuad = getEndQuad(xCenter, yCenter);

	double trapezoidArea = 0.0;
	double arcSectorArea = 0.0;
	double triangleArea = 0.0;

	if ((beginQuad < geol_BottomLeft && endQuad < geol_BottomLeft) || (beginQuad > geol_TopLeft && endQuad > geol_TopLeft)) {
		// Arc is all above or below x axis
		
		GEOL_Point *beginPoint = NULL;
		GEOL_Point *endPoint = NULL;
		if(versus() == geol_ArcClockwise) {
			beginPoint = begin();
			endPoint = end();
		}
		else {
			beginPoint = end();
			endPoint = begin();
		}

		double alfa = endPoint -> angle(xCenter, yCenter);
		double beta = beginPoint -> angle(xCenter, yCenter);
		
		if (fabs(alfa - GEOL_PI) < GEOL_EQUAL_ANGLE) {
			if (endPoint -> x() < beginPoint -> x()) {
				alfa = alfa - GEOL_2PI;
			}
		}
		if (fabs(beta - GEOL_PI) < GEOL_EQUAL_ANGLE) {
			if (endPoint -> x() > beginPoint -> x()) {
				beta = beta - GEOL_2PI;
			}
		}

		trapezoidArea = ((endPoint -> x() - beginPoint -> x()) * (beginPoint -> y() + endPoint -> y())) / 2.0;
		arcSectorArea = ((beta - alfa) * radius() * radius()) / 2.0;
		triangleArea = (radius() * radius() * sin(beta - alfa)) / 2.0;
	}
	else {
		// Arc cross x axis
		
		GEOL_Point *beginPoint = NULL;
		GEOL_Point *endPoint = NULL;
		if(versus() == geol_ArcClockwise) {
			beginPoint = begin();
			endPoint = end();
		}
		else {
			beginPoint = end();
			endPoint = begin();
		}

		double alfa = endPoint -> angle(xCenter, yCenter);
		double beta = beginPoint -> angle(xCenter, yCenter);

		if (beginQuad == geol_TopRight || beginQuad == geol_TopLeft) {
			// begin above x axis, end below
			
			double xQuad = xCenter + radius();
			double yQuad = yCenter;
			
			double deltaAngle = fabs(beta - alfa);
			if (deltaAngle > GEOL_PI) {
				deltaAngle = GEOL_2PI - deltaAngle;
			}

			trapezoidArea = ((xQuad - beginPoint -> x()) * (beginPoint -> y() + yQuad)) / 2.0;
			trapezoidArea += ((endPoint -> x() - xQuad) * (endPoint -> y() + yQuad)) / 2.0;
			arcSectorArea = (deltaAngle * radius() * radius()) / 2.0;
			triangleArea = (radius() * radius() * sin(-alfa)) / 2.0;
			triangleArea += (radius() * radius() * sin(beta)) / 2.0;
		}
		else {
			// begin below x axis, end above
			
			double xQuad = xCenter - radius();
			double yQuad = yCenter;
			
			double deltaAngle = fabs(alfa - beta);
			if (deltaAngle > GEOL_PI) {
				deltaAngle = GEOL_2PI - deltaAngle;
			}

			trapezoidArea = ((xQuad - beginPoint -> x()) * (beginPoint -> y() + yQuad)) / 2.0;
			trapezoidArea += ((endPoint -> x() - xQuad) * (endPoint -> y() + yQuad)) / 2.0;
			arcSectorArea = (deltaAngle * radius() * radius()) / 2.0;
			triangleArea = (radius() * radius() * sin(alfa)) / 2.0;
			triangleArea += (radius() * radius() * sin(-beta)) / 2.0;
		}
	}

	ret = trapezoidArea + arcSectorArea - triangleArea;
	
	if (versus() == geol_ArcCounterClockwise) {
		ret = -ret;
	}
	
	return ret;
}


/*!
Compute the direction of the tangent to the arc at a given point, just computig the perpendicular
to the direction from the center of the arc to the given point

\param thePoint
Tangent point on the arc
\param theDir
On output the direction of the tangent to the arc at point thePoint
*/
void GEOL_Arc::direction(GEOL_Point* theDir, const GEOL_Point* thePoint) const {
	if (theDir && thePoint) {
		direction(theDir, thePoint -> x(), thePoint -> y());
	}
}



/*!
Compute the direction of the tangent to the arc at a given point, just computig the perpendicular
to the direction from the center of the arc to the given point

\param theDir
On output the direction of the tangent to the arc at point thePoint
\param theXCoord
X coordinate of the tangent point on the arc
\param theYCoord
Y coordinate of the tangent point on the arc
*/
void GEOL_Arc::direction(GEOL_Point* theDir, double theXCoord, double theYCoord) const {
	if (theDir) {
		double xCenter = 0.0;
		double yCenter = 0.0;
		center(xCenter, yCenter);
		theDir -> x(theXCoord - xCenter);
		theDir -> y(theYCoord - yCenter);
		
		double tmp = theDir -> x();
		theDir -> x(theDir -> y());
		theDir -> y(-tmp);
		
		if(versus() == geol_ArcClockwise) {
		}
		else {
			double tmp = theDir -> x();
			theDir -> x(-theDir -> y());
			theDir -> y(-tmp);
		}
	}
}


/*!
Return the quadrant of the begin point of the arc

\param theXCenter
x coordinate of the arc center
\param theYCenter
y coordinate of the arc center

\return
The quadrant in the usual way
geol_TopRight    = x+y+ (0)
geol_TopLeft     = x+y- (1)
geol_BottomLeft  = x-y- (2)
geol_BottomRight = x-y+ (3)
*/
GEOL_Quadrant GEOL_Arc::getBeginQuad(double theXCenter, double theYCenter) const {
	return begin() -> quadrant(theXCenter, theYCenter);
}


/*!
Return the quadrant of the end point of the arc

\param theXCenter
x coordinate of the arc center
\param theYCenter
y coordinate of the arc center

\return
The quadrant in the usual way
geol_TopRight    = x+y+ (0)
geol_TopLeft     = x+y- (1)
geol_BottomLeft  = x-y- (2)
geol_BottomRight = x-y+ (3)
*/
GEOL_Quadrant GEOL_Arc::getEndQuad(double theXCenter, double theYCenter) const {
	return end() -> quadrant(theXCenter, theYCenter);
}


/*!
\return
The bounding box of the arc
*/
GEOL_BBox GEOL_Arc::getBBox() {
	if (!mBBox || !mBBox -> isValid()) {
		GEOL_BBox bbox;
		
		GEOL_ArcVersus arcVersus = versus();
		double xCenter = 0.0;
		double yCenter = 0.0;
		center(xCenter, yCenter);
		
		GEOL_Point *beginPoint = begin();
		GEOL_Point *endPoint = end();

		GEOL_Quadrant beginQuad = getBeginQuad(xCenter, yCenter);		
		GEOL_Quadrant endQuad = getEndQuad(xCenter, yCenter);

		bool quadCrossingFlags[4];
		for (int i = 0 ; i < 4 ; i++) {
			quadCrossingFlags[i] = false;
		}
		
		if (beginQuad != endQuad) {
			GEOL_Quadrant quad = beginQuad;
			do {
				if (arcVersus == geol_ArcClockwise) {
					quadCrossingFlags[quad] = true;
					quad = getPrevQuadrant(quad);
				}
				else {
					quad = getNextQuadrant(quad);
					quadCrossingFlags[quad] = true;
				}
			}
			while (quad != endQuad);
		}

		if (beginPoint -> x() < endPoint -> x()) {
			bbox.setMinX(beginPoint -> x());
			bbox.setMaxX(endPoint -> x());
		}
		else {
			bbox.setMinX(endPoint -> x());
			bbox.setMaxX(beginPoint -> x());
		}
		if (beginPoint -> y() < endPoint -> y()) {
			bbox.setMinY(beginPoint -> y());
			bbox.setMaxY(endPoint -> y());
		}
		else {
			bbox.setMinY(endPoint -> y());
			bbox.setMaxY(beginPoint -> y());
		}
		
		if (quadCrossingFlags[0]) { // geol_BottomRight to geol_TopRight crossing
			bbox.setMaxX(xCenter + fabs(mRadius));
		}
		else if (quadCrossingFlags[1]) {	// geol_TopRight to geol_TopLeft crossing
			bbox.setMaxY(yCenter + fabs(mRadius));
		}
		else if (quadCrossingFlags[2]) { // geol_TopLeft to geol_BottomLeft crossing
			bbox.setMinX(xCenter - fabs(mRadius));
		}
		else if (quadCrossingFlags[3]) { // geol_BottomLeft to geol_BottomRight crossing
			bbox.setMinY(yCenter - fabs(mRadius));
		}
		
		setBBox(bbox);
	}
	
	return *mBBox;
}


/*!
Translate an arc

\param theDX
x translation
\param theDY
y translation
*/
void GEOL_Arc::translate(double theDX, double theDY) {
	((GEOL_Point*)getBeginEntity()) -> translate(theDX, theDY);
	((GEOL_Point*)getEndEntity()) -> translate(theDX, theDY);
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


/*!
Load the arc from a binary file

\param theStream
Binary file

\return
- true if the load operation succeed
- false otherwise
*/
bool GEOL_Arc::LoadBinary(ifstream *theStream) {
	if (!theStream)
		return false;

	GEOL_ObjectType objType = geol_Point;

	bool ret = !theStream -> bad();
	if (ret) {
		GEOL_Context *context = getContext();
		
		ret = context -> loadBinaryObjectType(theStream, objType);
		if (ret && objType == geol_Point) {
			GEOL_Point *pt1 = context -> createPoint();
			ret = ((GEOL_Entity*)pt1) -> LoadBinary(theStream);
			if (ret) {
				ret = context -> loadBinaryObjectType(theStream, objType);
				if (ret && objType == geol_Point) {
					GEOL_Point *pt2 = context -> createPoint();
					ret = ((GEOL_Entity*)pt2) -> LoadBinary(theStream);
					if (ret) {
						setBeginEntity(pt1);
						setEndEntity(pt2);
						theStream -> read((char*)(&mRadius), sizeof(double));
						theStream -> read((char*)(&mLength), sizeof(double));
						ret = laodBinaryObjectAttributes(theStream);
					}
				}
			}
		}
	}

	return ret;
}


/*!
Save the arc in a binary file

\param theStream
Binary file

\return
- true if the save operation succeed
- false otherwise
*/
bool GEOL_Arc::SaveBinary(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		ret = saveBinaryObjectInfo(theStream);
		if (ret) {
			ret = ((GEOL_Entity*)getBeginEntity()) -> SaveBinary(theStream);
			if (ret) {
				ret = ((GEOL_Entity*)getEndEntity()) -> SaveBinary(theStream);
				if (ret) {
					theStream -> write((char*)(&mRadius), sizeof(double));
					theStream -> write((char*)(&mLength), sizeof(double));
					ret = saveBinaryObjectAttributes(theStream);
				}			
			}
		}
	}
	if (ret) {
		ret = !theStream -> bad();
	}
	
	GEOL_AttributeValue attrVal;
	attrVal.GEOL_AttrVoidValue = NULL;
	addAttribute(attrVal, GEOL_Attribute::GEOL_AttrVoid, GEOL_ID_SAVED);

	return ret;
}


/*!
Load in ISO format is entilrely performed at GEOL_Profile level

\return
Always true
*/
bool GEOL_Arc::LoadISO(ifstream *theStream) {
	return true;
}


/*!
Save the arc in ISO format on a file

\param theStream
ISO file

\return
- true if the save operation succeed
- false otherwise
*/
bool GEOL_Arc::SaveISO(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		double xCenter = 0.0;
		double yCenter = 0.0;
		center(xCenter, yCenter);
		
		if (versus() == geol_ArcClockwise) {
			(*theStream) << "G2";
		}
		else {
			(*theStream) << "G3";
		}
		(*theStream) << " X" << end() -> x() << " Y" << end() -> y() << " I" << xCenter << " J" << yCenter << endl;
	}
	
	return ret;
}
