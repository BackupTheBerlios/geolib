/***************************************************************************
							  GEOL_Container.cpp
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
#include "GEOL_Container.h"


GEOL_Container::GEOL_Container() {
	//mEntityIt = pEntityList.begin();
	//mContainerIt = pContainerList.begin();
}


GEOL_Container::~GEOL_Container() {
	removeAllEntities();
	removeAllContainers();
}


/*!
Add a new entity to the entities list, and increment the reference counter of the entity added.
If the entity is already contained don't add and return false

\param theNewEntity
Entity to add

\return
- true if the new entity has been added correctly
- false if theNewEntity is NULL
*/
bool GEOL_Container::addEntity(GEOL_Entity *theNewEntity) {
	if (!theNewEntity)
		return false;
	if (isContained(theNewEntity))
		return false;
	
	pEntityList.push_back(theNewEntity);
	((GEOL_Object*)theNewEntity) -> incRefCount();
	return true;
}




/*!
Remove an entity from the entities list, the entity is removed from the list, its reference counter is
decremented, and if the counter is zero the entity will be deleted

\param theEntity
Entity to remove

\return
- true if the new entity has been removed correctly
- false if theEntity is NULL or is not in the list
*/
bool GEOL_Container::removeEntity(GEOL_Entity *theEntity) {
	bool ret = false;
	
	if (theEntity) {
		list<GEOL_Entity*>::iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && *entityIt != theEntity ; entityIt++) {}
		if (entityIt == pEntityList.end()) {
			ret = false;
		}
		else {
			GEOL_Entity *toDel = *entityIt;
			pEntityList.remove(toDel);
			if (!((GEOL_Object*)toDel) -> decRefCount()) {
				ret = getContext() -> deleteObject((GEOL_Object*)toDel, true);
			}
		}
	}

	return ret;

/*
	if (!theEntity)
		return false;

	if (*mEntityIt != theEntity) {
		for (mEntityIt = pEntityList.begin() ; mEntityIt != pEntityList.end() && *mEntityIt != theEntity ; mEntityIt++) {}
	}

	if (mEntityIt == pEntityList.end()) {
		return false;
	}
	else {
		GEOL_Entity *toDel = *mEntityIt;
		pEntityList.remove(toDel);
		if (!((GEOL_Object*)toDel) -> decRefCount()) {
			if (!getContext() -> deleteObject((GEOL_Object*)toDel, true)) {
				return false;
			}
		}
	}
	return true;*/
}


/*!
Just detach an entity from its container

\param theEntity
Entity to detach

\return
- true if the new entity has been detached correctly
- false if theEntity is NULL or is not in the list
*/
bool GEOL_Container::detachEntity(GEOL_Entity *theEntity) {
	bool ret = false;
	
	if (theEntity) {	
		list<GEOL_Entity*>::iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && *entityIt != theEntity ; entityIt++) {}
		if (entityIt == pEntityList.end()) {
			ret = false;
		}
		else {
			GEOL_Entity *toDel = *entityIt;
			((GEOL_Object*)toDel) -> decRefCount();
			pEntityList.remove(toDel);
			ret = true;
		}
	}
	
	return ret;

/*
	if (!theEntity)
		return false;

	if (*mEntityIt != theEntity) {
		for (mEntityIt = pEntityList.begin() ; mEntityIt != pEntityList.end() && *mEntityIt != theEntity ; mEntityIt++) {}
	}

	if (mEntityIt == pEntityList.end()) {
		return false;
	}
	else {
		GEOL_Entity *toDel = *mEntityIt;
		((GEOL_Object*)toDel) -> decRefCount();
		pEntityList.remove(toDel);
	}
	return true; */
}


/*!
Remove all entities from the container,  the entities are NOT destroyed but simply removed from the list, destruction
occours only if its reference counter is 0
*/
void GEOL_Container::removeAllEntities() {
	for ( ; !pEntityList.empty() ; ) {
		GEOL_Entity *toDel = pEntityList.front();

		if (!((GEOL_Object*)toDel) -> decRefCount()) {
			getContext() -> deleteObject((GEOL_Object*)toDel, true);
		}

		if (!pEntityList.empty()) {
			pEntityList.pop_front();
		}
	}
	
	pEntityList.clear();
}



/*!
Get the next of a given entitiy in the list

\param theEntity
Entity from wich get the next

\return
The next entity of theEntity in the list, or NULL if theEntity is NULL, is the last entity in the list, or it is not
in the list
*/
GEOL_Entity* GEOL_Container::getNextEntity(const GEOL_Entity *theEntity) const {
	GEOL_Entity *ret = NULL;

	if (theEntity) {
		list<GEOL_Entity*>::const_iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && *entityIt != theEntity ; entityIt++) {}
		if (entityIt != pEntityList.end()) {
			entityIt++;
			if (entityIt != pEntityList.end()) {
				ret = *entityIt;
			}
		}
	}
	
	return ret;
}



