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
bool GEOL_Profile::isClosed() const {
	bool ret = false;
	
	if (getNumOfEntities() > 1) {
		GEOL_Entity *lastProfileEntity = const_cast<GEOL_Profile*>(this) -> getLastEntity();
		GEOL_Entity *firstProfileEntity = const_cast<GEOL_Profile*>(this) -> getFirstEntity();
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
	invalidateBBox();
	
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
	invalidateBBox();
	
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
	invalidateBBox();
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
	list<GEOL_Entity*>::iterator entityIt;	
	for (entityIt = pEntityList.begin() ; !foundFlag && entityIt != pEntityList.end() ; entityIt++) {
		if (*entityIt == theEntity) {
			foundFlag = true;
		}
		else {
			prevEntity = *entityIt;
		}
	}
	
	// Entity not found
	if (!foundFlag) {
		return false;
	}
	
	bool ret = false;
	if (theReshapeFlag) {
		GEOL_Entity *nextEntity = NULL;
		entityIt++;
		if (entityIt != pEntityList.end()) {
			nextEntity = *entityIt;
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
	invalidateBBox();
	
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

	list<GEOL_Container*>::const_iterator contIt;
	for (contIt = pContainerList.begin() ; ret && contIt != pContainerList.end() ; ) {
		GEOL_Container *cont = *contIt;
		contIt++;
		if ((GEOL_Object*)cont == theObject) {
			ret = detachContainer(cont);
		}
	}
	
	list<GEOL_Entity*>::const_iterator entIt;
	for (entIt = pEntityList.begin() ; ret && entIt != pEntityList.end() ; ) {
		GEOL_Entity *ent = *entIt;
		entIt++;
		if ((GEOL_Object*)ent == theObject) {
			ret = detachEntity(ent);
		}
	}

	return ret;
}



/*!
\return
The length of profile contour
*/
double GEOL_Profile::length() const {
	double ret = 0.0;
	
	list<GEOL_Entity*>::const_iterator entityIt;
	for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() ; entityIt++) {
		ret += (*entityIt) -> length();
	}
	
	return ret;
}



/*!
\return
The area of a closed profile, or 0 if the profile is open
*/
double GEOL_Profile::area() const {
	double ret = 0.0;
	
	if (isClosed()) {
		list<GEOL_Entity*>::const_iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() ; entityIt++) {
			ret += (*entityIt) -> area();
		}
		ret = fabs(ret);
	}
	
	return ret;
}



/*!
Se il profilo è chiuso.
Cerca il punto e il segmento più a sinistra nel profilo (leftPt e leftSeg)
Si avranno le seguenti possibilità :
	- leftPt è uguale a una delle estremità leftSeg
		- Considera il segmento precedente a leftSeg se leftPt è sull' estremità sinistra e
		  il segmento seguente se è sull' estremità destra, chiama i due segmenti segPrev e segNext
				- Le direzioni dei due segmenti sono opposte
						Situazione indecidibile, occorre fare la somma delle differenze angolari di tutti i segmenti
						del profilo
				- Le direzioni dei due segmenti sono uguali
						Se la direzione di segPrev (in y) è > 0 (il segmento sale) la direzione del profilo è oraria
						Se la direzione di segPrev (in y) è < 0 (il segmento scende) la direzione del profilo è antioraria						
				- Le direzioni dei due segmenti sono diverse ma non opposte
						Se la direzione di segPrev è a sinistra rispetto a quella di segNext segPrev sta sotto
						segNext quindi il profilo è orario
						Se la direzione di segPrev è a destra rispetto a quella di segNext segPrev sta sopra segNext
						quindi il profilo è antiorario
	- leftPt non è uguale a nessuna delle estremità di leftSeg
		Il segmento è certamente un arco, il verso del  profilo è uguale a quello dell' arco
*/
GEOL_ProfVersus GEOL_Profile::versus() const {
	GEOL_ProfVersus ret = geol_ProfInvalid;
	
	if (!isClosed()) {
		return ret;
	}
	
	double xLeftmost = DBL_MAX;
	GEOL_Entity *xLeftmostEntity = NULL;
	if (getXLeftmost(xLeftmost, xLeftmostEntity) && xLeftmostEntity) {
		if (fabs(((GEOL_Point*)(xLeftmostEntity -> getBeginEntity())) -> x() - xLeftmost) < GEOL_EQUAL_POINT ||
			fabs(((GEOL_Point*)(xLeftmostEntity -> getEndEntity())) -> x() - xLeftmost) < GEOL_EQUAL_POINT) {
			// Il punto più a sx del profilo è un' estremità del segmento più a sx
		
			// Segmenti precedente e seguente il punto più a sx del profilo	
			GEOL_Entity *segNext = NULL;
			GEOL_Entity *segPrev = NULL;
			if (fabs(((GEOL_Point*)(xLeftmostEntity -> getBeginEntity())) -> x() - xLeftmost) < GEOL_EQUAL_POINT) {
				// Punto più a sx sull' estremità iniziale del segmento
				segNext = xLeftmostEntity;
				segPrev = getPrevEntity(xLeftmostEntity);
				if (!segPrev) {
					segPrev = getLastEntity();
				}
			}
			else {
				// Punto più a sx sull' estremità finale del segmento
				segPrev = xLeftmostEntity;
				segNext = getNextEntity(xLeftmostEntity);
				if (!segNext) {
					segNext = getFirstEntity();
				}
			}
			
			GEOL_Point *segPrevDir = getContext() -> createPoint();
			segPrev -> direction(segPrevDir, (GEOL_Point*)(segPrev -> getEndEntity()));
			GEOL_Point *segNextDir = getContext() -> createPoint();
			segNext -> direction(segNextDir, (GEOL_Point*)(segNext -> getBeginEntity()));
			
			if (segPrevDir -> isOpposite(*segNextDir)) {
				// I due segmenti opposta
				
				// Calcolare le differenze angolari
				// Si sommano le differenze angolari di tutte le coppie consecutive di segmenti del profilo
				// se la somma è 2*PI il profile è antiorario, se è -2*PI il profilo è orario
				double angDiffSum = 0.0;
				for (GEOL_Entity *entity = getFirstEntity() ; entity ; entity = getNextEntity(entity)) {
					GEOL_Entity *nextEntity = getNextEntity(entity);
					if (!nextEntity) {
						nextEntity = getFirstEntity();
					}
					angDiffSum += entity -> angleWith(nextEntity);
				}
				if (fabs(angDiffSum - (2,0 * GEOL_PI)) < GEOL_EQUAL_DIST) {
					ret = geol_ProfCounterClockwise;
				}
				else if (fabs(angDiffSum + (2,0 * GEOL_PI)) < GEOL_EQUAL_DIST) {
					ret = geol_ProfClockwise;					
				}
				else {
					ret = geol_ProfInvalid;
				}
			}
			else if (segPrevDir -> isParallel(*segNextDir)) {
				// I due segmenti sono paralleli
				
				if (segPrevDir -> y() > 0.0) {
					ret = geol_ProfClockwise;
				}
				else {
					ret = geol_ProfCounterClockwise;
				}
			}
			else {
				// I due segmenti non sono ne paralleli ne opposti
				
				if (segPrevDir -> isAtLeft(*segNextDir)) {
					ret = geol_ProfClockwise;
				}
				else {
					ret = geol_ProfCounterClockwise;
				}
			}

			if (segPrevDir) {
				getContext() -> deleteObject(segPrevDir);
			}
			if (segNextDir) {
				getContext() -> deleteObject(segNextDir);
			}
		}
		else {
			// Il punto più a sx del profilo NON è un' estremità del segmento più a sx, xLeftmostEntity deve essere un arco
			// e il verso del profilo è uguale a quello dell' arco
			
			if (xLeftmostEntity -> isArc()) {
				if (((GEOL_Arc*)xLeftmostEntity) -> versus() == geol_ArcClockwise) {
					ret = geol_ProfClockwise;
				}
				else {
					ret = geol_ProfCounterClockwise;
				}
			}
			else {
				ret = geol_ProfInvalid;
			}
		}
	}
	
	return ret;
}



/*!
Get the leftmost x coord and the leftmost entity (segment or arc) in the profile

\param theXLeftmost
On output contains the leftmost x coord
\param theLeftmostEntity
On output contains the pointer to the leftmost entity

\return
- true if the leftmost point/entity has been founded successfully
- false otherwise
*/
bool GEOL_Profile::getXLeftmost(double& theXLeftmost, GEOL_Entity* &theLeftmostEntity) const {
	theLeftmostEntity = NULL;
	theXLeftmost = DBL_MAX;
	bool ret = true;
	std::list<GEOL_Entity*>::const_iterator entityIt;	
	for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && ret ; entityIt++) {
		GEOL_Entity *entity = *entityIt;
		if (entity -> isSegment()) {
			GEOL_Segment *seg = (GEOL_Segment*)entity;
			if (seg -> begin() -> x() < theXLeftmost) {
				theXLeftmost = seg -> begin() -> x();
				theLeftmostEntity = entity;
			}
			else if (seg -> end() -> x() < theXLeftmost) {
				theXLeftmost = seg -> end() -> x();
				theLeftmostEntity = entity;
			}

		}
		else if (entity -> isArc()) {
			GEOL_Arc *arc = (GEOL_Arc*)entity;
			if ((arc -> getBBox()).getMinX() < theXLeftmost) {
				theXLeftmost = (arc -> getBBox()).getMinX();
				theLeftmostEntity = entity;
			}
		}
		else {
			theLeftmostEntity = NULL;
			ret = false;
		}
	}
	
	return ret;
}



