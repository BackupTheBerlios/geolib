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
	mObjType = geol_Profile;
}


GEOL_Profile::~GEOL_Profile() {
}


/*!
Check if the profile is closed, a profile is a list of concatenated segments, so to verify its clousure is enough to
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
bool GEOL_Profile::addEdgeTail(GEOL_Entity *theNewEntity) {
	if (!theNewEntity)
		return false;
	if (!theNewEntity -> isSegment() && !theNewEntity -> isArc())
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
bool GEOL_Profile::addEdgeFront(GEOL_Entity *theNewEntity) {
	if (!theNewEntity)
		return false;
	if (!theNewEntity -> isSegment() && !theNewEntity -> isArc())
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
Remove and destroy a segment/arc from the profile, the resulting profile can be reshaped extendig the adiacent
segments to keep the profile "connection"

\param theEntity
Entity (segment or arc) to remove
\param theReshapeFlag
If true the profile is reshaped in order to mantain its connection, that is stay close if the profile is closed
and don't brake in many pieces.
If false removal of a segment from a closed profile generates an open profile, and removal of a segment from an open
profile will be allowed only from its extremity

\return
- ture if the entity is succesfully removed
- false otherwise, removal not allowed or error
*/
bool GEOL_Profile::removeEdge(GEOL_Entity *theEntity, bool theReshapeFlag) {
	if (!theEntity)
		return false;
		
	return releaseEdgeAndReshape(theEntity, theReshapeFlag, true);
}



/*!
Remove all the edges from profile, destroy them if its reference counter is 0
*/
void GEOL_Profile::removeAllEdges() {
	return removeAllEntities();
}



/*!
Detach a segment/arc from the profile, the resulting profile can be reshaped extendig the adiacent
segments to keep the profile "connection"

\param theEntity
Entity (segment or arc) to detach
\param theReshapeFlag
If true the profile is reshaped in order to mantain its connection, that is stay close if the profile is closed
and don't brake in many pieces.
If false removal of a segment from a closed profile generates an open profile, and removal of a segment from an open
profile will be allowed only from its extremity

\return
- ture if the entity is succesfully detached
- false otherwise, detach not allowed or error
*/
bool GEOL_Profile::detachEdge(GEOL_Entity *theEntity, bool theReshapeFlag) {
	if (!theEntity)
		return false;
		
	return releaseEdgeAndReshape(theEntity, theReshapeFlag, false);
}


/*!
Remove or detach a entity from the profile and reshepe the profile

\param theEntity
Entity to remove or detach
\param theReshapeFlag
If true the profile is reshaped after removal
\param theRemoveFlag
If true the entity is removed from the profile, if false the entity is simply detached

\return
- ture if the entity is succesfully removed/detached
- false otherwise, remove/detach not allowed or error
*/
bool GEOL_Profile::releaseEdgeAndReshape(GEOL_Entity *theEntity, bool theReshapeFlag, bool theRemoveFlag) {
	if (!theEntity)
		return false;
	
	bool closedFlag = isClosed();
	
	// Removal/Detach not allowed
	if (closedFlag && theReshapeFlag && getNumOfEntities() <= 3) {
		return false;
	}
	
	// Search the entity to remove/detach within the profile
	GEOL_Entity *prevEntity = NULL;
	if (closedFlag) {
		prevEntity = getLastEntity();
	}
	bool foundFlag = false;
	for (mEntityIt = pEntityList.begin() ; !foundFlag && mEntityIt != pEntityList.end() ; mEntityIt++) {
		if (*mEntityIt == theEntity) {
			foundFlag = true;
		}
		else {
			prevEntity = *mEntityIt;
		}
	}
	
	// Entity not found
	if (!foundFlag) {
		return false;
	}
	
	bool ret = false;
	if (theReshapeFlag) {
		GEOL_Entity *nextEntity = NULL;
		mEntityIt++;
		if (mEntityIt != pEntityList.end()) {
			nextEntity = *mEntityIt;
		}
		else {
			if (closedFlag) {
				nextEntity = getFirstEntity();
			}
		}
		
		// Remove/Detach the entity and reshape the profile
		if (theRemoveFlag) {
			ret = removeEntity(theEntity);
		}
		else {
			ret = detachEntity(theEntity);
		}
		if (ret) {
			if (prevEntity && nextEntity) {
				assert(prevEntity != nextEntity);
				*((GEOL_Point*)(prevEntity -> getEndEntity())) = *((GEOL_Point*)(nextEntity -> getBeginEntity()));
			}
		}
	}
	else {
		// Removal/Detach not allowed
		if (!closedFlag && theEntity != getLastEntity() && theEntity != getFirstEntity()) {
			ret = false;
		}
		else {
			// Remove/Detach the entity
			if (theRemoveFlag) {
				ret = removeEntity(theEntity);
			}
			else {
				ret = detachEntity(theEntity);
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



double GEOL_Profile::length() const {
	double ret = 0.0;
	list<GEOL_Entity*>::const_iterator entityIt;
	for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() ; entityIt++) {
		ret += (*entityIt) -> length();
	}
	
	return ret;
}



GEOL_BBox GEOL_Profile::getBBox() {
	if (!mBBox || !mBBox -> isValid()) {
		GEOL_BBox bbox;
		// to do
		setBBox(bbox);
	}
	
	return *mBBox;
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