/*!
Get the previous of a given entitiy in the list

\param theEntity
Entity from wich get the previous

\return
The previous entity of theEntity in the list, or NULL if theEntity is NULL, is the first entity in the list, or it is not
in the list
*/
GEOL_Entity* GEOL_Container::getPrevEntity(const GEOL_Entity *theEntity) const {
	GEOL_Entity *ret = NULL;

	if (theEntity) {
		list<GEOL_Entity*>::const_iterator entityIt;
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && *entityIt != theEntity ; entityIt++) {}
		if (entityIt != pEntityList.end()) {
			if (entityIt != pEntityList.begin()) {
				entityIt--;
				ret = *entityIt;
			}		
		}
	}
	
	return ret;
}




/*!
Add a new container to the containers list, and increment the reference counter of the container added.
If the container ir already contained or originates a cycle don't add and return false

\param theNewContainer
Container to add

\return
- true if the new container is added correctly
- false if theNewContainer is NULL
*/
bool GEOL_Container::addContainer(GEOL_Container *theNewContainer) {
	if (!theNewContainer)
		return false;
	if (isContained(theNewContainer))
		return false;
	
	pContainerList.push_back(theNewContainer);
	if (checkForContainmentCycles()) {
		pContainerList.remove(theNewContainer);
		return false;
	}
	else {
		((GEOL_Object*)theNewContainer) -> incRefCount();
	}
	
	return true;
}



/*!
Remove a container from the containers list, the container is removed from the list, its reference counter is
decremented, and if the counter is zero the container will be deleted

\param theContainer
Container to remove

\return
- true if the new container is removed correctly
- false if theContainer is NULL or is not in the list
*/
bool GEOL_Container::removeContainer(GEOL_Container *theContainer) {
	bool ret = false;

	if (theContainer) {
		list<GEOL_Container*>::iterator containerIt;
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && *containerIt != theContainer ; containerIt++) {}
		if (containerIt == pContainerList.end()) {
			ret = false;
		}
		else {
			GEOL_Container *toDel = *containerIt;
			pContainerList.remove(toDel);
			if (!((GEOL_Object*)toDel) -> decRefCount()) {
				ret = getContext() -> deleteObject((GEOL_Object*)toDel, true);
			}
		}
	}
	
	return ret;
/*	if (!theContainer)
		return false;

	if (*mContainerIt != theContainer) {
		for (mContainerIt = pContainerList.begin() ; mContainerIt != pContainerList.end() && *mContainerIt != theContainer ; mContainerIt++) {}
	}

	if (mContainerIt == pContainerList.end()) {
		return false;
	}
	else {
		GEOL_Container *toDel = *mContainerIt;
		pContainerList.remove(toDel);
		if (!((GEOL_Object*)toDel) -> decRefCount()) {
			if (!getContext() -> deleteObject((GEOL_Object*)toDel, true)) {
				return false;
			}
		}
	}
	
	return true;*/
}



/*!
Just detach a container from its container

\param theContainer
Container to detach

\return
- true if the new container is removed correctly
- false if theContainer is NULL or is not in the list
*/
bool GEOL_Container::detachContainer(GEOL_Container *theContainer) {
	bool ret = false;
	
	if (theContainer) {
		list<GEOL_Container*>::iterator containerIt;
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && *containerIt != theContainer ; containerIt++) {}

		if (containerIt == pContainerList.end()) {
			ret = false;
		}
		else {
			GEOL_Container *toDel = *containerIt;
			((GEOL_Object*)toDel) -> decRefCount();
			pContainerList.remove(toDel);
			ret = true;
		}
	}
	
	return ret;
/*	if (!theContainer)
		return false;
	
	if (*mContainerIt != theContainer) {
		for (mContainerIt = pContainerList.begin() ; mContainerIt != pContainerList.end() && *mContainerIt != theContainer ; mContainerIt++) {}
	}

	if (mContainerIt == pContainerList.end()) {
		return false;
	}
	else {
		GEOL_Container *toDel = *mContainerIt;
		((GEOL_Object*)toDel) -> decRefCount();
		pContainerList.remove(toDel);
	}
	
	return true;*/
}


/*!
Remove all container from the container, the containers are NOT destroyed but simply removed from the list
*/
void GEOL_Container::removeAllContainers() {
	for ( ; !pContainerList.empty() ; ) {
		GEOL_Container *toDel = pContainerList.front();

		if (!((GEOL_Object*)toDel) -> decRefCount()) {
			getContext() -> deleteObject((GEOL_Object*)toDel, true);
		}

		if (!pContainerList.empty()) {
			pContainerList.pop_front();
		}
	}
	
	pContainerList.clear();
}


