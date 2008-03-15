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

#include "GEOL_Point.h"
#include "GEOL_Segment.h"
#include "GEOL_Arc.h"
#include "GEOL_Profile.h"
#include "GEOL_PoliProfile.h"

#include "GEOL_Context.h"


GEOL_Context::GEOL_Context() {
	pObjectList.clear();
	pDeletionObjList.clear();
	
	/*
	int objectSize = sizeof(GEOL_Object);
	int pointSize = sizeof(GEOL_Point);
	int segmentSize = sizeof(GEOL_Segment);
	int arcSize = sizeof(GEOL_Arc);
	int profileSize = sizeof(GEOL_Profile);
	int itSize = sizeof(list<GEOL_Attribute*>::iterator);
	int listSize = sizeof(list<GEOL_Attribute*>);
	*/
}


/*!
The destructor remove and destroy all the objects of the context
*/
GEOL_Context::~GEOL_Context() {
	removeAllObjects();
}


/*!
\return
Number of objects in the context
*/
int GEOL_Context::getSize() {
	return pObjectList.size();
}


/*!
Create a point with null coordinates

\return
The pointer to the new point
*/
GEOL_Point* GEOL_Context::createPoint() {
	GEOL_Point *newPoint = new GEOL_Point();
	addObject((GEOL_Object*)newPoint);
	
	return newPoint;
}


/*!
Create a new point with the provided coordinates

\param theXCoord
Horizontal coordinate of the new point
\param theYCoord
Vertical coordinate of the new point

\return
The pointer to the new point
*/
GEOL_Point* GEOL_Context::createPoint(double theXCoord, double theYCoord) {
	GEOL_Point *newPoint = new GEOL_Point(theXCoord, theYCoord);
	addObject((GEOL_Object*)newPoint);
	
	return newPoint;
}


/*!
Create a copy of a point

\param thePoint
Point to copy

\return
The pointer of the new point
*/
GEOL_Point* GEOL_Context::createPoint(const GEOL_Point& thePoint) {
	GEOL_Point *newPoint = new GEOL_Point(thePoint);
	addObject((GEOL_Object*)newPoint);
	
	return newPoint;
}


/*!
Create a new segment with null points

\return
The pointer to the new segment
*/
GEOL_Segment* GEOL_Context::createSegment() {
	GEOL_Segment *newSegment = new GEOL_Segment();
	addObject((GEOL_Object*)newSegment);
	
	return newSegment;
}


/*!
Create a new segment copying the extreme points provided, equal points not allowed

\param theBeginPoint
Begin point of the segment
\param theEndPoint
End point of the segment

\return
The pointer to the new segment, or NULL if begin and end points are equal
*/
GEOL_Segment* GEOL_Context::createSegment(const GEOL_Point& theBeginPoint, const GEOL_Point& theEndPoint) {
	if (theBeginPoint == theEndPoint) {
		return NULL;
	}
	
	GEOL_Point *newBeginPoint = new GEOL_Point(theBeginPoint);
	GEOL_Point *newEndPoint = new GEOL_Point(theEndPoint);
	GEOL_Segment *newSegment = new GEOL_Segment(newBeginPoint, newEndPoint);
	
	addObject((GEOL_Object*)newBeginPoint);
	addObject((GEOL_Object*)newEndPoint);
	addObject((GEOL_Object*)newSegment);
	
	return newSegment;
}


/*!
Create a new segment with the extreme points provided

\param theBeginPoint
Begin point of the segment
\param theEndPoint
End point of the segment

\return
The pointer to the new segment, or NULL if begin and end points are equal or points hat too high reference count
*/
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
	addObject((GEOL_Object*)newSegment);

	return newSegment;
}


/*!
Create a copy of the segment provided

\param theSegment
Segment to copy

\return
The pointer to the new segment
*/
GEOL_Segment* GEOL_Context::createSegment(const GEOL_Segment& theSegment) {
	GEOL_Point *newBeginPoint = new GEOL_Point(*(theSegment.begin()));
	GEOL_Point *newEndPoint = new GEOL_Point(*(theSegment.end()));
	GEOL_Segment *newSegment = new GEOL_Segment(newBeginPoint, newEndPoint);

	addObject((GEOL_Object*)newSegment);

	return newSegment;
}


