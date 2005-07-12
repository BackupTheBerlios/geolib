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
