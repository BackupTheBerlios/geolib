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
The angle between two entities, or DBL_MAX if one of the entities is not a segment/arc.
The segments is supposed adjacent, the first segmetn is ths, and the second is the parameter

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

		getContext() -> deleteObject(segPrevDir);
		segPrevDir = NULL;
		getContext() -> deleteObject(segNextDir);
		segNextDir = NULL;
	}
	
	return ret;
}


/*!
Compute the intersection points between this and the specified entity

\param theEntity
Entity to check

\return
The list of intersections between this and theEntity
*/
GEOL_Intersections GEOL_Entity::intersect(GEOL_Entity* theEntity) {
	GEOL_Intersections ret;
	
	if (theEntity && (theEntity -> isSegment() || theEntity -> isArc()) && (isSegment() || isArc())) {
		if ((getBBox().isIntersected(theEntity -> getBBox()))) {
			// Le bbox delle due entitˆ si intersecano, cerco le intersezioni
			
			if (isSegment() && theEntity -> isSegment()) {
				// Segment - Segment intersection

				double x1s = ((GEOL_Segment*)this) -> begin() -> x();
				double y1s = ((GEOL_Segment*)this) -> begin() -> y();
				double x1e = ((GEOL_Segment*)this) -> end() -> x();
				double y1e = ((GEOL_Segment*)this) -> end() -> y();
				double x2s = ((GEOL_Segment*)theEntity) -> begin() -> x();
				double y2s = ((GEOL_Segment*)theEntity) -> begin() -> y();
				double x2e = ((GEOL_Segment*)theEntity) -> end() -> x();
				double y2e = ((GEOL_Segment*)theEntity) -> end() -> y();
				double d = (x1s - x1e)*(y2s - y2e) - (y1s - y1e)*(x2s - x2e);
				if (fabs(d) > GEOL_NOTNULL) {
					// Segments not parallel, compute the intersection point, and verify it against the bounding box
					// of the segments
					
					double n1 = (x1s * x1e) - (y1s * y1e);
					double n2 = (x2s * x2e) - (y2s * y2e);
					double xi = ((n1 * (x2s - x2e)) - (n2 * (x1s - x1e))) / d;
					double yi = ((n1 * (y2s - y2e)) - (n2 * (y1s - y1e))) / d;

					if (getBBox().isPointInBBox(xi, yi) && (theEntity -> getBBox()).isPointInBBox(xi, yi)) {
						// The intersection point belongs to the bounding box, add it to the output list

						GEOL_Inters newInters(xi, yi, this, theEntity);
						newInters.checkIntersTypeAndPos();
						ret.pushBack(newInters);
					}
				}
				else {
					// Parallel segments, can be disjoint, overlapped or with an extremity point in common.
					// In any case ther is no clear intersections between segments
				}
			}
			else if (isArc() && theEntity -> isArc()) {
				// Arc - Arc intersection

				GEOL_Point *c1 = getContext() -> createPoint();
				GEOL_Point *c2 = getContext() -> createPoint();
				((GEOL_Arc*)this) -> center(*c1);
				((GEOL_Arc*)theEntity) -> center(*c2);
				
				if (c1 -> pointDistance(c2) < GEOL_EQUAL_POINT) {
					// The arcs has the same center point, no intersections
				}
				else {
					// Diiferent center points, comput the intersections, we can found 0, 1 or 2 intersection points
					
					double d = c1 -> pointDistance(c2);
					double dx = c2 -> x() - c1 -> x();
					double dy = c2 -> y() - c1 -> y();
					double dSqr = d * d;
					double r0 = ((GEOL_Arc*)this) -> radius();
					double r0Sqr = r0 * r0;
					double r1 = ((GEOL_Arc*)theEntity) -> radius();
					double r1Sqr = r1 * r1;
					double a = (r0Sqr - r1Sqr + dSqr) / (2,0 * d);
					double xp = c1 -> x() + (dx * (a / d));
					double yp = c1 -> y() + (dy * (a / d));
					double h = sqrt(r0Sqr - (a * a));
					double xi1 = xp + h * (dy / d);
					double yi1 = yp + h * (dx / d);
					double xi2 = xp - h * (dy / d);
					double yi2 = yp - h * (dx / d);

					if (getBBox().isPointInBBox(xi1, yi1) && (theEntity -> getBBox()).isPointInBBox(xi1, yi1)) {
						// The first intersection belongs to the bbox, add it to the output list

						GEOL_Inters newInters(xi1, yi1, this, theEntity);
						newInters.checkIntersTypeAndPos();
						ret.pushBack(newInters);
					}
					if (getBBox().isPointInBBox(xi2, yi2) && (theEntity -> getBBox()).isPointInBBox(xi2, yi2)) {
						// The second intersection belongs to the bbox, add it to the output list

						GEOL_Inters newInters(xi2, yi2, this, theEntity);
						newInters.checkIntersTypeAndPos();
						ret.pushBack(newInters);
					}
				}
				
				// Delete center points
				getContext() -> deleteObject(c1);
				c1 = NULL;
				getContext() -> deleteObject(c2);
				c2 = NULL;
			}
			else if ((isSegment() && theEntity -> isArc()) || (isArc() && theEntity -> isSegment())) {
				// Arc - Segment intersection

				GEOL_Segment *segment = NULL;
				GEOL_Arc *arc = NULL;
				if (isSegment()) {
					segment = (GEOL_Segment*)this;
					arc = (GEOL_Arc*)theEntity;
				}
				else {
					segment = (GEOL_Segment*)theEntity;
					arc = (GEOL_Arc*)this;
				}

				// Translate the segment, as if the arc is centerd in the origin
				double xTras = 0.0;
				double yTras = 0.0;
				arc -> center(xTras, yTras);
				segment -> translate(-xTras, -yTras);

				// Compute the intersection points
				double r = arc -> radius();
				double dx = segment -> end() -> x() - segment -> begin() -> x();
				double dy = segment -> end() -> y() - segment -> begin() -> y();
				double dxy = sqrt(dx*dx + dy*dy);
				double dxySqr = dxy*dxy;
				double det = (segment -> begin() -> x() * segment -> end() -> y()) - (segment -> end() -> x() * segment -> begin() -> y());
				double delta = (r*r*dxySqr) - (det*det);

				if (fabs(delta) < GEOL_NOTNULL) {
					// Two coincident intersections, the segment is tangent to the arc no intersections added
				}
				else {
					if (delta > 0.0) {
						// Two distinct intersections

						double deltaSqrt = sqrt(delta);
						double xi1 = ((det*dy) + ((dy < 0.0 ? -1.0 : 1.0) * deltaSqrt)) / dxySqr;
						double yi1 = ((-det*dy) + (fabs(dy) * deltaSqrt)) / dxySqr;
						double xi2 = ((det*dy) - ((dy < 0.0 ? -1.0 : 1.0) * deltaSqrt)) / dxySqr;
						double yi2 = ((-det*dy) - (fabs(dy) * deltaSqrt)) / dxySqr;

						if (getBBox().isPointInBBox(xi1, yi1) && (theEntity -> getBBox()).isPointInBBox(xi1, yi1)) {
							// The first intersecion belongs to the bbox, add it to the output list

							GEOL_Inters newInters(xi1, yi1, this, theEntity);
							newInters.checkIntersTypeAndPos();
							ret.pushBack(newInters);
						}
						if (getBBox().isPointInBBox(xi2, yi2) && (theEntity -> getBBox()).isPointInBBox(xi2, yi2)) {
							// The second intersecion belongs to the bbox, add it to the output list

							GEOL_Inters newInters(xi2, yi2, this, theEntity);
							newInters.checkIntersTypeAndPos();
							ret.pushBack(newInters);
						}
					}
					else {
						// No intersections
					} 
				}

				// Restor the original segment position
				segment -> translate(xTras, yTras);
			}
			else {
				// One of the entities is not a segment nor an arc, no intersections
			}
		}
	}
	
	return ret;
}


