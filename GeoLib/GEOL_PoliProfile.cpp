/***************************************************************************
							 GEOL_PoliProfile.cpp
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
#include "GEOL_PoliProfile.h"


GEOL_PoliProfile::GEOL_PoliProfile() {
	mObjType = geol_PoliProfile;
}


GEOL_PoliProfile::~GEOL_PoliProfile() {
}


/*!
Add a profile to the poliprofile

\param theNewProfile
Profile to add

\return
- true if the operation succeed
- false otherwise
*/
bool GEOL_PoliProfile::addProfile(GEOL_Profile *theNewProfile) {
	if (!theNewProfile)
		return false;
		
	invalidateBBox();
	return addContainer(theNewProfile);
}


/*!
Remove a profile from the poliprofile, the profile is destroyed if its reference counter is 0

\param theProfile
Profile to remove

\return
- true if the operation succeed
- false otherwise
*/
bool GEOL_PoliProfile::removeProfile(GEOL_Profile *theProfile) {
	if (!theProfile)
		return false;

	invalidateBBox();
	return removeContainer(theProfile);
}


/*!
Detach a profile from the poliprofile, the profile is NOT destroyed, just detached from the list

\param theProfile
Profile to detach

\return
- true if the operation succeed
- false otherwise
*/
bool GEOL_PoliProfile::detachProfile(GEOL_Profile *theProfile) {
	if (!theProfile)
		return false;

	invalidateBBox();
	return detachContainer(theProfile);
}


/*!
\return
The bounding box of the poliprofile
*/
GEOL_BBox GEOL_PoliProfile::getBBox() {
	if (!mBBox || !mBBox -> isValid()) {
		GEOL_BBox bbox;
		
		list<GEOL_Container*>::const_iterator containerIt;
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() ; containerIt++) {
			bbox = bbox + (*containerIt) -> getBBox();
		}
		list<GEOL_Entity*>::const_iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() ; entityIt++) {
			bbox = bbox + (*entityIt) -> getBBox();
		}

		setBBox(bbox);
	}
	
	return *mBBox;
}


/*!
Load the poliprofile from a binary file

\param theStream
Binary file

\return
- true if the load operation succeed
- false otherwise
*/
bool GEOL_PoliProfile::LoadBinary(ifstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		GEOL_Context *context = getContext();

		// Read the number of containers
		int containersNum = 0;
		theStream -> read((char*)(&containersNum), sizeof(int));

		GEOL_Container *newContainer = NULL;
		for (int i = 0 ; i < containersNum && ret ; i++) {
			
			// Read the container type
			GEOL_ObjectType objType = geol_Point;
			ret = getContext() -> loadBinaryObjectType(theStream, objType);
			
			if (ret) {
				switch(objType) {
					case geol_PoliProfile:
						{
							GEOL_PoliProfile *poliprofile = context -> createPoliProfile();
							ret = poliprofile -> LoadBinary(theStream);
							if (ret) {
								newContainer = poliprofile;
							}
						}
						break;
					case geol_Profile:
						{
							GEOL_Profile *profile = context -> createProfile();
							ret = ((GEOL_Object*)profile) -> LoadBinary(theStream);
							if (ret) {
								newContainer = profile;
							}
						}
						break;
					default:
						ret = false;
				}
			}
			if (ret) {
				ret = addContainer(newContainer);
			}
		}
		if (ret) {

			// Read the number of entities
			int entitiesNum = 0;
			theStream -> read((char*)(&entitiesNum), sizeof(int));

			GEOL_Entity *newEntity = NULL;
			for (int i = 0 ; i < entitiesNum && ret ; i++) {
				
				// Read the entity type
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

			// Read the entity attributes
			ret = laodBinaryObjectAttributes(theStream);		
		}
	}
	if (ret) {
		ret = !theStream -> bad();
	}
	
	return ret;
}


/*!
Save the poliprofile in a binary file

\param theStream
Binary file

\return
- true if the save operation succeed
- false otherwise
*/
bool GEOL_PoliProfile::SaveBinary(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		ret = saveBinaryObjectInfo(theStream);
	}
	if (ret) {
		int containersNum = getNumOfContainers();
		theStream -> write((char*)(&containersNum), sizeof(int));
		
		std::list<GEOL_Container*>::const_iterator containerIt;
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && ret ; containerIt++) {
			ret = (*containerIt) -> SaveBinary(theStream);
		}
		
		int entitiesNum = getNumOfEntities();
		theStream -> write((char*)(&entitiesNum), sizeof(int));
		std::list<GEOL_Entity*>::const_iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && ret ; entityIt++) {
			ret = (*entityIt) -> SaveBinary(theStream);
		}
		ret = saveBinaryObjectAttributes(theStream);
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
Load a poliprofile from an ISO file

\param theStream
ISO file

\return
- true if the load operation succeed
- false otherwise
*/
bool GEOL_PoliProfile::LoadISO(ifstream *theStream) {
	if (!theStream)
		return false;

	bool ret = true;
	GEOL_Context *context = getContext();
	
	while (ret && !theStream -> eof()) {
		GEOL_Profile *profile = context -> createProfile();
		ret = ((GEOL_Object*)profile) -> LoadISO(theStream);
		if (ret) {
			ret = addProfile(profile);
		}
	}
	
	return ret;
}


/*!
Save the poliprofile in ISO format on a file

\param theStream
ISO file

\return
- true if the save operation succeed
- false otherwise
*/
bool GEOL_PoliProfile::SaveISO(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret && getNumOfContainers() > 0) {
		std::list<GEOL_Container*>::const_iterator containerIt;
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && ret ; containerIt++) {
			(*containerIt) -> SaveISO(theStream);
		}
	}

	return ret;
}
