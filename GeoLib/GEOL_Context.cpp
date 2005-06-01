/***************************************************************************
                               GEOL_Context.cpp
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

#include "GEOL_Object.h"

#include "GEOL_Point.h"
#include "GEOL_Segment.h"
#include "GEOL_Arc.h"
#include "GEOL_Profile.h"
#include "GEOL_PoliProfile.h"

#include "GEOL_Context.h"


GEOL_Context::GEOL_Context() {
}

/*!
The destructor remove and destroy all the objects of the context
*/
GEOL_Context::~GEOL_Context() {
	removeAllObjects();
}



GEOL_Point* GEOL_Context::createPoint() {
	GEOL_Point *newPoint = new GEOL_Point();
	if (newPoint)
		addObject((GEOL_Object*)newPoint);
	return newPoint;
}


GEOL_Point* GEOL_Context::createPoint(double theXCoord, double theYCoord) {
	GEOL_Point *newPoint = new GEOL_Point(theXCoord, theYCoord);
	if (newPoint)
		addObject((GEOL_Object*)newPoint);
	return newPoint;
}


GEOL_Point* GEOL_Context::createPoint(const GEOL_Point& thePoint) {
	GEOL_Point *newPoint = new GEOL_Point(thePoint);
	if (newPoint)
		addObject((GEOL_Object*)newPoint);
	return newPoint;
}


GEOL_Segment* GEOL_Context::createSegment() {
	GEOL_Segment *newSegment = new GEOL_Segment();
	if (newSegment)
		addObject((GEOL_Object*)newSegment);
	return newSegment;
}


GEOL_Segment* GEOL_Context::createSegment(const GEOL_Point& theBeginPoint, const GEOL_Point& theEndPoint) {
	if (theBeginPoint == theEndPoint) {
		return NULL;
	}
	
	GEOL_Point *newBeginPoint = new GEOL_Point(theBeginPoint);
	if (!newBeginPoint)
		return NULL;
	GEOL_Point *newEndPoint = new GEOL_Point(theEndPoint);
	if (!newEndPoint) {
		delete newBeginPoint;
		return NULL;
	}
	GEOL_Segment *newSegment = new GEOL_Segment(newBeginPoint, newEndPoint);
	if (!newSegment) {
		delete newBeginPoint;
		delete newEndPoint;
		return NULL;
	}
	
	addObject((GEOL_Object*)newBeginPoint);
	addObject((GEOL_Object*)newEndPoint);
	addObject((GEOL_Object*)newSegment);
	return newSegment;
}


GEOL_Segment* GEOL_Context::createSegment(GEOL_Point* theBeginPoint, GEOL_Point* theEndPoint) {
	if ((*theBeginPoint) == (*theEndPoint)) {
		return NULL;
	}
	
	bool refCountFlag = false;
	refCountFlag = theBeginPoint -> incRefCount();
	if (!refCountFlag)
		return NULL;
	refCountFlag = theEndPoint -> incRefCount();
	if (!refCountFlag)
		return NULL;
	
	GEOL_Segment *newSegment = new GEOL_Segment(theBeginPoint, theEndPoint);
	if (newSegment) {
		addObject((GEOL_Object*)newSegment);
	}
	return newSegment;
}


GEOL_Segment* GEOL_Context::createSegment(const GEOL_Segment& theSegment) {
	GEOL_Point *newBeginPoint = new GEOL_Point(*(theSegment.begin()));
	if (!newBeginPoint)
		return NULL;
	GEOL_Point *newEndPoint = new GEOL_Point(*(theSegment.end()));
	if (!newEndPoint) {
		delete newBeginPoint;
		return NULL;
	}
	GEOL_Segment *newSegment = new GEOL_Segment(newBeginPoint, newEndPoint);
	if (!newSegment) {
		delete newBeginPoint;
		delete newEndPoint;
		return NULL;
	}
	addObject((GEOL_Object*)newSegment);
	return newSegment;
}


