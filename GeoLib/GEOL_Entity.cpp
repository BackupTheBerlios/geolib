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


/*!
Entities is unaffected by the destruction of other objects, so this method does nothing except the check of
parameters correctness

\param theObject
The object that will be destroyed

\return
- true if theObject is non null
- false otherwise
*/
bool GEOL_Entity::notifyDestruction(GEOL_Object *theObject) {
	if (theObject)
		return true;
	else
		return false;
}

