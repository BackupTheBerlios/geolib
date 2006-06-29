/***************************************************************************
                                 GEOL_Container.h
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

#ifndef GEOL_CONTAINER_H
#define GEOL_CONTAINER_H

#include "GEOL_Object.h"

class GEOL_Entity;

/*!
Abstract class for geometrical containers, primitive geometrical objects are grouped together to form up other
objects such as polygons, this class manage the relationship between containers and contained objects that can
be entities or other containers.
*/
class GEOL_Container : public GEOL_Object {
friend class TestBasicIntegrity;
public:
	GEOL_Container();
	virtual ~GEOL_Container() = 0;
	
	int getNumOfEntities() const;
	int getNumOfContainers() const;
	int getNumOfObjects() const;
	
	GEOL_Entity* getFirstEntity();
	GEOL_Entity* getNextEntity(const GEOL_Entity *theEntity);
	GEOL_Entity* getPrevEntity(const GEOL_Entity *theEntity);
	GEOL_Entity* getLastEntity();
	
	
	GEOL_Container* getFirstContainer();
	GEOL_Container* getNextContainer(const GEOL_Container *theContainer);
	GEOL_Container* getPrevContainer(const GEOL_Container *theContainer);
	GEOL_Container* getLastContainer();
	
	bool isContained(const GEOL_Entity *theEntity) const;
	bool isContained(const GEOL_Container *theContainer) const;
	
	virtual bool notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag) = 0;

protected:
	bool addEntity(GEOL_Entity *theNewEntity);
	bool removeEntity(GEOL_Entity *theEntity);
	bool detachEntity(GEOL_Entity *theEntity);
	void removeAllEntities();

	bool addContainer(GEOL_Container *theNewContainer);
	bool removeContainer(GEOL_Container *theContainer);
	bool detachContainer(GEOL_Container *theContainer);
	void removeAllContainers();

	bool checkForContainmentCycles();

	/*!
	List of entities contained
	*/
	list<GEOL_Entity*> pEntityList;
	
	/*!
	Lis of containers contained
	*/
	list<GEOL_Container*> pContainerList;
	
	/*!
	Entities iterator
	*/
	list<GEOL_Entity*>::iterator mEntityIt;
	
	/*!
	Containers itertor
	*/
	list<GEOL_Container*>::iterator mContainerIt;
};


/*!
\return
The first entity contained
*/
inline GEOL_Entity* GEOL_Container::getFirstEntity() {
	if (pEntityList.size() == 0) {
		return NULL;
	}
	else {
		mEntityIt = pEntityList.begin();
		return pEntityList.front();
	}
}


/*!
\return
The last entity contained
*/
inline GEOL_Entity* GEOL_Container::getLastEntity() {
	if (pEntityList.size() == 0) {
		return NULL;
	}
	else {
		mEntityIt = pEntityList.end();
		return pEntityList.back();
	}
}


/*!
\return
The first container contained
*/
inline GEOL_Container* GEOL_Container::getFirstContainer() {
	if (pContainerList.size() == 0) {
		return NULL;
	}
	else {
		mContainerIt = pContainerList.begin();
		return pContainerList.front();
	}
}


/*!
\return
The last container contained
*/
inline GEOL_Container* GEOL_Container::getLastContainer() {
	if (pContainerList.size() == 0) {
		return NULL;
	}
	else {
		mContainerIt = pContainerList.end();
		return pContainerList.back();
	}
}



#endif