/*!
Create a new segment with the coordinates provided

\param theXStart
Horizontal coordinate of the segment start point
\param theYStart
Vertical coordinate of the segment start point
\param theXEnd
Horizontal coordinate of the segment end point
\param theYEnd
Vertical coordinate of the segment end point

\return
The pointer to the new segment, or NULL if begin and end points are equal
*/
GEOL_Segment* GEOL_Context::createSegment(double theXStart, double theYStart, double theXEnd, double theYEnd) {
	GEOL_Point *beginPoint = new GEOL_Point(theXStart, theYStart);
	GEOL_Point *endPoint = new GEOL_Point(theXEnd, theYEnd);
	
	if ((*beginPoint) == (*endPoint)) {
		delete beginPoint;
		delete endPoint;
		return NULL;
	}

	GEOL_Segment *newSegment = new GEOL_Segment(beginPoint, endPoint);

	addObject((GEOL_Object*)beginPoint);
	addObject((GEOL_Object*)endPoint);
	addObject((GEOL_Object*)newSegment);

	return newSegment;
}


/*!
Create a new arc with null points and radius

\return
The pointer to the new arc
*/
GEOL_Arc* GEOL_Context::createArc() {
	GEOL_Arc *newArc = new GEOL_Arc();
	addObject((GEOL_Object*)newArc);
	
	return newArc;
}


/*!
Create a new arc with the specified extreme points, radius and versus

\param theBeginPoint
Begin point of the arc
\param theEndPoint
End point of the arc
\param theRadius
Radius of the arc
\param theVersus
Versus of the new arc (colckwise or counterclockwise)

\return
The pointer to the new arc, or NULL if begin and end points are equal or rafis is too short
*/
GEOL_Arc* GEOL_Context::createArc(const GEOL_Point& theBeginPoint, const GEOL_Point& theEndPoint, double theRadius, GEOL_Arc::GEOL_ArcVersus theVersus) {
	double halfPointDist = theBeginPoint.pointDistance(theEndPoint) / 2.0;
	if (theBeginPoint == theEndPoint || (theRadius < halfPointDist && fabs(theRadius - halfPointDist) > GEOL_EQUAL_DIST)) {
		return NULL;
	}

	GEOL_Point *newBeginPoint = new GEOL_Point(theBeginPoint);
	GEOL_Point *newEndPoint = new GEOL_Point(theEndPoint);
	GEOL_Arc *newArc = new GEOL_Arc(newBeginPoint, newEndPoint, theRadius, theVersus);

	addObject((GEOL_Object*)newBeginPoint);
	addObject((GEOL_Object*)newEndPoint);
	addObject((GEOL_Object*)newArc);

	return newArc;
}


/*!
Create a copy of the arc provided

\param theArc
Arc to copy

\return
The pointer to the new arc
*/
GEOL_Arc* GEOL_Context::createArc(const GEOL_Arc& theArc) {
	GEOL_Point *newBeginPoint = new GEOL_Point(*(theArc.begin()));
	GEOL_Point *newEndPoint = new GEOL_Point(*(theArc.end()));
	GEOL_Arc *newArc = new GEOL_Arc(newBeginPoint, newEndPoint, theArc.radius(), theArc.versus());

	addObject((GEOL_Object*)newBeginPoint);
	addObject((GEOL_Object*)newEndPoint);
	addObject((GEOL_Object*)newArc);

	return newArc;
}


