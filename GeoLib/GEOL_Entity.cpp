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

#include "GEOL_Context.h"
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
	if (thisEntity -> isPoint() && entity -> isPoint()) {
		if ((*((GEOL_Point*)thisEntity)) == (*((GEOL_Point*)entity))) {
			ret = true;
		}
	}
	else if (thisEntity -> isSegment() && entity -> isSegment()) {
		if ((*((GEOL_Segment*)thisEntity)) == (*((GEOL_Segment*)entity))) {
			ret = true;
		}
	}
	else if (thisEntity -> isArc() && entity -> isArc()) {
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
	if (theBegin) {
		char *entityPtr = (char*)&theBegin;
		char *beginPtr = (char*)&mBegin;
		
		for (short i = 0 ; i < 4 ; i++) {
			*(beginPtr + i) = *(entityPtr + i);
		}
	}
	else {
		mBegin = 0.0;
	}
}

/*!
Set the second entity component (x coordinate for GEOL_Point)

\param theEnd
Pointer to the entity to set
*/
void GEOL_Entity::setEndEntity(GEOL_Entity *theEnd) {
	if (theEnd) {
		char *entityPtr = (char*)&theEnd;
		char *endPtr = (char*)&mEnd;
		
		for (short i = 0 ; i < 4 ; i++) {
			*(endPtr + i) = *(entityPtr + i);
		}
	}
	else {
		mEnd = 0.0;
	}
}



/*!
\return
The angle between two entities, or DBL_MAX if oun of the entities is not a segmento/arc

\param theEntity
The other entity 

\return
The angle between this and theEntity (supposed to be adjacent segments/arcs), or DBL_MAX if theEntiy
is NULL, or entities are not segments/arcs
*/
double GEOL_Entity::angleWith(const GEOL_Entity* theEntity) const {
	double ret = DBL_MAX;

	if (theEntity && (isSegment() || isArc()) && (theEntity -> isSegment() || theEntity -> isArc())) {
		GEOL_Point *segPrevDir = getContext() -> createPoint();
		direction(segPrevDir, (GEOL_Point*)getEndEntity());
		GEOL_Point *segNextDir = getContext() -> createPoint();
		theEntity -> direction(segNextDir, (GEOL_Point*)(theEntity -> getBeginEntity()));
		
		ret = atan2(segPrevDir -> y(), segPrevDir -> x()) - atan2(segNextDir -> y(), segNextDir -> x());
		if (ret < -GEOL_PI)
			ret += 2,0 * GEOL_PI;
		else if (ret > GEOL_PI)
			ret -= 2,0 * GEOL_PI;
	}
	
	return ret;
}