GEOL_Segment* GEOL_Context::createSegment(double theXStart, double theYStart, double theXEnd, double theYEnd) {
	GEOL_Point *beginPoint = new GEOL_Point(theXStart, theYStart);
	if (!beginPoint)
		return NULL;
	GEOL_Point *endPoint = new GEOL_Point(theXEnd, theYEnd);
	if (!endPoint) {
		delete beginPoint;
		return NULL;
	}
	
	if ((*beginPoint) == (*endPoint)) {
		delete beginPoint;
		delete endPoint;
		return NULL;
	}

	GEOL_Segment *newSegment = new GEOL_Segment(beginPoint, endPoint);
	if (!newSegment) {
		delete beginPoint;
		delete endPoint;
		return NULL;
	}
	addObject((GEOL_Object*)beginPoint);
	addObject((GEOL_Object*)endPoint);
	addObject((GEOL_Object*)newSegment);
	return newSegment;
}


GEOL_Arc* GEOL_Context::createArc() {
	GEOL_Arc *newArc = new GEOL_Arc();
	if (newArc)
		addObject((GEOL_Object*)newArc);
	return newArc;
}


GEOL_Arc* GEOL_Context::createArc(const GEOL_Point& theBeginPoint, const GEOL_Point& theEndPoint, double theRadius, GEOL_ArcVersus theVersus) {
	double halfPointDist = theBeginPoint.pointDistance(theEndPoint) / 2.0;
	if (theBeginPoint == theEndPoint || (theRadius < halfPointDist && fabs(theRadius - halfPointDist) > GEOL_EQUAL_DIST)) {
		return NULL;
	}

	GEOL_Point *newBeginPoint = new GEOL_Point(theBeginPoint);
	if (!newBeginPoint)
		return NULL;
	GEOL_Point *newEndPoint = new GEOL_Point(theEndPoint);
	if (!newEndPoint) {
		delete newBeginPoint;
		return NULL;
	}
	GEOL_Arc *newArc = new GEOL_Arc(newBeginPoint, newEndPoint, theRadius, theVersus);
	if (!newArc) {
		delete newBeginPoint;
		delete newEndPoint;
		return NULL;
	}
	addObject((GEOL_Object*)newBeginPoint);
	addObject((GEOL_Object*)newEndPoint);
	addObject((GEOL_Object*)newArc);
	return newArc;
}


GEOL_Arc* GEOL_Context::createArc(const GEOL_Arc& theArc) {
	GEOL_Point *newBeginPoint = new GEOL_Point(*(theArc.begin()));
	if (!newBeginPoint)
		return NULL;
	GEOL_Point *newEndPoint = new GEOL_Point(*(theArc.end()));
	if (!newEndPoint) {
		delete newBeginPoint;
		return NULL;
	}
	GEOL_Arc *newArc = new GEOL_Arc(newBeginPoint, newEndPoint, theArc.radius(), theArc.versus());
	if (!newArc) {
		delete newBeginPoint;
		delete newEndPoint;
		return NULL;
	}
	addObject((GEOL_Object*)newBeginPoint);
	addObject((GEOL_Object*)newEndPoint);
	addObject((GEOL_Object*)newArc);
	return newArc;
}


GEOL_Arc* GEOL_Context::createArc(double theXStart, double theYStart, double theXEnd, double theYEnd, double theRadius, GEOL_ArcVersus theVersus) {
	GEOL_Point *beginPoint = new GEOL_Point(theXStart, theYStart);
	if (!beginPoint)
		return NULL;
	GEOL_Point *endPoint = new GEOL_Point(theXEnd, theYEnd);
	if (!endPoint) {
		delete beginPoint;
		return NULL;
	}

	double halfPointDist = beginPoint -> pointDistance(*endPoint) / 2.0;
	if ((*beginPoint) == (*endPoint) || (theRadius < halfPointDist && fabs(theRadius - halfPointDist) > GEOL_EQUAL_DIST)) {
		delete beginPoint;
		delete endPoint;
		return NULL;
	}

	GEOL_Arc *newArc = new GEOL_Arc(beginPoint, endPoint, theRadius, theVersus);
	if (!newArc) {
		delete beginPoint;
		delete endPoint;
		return NULL;
	}
	addObject((GEOL_Object*)beginPoint);
	addObject((GEOL_Object*)endPoint);
	addObject((GEOL_Object*)newArc);
	return newArc;
}