/*!
Create a new arc with the specified coordinates, radius and versus

\param theXStart
Horizontal coordinate of the arc start point
\param theYStart
Vertical coordinate of the arc start point
\param theXEnd
Horizontal coordinate of the arc end point
\param theYEnd
Vertical coordinate of the arc end point
\param theRadius
Radius of the arc
\param theVersus
Versus of the new arc (colckwise or counterclockwise)

\return
The pointer to the new arc, or NULL if begin and end points are equal or rafis is too short
*/
GEOL_Arc* GEOL_Context::createArc(double theXStart, double theYStart, double theXEnd, double theYEnd, double theRadius, GEOL_Arc::GEOL_ArcVersus theVersus) {
	GEOL_Point *beginPoint = new GEOL_Point(theXStart, theYStart);
	GEOL_Point *endPoint = new GEOL_Point(theXEnd, theYEnd);

	double halfPointDist = beginPoint -> pointDistance(*endPoint) / 2.0;
	if ((*beginPoint) == (*endPoint) || (theRadius < halfPointDist && fabs(theRadius - halfPointDist) > GEOL_EQUAL_DIST)) {
		delete beginPoint;
		delete endPoint;
		return NULL;
	}

	GEOL_Arc *newArc = new GEOL_Arc(beginPoint, endPoint, theRadius, theVersus);

	addObject((GEOL_Object*)beginPoint);
	addObject((GEOL_Object*)endPoint);
	addObject((GEOL_Object*)newArc);

	return newArc;
}


/*!
Create a new empty profile

\return
The pointer to the new profile
*/
GEOL_Profile* GEOL_Context::createProfile() {
	GEOL_Profile *newProfile = new GEOL_Profile();
	addObject((GEOL_Object*)newProfile);
	
	return newProfile;
}


/*!
Create a copy of the profile provided

\param theProfile
Profile to copy

\return
The pointer to the new profile if the copy succeed, NULL otherwise
*/	
GEOL_Profile* GEOL_Context::createProfile(const GEOL_Profile& theProfile) {
	GEOL_Profile *newProfile = createProfile();
	
	bool addEntityFlag = true;
	list<GEOL_Entity*>::const_iterator entityIt;
	for (entityIt = (theProfile.pEntityList).begin() ; addEntityFlag && entityIt != theProfile.pEntityList.end() ; entityIt++) {
		GEOL_Entity *entity = *entityIt;
		if (entity -> isSegment()) {
			GEOL_Segment *newSegment = createSegment(*((GEOL_Segment*)entity));
			addEntityFlag = newProfile -> addEdgeTail(newSegment);
			if (!addEntityFlag) {
				deleteSegment(newSegment);
			}
		}
		else if (entity -> isArc()) {
			GEOL_Arc* newArc = createArc(*((GEOL_Arc*)entity));
			addEntityFlag = newProfile -> addEdgeTail(newArc);
			if (!addEntityFlag) {
				deleteArc(newArc);
			}
		}
	}
	
	if (!addEntityFlag) {
		deleteProfile(newProfile);
		newProfile = NULL;
	}

	return newProfile;
}


/*!
Create a new empty poliprofile

\return
The pointer to the new poliprofile
*/
GEOL_PoliProfile* GEOL_Context::createPoliProfile() {
	GEOL_PoliProfile *newPoliProfile = new GEOL_PoliProfile();
	addObject((GEOL_Object*)newPoliProfile);
	
	return newPoliProfile;
}


