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
	mObjType = geol_Point;
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
	mObjType = geol_Point;
}


/*!
Copy constructor
*/
GEOL_Point::GEOL_Point(const GEOL_Point& thePoint) : GEOL_Entity() {
	mBegin = thePoint.x();
	mEnd = thePoint.y();
	mObjType = geol_Point;
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
Points is unaffected by the destruction of other objects, so this method does nothing except the check of
parameters correctness

\param theObject
The object that will be destroyed

\return
- true if theObject is non null
- false otherwise
*/
bool GEOL_Point::notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag) {
	theDestroyFlag = false;
	if (theObject)
		return true;
	else
		return false;
}



GEOL_BBox GEOL_Point::getBBox() {
	return GEOL_BBox();
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


/*!
Load point's data from a strem in binary format

\param theStream
The stream to read from

\return
- true if the read operation succeed
- false otherwise
*/
bool GEOL_Point::LoadBinary(std::ifstream *theStream) {
	if (!theStream)
		return false;
		
	bool ret = !theStream -> bad();
	if (ret) {
		double xCoord = 0.0;
		double yCoord = 0.0;
		theStream -> read((char*)(&xCoord), sizeof(double));
		theStream -> read((char*)(&yCoord), sizeof(double));
		ret = !theStream -> bad();
		if (ret) {
			x(xCoord);
			y(yCoord);
			ret = laodBinaryObjectAttributes(theStream);
		}	
	}
	return ret;
}


/*!
Save point's data on a stream in binary mode

\param theStream
The stream to write on

\return
- true if the write operation succeed
- false otherwise
*/
bool GEOL_Point::SaveBinary(std::ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		ret = saveBinaryObjectInfo(theStream, geol_Point);
	}
	if (ret) {
		double xCoord = x();
		double yCoord = y();
		theStream -> write((char*)(&xCoord), sizeof(double));
		theStream -> write((char*)(&yCoord), sizeof(double));
		ret = saveBinaryObjectAttributes(theStream);		
	}

	if (ret) {
		GEOL_AttributeValue attrVal;
		attrVal.GEOL_AttrVoidValue = NULL;
		addAttribute(attrVal, GEOL_AttrVoid, "saved");
	}

	return ret;
}

bool GEOL_Point::LoadISO(std::ifstream *theStream) {
	if (!theStream)
		return false;

	return false;
}