/*!
Check if a segment is at left respect to another segment given an intersection point, taking the orientation
of this segment determine if the other segment is at left of this or not

\param theEntity
The other entity to check
\param theXCoord
X coord of intersection point
\param theYCoord
Y coord of intersection point

\return
- geol_Undef if no orientation can be determined
- geol_Left if this is at left of theEntity
- geol_Right if this is at right of theEntity
*/
GEOL_Entity::GEOL_Side GEOL_Entity::isAtLeft(GEOL_Entity *theEntity, double theXCoord, double theYCoord) {
	GEOL_Side ret = geol_Undef;
	if (theEntity && (isSegment() || isArc()) && (theEntity -> isSegment() || theEntity -> isArc())) {
		GEOL_Point *thisDir = getContext() -> createPoint();
		direction(thisDir, theXCoord, theYCoord);
		GEOL_Point *entityDir = getContext() -> createPoint();
		theEntity -> direction(entityDir, theXCoord, theYCoord);
		if (thisDir -> isAtLeft(*entityDir)) {
			ret = geol_Left;
		}
		else {
			ret = geol_Right;
		}
	}

	return ret;
}


/*!
Check if a segment is at left respect to another segment given an intersection point

\param theEntity
The other entity to check
\param thePoint
Intersection point

\return
- geol_Undef if no orientation can be determined
- geol_Left if this is at left of theEntity
- geol_Right if this is at right of theEntity
*/
GEOL_Entity::GEOL_Side GEOL_Entity::isAtLeft(GEOL_Entity *theEntity, GEOL_Point *thePoint) {
	if (theEntity && thePoint) {
		return isAtLeft(theEntity, thePoint -> x(), thePoint -> y());
	}
	else {
		return geol_Undef;
	}
}


