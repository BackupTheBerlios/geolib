/***************************************************************************
								GEOL_Entity.cpp
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

#include "GEOL_Entity.h"



GEOL_Entity::GEOL_Entity() {
	mBegin = 0.0;
	mEnd = 0.0;
}


GEOL_Entity::~GEOL_Entity() {
	mBegin = 0.0;
	mEnd = 0.0;
}


/*!
Equality operator, two entities are equal if they are of the same type and are equal

\param theEntity
Entity to compare with this

\return
- true if the entities are equal
- false otherwise
*/
bool GEOL_Entity::operator==(const GEOL_Entity& theEntity) const {
	bool ret = false;
	
	GEOL_Entity *thisEntity = (GEOL_Entity*)this;
	GEOL_Entity *entity = (GEOL_Entity*)(&theEntity);
	if (dynamic_cast<GEOL_Point*>(thisEntity) && dynamic_cast<GEOL_Point*>(entity)) {
		if ((*((GEOL_Point*)thisEntity)) == (*((GEOL_Point*)entity))) {
			ret = true;
		}
	}
	else if (dynamic_cast<GEOL_Segment*>(thisEntity) && dynamic_cast<GEOL_Segment*>(entity)) {
		if ((*((GEOL_Segment*)thisEntity)) == (*((GEOL_Segment*)entity))) {
			ret = true;
		}
	}
	else if (dynamic_cast<GEOL_Arc*>(thisEntity) && dynamic_cast<GEOL_Arc*>(entity)) {
		if ((*((GEOL_Arc*)thisEntity)) == (*((GEOL_Arc*)entity))) {
			ret = true;
		}
	}
	
	return ret;
}


/*!
Inequality operator

\sa operator==(const GEOL_Entity& theEntity)
*/
bool GEOL_Entity::operator!=(const GEOL_Entity& theEntity) const {
	return !((*this) == theEntity);
}



/*!
\return
The first entity component pointer (x coordinate for GEOL_Point)
*/
const GEOL_Entity* GEOL_Entity::getBeginEntity() const {
	GEOL_Entity *ret = NULL;
	
	char *entityPtr = (char*)&ret;
	char *beginPtr = (char*)&mBegin;
	
	for (short i = 0 ; i < 4 ; i++) {
		*(entityPtr + i) = *(beginPtr + i);
	}
	
	return ret;
}


/*!
\return
The second entity component pointer (y coordinate for GEOL_Point)
*/
const GEOL_Entity* GEOL_Entity::getEndEntity() const {
	GEOL_Entity *ret = NULL;
	
	char *entityPtr = (char*)&ret;
	char *endPtr = (char*)&mEnd;
	
	for (short i = 0 ; i < 4 ; i++) {
		*(entityPtr + i) = *(endPtr + i);
	}
	
	return ret;
}


/*!
Set the first entity component (x coordinate for GEOL_Point)

\param theBegin
Pointer to the entity to set
*/
void GEOL_Entity::setBeginEntity(GEOL_Entity *theBegin) {
	char *entityPtr = (char*)&theBegin;
	char *beginPtr = (char*)&mBegin;
	
	for (short i = 0 ; i < 4 ; i++) {
		*(beginPtr + i) = *(entityPtr + i);
	}
}

/*!
Set the second entity component (x coordinate for GEOL_Point)

\param theEnd
Pointer to the entity to set
*/
void GEOL_Entity::setEndEntity(GEOL_Entity *theEnd) {
	char *entityPtr = (char*)&theEnd;
	char *endPtr = (char*)&mEnd;
	
	for (short i = 0 ; i < 4 ; i++) {
		*(endPtr + i) = *(entityPtr + i);
	}
}