GEOL_Profile* GEOL_Context::createProfile() {
	GEOL_Profile *newProfile = new GEOL_Profile();
	if (newProfile)
		addObject((GEOL_Object*)newProfile);
	return newProfile;
}

	
GEOL_PoliProfile* GEOL_Context::createPoliProfile() {
	GEOL_PoliProfile *newPoliProfile = new GEOL_PoliProfile();
	if (newPoliProfile)
		addObject((GEOL_Object*)newPoliProfile);
	return newPoliProfile;
}


/*!
Add a new object to the context at the end of the objects list

\param theNewObject
The object to add

\return
- true if the new object has been added correctly
- false otherwise
*/
bool GEOL_Context::addObject(GEOL_Object *theNewObject) {
	if (!theNewObject)
		return false;
	
	pObjectList.push_back(theNewObject);
	
	theNewObject -> setContext(this);
	
	return true;
}


/*!
Remove an object from the context, the object IS DESTROYED objects lives only within a context

\param theObject
The object to remove

\return
- true if the object has been removed correctly
- false otherwise
*/
bool GEOL_Context::removeObject(GEOL_Object *theObject) {
	if (!theObject)
		return false;

	if (*objectIt != theObject) {
		for (objectIt = pObjectList.begin() ; objectIt != pObjectList.end() && *objectIt != theObject ; objectIt++) {}
	}

	if (objectIt == pObjectList.end()) {
		return false;
	}
	else {
		GEOL_Object *toDel = *objectIt;
		
		if (dynamic_cast<GEOL_Point*>(toDel)) {
			deletePoint((GEOL_Point*)toDel);
		}
		else if (dynamic_cast<GEOL_Segment*>(toDel)) {
			deleteSegment((GEOL_Segment*)toDel);
		}
		else if (dynamic_cast<GEOL_Arc*>(toDel)) {
			deleteArc((GEOL_Arc*)toDel);
		}
		else if (dynamic_cast<GEOL_Profile*>(toDel)) {
			deleteProfile((GEOL_Profile*)toDel);
		}
		else if (dynamic_cast<GEOL_PoliProfile*>(toDel)) {
			deletePoliProfile((GEOL_PoliProfile*)toDel);
		}		
	}
	
	return true;
}


/*!
Remove and DESTROY all the objects of the context
*/
void GEOL_Context::removeAllObjects() {
	for (objectIt = pObjectList.begin() ; objectIt != pObjectList.end() ; ) {
		GEOL_Object *toDel = *objectIt;
		objectIt++;
		
		bool deleteFlag = false;
		if (dynamic_cast<GEOL_Segment*>(toDel)) {
			deleteSegment((GEOL_Segment*)toDel);
			deleteFlag = true;
		}
		else if (dynamic_cast<GEOL_Arc*>(toDel)) {
			deleteArc((GEOL_Arc*)toDel);
			deleteFlag = true;
		}
		else if (dynamic_cast<GEOL_Profile*>(toDel)) {
			deleteProfile((GEOL_Profile*)toDel);
		}
		else if (dynamic_cast<GEOL_PoliProfile*>(toDel)) {
			deletePoliProfile((GEOL_PoliProfile*)toDel);
		}		

		if (deleteFlag) {
			objectIt = pObjectList.begin();
		}
	}

	for (objectIt = pObjectList.begin() ; objectIt != pObjectList.end() ; ) {
		GEOL_Object *toDel = *objectIt;
		objectIt++;

		if (dynamic_cast<GEOL_Point*>(toDel)) {
			deletePoint((GEOL_Point*)toDel);
		}
	}
}


