/***************************************************************************
								GEOL_Profile.cpp
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
#include "GEOL_Entity.h"
#include "GEOL_Point.h"
#include "GEOL_Segment.h"
#include "GEOL_Arc.h"
#include "GEOL_Profile.h"



GEOL_Profile::GEOL_Profile() {
}


GEOL_Profile::~GEOL_Profile() {
}


/*!
Check if the profile is closed, a profile is a list of concatenated segments, so to verify its closure is enough to
verify that its extreme points are equal

\return
- true if the profile is closed, i.e. has more than one segment and the extreme points are equal
- false otherwise
*/
bool GEOL_Profile::isClosed() {
	bool ret = false;
	
	if (getNumOfEntities() > 1) {
		GEOL_Entity *lastProfileEntity = getLastEntity();
		GEOL_Entity *firstProfileEntity = getFirstEntity();
		if (lastProfileEntity && firstProfileEntity) {
			GEOL_Point *lastProfilePoint = (GEOL_Point*)(lastProfileEntity -> getEndEntity());
			GEOL_Point *firstProfilePoint = (GEOL_Point*)(firstProfileEntity -> getBeginEntity());
			if (lastProfilePoint && firstProfilePoint && (*lastProfilePoint) == (*firstProfilePoint)) {
				ret = true;
			}
		}
	}
	
	return ret;
}


/*
Add a new segment or arc at the end of the profile, the profile has to be open and the new segment begin point has to
coincide with the profile last point

\param theNewEntity
Segment or arc to add

\return
- true if the new segment or arc is added succesfully
- false otherwise
*/
bool GEOL_Profile::addEntityTail(GEOL_Entity *theNewEntity) {
	if (!theNewEntity)
		return false;
	if (dynamic_cast<GEOL_Point*>(theNewEntity))
		return false;
	if (isClosed())
		return false;
		
	bool ret = false;
		
	if (getNumOfEntities() == 0) {
		pEntityList.push_back(theNewEntity);
		ret = true;
	}
	else {
		GEOL_Entity *lastProfileEntity = getLastEntity();
		if (lastProfileEntity && (*lastProfileEntity) != (*theNewEntity)) {
			GEOL_Point *lastProfilePoint = (GEOL_Point*)(lastProfileEntity -> getEndEntity());
			GEOL_Point *entityBeginPoint = (GEOL_Point*)(theNewEntity -> getBeginEntity());
			if (lastProfilePoint && entityBeginPoint && (*lastProfilePoint) == (*entityBeginPoint)) {
				pEntityList.push_back(theNewEntity);
				ret = true;
			}
		}
	}
	
	return ret;
}


/*
Add a new segment or arc at the beginning of the profile, the profile has to be open and the new segment end point has to
coincide with the profile first point

\param theNewEntity
Segment or arc to add

\return
- true if the new segment or arc is added succesfully
- false otherwise
*/
bool GEOL_Profile::addEntityFront(GEOL_Entity *theNewEntity) {
	if (!theNewEntity)
		return false;
	if (dynamic_cast<GEOL_Point*>(theNewEntity))
		return false;
	if (isClosed())
		return false;
		
	bool ret = false;
		
	if (getNumOfEntities() == 0) {
		pEntityList.push_front(theNewEntity);
		ret = true;
	}
	else {
		GEOL_Entity *firstProfileEntity = getFirstEntity();
		if (firstProfileEntity && (*firstProfileEntity) != (*theNewEntity)) {
			GEOL_Point *firstProfilePoint = (GEOL_Point*)(firstProfileEntity -> getBeginEntity());
			GEOL_Point *entityEndPoint = (GEOL_Point*)(theNewEntity -> getEndEntity());
			if (firstProfilePoint && entityEndPoint && (*firstProfilePoint) == (*entityEndPoint)) {
				pEntityList.push_front(theNewEntity);
				ret = true;
			}
		}
	}
	
	return ret;
}




/*!
Remove the object passed from the list of object contained

\param theObject
Object that will be destroyed
\param theDestroyFlag
On output is true if the object notified has to be destroyed by the context, false otherwise

\return
- true if the notification is correctly carried out
- false otherwise, or it theObject is NULL
*/
bool GEOL_Profile::notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag) {
	theDestroyFlag = false;
	if (!theObject)
		return false;
	
	bool ret = true;
	return ret;
}


bool GEOL_Profile::LoadBinary(std::ifstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		GEOL_Context *context = getContext();

		int entitiesNum = 0;
		theStream -> read((char*)(&entitiesNum), sizeof(int));
		GEOL_Entity *newEntity = NULL;
		for (int i = 0 ; i < entitiesNum && ret ; i++) {
			GEOL_ObjectType objType = geol_Point;
			ret = getContext() -> loadBinaryObjectType(theStream, objType);
			if (ret) {
				switch(objType) {
					case geol_Point:
						{
							GEOL_Point *point = context -> createPoint();
							ret = ((GEOL_Entity*)point) -> LoadBinary(theStream);
							if (ret) {
								newEntity = point;
							}
						}
						break;
					case geol_Segment:
						{
							GEOL_Segment *segment = context -> createSegment();
							ret = ((GEOL_Entity*)segment) -> LoadBinary(theStream);
							if (ret) {
								newEntity = segment;
							}
						}
						break;
					case geol_Arc:
						{
							GEOL_Arc *arc = context -> createArc();
							ret = ((GEOL_Entity*)arc) -> LoadBinary(theStream);
							if (ret) {
								newEntity = arc;
							}
						}
						break;
					default:
						ret = false;
				}
			}
			if (ret) {
				ret = addEntity(newEntity);
			}
		}
		ret = laodBinaryObjectAttributes(theStream);
	}
	
	return ret;
}

bool GEOL_Profile::SaveBinary(std::ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		ret = saveBinaryObjectInfo(theStream, geol_Profile);
	}
	if (ret) {
		int entitiesNum = getNumOfEntities();
		theStream -> write((char*)(&entitiesNum), sizeof(int));
		for (GEOL_Entity *entity = getFirstEntity() ; entity && ret ; entity = getNextEntity(entity)) {
			ret = entity -> SaveBinary(theStream);
		}
		ret = saveBinaryObjectAttributes(theStream);
	}
	if (ret) {
		ret = !theStream -> bad();
	}
	
	GEOL_AttributeValue attrVal;
	attrVal.GEOL_AttrVoidValue = NULL;
	addAttribute(attrVal, GEOL_AttrVoid, "saved");

	return ret;
}

bool GEOL_Profile::LoadISO(std::ifstream *theStream) {
	if (!theStream)
		return false;

	return false;
}
