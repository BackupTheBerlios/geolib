/***************************************************************************
								GEOL_Profile.h
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

#ifndef GEOL_PROFILE_H
#define GEOL_PROFILE_H


#include "GEOL_Container.h"

/*!
A simple container constituted only with primitive objects, in a profile arcs and segments is placed side by side to form an
open strip or a closed shape.
*/
class GEOL_Profile : public GEOL_Container {
friend class GEOL_Context;
public:
	bool addEdgeTail(GEOL_Entity *theNewEntity);
	bool addEdgeFront(GEOL_Entity *theNewEntity);
	
	bool removeEdge(GEOL_Entity *theEntity, bool theReshapeFlag);
	bool detachEdge(GEOL_Entity *theEntity, bool theReshapeFlag);
	void removeAllEdges();
	
	bool isClosed() const;
	double length() const;
	double area() const;

	bool notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag);

	GEOL_BBox getBBox();

protected:
	GEOL_Profile();
	~GEOL_Profile();
	
	bool releaseEdgeAndReshape(GEOL_Entity *theEntity, bool theReshapeFlag, bool theRemoveFlag);

	virtual bool LoadBinary(ifstream *theStream);
	virtual bool SaveBinary(ofstream *theStream);
	virtual bool LoadISO(ifstream *theStream);
};



#endif