/*!
Get the next of the specified object in the objects list

\theObject
Object from wich get the next

\return
The next of theObject within the objects list, or NULL if theObject dosent belong to the list or is the last element
*/
GEOL_Object* GEOL_Context::getNextObject(const GEOL_Object *theObject) {
	if (*objectIt != theObject) {
		for (objectIt = pObjectList.begin() ; objectIt != pObjectList.end() && *objectIt != theObject ; objectIt++) {}
	}

	GEOL_Object *ret = NULL;

	if (objectIt != pObjectList.end()) {
		objectIt++;
		if (objectIt != pObjectList.end()) {
			ret = *objectIt;
		}
	}

	return ret;
}


/*!
Get the previous of the specified object in the objects list

\theObject
Object from wich get the previous

\return
The previous of theObject within the objects list, or NULL if theObject dosent belong to the list or is the first element

*/
GEOL_Object* GEOL_Context::getPrevObject(const GEOL_Object *theObject) {
	if (*objectIt != theObject) {
		for (objectIt = pObjectList.begin() ; objectIt != pObjectList.end() && *objectIt != theObject ; objectIt++) {}
	}

	GEOL_Object *ret = NULL;

	if (objectIt != pObjectList.end()) {
		if (objectIt != pObjectList.begin()) {
			objectIt--;
			ret = *objectIt;
		}		
	}

	return ret;
}


/*!
Notify to all the objects within the context the imminent destruction of an object

\param theObject
The object that will be destroyed

\return
- true if the notification is correctly carried out
- false otherwise
*/
bool GEOL_Context::notifyDestruction(GEOL_Object *theObject) {
	if (!theObject)
		return false;
	
	bool ret = true;
	list<GEOL_Object*>::iterator it;
	for (it = pObjectList.begin() ; ret && it != pObjectList.end() ; it++) {
		GEOL_Object *obj = *it;
		
		if (obj != theObject) {
			ret = obj -> notifyDestruction(theObject);
		}
	}

	return ret;
}


/*!
Delete an object from the context, the object will be deallocated if its reference counter becomes negative

\param theObject
Object to delete
\param theNotifyFlag
Notify flag, if true the object destruction will be notified to other objects

\return
- true if the object has been deleted succesfully
- false otherwise
*/
bool GEOL_Context::deleteObject(GEOL_Object *theObject, bool theNotifyFlag) {
	if (!theObject)
		return false;
		
	bool ret = false;
	
	if (ret) {
		if (dynamic_cast<GEOL_Point*>(theObject)) {
			ret = deletePoint((GEOL_Point*)theObject, theNotifyFlag);
		}
		else if (dynamic_cast<GEOL_Segment*>(theObject)) {
			ret = deleteSegment((GEOL_Segment*)theObject, theNotifyFlag);
		}
		else if (dynamic_cast<GEOL_Arc*>(theObject)) {
			ret = deleteArc((GEOL_Arc*)theObject, theNotifyFlag);
		}
		else if (dynamic_cast<GEOL_Profile*>(theObject)) {
			ret = deleteProfile((GEOL_Profile*)theObject, theNotifyFlag);
		}
		else if (dynamic_cast<GEOL_PoliProfile*>(theObject)) {
			ret = deletePoliProfile((GEOL_PoliProfile*)theObject, theNotifyFlag);
		}
		else {
			ret = false;
		}
	}
	
	return ret;
}



