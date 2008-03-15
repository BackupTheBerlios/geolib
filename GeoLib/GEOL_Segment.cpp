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
	mObjType = geol_Segment;
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
	mObjType = geol_Segment;
}


/*!
Default destructor, delete extremity points if its reference counter vanishes
*/
GEOL_Segment::~GEOL_Segment() {
	GEOL_Point *beginPoint = (GEOL_Point*)(getBeginEntity());		
	GEOL_Point *endPoint = (GEOL_Point*)(getEndEntity());

	mBegin = 0.0;
	mEnd = 0.0;
	mLength = 0.0;

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
}


/*!
Set the begin point of the segment, update the segment length and invalidate the bbox

\param theBeginPoint
New begin point of the segment
*/
void GEOL_Segment::begin(const GEOL_Point& theBeginPoint) {
	GEOL_Point *pointPtr = (GEOL_Point*)getBeginEntity();
	*(pointPtr) = theBeginPoint;
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	invalidateBBox();
}


/*!
Set the end point of the segment, update the segment length and invalidate the bbox

\param theEndPoint
New end point of the segment
*/
void GEOL_Segment::end(const GEOL_Point& theEndPoint) {
	GEOL_Point *pointPtr = (GEOL_Point*)getEndEntity();
	*(pointPtr) = theEndPoint;
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	invalidateBBox();
}


/*!
Set the begin point of the segment, update the segment length and invalidate the bbox

\param theXCoord
X coordinate of new begin point of the segment
\param theYCoord
Y coordinate of new begin point of the segment
*/
void GEOL_Segment::begin(double theXCoord, double theYCoord) {
	((GEOL_Point*)getBeginEntity()) -> x(theXCoord);
	((GEOL_Point*)getBeginEntity()) -> y(theYCoord);
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	invalidateBBox();
}


/*!
Set the end point of the segment, update the segment length and invalidate the bbox

\param theXCoord
X coordinate of new end point of the segment
\param theYCoord
Y coordinate of new end point of the segment
*/
void GEOL_Segment::end(double theXCoord, double theYCoord) {
	((GEOL_Point*)getEndEntity()) -> x(theXCoord);
	((GEOL_Point*)getEndEntity()) -> y(theYCoord);
	mLength = ((GEOL_Point*)getBeginEntity()) -> pointDistance((GEOL_Point*)getEndEntity());
	invalidateBBox();
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
Check if an entity is one of the end points of the segment

\param theEntity
Entity to check

\return
- true if the entity is an end point of the segment
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
Notification on object destruction, if the object destroyed is an end point of the segment, the segment itself has
to be destroyed

\param theObject
Object destroyed
\param theDestroyFlag
On exit its true if the segment itself has to be destroyed, false otherwise

\return
- true if the notification succeeds
- false otherwise
*/
bool GEOL_Segment::notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag) {
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
The area between the segment and the x axis (integral), is a signed value that is if the segment direction is x- or
the segment is under the x axis the area will be negative
*/
double GEOL_Segment::area() const {
	GEOL_Point *beginPoint = begin();
	GEOL_Point *endPoint = end();
	double ret = ((endPoint -> y() + beginPoint -> y()) * (endPoint -> x() - beginPoint -> x()) ) / 2.0;

	return ret;
}


/*!
Get the direction of the segment tangent to the point on segment specified

\param theDir
On output contains the tangent direction
\param thePoint
Tangential point on the segment
*/
void GEOL_Segment::direction(GEOL_Point* theDir, const GEOL_Point* thePoint) const {
	if (theDir) {
		direction(theDir, thePoint -> x(), thePoint -> y());	
	}
}


/*!
Get the direction of the segment tangent to the point on segment specified

\param theDir
On output contains the tangent direction
\param theXCoord
X coordinate of the tangential point on the segment
\param theYCoord
Y coordinate of the tangential point on the segment
*/
void GEOL_Segment::direction(GEOL_Point* theDir, double theXCoord, double theYCoord) const {
	if (theDir) {
		theDir -> x(end() -> x() - begin() -> x());
		theDir -> y(end() -> y() - begin() -> y());
	}
}


/*!
\return
The bounding box of the segment
*/
GEOL_BBox GEOL_Segment::getBBox() {
	if (!mBBox || !mBBox -> isValid()) {
		GEOL_BBox bbox;
		GEOL_Point *beginPoint = begin();
		GEOL_Point *endPoint = end();
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
		setBBox(bbox);
	}
	
	return *mBBox;
}


/*!
Translate a segment

\param theDX
x translation
\param theDY
y translation
*/
void GEOL_Segment::translate(double theDX, double theDY) {
	((GEOL_Point*)getBeginEntity()) -> translate(theDX, theDY);
	((GEOL_Point*)getEndEntity()) -> translate(theDX, theDY);
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


/*!
Load the segment from a binary file

\param theStream
Binary file

\return
- true if the load operation succeed
- false otherwise
*/
bool GEOL_Segment::LoadBinary(ifstream *theStream) {
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
Save the segment in a binary file

\param theStream
Binary file

\return
- true if the save operation succeed
- false otherwise
*/
bool GEOL_Segment::SaveBinary(ofstream *theStream) {
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
bool GEOL_Segment::LoadISO(ifstream *theStream) {
	return true;
}


/*!
Save the segment in ISO format on a file

\param theStream
ISO file

\return
- true if the save operation succeed
- false otherwise
*/
bool GEOL_Segment::SaveISO(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		(*theStream) << "G1 X" << end() -> x() << " Y" << end() -> y() << endl;
	}
	
	return ret;
}