/*!
\return
The bounding box of the profile
*/
GEOL_BBox GEOL_Profile::getBBox() {
	if (!mBBox || !mBBox -> isValid()) {
		GEOL_BBox bbox;
		
		list<GEOL_Entity*>::const_iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() ; entityIt++) {
			bbox = bbox + (*entityIt) -> getBBox();
		}
		
		setBBox(bbox);
	}
	
	return *mBBox;
}



bool GEOL_Profile::LoadBinary(ifstream *theStream) {
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

bool GEOL_Profile::SaveBinary(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		ret = saveBinaryObjectInfo(theStream);
	}
	if (ret) {
		int entitiesNum = getNumOfEntities();
		theStream -> write((char*)(&entitiesNum), sizeof(int));

		list<GEOL_Entity*>::iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && ret ; entityIt++) {
			GEOL_Entity *entity = *entityIt;
			ret = entity -> SaveBinary(theStream);
		}
		ret = saveBinaryObjectAttributes(theStream);
	}
	if (ret) {
		ret = !theStream -> bad();
	}
	
	GEOL_AttributeValue attrVal;
	attrVal.GEOL_AttrVoidValue = NULL;
	addAttribute(attrVal, GEOL_AttrVoid, GEOL_ID_SAVED);

	return ret;
}

bool GEOL_Profile::LoadISO(ifstream *theStream) {
	if (!theStream)
		return false;

	return false;
}

bool GEOL_Profile::SaveISO(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret && getNumOfEntities() > 0) {
		list<GEOL_Entity*>::iterator entityIt;
		entityIt = pEntityList.begin();
		GEOL_Entity *entity = *entityIt;
		GEOL_Point *beginPoint = (GEOL_Point*)(entity -> getBeginEntity());
		(*theStream) << "G0 X" << beginPoint -> x() << " Y" << beginPoint -> y() << endl;
		for ( ; entityIt != pEntityList.end() && ret ; entityIt++) {
			entity = *entityIt;
			ret = entity -> SaveISO(theStream);		
		}
	}
	
	return ret;
}