/*!
Create a copy of the poliprofile provided

\param thePoliProfile
Poliprofile to copy

\return
The pointer to the new poliprofile if the copy succeed, NULL otherwise
*/
GEOL_PoliProfile* GEOL_Context::createPoliProfile(const GEOL_PoliProfile& thePoliProfile) {
	GEOL_PoliProfile *newPoliProfile = createPoliProfile();
	
	bool addContainerFlag = true;
	list<GEOL_Container*>::const_iterator containerIt;
	for (containerIt = (thePoliProfile.pContainerList).begin() ; addContainerFlag && containerIt != thePoliProfile.pContainerList.end() ; containerIt++) {
		GEOL_Container *container = *containerIt;
		if (container -> isProfile()) {
			GEOL_Profile *newProfile = createProfile(*((GEOL_Profile*)container));
			if (newProfile) {
				addContainerFlag = newPoliProfile -> addProfile(newProfile);
				if (!addContainerFlag) {
					deleteProfile(newProfile);
				}
			}
			else {
				addContainerFlag = false;
			}
		}
		else if (container -> isPoliProfile()) {
			GEOL_PoliProfile *newPoli = createPoliProfile(*((GEOL_PoliProfile*)container)); // WARNING recursive call
			if (newPoli) {
				addContainerFlag = newPoliProfile -> addContainer(newPoli);
				if (!addContainerFlag) {
					deletePoliProfile(newPoli);
				}
			}
			else {
				addContainerFlag = false;
			}
		}
	}
	
	if (addContainerFlag) {
		bool addEntityFlag = true;
		list<GEOL_Entity*>::const_iterator entityIt;
		for (entityIt = (thePoliProfile.pEntityList).begin() ; addEntityFlag && entityIt != thePoliProfile.pEntityList.end() ; entityIt++) {
			GEOL_Entity *entity = *entityIt;
			if (entity -> isSegment()) {
				GEOL_Segment *newSegment = createSegment(*((GEOL_Segment*)entity));
				addEntityFlag = newPoliProfile -> addEntity((GEOL_Entity*)newSegment);
				if (!addEntityFlag) {
					deleteSegment(newSegment);
				}
			}
			else if (entity -> isArc()) {
				GEOL_Arc* newArc = createArc(*((GEOL_Arc*)entity));
				addEntityFlag = newPoliProfile -> addEntity((GEOL_Entity*)newArc);
				if (!addEntityFlag) {
					deleteArc(newArc);
				}
			}
		}
		
		if (!addEntityFlag) {
			deletePoliProfile(newPoliProfile);
			newPoliProfile = NULL;
		}
	}
	else {
		deletePoliProfile(newPoliProfile);
		newPoliProfile = NULL;
	}
	
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
Remove and DESTROY all the objects of the context
*/
void GEOL_Context::removeAllObjects() {
	for ( ; !pObjectList.empty() ; ) {
		GEOL_Object *toDel = pObjectList.front();
		
		// Delete object attributes
		toDel -> removeAllAttributes();
		
		if (toDel -> isPoint()) {
			deletePoint((GEOL_Point*)toDel, true);
		}
		else if (toDel -> isSegment()) {
			((GEOL_Entity*)toDel) -> setBeginEntity(NULL);
			((GEOL_Entity*)toDel) -> setEndEntity(NULL);
			deleteSegment((GEOL_Segment*)toDel, true);
		}
		else if (toDel -> isArc()) {
			((GEOL_Entity*)toDel) -> setBeginEntity(NULL);
			((GEOL_Entity*)toDel) -> setEndEntity(NULL);
			deleteArc((GEOL_Arc*)toDel, true);
		}
		else if (toDel -> isProfile()) {
			deleteProfile((GEOL_Profile*)toDel, true);
		}
		else if (toDel -> isPoliProfile()) {
			deletePoliProfile((GEOL_PoliProfile*)toDel, true);
		}		
	}
}


/*!
\return
The first object of the context
*/
GEOL_Object* GEOL_Context::getFirstObject() {
	if (pObjectList.size() == 0) {
		return NULL;
	}
	else {
		return pObjectList.front();
	}
}


/*!
\return
The last object of the context
*/
GEOL_Object* GEOL_Context::getLastObject() {
	if (pObjectList.size() == 0) {
		return NULL;
	}
	else {
		return pObjectList.back();
	}
}


/*!
Get the next of the specified object in the objects list

\theObject
Object from which get the next

\return
The next of theObject within the objects list, or NULL if theObject doesn't belong to the list or is the last element
*/
GEOL_Object* GEOL_Context::getNextObject(const GEOL_Object *theObject) {
	GEOL_Object *ret = NULL;

	if (theObject) {
		list<GEOL_Object*>::iterator objectIt;
		for (objectIt = pObjectList.begin() ; objectIt != pObjectList.end() && *objectIt != theObject ; objectIt++) {}
		if (objectIt != pObjectList.end()) {
			objectIt++;
			if (objectIt != pObjectList.end()) {
				ret = *objectIt;
			}
		}
	}
	
	return ret;
}


/*!
Get the previous of the specified object in the objects list

\theObject
Object from which get the previous

\return
The previous of theObject within the objects list, or NULL if theObject dosent belong to the list or is the first element
*/
GEOL_Object* GEOL_Context::getPrevObject(const GEOL_Object *theObject) {
	GEOL_Object *ret = NULL;

	if (theObject) {
		list<GEOL_Object*>::iterator objectIt;
		for (objectIt = pObjectList.begin() ; objectIt != pObjectList.end() && *objectIt != theObject ; objectIt++) {}
		if (objectIt != pObjectList.end()) {
			if (objectIt != pObjectList.begin()) {
				objectIt--;
				ret = *objectIt;
			}		
		}
	}
	
	return ret;	
}


/*!
Set up a objects iterator

\param theIterator
On output contains the iterator at the beginning of objects list
*/
void GEOL_Context::setObjectIterator(std::list<GEOL_Object*>::const_iterator &theIterator) const {
	theIterator = pObjectList.begin();
}


/*!
Check if the iterator has reached the end of objects list

\return
- true if theIterator has reached the end of objects list
- false otherwise
*/
bool GEOL_Context::isEndOfObjects(std::list<GEOL_Object*>::const_iterator &theIterator) const {
	if (theIterator == pObjectList.end())
		return true;
	else
		return false;
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
			bool destroyFlag = false;
			ret = obj -> notifyDestruction(theObject, destroyFlag);
			if (destroyFlag) {
				pDeletionObjList.push_back(obj);
			}
		}
	}

	return ret;
}


