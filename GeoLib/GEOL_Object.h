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

#include "GEOL_BBox.h"
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
	virtual ~GEOL_Object();
	
	GEOL_Context* getContext() const;
	void setContext(GEOL_Context *theContext);
	
	//************************
	//* Notification mechanism
	//************************
	virtual bool notifyDestruction(GEOL_Object *theObject, bool &theDestroyFlag) = 0;
	
	//*******************
	//* Object attributes
	//*******************
	bool addAttribute(GEOL_AttributeValue theAttrValue, GEOL_AttributeType theAttrType, int theAttrID);
	bool addAttribute(GEOL_Attribute *theAttr);
	bool removeAttribute(int theAttrID);
	bool removeAttribute(GEOL_Attribute *theAttr);
	void removeAllAttributes();
	GEOL_Attribute *getFirstAttribute();
	GEOL_Attribute *getLastAttribute();
	GEOL_Attribute *getNextAttribute(GEOL_Attribute *theAttr);
	GEOL_Attribute *getPrevAttribute(GEOL_Attribute *theAttr);
	int getAttributesNum();
	GEOL_Attribute *getAttributeFromID(int theAttrID);
	virtual GEOL_BBox getBBox() = 0;
	void setBBox(GEOL_BBox theBBox);
	
	//********************
	//* Reference counting
	//********************
	unsigned char getRefCount();
	bool incRefCount();
	bool decRefCount();
	void resetRefCount();
	
	//****************************
	//* Object type identification
	//****************************
	bool isPoint() const;
	bool isSegment() const;
	bool isArc() const;
	bool isProfile() const;
	bool isPoliProfile() const;
	bool isEntity() const;
	bool isContainer() const;

	//****************************
	//* Bounding Box
	//****************************
	void invalidateBBox();

protected:
	bool saveBinaryObjectInfo(ofstream *theStream);
	bool saveBinaryObjectAttributes(ofstream *theStream);
	bool laodBinaryObjectAttributes(ifstream *theStream);

	/*!
	Type of the object
	*/
	GEOL_ObjectType mObjType;

	/*!
	Pointer to the bounding box of the object
	*/
	GEOL_BBox *mBBox;
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
inline GEOL_Context* GEOL_Object::getContext() const {
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
\return
The reference counter of the object
*/
inline unsigned char GEOL_Object::getRefCount() {
	return mRefCount;
}


/*!
\return
- true if the object is a point
- false otherwise
*/
inline bool GEOL_Object::isPoint() const {
	return mObjType == geol_Point;
}


/*!
\return
- true if the object is a segment
- false otherwise
*/
inline bool GEOL_Object::isSegment() const {
	return mObjType == geol_Segment;
}


/*!
\return
- true if the object is a arc
- false otherwise
*/
inline bool GEOL_Object::isArc() const {
	return mObjType == geol_Arc;
}


/*!
\return
- true if the object is a profile
- false otherwise
*/
inline bool GEOL_Object::isProfile() const {
	return mObjType == geol_Profile;
}


/*!
\return
- true if the object is a poliprofile
- false otherwise
*/
inline bool GEOL_Object::isPoliProfile() const {
	return mObjType == geol_PoliProfile;
}

#endif