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

/*!
This abstract class rapresents the primitive geometrical objects such as points, segments and arcs, contains two pointers
to other objects of this class, used to store the x,y coordinates for points
*/
class GEOL_Entity : public GEOL_Object {
public:
	GEOL_Entity();
	virtual ~GEOL_Entity() = 0;
	
	const GEOL_Entity* getBeginEntity() const;
	const GEOL_Entity* getEndEntity() const;

	void setBeginEntity(GEOL_Entity *theBegin);
	void setEndEntity(GEOL_Entity *theEnd);
	
	bool notifyDestruction(GEOL_Object *theObject);

protected:
	/*!
	Pointers to the first entity component (x coordinate for GEOL_Point)
	*/
	double mBegin;

	/*!
	Pointers to the second entity component (y coordinate for GEOL_Point)
	*/
	double mEnd;
	
private:
};






#endif