/*!
Delete an object from the context, the object will be deallocated if its reference counter becomes negative
This is the only allowed way to destroy an object, the object is deleted, this deletion is notified, the
notification procedure eventually adds other objects to delete in the pDeletionObjList list, the method ends
when the list of objects to delete is empty

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
		
	bool ret = true;
	
	pDeletionObjList.clear();
	pDeletionObjList.push_back(theObject);
	
	for ( ; ret && !pDeletionObjList.empty() ; ) {
		GEOL_Object *toDel = pDeletionObjList.front();

		if (toDel -> isPoint()) {
			ret = deletePoint((GEOL_Point*)toDel, theNotifyFlag);
		}
		else if (toDel -> isSegment()) {
			ret = deleteSegment((GEOL_Segment*)toDel, theNotifyFlag);
		}
		else if (toDel -> isArc()) {
			ret = deleteArc((GEOL_Arc*)toDel, theNotifyFlag);
		}
		else if (toDel -> isProfile()) {
			ret = deleteProfile((GEOL_Profile*)toDel, theNotifyFlag);
		}
		else if (toDel -> isPoliProfile()) {
			ret = deletePoliProfile((GEOL_PoliProfile*)toDel, theNotifyFlag);
		}
		else {
			ret = false;
		}
		
		if (!pDeletionObjList.empty()) {
			pDeletionObjList.pop_front();
		}
	}

	pDeletionObjList.clear();

	return ret;
}


/*!
Delete a point, the refernce counter is resetted and a destroy notification message is sended

\param thePoint
Point to delete
\param theNotifyFlag
Notify flag, if true the point destruction will be notified to other objects

\return
- true if the point is correctly removed from memory
- false otherwise
*/
bool GEOL_Context::deletePoint(GEOL_Point *thePoint, bool theNotifyFlag) {
	if (!thePoint)
		return false;
		
	bool ret = false;

	thePoint -> resetRefCount();
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
	
	return ret;
}


/*!
Delete a segment, the refernce counter is resetted and a destroy notification message is sended

\param theSegment
Segment to destroy
\param theNotifyFlag
Notify flag, if true the segment destruction will be notified to other objects

\return
- true if the segment is correctly removed from memory
- false otherwise
*/
bool GEOL_Context::deleteSegment(GEOL_Segment *theSegment, bool theNotifyFlag) {
	if (!theSegment)
		return false;
		
	bool ret = false;

	theSegment -> resetRefCount();
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
	
	return ret;
}


