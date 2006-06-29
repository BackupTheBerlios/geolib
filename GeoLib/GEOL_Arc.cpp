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
	mObjType = geol_Arc;
}


/*!
Default destructor
*/
GEOL_Arc::~GEOL_Arc() {
	GEOL_Point *beginPoint = (GEOL_Point*)(getBeginEntity());
	GEOL_Point *endPoint = (GEOL_Point*)(getEndEntity());
	if (beginPoint) {
		getContext() -> deleteObject(beginPoint);
	}
	if (endPoint) {
		getContext() -> deleteObject(endPoint);
	}
	mBegin = 0.0;
	mEnd = 0.0;
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
Check if the entity passed is one of the end points of the segment

\param theEntity
Entity to check

\return
- true if the entity passed is an end point of the segment
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




GEOL_BBox GEOL_Arc::getBBox() {
	if (!mBBox || !mBBox -> isValid()) {
		GEOL_BBox bbox;
		
		GEOL_ArcVersus arcVersus = versus();
		double xCenter = 0.0;
		double yCenter = 0.0;
		center(xCenter, yCenter);
		
		GEOL_Point *beginPoint = begin();
		GEOL_Point *endPoint = end();

		int beginQuad = -1;
		if (beginPoint -> x() <= xCenter && beginPoint -> y() > yCenter) {
			beginQuad = 0;
		}
		else if (beginPoint -> x() > xCenter && beginPoint -> y() > yCenter) {
			beginQuad = 1;
		}
		else if (beginPoint -> x() > xCenter && beginPoint -> y() <= yCenter) {
			beginQuad = 2;
		}
		else if (beginPoint -> x() <= xCenter && beginPoint -> y() <= yCenter) {
			beginQuad = 3;
		}
		
		int endQuad = -1;
		if (endPoint -> x() <= xCenter && endPoint -> y() > yCenter) {
			endQuad = 0;
		}
		else if (endPoint -> x() > xCenter && endPoint -> y() > yCenter) {
			endQuad = 1;
		}
		else if (endPoint -> x() > xCenter && endPoint -> y() <= yCenter) {
			endQuad = 2;
		}
		else if (endPoint -> x() <= xCenter && endPoint -> y() <= yCenter) {
			endQuad = 3;
		}

		bool quadCrossingFlags[4];
		for (int i = 0 ; i < 4 ; i++) {
			quadCrossingFlags[i] = false;
		}
		
		if (beginQuad != endQuad) {
			int quad = beginQuad;
			do {
				if (arcVersus == GEOL_ArcClockwise) {
					quad = (quad + 1) % 4;
				}
				else {
					quad = (quad - 1) % 4;
				}
				quadCrossingFlags[quad] = true;
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
		
		if (quadCrossingFlags[0]) {
			bbox.setMinX(xCenter - mRadius);
		}
		else if (quadCrossingFlags[1]) {
			bbox.setMaxY(yCenter + mRadius);
		}
		else if (quadCrossingFlags[2]) {
			bbox.setMaxX(xCenter + mRadius);
		}
		else if (quadCrossingFlags[3]) {
			bbox.setMinY(yCenter - mRadius);
		}
		
		setBBox(bbox);
	}
	
	return *mBBox;
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


bool GEOL_Arc::LoadBinary(std::ifstream *theStream) {
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

bool GEOL_Arc::SaveBinary(std::ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		ret = saveBinaryObjectInfo(theStream, geol_Arc);
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
	addAttribute(attrVal, GEOL_AttrVoid, "saved");

	return ret;
}

bool GEOL_Arc::LoadISO(std::ifstream *theStream) {
	if (!theStream)
		return false;

	return false;
}
