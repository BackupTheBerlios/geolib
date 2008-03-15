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
	invalidateBBox();
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
Compute the orientation versus of a closed profile, if this is a closed profile will do the follow :
Find the leftmost profile point leftPt, and its segment leftSeg, then
- If leftPt is one of the end point of leftSeg
	- Get the segment before leftSeg if leftPt is its start point, or the segment after leftSeg if leftPt
	  is its end point, so we will have segPrev and segNext, as the segments before and after leftPt
		- segPrev and segNext have opposite directions
			- Sum up all the angular differences of adjacent segments, at the end if the sum is equal to 2*PI
			  the profile is clockwise oriented, otherwise is counterclockwise oriented
		- segPrev and segNext have the same directions
			- If the segPrev direction on y axis is positive (ascending segment) the profile is clockwise oriented
			- If the segPrev direction on y axis is negative (descending segment) the profile is counterclockwise oriented
		- segPrev and segNext have different, but not opposite, directions
			- If segPrev direction is at left with respect to segNext direction the profile is clockwise oriented
			- If segPrev direction is at right with respect to segNext direction the profile is counterclockwise oriented
- If leftPt is not a segment end point, for sure it is an a circle arc segment, so the profile orientation is the same
  of the orientation of the arc

\return
- geol_ProfClockwise if this is a clockwise oriented profile
- geol_ProfCounterClockwise if this is a counterclockwise oriented profile
- geol_ProfInvalid if tis is an open or invalid profile
*/
GEOL_Profile::GEOL_ProfVersus GEOL_Profile::versus() const {
	GEOL_ProfVersus ret = geol_ProfInvalid;
	
	if (!isClosed()) {
		return ret;
	}
	
	double xLeftmost = DBL_MAX;
	GEOL_Entity *xLeftmostEntity = NULL;
	if (getXLeftmost(xLeftmost, xLeftmostEntity) && xLeftmostEntity) {
		if (fabs(((GEOL_Point*)(xLeftmostEntity -> getBeginEntity())) -> x() - xLeftmost) < GEOL_EQUAL_POINT ||
			fabs(((GEOL_Point*)(xLeftmostEntity -> getEndEntity())) -> x() - xLeftmost) < GEOL_EQUAL_POINT) {
			// The leftmost point is an end point of a segment
		
			// Get the segments before and after the leftmost point	
			GEOL_Entity *segNext = NULL;
			GEOL_Entity *segPrev = NULL;
			if (fabs(((GEOL_Point*)(xLeftmostEntity -> getBeginEntity())) -> x() - xLeftmost) < GEOL_EQUAL_POINT) {
				segNext = xLeftmostEntity;
				segPrev = getPrevEntity(xLeftmostEntity);
				if (!segPrev) {
					segPrev = getLastEntity();
				}
			}
			else {
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
				// segPrev and segNext have opposite directions
				
				// Sum up the angular differences of the adjacent segments
				double angDiffSum = 0.0;
				for (GEOL_Entity *entity = getFirstEntity() ; entity ; entity = getNextEntity(entity)) {
					GEOL_Entity *nextEntity = getNextEntity(entity);
					if (!nextEntity) {
						nextEntity = getFirstEntity();
					}
					angDiffSum += entity -> angleWith(nextEntity);
				}
				if (fabs(angDiffSum - (2,0 * GEOL_PI)) < GEOL_EQUAL_DIST) {
					// Sum of angular differences is equal to -2*PI, the profile is counterclockwise oriented
					ret = geol_ProfCounterClockwise;
				}
				else if (fabs(angDiffSum + (2,0 * GEOL_PI)) < GEOL_EQUAL_DIST) {
					// Sum of angular differences is equal to 2*PI, the profile is clockwise oriented
					ret = geol_ProfClockwise;					
				}
				else {
					// Sum of angular differences is not valid
					ret = geol_ProfInvalid;
				}
			}
			else if (segPrevDir -> isParallel(*segNextDir)) {
				// segPrev and segNext have the same directions
				
				if (segPrevDir -> y() > 0.0) {
					ret = geol_ProfClockwise;
				}
				else {
					ret = geol_ProfCounterClockwise;
				}
			}
			else {
				// segPrev and segNext have different, but not opposite, directions
				
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
			// The leftmost point is not an end point of a segment, it has to be on a circle arc if so the profile direction
			// is the same of the arc direction, otherwise the profile has an invalid orientation
			
			if (xLeftmostEntity -> isArc()) {
				if (((GEOL_Arc*)xLeftmostEntity) -> versus() == GEOL_Arc::geol_ArcClockwise) {
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


/*!
Load the profile from a binary file

\param theStream
Binary file

\return
- true if the load operation succeed
- false otherwise
*/
bool GEOL_Profile::LoadBinary(ifstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();
	if (ret) {
		GEOL_Context *context = getContext();

		// Read the number of entities in the profile
		int entitiesNum = 0;
		theStream -> read((char*)(&entitiesNum), sizeof(int));
		
		GEOL_Entity *newEntity = NULL;
		for (int i = 0 ; i < entitiesNum && ret ; i++) {
			
			// Read the object type
			GEOL_ObjectType objType = geol_Point;
			ret = context -> loadBinaryObjectType(theStream, objType);
			
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

		// Load the object attributes
		ret = laodBinaryObjectAttributes(theStream);
	}
	
	return ret;
}


/*!
Save the profile in a binary file

\param theStream
Binary file

\return
- true if the save operation succeed
- false otherwise
*/
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
	addAttribute(attrVal, GEOL_Attribute::GEOL_AttrVoid, GEOL_ID_SAVED);

	return ret;
}


/*!
Load a profile from an ISO file

\param theStream
ISO file

\return
- true if the load operation succeed
- false otherwise
*/
bool GEOL_Profile::LoadISO(ifstream *theStream) {
	if (!theStream)
		return false;
	bool ret = true;
	
	GEOL_Context *context = getContext();

	// G0 code founded (has to be the first)
	bool g0Flag = false;
	// Position on the file before the nex real line operation
	streampos actualPos = 0;
	
	// Previous (x,y,z) position
	double xPos = DBL_MAX;
	double yPos = DBL_MAX;
	double zPos = DBL_MAX;
	
	// Actual (x,y,z) position
	double xRead = DBL_MAX;
	double yRead = DBL_MAX;
	double zRead = DBL_MAX;
	
	// Center point of an arc
	double iRead = DBL_MAX;
	double jRead = DBL_MAX;
	
	// Text line readed
	std::string lineStr;
	
	bool exitFlag = false;
	do {

		// Read the next line
		actualPos = theStream -> tellg();
		ret = readLineFromISOFile(theStream, lineStr);
		
		if (lineStr.empty() && theStream -> eof()) {
			// End of file reached, exit

			ret = true;
			exitFlag = true;
		}
		
		if (ret && !exitFlag) {
			if (lineStr[0] != 'G' || (getNumOfEntities() == 0 && lineStr[1] != '0' && !g0Flag)) {
				// If the first command is not a G0, exit with error
				ret = false;
			}
			else {

				// Get coordinates values
				ret = getCoordFromISOFileLine(lineStr, xRead, yRead, zRead, iRead, jRead);
				
				if (ret) {
					switch (lineStr[1]) {
						case '0':								// G0
							{
								if (getNumOfEntities() == 0) {
									xPos = xRead;
									yPos = yRead;
									zPos = zRead;
								}
								else {
									theStream -> seekg(actualPos);
									exitFlag = true;
								}
								g0Flag = true;
							}
							break;
						case '1':								// G1 Straight segment
							{
								GEOL_Entity *newSeg = context -> createSegment(xPos, yPos, xRead, yRead);
								if (newSeg) {
									ret = addEdgeTail(newSeg);
								}
								else {
									ret = false;
								}
							}
							break;
						case '2':								// G2 Clockwise arc
						case '3':								// G3 Counterclockwise arc
							{
								double dx = xRead - iRead;
								double dy = yRead - jRead;
								double radius = sqrt(dx*dx + dy*dy);
								GEOL_Arc::GEOL_ArcVersus arcVersus = lineStr[1] == '2' ? GEOL_Arc::geol_ArcClockwise : GEOL_Arc::geol_ArcCounterClockwise;
								GEOL_Entity *newArc = context -> createArc(xPos, yPos, xRead, yRead, radius, arcVersus);
								if (newArc) {
									ret = addEdgeTail(newArc);
								}
								else {
									ret = false;
								}
							}
							break;
						default:
							ret = false;
							break;
					}
				}
				xPos = xRead;
				yPos = yRead;
				zPos = zRead;
			}
		}
	}
	while (ret && !exitFlag);

	return ret;
}


/*!
Save the profile in ISO format on a file

\param theStream
ISO file

\return
- true if the save operation succeed
- false otherwise
*/
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
