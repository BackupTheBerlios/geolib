/***************************************************************************
                                 GEOL_Entity.h
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


#ifndef GEOL_ENTITY_H
#define GEOL_ENTITY_H


#include "GEOL_Object.h"
#include "GEOL_Intersections.h"


class GEOL_Point;


/*!
This abstract class represents the primitive geometrical objects such as points, segments and arcs, contains two pointers
to other objects of this class, used to store the x,y coordinates for points
*/
class GEOL_Entity : public GEOL_Object {
public:

	/*!
	Side of a segment respect to another segment, from a given point of intersection
	*/
	typedef enum {
					geol_Undef,		/// Undefined situation
					geol_Left,		/// The first segment of the pair is at left of the second
					geol_Right		/// The first segment of the pair is at right of the second
				 } GEOL_Side;

	GEOL_Entity();
	virtual ~GEOL_Entity();
	
	bool operator==(const GEOL_Entity& theEntity) const;
	bool operator!=(const GEOL_Entity& theEntity) const;

	const GEOL_Entity* getBeginEntity() const;
	const GEOL_Entity* getEndEntity() const;

	void setBeginEntity(GEOL_Entity *theBegin);
	void setEndEntity(GEOL_Entity *theEnd);
	double angleWith(const GEOL_Entity* theEntity) const;

	GEOL_Intersections intersect(GEOL_Entity* theEntity);
	GEOL_Side isAtLeft(GEOL_Entity *theEntity, double theXCoord, double theYCoord);
	GEOL_Side isAtLeft(GEOL_Entity *theEntity, GEOL_Point *thePoint);
	
	virtual bool notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag) = 0;
	virtual bool isEndPoint(const GEOL_Entity *theEntity) = 0;
	virtual double length() const = 0;
	virtual double area() const = 0;
	virtual void direction(GEOL_Point* theDir, const GEOL_Point* thePoint) const = 0;
	virtual void direction(GEOL_Point* theDir, double theXCoord, double theYCoord) const = 0;
	
	virtual void translate(double theDX, double theDY) = 0;

protected:
	/*!
	Pointers to the first entity component (x coordinate for GEOL_Point)
	*/
	double mBegin;

	/*!
	Pointers to the second entity component (y coordinate for GEOL_Point)
	*/
	double mEnd;
};


#endif
