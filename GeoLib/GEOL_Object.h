/***************************************************************************
                                 GEOL_Object.h
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


#ifndef GEOL_OBJECT_H
#define GEOL_OBJECT_H

#include "GEOL_Attribute.h"
#include "GEOL_Persistency.h"

class GEOL_Context;

/*!
This abstract class is the abstraction of a geometrical object, at this level all abjects within a context has the
same properties and functionalities, all the aspects common to all objects is managed within this class.
*/
class GEOL_Object : public GEOL_Persistency {
public:
	GEOL_Object();
	virtual ~GEOL_Object() = 0;
	
	GEOL_Context* getContext();
	void setContext(GEOL_Context *theContext);
	
	virtual bool notifyDestruction(GEOL_Object *theObject) = 0;
	
	bool addAttribute(GEOL_AttributeValue theAttrValue, GEOL_AttributeType theAttrType, char *theAttrID);
	bool addAttribute(GEOL_Attribute *theAttr);
	bool removeAttribute(char *theAttrID);
	bool removeAttribute(GEOL_Attribute *theAttr);
	void removeAllAttributes();
	
	GEOL_Attribute *getFirstAttribute();
	GEOL_Attribute *getLastAttribute();
	GEOL_Attribute *getNextAttribute(GEOL_Attribute *theAttr);
	GEOL_Attribute *getPrevAttribute(GEOL_Attribute *theAttr);
	
	int getAttributesNum();
	
	GEOL_Attribute *getAttributeFromID(char *theAttrID);
	
	unsigned char getRefCount();
	bool incRefCount();
	bool decRefCount();

protected:
	bool saveBinaryObjectInfo(std::ofstream *theStream, GEOL_ObjectType theObjectType);

private:
	/*!
	Pointer to the context that owns the object
	*/
	GEOL_Context *mContext;
	
	/*!
	Object reference counter
	*/
	unsigned char mRefCount;
	
	/*!
	List of attributes
	*/
	list<GEOL_Attribute*> pAttributeList;
	
	/*!
	Attributes iterator
	*/
	list<GEOL_Attribute*>::iterator attributeIt;	
};


/*!
\return
The pointer to the context
*/
inline GEOL_Context* GEOL_Object::getContext() {
	return mContext;
}


/*!
Set the pointer to the context

\param theContext
Pointer to the context
*/
inline void GEOL_Object::setContext(GEOL_Context *theContext) {
	mContext = theContext;
}


/*!
\return
The first attribute of the object
*/
inline GEOL_Attribute* GEOL_Object::getFirstAttribute() {
	if (pAttributeList.size() == 0) {
		return NULL;
	}
	else {
		return pAttributeList.front();
	}
}

/*!
\return
Number of attributes of the object
*/
inline int GEOL_Object::getAttributesNum() {
	return pAttributeList.size();
}

/*!
\return
The last attribute of the object
*/
inline GEOL_Attribute* GEOL_Object::getLastAttribute() {
	if (pAttributeList.size() == 0) {
		return NULL;
	}
	else {
		return pAttributeList.back();
	}
}


/*!
Return the reference counter of the object
*/
inline unsigned char GEOL_Object::getRefCount() {
	return mRefCount;
}


#endif