/*!
Get the next of a given container in the list

\param theContainer
Container from wich get the next

\return
The next container of theContainer in the list, or NULL if theContainer is NULL, is the last container in the list, or it is not
in the list
*/
GEOL_Container* GEOL_Container::getNextContainer(const GEOL_Container *theContainer) const {
	GEOL_Container *ret = NULL;
	
	if (theContainer) {
		list<GEOL_Container*>::const_iterator containerIt;
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && *containerIt != theContainer ; containerIt++) {}
		if (containerIt != pContainerList.end()) {
			containerIt++;
			if (containerIt != pContainerList.end()) {
				ret = *containerIt;
			}
		}
	}
	
	return ret;
}



/*!
Get the previous of a given container in the list

\param theContainer
Container from wich get the previous

\return
The previous container of theContainer in the list, or NULL if theContainer is NULL, is the first container in the list, or it is not
in the list
*/
GEOL_Container* GEOL_Container::getPrevContainer(const GEOL_Container *theContainer) const {
	GEOL_Container *ret = NULL;

	if (theContainer) {
		list<GEOL_Container*>::const_iterator containerIt;
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && *containerIt != theContainer ; containerIt++) {}
		if (containerIt != pContainerList.end()) {
			if (containerIt != pContainerList.begin()) {
				containerIt--;
				ret = *containerIt;
			}		
		}
	}
	
	return ret;
}


/*!
\param theEntity
The entity to search for into the entities list

\return
- true is theEntity is contained in the entities list
- false otherwise
*/
bool GEOL_Container::isContained(const GEOL_Entity *theEntity) const {
	bool ret = false;
	
	list<GEOL_Entity*>::const_iterator it;
	for (it = pEntityList.begin() ; it != pEntityList.end() && !ret ; it++) {
		if ((*it) == theEntity)
			ret = true;
	}

	return ret;
}


/*!
Check if a container is contained in this

\param theContainer
Container to check

\return
- true if the theContainer is contained within this
- false otherwise
*/
bool GEOL_Container::isContained(const GEOL_Container *theContainer) const {
	bool ret = false;
	
	list<GEOL_Container*>::const_iterator it;
	for (it = pContainerList.begin() ; it != pContainerList.end() && !ret ; it++) {
		if ((*it) == theContainer)
			ret = true;
	}

	return ret;
}


/*!
Check for any cycle in the containment graph with this container as a part of it

\return
- true if this container belongs to a cycle
- false otherwise
*/
bool GEOL_Container::checkForContainmentCycles() {
	list<GEOL_Container*> markedContainers;	
	
	list<GEOL_Container*>::const_iterator it;
	for (it = pContainerList.begin() ; it != pContainerList.end() ; it++) {
		markedContainers.push_back(*it);
	}
	
	bool cycleFounded = false;
	for (it = markedContainers.begin() ; it != markedContainers.end() && !cycleFounded ; it++) {
		GEOL_Container *markedCont = *it;
		
		std::list<GEOL_Container*>::const_iterator containerIt;
		markedCont -> setContainerIterator(containerIt);
		for ( ; !markedCont -> isEndOfContainers(containerIt) && !cycleFounded ; containerIt++) {
			GEOL_Container *container = *containerIt;
					
			list<GEOL_Container*>::const_iterator markedIt;
			for (it = markedContainers.begin() ; it != markedContainers.end() && !cycleFounded ; it++) {
				if (*it == container) {
					cycleFounded = true;
				}
			}
			
			if (!cycleFounded) {
				markedContainers.push_back(container);
			}
			
			container = markedCont -> getNextContainer(container);
		}
		
	}
	
	return cycleFounded;
}




/*!
\return
The number of entities in the container
*/
int GEOL_Container::getNumOfEntities() const {
	return pEntityList.size();
}


/*!
\return
The number of other containers in the container
*/
int GEOL_Container::getNumOfContainers() const {
	return pContainerList.size();
}


/*!
\return
The total number of objects in the container
*/
int GEOL_Container::getNumOfObjects() const {
	return pEntityList.size() + pContainerList.size();
}


void GEOL_Container::setEntityIterator(std::list<GEOL_Entity*>::const_iterator &theIterator) const {
	theIterator = pEntityList.begin();
}


bool GEOL_Container::isEndOfEntities(std::list<GEOL_Entity*>::const_iterator &theIterator) const {
	if (theIterator == pEntityList.end())
		return true;
	else
		return false;
}


void GEOL_Container::setContainerIterator(std::list<GEOL_Container*>::const_iterator &theIterator) const {
	theIterator = pContainerList.begin();
}


bool GEOL_Container::isEndOfContainers(std::list<GEOL_Container*>::const_iterator &theIterator) const {
	if (theIterator == pContainerList.end())
		return true;
	else
		return false;
}

