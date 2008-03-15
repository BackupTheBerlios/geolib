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
friend class TestBasicIntegrity;		// Required for testing class
public:
	GEOL_Container();
	virtual ~GEOL_Container();
	
	int getNumOfEntities() const;
	int getNumOfContainers() const;
	int getNumOfObjects() const;
	
	GEOL_Entity* getFirstEntity() const;
	GEOL_Entity* getNextEntity(const GEOL_Entity *theEntity) const;
	GEOL_Entity* getPrevEntity(const GEOL_Entity *theEntity) const;
	GEOL_Entity* getLastEntity() const;
	
	
	GEOL_Container* getFirstContainer() const;
	GEOL_Container* getNextContainer(const GEOL_Container *theContainer) const;
	GEOL_Container* getPrevContainer(const GEOL_Container *theContainer) const;
	GEOL_Container* getLastContainer() const;
	
	bool isContained(const GEOL_Entity *theEntity) const;
	bool isContained(const GEOL_Container *theContainer) const;
	
	bool notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag);

	void setEntityIterator(std::list<GEOL_Entity*>::const_iterator &theIterator) const;
	bool isEndOfEntities(std::list<GEOL_Entity*>::const_iterator &theIterator) const;
	void setContainerIterator(std::list<GEOL_Container*>::const_iterator &theIterator) const;
	bool isEndOfContainers(std::list<GEOL_Container*>::const_iterator &theIterator) const;
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
	List of containers contained
	*/
	list<GEOL_Container*> pContainerList;
};


/*!
\return
The first entity contained
*/
inline GEOL_Entity* GEOL_Container::getFirstEntity() const {
	if (pEntityList.size() == 0) {
		return NULL;
	}
	else {
		return pEntityList.front();
	}
}


/*!
\return
The last entity contained
*/
inline GEOL_Entity* GEOL_Container::getLastEntity() const {
	if (pEntityList.size() == 0) {
		return NULL;
	}
	else {
		return pEntityList.back();
	}
}


/*!
\return
The first container contained
*/
inline GEOL_Container* GEOL_Container::getFirstContainer() const {
	if (pContainerList.size() == 0) {
		return NULL;
	}
	else {
		return pContainerList.front();
	}
}


/*!
\return
The last container contained
*/
inline GEOL_Container* GEOL_Container::getLastContainer() const {
	if (pContainerList.size() == 0) {
		return NULL;
	}
	else {
		return pContainerList.back();
	}
}



#endif