/*!
Decrement the reference counter of a point, if the refernce counter is 0 remove the point from memory

\param thePoint
Point to delete
\param theNotifyFlag
Notify flag, if true the point destruction will be notified to other objects

\return
- true if the point is removed from memory (reference counter 0)
- false otherwise
*/
bool GEOL_Context::deletePoint(GEOL_Point *thePoint, bool theNotifyFlag) {
	bool ret = false;
	
	if (!thePoint -> decRefCount()) {
		bool okNotify = true;
		if (theNotifyFlag) {
			okNotify = notifyDestruction(thePoint);
		}
		
		if (okNotify) {
			pObjectList.remove(thePoint);
			delete thePoint;
			ret = true;
		}
		else {
			ret = false;
		}
	}
	
	return ret;
}


/*!
Decrement the reference counter of a segment, if the refernce counter is 0 remove the segment from memory

\param theSegment
Segment to destroy
\param theNotifyFlag
Notify flag, if true the segment destruction will be notified to other objects

\return
- true if the segment is removed from memory (reference counter 0)
- false otherwise
*/
bool GEOL_Context::deleteSegment(GEOL_Segment *theSegment, bool theNotifyFlag) {
	bool ret = false;

	if (!theSegment -> decRefCount()) {
		bool okNotify = true;
		if (theNotifyFlag) {
			okNotify = notifyDestruction(theSegment);
		}
		
		if (okNotify) {
			pObjectList.remove(theSegment);
			delete theSegment;
			ret = true;
		}
		else {
			ret = false;
		}
	}
	
	return ret;
}


/*!
Decrement the reference counter of an arc, if the refernce counter is 0 remove the arc from memory

\param theArc
Arc to delete
\param theNotifyFlag
Notify flag, if true the arc destruction will be notified to other objects

\return
- true if the arc is removed from memory (reference counter 0)
- false otherwise
*/
bool GEOL_Context::deleteArc(GEOL_Arc *theArc, bool theNotifyFlag) {
	bool ret = false;

	if (!theArc -> decRefCount()) {
		bool okNotify = true;
		if (theNotifyFlag) {
			okNotify = notifyDestruction(theArc);
		}
		
		if (okNotify) {
			pObjectList.remove(theArc);
			delete theArc;
			ret = true;
		}
		else {
			ret = false;
		}
	}
	
	return ret;
}


/*!
Decrement the reference counter of a profile, if the refernce counter is 0 remove the profile from memory

\param theProfile
Profile to delete
\param theNotifyFlag
Notify flag, if true the profile destruction will be notified to other objects

\return
- true if the profile is removed from memory (reference counter 0)
- false otherwise
*/
bool GEOL_Context::deleteProfile(GEOL_Profile *theProfile, bool theNotifyFlag) {
	bool ret = false;

	if (!theProfile -> decRefCount()) {
		bool okNotify = true;
		if (theNotifyFlag) {
			okNotify = notifyDestruction(theProfile);
		}

		if (okNotify) {
			pObjectList.remove(theProfile);
			delete theProfile;
			ret = true;
		}
		else {
			ret = false;
		}
	}
	
	return ret;
}


/*!
Decrement the reference counter of a poliprofile, if the refernce counter is 0 remove the poliprofile from memory

\param thePoliProfile
Poliprofile to delete
\param theNotifyFlag
Notify flag, if true the poliprofile destruction will be notified to other objects

\return
- true if the poliprofile is removed from memory (reference counter 0)
- false otherwise
*/
bool GEOL_Context::deletePoliProfile(GEOL_PoliProfile *thePoliProfile, bool theNotifyFlag) {
	bool ret = false;

	if (!thePoliProfile -> decRefCount()) {
		bool okNotify = true;
		if (theNotifyFlag) {
			okNotify = notifyDestruction(thePoliProfile);
		}
		
		if (okNotify) {
			pObjectList.remove(thePoliProfile);
			delete thePoliProfile;
			ret = true;
		}
		else {
			ret = false;
		}
	}
	
	return ret;
}