/*!
Delete an arc, the refernce counter is resetted and a destroy notification message is sended

\param theArc
Arc to delete
\param theNotifyFlag
Notify flag, if true the arc destruction will be notified to other objects

\return
- true if the arc is correctly removed from memory
- false otherwise
*/
bool GEOL_Context::deleteArc(GEOL_Arc *theArc, bool theNotifyFlag) {
	bool ret = false;

	theArc -> resetRefCount();
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
	
	return ret;
}


/*!
Delete a profile, the refernce counter is resetted and a destroy notification message is sended

\param theProfile
Profile to delete
\param theNotifyFlag
Notify flag, if true the profile destruction will be notified to other objects

\return
- true if the profile is correctly removed from memory
- false otherwise
*/
bool GEOL_Context::deleteProfile(GEOL_Profile *theProfile, bool theNotifyFlag) {
	if (!theProfile)
		return false;
		
	bool ret = false;

	theProfile -> resetRefCount();
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
	
	return ret;
}


/*!
Delete a poliprofile, the refernce counter is resetted and a destroy notification message is sended

\param thePoliProfile
Poliprofile to delete
\param theNotifyFlag
Notify flag, if true the poliprofile destruction will be notified to other objects

\return
- true if the poliprofile is correctly removed from memory
- false otherwise
*/
bool GEOL_Context::deletePoliProfile(GEOL_PoliProfile *thePoliProfile, bool theNotifyFlag) {
	if (!thePoliProfile)
		return false;
		
	bool ret = false;

	thePoliProfile -> resetRefCount();
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
	
	return ret;
}


/*!
Find the container of a given entity

\param theEntity
The given entity

\return
The pointer to the container that contains theEntity, or NULL if theEntity is null, or is not an entity or has
no container
*/
GEOL_Object* GEOL_Context::getEntityContainer(const GEOL_Object *theEntity) const {
	if (!theEntity || !theEntity -> isEntity())
		return NULL;
		
	GEOL_Object *ret = NULL;
	list<GEOL_Object*>::const_iterator objIt;
	for (objIt = pObjectList.begin() ; !ret  && objIt != pObjectList.end() ; objIt++) {
		GEOL_Object *obj = *objIt;
		if (obj && obj -> isContainer()) {
			if (((GEOL_Container*)obj) -> isContained((GEOL_Entity*)theEntity)) {
				ret = obj;
			}
		}
	}
	
	return ret;
}


/*!
Find the container of another container

\param theContainer
The given container

\return
The pointer to the container that contains theContainer, or NULL if theContainer is null, or is not a container or has
no parent container
*/
GEOL_Object* GEOL_Context::getParentContainer(const GEOL_Object *theContainer) const {
	if (!theContainer || !theContainer -> isContainer())
		return NULL;
		
	GEOL_Object *ret = NULL;
	list<GEOL_Object*>::const_iterator objIt;
	for (objIt = pObjectList.begin() ; !ret  && objIt != pObjectList.end() ; objIt++) {
		GEOL_Object *obj = *objIt;
		if (obj && obj -> isContainer()) {
			if (((GEOL_Container*)obj) -> isContained((GEOL_Container*)theContainer)) {
				ret = obj;
			}
		}
	}
	
	return ret;
}


