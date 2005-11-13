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

#include "GEOL_Container.h"


GEOL_Container::GEOL_Container() {
	entityIt = pEntityList.begin();
	containerIt = pContainerList.begin();
}


GEOL_Container::~GEOL_Container() {
	removeAllEntities();
	removeAllContainers();
}


/*!
Add a new entity to the entities list

\param theNewEntity
Entity to add

\return
- true if the new entity has been added correctly
- false if theNewEntity is NULL
*/
bool GEOL_Container::addEntity(GEOL_Entity *theNewEntity) {
	if (!theNewEntity)
		return false;
	
	pEntityList.push_back(theNewEntity);
	return true;
}




/*!
Remove an entity from the entities list, the entity is NOT destroyed but simply removed from the list

\param theEntity
Entity to remove

\return
- true if the new entity has been removed correctly
- false if theEntity is NULL or is not in the list
*/
bool GEOL_Container::removeEntity(GEOL_Entity *theEntity) {
	if (!theEntity)
		return false;

	if (*entityIt != theEntity) {
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && *entityIt != theEntity ; entityIt++) {}
	}

	if (entityIt == pEntityList.end()) {
		return false;
	}
	else {
		GEOL_Entity *toDel = *entityIt;
		entityIt++;		
		pEntityList.remove(toDel);
	}
	
	return true;
}



/*!
Remove all entities from the container,  the entities are NOT destroyed but simply removed from the list
*/
void GEOL_Container::removeAllEntities() {
	pEntityList.clear();
	/*for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() ; ) {
		GEOL_Entity *toDel = *entityIt;	
		entityIt++;
		pEntityList.remove(toDel);
	}*/
}



/*!
Get the next of a given entitiy in the list

\param theEntity
Entity from wich get the next

\return
The next entity of theEntity in the list, or NULL if theEntity is NULL, is the last entity in the list, or it is not
in the list
*/
GEOL_Entity* GEOL_Container::getNextEntity(const GEOL_Entity *theEntity) {
	if (*entityIt != theEntity) {
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && *entityIt != theEntity ; entityIt++) {}
	}

	GEOL_Entity *ret = NULL;

	if (entityIt != pEntityList.end()) {
		entityIt++;
		if (entityIt != pEntityList.end()) {
			ret = *entityIt;
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
GEOL_Entity* GEOL_Container::getPrevEntity(const GEOL_Entity *theEntity) {
	if (*entityIt != theEntity) {
		for (entityIt = pEntityList.begin() ; entityIt != pEntityList.end() && *entityIt != theEntity ; entityIt++) {}
	}

	GEOL_Entity *ret = NULL;

	if (entityIt != pEntityList.end()) {
		if (entityIt != pEntityList.begin()) {
			entityIt--;
			ret = *entityIt;
		}		
	}

	return ret;
}




/*!
Add a new container to the containers list

\param theNewContainer
Container to add

\return
- true if the new container is added correctly
- false if theNewContainer is NULL
*/
bool GEOL_Container::addContainer(GEOL_Container *theNewContainer) {
	if (!theNewContainer)
		return false;
	
	pContainerList.push_back(theNewContainer);
	if (checkForContainmentCycles()) {
		pContainerList.remove(theNewContainer);
		return false;
	}
	
	return true;
}



/*!
Remove a container from the containers list, the container is NOT destroyed but simply removed from the list

\param theContainer
Container to remove

\return
- true if the new container is removed correctly
- false if theContainer is NULL or is not in the list
*/
bool GEOL_Container::removeContainer(GEOL_Container *theContainer) {
	if (!theContainer)
		return false;

	if (*containerIt != theContainer) {
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && *containerIt != theContainer ; containerIt++) {}
	}

	if (containerIt == pContainerList.end()) {
		return false;
	}
	else {
		GEOL_Container *toDel = *containerIt;
		containerIt++;
		pContainerList.remove(toDel);
	}
	
	return true;
}



/*!
Remove all container from the container, the containers are NOT destroyed but simply removed from the list
*/
void GEOL_Container::removeAllContainers() {
	pContainerList.clear();
	/*for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() ; ) {
		GEOL_Container *toDel = *containerIt;		
		containerIt++;
		pContainerList.remove(toDel);
	}*/
}


/*!
Get the next of a given container in the list

\param theContainer
Container from wich get the next

\return
The next container of theContainer in the list, or NULL if theContainer is NULL, is the last container in the list, or it is not
in the list
*/
GEOL_Container* GEOL_Container::getNextContainer(const GEOL_Container *theContainer) {
	if (*containerIt != theContainer) {
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && *containerIt != theContainer ; containerIt++) {}
	}

	GEOL_Container *ret = NULL;

	if (containerIt != pContainerList.end()) {
		containerIt++;
		if (containerIt != pContainerList.end()) {
			ret = *containerIt;
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
GEOL_Container* GEOL_Container::getPrevContainer(const GEOL_Container *theContainer) {
	if (*containerIt != theContainer) {
		for (containerIt = pContainerList.begin() ; containerIt != pContainerList.end() && *containerIt != theContainer ; containerIt++) {}
	}

	GEOL_Container *ret = NULL;

	if (containerIt != pContainerList.end()) {
		if (containerIt != pContainerList.begin()) {
			containerIt--;
			ret = *containerIt;
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
		
		GEOL_Container *container = markedCont -> getFirstContainer();
		for ( ; container && !cycleFounded ; ) {
			
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
int GEOL_Container::getNumOfEntities() {
	return pEntityList.size();
}


/*!
\return
The number of other containers in the container
*/
int GEOL_Container::getNumOfContainers() {
	return pContainerList.size();
}


/*!
\return
The total number of objects in the container
*/
int GEOL_Container::getNumOfObjects() {
	return pEntityList.size() + pContainerList.size();
}