bool GEOL_Context::saveContext(std::ofstream *theStream) {
	if (!theStream)
		return false;
	
	bool ret = true;
	GEOL_Object *obj = getFirstObject();
	for ( ; ret && obj ; obj = getNextObject(obj)) {
		if (dynamic_cast<GEOL_PoliProfile*>(obj)) {
			ret = ((GEOL_PoliProfile*)obj) -> SaveBinary(theStream);
		}
	}
	
	obj = getFirstObject();
	for ( ; ret && obj ; obj = getNextObject(obj)) {
		if (dynamic_cast<GEOL_Profile*>(obj)) {
			GEOL_Attribute *attr = obj -> getAttributeFromID("saved");
			if (!attr) {
				ret = ((GEOL_Profile*)obj) -> SaveBinary(theStream);
			}
		}
	}	

	obj = getFirstObject();
	for ( ; ret && obj ; obj = getNextObject(obj)) {
		if (dynamic_cast<GEOL_Segment*>(obj)) {
			GEOL_Attribute *attr = obj -> getAttributeFromID("saved");
			if (!attr) {
				ret = ((GEOL_Segment*)obj) -> SaveBinary(theStream);
			}
		}
		if (dynamic_cast<GEOL_Arc*>(obj)) {
			GEOL_Attribute *attr = obj -> getAttributeFromID("saved");
			if (!attr) {
				ret = ((GEOL_Arc*)obj) -> SaveBinary(theStream);
			}
		}
	}	

	obj = getFirstObject();
	for ( ; ret && obj ; obj = getNextObject(obj)) {
		GEOL_Attribute *attr = obj -> getAttributeFromID("saved");
		if (!attr) {
			ret = obj -> SaveBinary(theStream);
		}
	}	

	for (obj = getFirstObject() ; obj ; obj = getNextObject(obj)) {
		obj -> removeAttribute("saved");
	}
	
	return ret;
}


bool GEOL_Context::loadContext(std::ifstream *theStream) {
	if (!theStream)
		return false;
	
	bool ret = !theStream -> bad();
	if (ret) {
		while (ret && !theStream -> eof()) {
			GEOL_ObjectType objType = geol_Point;
			ret = loadBinaryObjectType(theStream, objType);
			if (ret && !theStream -> eof()) {
				switch (objType) {
					case geol_Point:
						{
							GEOL_Point *newPoint = createPoint(0.0, 0.0);
							if (newPoint) {
								ret = newPoint -> LoadBinary(theStream);
							}
							else {
								ret = false;
							}
						}
						break;
					case geol_Segment:
						{
							GEOL_Segment *newSegment = createSegment();
							if (newSegment) {
								ret = newSegment -> LoadBinary(theStream);
							}
							else {
								ret = false;
							}
						}
						break;
					case geol_Arc:
						{
							GEOL_Arc *newArc = createArc();
							if (newArc) {
								ret = newArc -> LoadBinary(theStream);
							}
							else {
								ret = false;
							}
						}
						break;
					case geol_Profile:
						{
							GEOL_Profile *newProfile = createProfile();
							if (newProfile) {
								ret = newProfile -> LoadBinary(theStream);
							}
							else {
								ret = false;
							}
						}
						break;
					case geol_PoliProfile:
						{
							GEOL_PoliProfile *newPoliProfile = createPoliProfile();
							if (newPoliProfile) {
								ret = newPoliProfile -> LoadBinary(theStream);
							}
							else {
								ret = false;
							}
						}
						break;
					default:
						ret = false;
				}
			}
		}
	}
	
	return ret;
}


bool GEOL_Context::loadBinaryObjectType(std::ifstream *theStream, GEOL_ObjectType& theObjectType) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		int objType = 0;
		theStream -> read((char*)(&objType), sizeof(int));
		theObjectType = (GEOL_ObjectType)objType;
	
		ret = !theStream -> bad();
	}
	
	return ret;
}

