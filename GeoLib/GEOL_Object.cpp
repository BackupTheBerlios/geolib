/***************************************************************************
							    GEOL_Object.cpp
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

#include "GEOL_Object.h"



GEOL_Object::GEOL_Object() {
	mContext = NULL;
	mRefCount = 0;
}


GEOL_Object::~GEOL_Object() {
	mContext = NULL;
	mRefCount = 0;
}


/*!
Add a new attribute to the attribute list with specified values, only if there is no other attributes
with the same id

\param theAttrValue
Value of the new attribute
\param theAttrType
Type of the new attribute
\param theAttrID
Id of the new attribute

\return
- true if the new attribute is correctly added to the list
- false otherwise
*/
bool GEOL_Object::addAttribute(void *theAttrValue, GEOL_AttributeType theAttrType, char *theAttrID) {
	if (!theAttrID)
		return false;
	
	list<GEOL_Attribute*>::iterator it;
	
	bool idFounded = false;
	for (it = pAttributeList.begin() ; it != pAttributeList.end() && !idFounded ; it++) {
		GEOL_Attribute *attr = *it;
		
		if (attr -> isEqualID(theAttrID)) {
			idFounded = true;
		}
	}
	
	if (idFounded) {
		return false;
	}
	else {
		GEOL_Attribute *newAttr = new GEOL_Attribute(theAttrValue, theAttrType, theAttrID);
		pAttributeList.push_back(newAttr);
	
		return true;
	}
}


/*!
Add a new attribute to the attribute list with specified values, only if there is no other attributes
with the same id

\param theAttr
The attribute to add

\return
- true if the new attribute is correctly added to the list
- false otherwise
*/
bool GEOL_Object::addAttribute(GEOL_Attribute *theAttr) {
	if (!theAttr)
		return false;
	
	list<GEOL_Attribute*>::iterator it;
	
	bool idFounded = false;
	for (it = pAttributeList.begin() ; it != pAttributeList.end() && !idFounded ; it++) {
		GEOL_Attribute *attr = *it;
		
		if (attr -> isEqualID(theAttr)) {
			idFounded = true;
		}
	}
	
	if (idFounded) {
		return false;
	}
	else {
		pAttributeList.push_back(theAttr);
	
		return true;
	}
}


/*!
Remove the attribute with the specified id, if there is any, the attribute IS DESRTOYED attributes lives only
within a object

\param theAttrID
Id of the attribute to remove

\return
- true if the new attribute is correctly removed from the list
- false otherwise
*/
bool GEOL_Object::removeAttribute(char *theAttrID) {
	if (!theAttrID)
		return false;
		
	list<GEOL_Attribute*>::iterator it;
	
	GEOL_Attribute *attrFound = NULL;
	for (it = pAttributeList.begin() ; it != pAttributeList.end() && !attrFound ; it++) {
		GEOL_Attribute *attr = *it;
		
		if (attr -> isEqualID(theAttrID)) {
			attrFound = attr;
		}
	}

	if (attrFound) {
		pAttributeList.remove(attrFound);
		delete attrFound;
		
		return true;
	}
	else {
		return false;
	}
}



/*!
Remove the attribute specified, if there is any, the attribute IS DESRTOYED attributes lives only
within a object

\param theAttr
The attribute to remove

\return
- true if the new attribute is correctly removed from the list
- false otherwise
*/
bool GEOL_Object::removeAttribute(GEOL_Attribute *theAttr) {
	if (!theAttr)
		return false;
		
	list<GEOL_Attribute*>::iterator it;
	
	GEOL_Attribute *attrFound = NULL;
	for (it = pAttributeList.begin() ; it != pAttributeList.end() && !attrFound ; it++) {
		GEOL_Attribute *attr = *it;
		
		if (attr == theAttr) {
			attrFound = attr;
		}
	}

	if (attrFound) {
		pAttributeList.remove(attrFound);
		delete attrFound;
		
		return true;
	}
	else {
		return false;
	}
}


/*!
Remove and destroy all attributes
*/
void GEOL_Object::removeAllAttributes() {
	list<GEOL_Attribute*>::iterator it;
	for (it = pAttributeList.begin() ; it != pAttributeList.end() ; ) {
		GEOL_Attribute *toDel = *it;		
		it++;
		pAttributeList.remove(toDel);
	}
}



/*!
Get the next of a given attribute

\param theAttr
Attribute from wich get the next

\return
The next attribute of theAttr in the list, or NULL if theAttr is NULL, is the last attribute in the list, or it
is not in the list
*/
GEOL_Attribute* GEOL_Object::getNextAttribute(GEOL_Attribute *theAttr) {
	if (*attributeIt != theAttr) {
		for (attributeIt = pAttributeList.begin() ; attributeIt != pAttributeList.end() && *attributeIt != theAttr ; attributeIt++) {}
	}

	GEOL_Attribute *ret = NULL;

	if (attributeIt != pAttributeList.end()) {
		attributeIt++;
		if (attributeIt != pAttributeList.end()) {
			ret = *attributeIt;
		}
	}

	return ret;
}



/*!
Get the previous of a given attribute

\param theAttr
Attribute from wich get the previous

\return
The previous attribute of theAttr in the list, or NULL if theAttr is NULL, is the first attribute in the list,
or it is not in the list
*/
GEOL_Attribute* GEOL_Object::getPrevAttribute(GEOL_Attribute *theAttr) {
	if (*attributeIt != theAttr) {
		for (attributeIt = pAttributeList.begin() ; attributeIt != pAttributeList.end() && *attributeIt != theAttr ; attributeIt++) {}
	}

	GEOL_Attribute *ret = NULL;

	if (attributeIt != pAttributeList.end()) {
		if (attributeIt != pAttributeList.begin()) {
			attributeIt--;
			ret = *attributeIt;
		}		
	}

	return ret;
}


/*!
Retrive the attribute of this object with the id passed

\param theAttrID
The id of the attribute to search for

\return
- The attribute with the passed id if it exists
- NULL if there isn't any attribute with the passed id
*/
GEOL_Attribute* GEOL_Object::getAttributeFromID(char *theAttrID) {
	if (!theAttrID)
		return NULL;
	
	list<GEOL_Attribute*>::iterator it;
	
	GEOL_Attribute *retAttribute = NULL;
	for (it = pAttributeList.begin() ; it != pAttributeList.end() && !retAttribute ; it++) {
		GEOL_Attribute *attr = *it;
		
		if (attr -> isEqualID(theAttrID)) {
			retAttribute = attr;
		}
	}
	
	return retAttribute;
}


/*!
Increment the object reference counter

\return
- true if the reference counter is < GEOL_MAX_REF_COUNT
- false otherwise
*/
bool GEOL_Object::incRefCount() {
	bool ret = false;
	if (mRefCount < GEOL_MAX_REF_COUNT) {
		mRefCount++;
		ret = true;
	}
	
	return ret;
}


/*!
Decrement the object reference counter

\return
- true if the reference counter is > 0
- false otherwise
*/
bool GEOL_Object::decRefCount() {
	bool ret = false;
	if (mRefCount > 0) {
		mRefCount--;
		ret = true;
	}
	
	return ret;
}