/*!
Save the context objects on the stream provided in binary format

\param theStream
Output stream
*/
bool GEOL_Context::saveContext(ofstream *theStream) {
	if (!theStream)
		return false;
	
	bool ret = true;
	if (!pObjectList.empty()) {
		std::list<GEOL_Object*>::iterator objIt;
		GEOL_Object *obj = NULL;
		
		objIt = pObjectList.begin();
		for ( ; ret && objIt != pObjectList.end() ; objIt++) {
			obj = *objIt;
			if (obj -> isPoliProfile()) {
				ret = ((GEOL_PoliProfile*)obj) -> SaveBinary(theStream);
			}
		}
		
		objIt = pObjectList.begin();
		for ( ; ret && objIt != pObjectList.end() ; objIt++) {
			obj = *objIt;
			if (obj -> isProfile()) {
				GEOL_Attribute *attr = obj -> getAttributeFromID(GEOL_ID_SAVED);
				if (!attr) {
					ret = ((GEOL_Profile*)obj) -> SaveBinary(theStream);
				}
			}
		}	

		objIt = pObjectList.begin();
		for ( ; ret && objIt != pObjectList.end() ; objIt++) {
			obj = *objIt;
			if (obj -> isSegment()) {
				GEOL_Attribute *attr = obj -> getAttributeFromID(GEOL_ID_SAVED);
				if (!attr) {
					ret = ((GEOL_Segment*)obj) -> SaveBinary(theStream);
				}
			}
			if (obj -> isArc()) {
				GEOL_Attribute *attr = obj -> getAttributeFromID(GEOL_ID_SAVED);
				if (!attr) {
					ret = ((GEOL_Arc*)obj) -> SaveBinary(theStream);
				}
			}
		}	

		objIt = pObjectList.begin();
		for ( ; ret && objIt != pObjectList.end() ; objIt++) {
			obj = *objIt;
			GEOL_Attribute *attr = obj -> getAttributeFromID(GEOL_ID_SAVED);
			if (!attr) {
				ret = obj -> SaveBinary(theStream);
			}
		}	

		objIt = pObjectList.begin();
		for ( ; ret && objIt != pObjectList.end() ; objIt++) {
			obj = *objIt;
			obj -> removeAttribute(GEOL_ID_SAVED);
		}
	}
	
	return ret;
}


/*!
Laod the context objects from the stream provided

\param theStream
Input stream
*/
bool GEOL_Context::loadContext(ifstream *theStream) {
	if (!theStream)
		return false;
	
	bool ret = !theStream -> bad();
	if (ret) {
		while (ret && !theStream -> eof()) {
			GEOL_Object::GEOL_ObjectType objType = GEOL_Object::geol_Point;
			ret = loadBinaryObjectType(theStream, objType);
			if (ret && !theStream -> eof()) {
				switch (objType) {
					case GEOL_Object::geol_Point:
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
					case GEOL_Object::geol_Segment:
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
					case GEOL_Object::geol_Arc:
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
					case GEOL_Object::geol_Profile:
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
					case GEOL_Object::geol_PoliProfile:
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


/*!
Read the type of the next object to read from the input stream

\param theStream
Input stream
\param theObjectType
On output contains the type of the next object to read

\return
- true if the object type is readed successfully
- false otherwise (streame becomes bad)
*/
bool GEOL_Context::loadBinaryObjectType(ifstream *theStream, GEOL_Object::GEOL_ObjectType& theObjectType) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		int objType = 0;
		theStream -> read((char*)(&objType), sizeof(int));
		theObjectType = (GEOL_Object::GEOL_ObjectType)objType;
	
		ret = !theStream -> bad();
	}
	
	return ret;
}


/*!
Save the context objects on the stream provided in g-code ISO format, only containers (PoliProfile and Profile)
will be saved

\param theStream
Output stream
*/
bool GEOL_Context::saveContextISO(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		std::list<GEOL_Object*>::iterator objIt;
		for (objIt = pObjectList.begin() ; ret && objIt != pObjectList.end() ; objIt++) {
			GEOL_Object *obj = *objIt;		
			if (obj -> isContainer() && !getParentContainer(obj)) {
				ret = obj -> SaveISO(theStream);
			}
		}
	}
	
	return ret;
}


/*!
Load the context objects from the stream provided in g-code ISO format, result will be a set of profiles

\param theStream
Input stream
*/
bool GEOL_Context::loadContextISO(ifstream *theStream) {
	if (!theStream)
		return false;

	bool ret = true;

	removeAllObjects();
	GEOL_PoliProfile *poliProfile = createPoliProfile();
	if (poliProfile) {
		ret = poliProfile -> LoadISO(theStream);
		if (!ret) {
			removeAllObjects();
		}
	}
	
	return ret;
}


