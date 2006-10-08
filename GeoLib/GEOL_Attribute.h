/***************************************************************************
							  GEOL_Attribute.h
                             -------------------
    begin                : 23.09.2004
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

#ifndef GEOL_ATTRIBUTE_H
#define GEOL_ATTRIBUTE_H

#include "GEOL_Persistency.h"

class GEOL_Entity;
class GEOL_Container;

/*!
Types of an attribute
*/
typedef enum { GEOL_AttrInt, GEOL_AttrDouble, GEOL_AttrString, GEOL_AttrEntity, GEOL_AttrContainer, GEOL_AttrVoid } GEOL_AttributeType;

union GEOL_AttributeValue {
	int GEOL_AttrIntValue;
	double GEOL_AttrDoubleValue;
	char *GEOL_AttrStringValue;
	GEOL_Entity *GEOL_AttrEntityValue;
	GEOL_Container *GEOL_AttrContainerValue;
	void *GEOL_AttrVoidValue;
};

/*!
Attribute of an object, every object can have a list of attributes that characterize objects with specific
auxiliary informations not belonging to the object structure, such as markers 
*/
class GEOL_Attribute : public GEOL_Persistency {
public:

	GEOL_Attribute();
	GEOL_Attribute(GEOL_AttributeValue theAttrValue, GEOL_AttributeType theAttrType, const char *theAttrID);
	~GEOL_Attribute();

	GEOL_AttributeValue getValue();
	void setValue(GEOL_AttributeValue theValue);

	GEOL_AttributeType getType();
	void setType(GEOL_AttributeType theType);

	void getID(char *theID) const;
	void setID(char *theID);
	
	bool isEqualID(const GEOL_Attribute *theAttribute);
	bool isEqualID(const char *theAttributeId);
	
	virtual bool LoadBinary(ifstream *theStream);
	virtual bool SaveBinary(ofstream *theStream);
	virtual bool LoadISO(ifstream *theStream);

private:

	/*!
	Value of the attribute, can be a number, a pointer to a geometrical entity, to a string or to any other
	kind of structure
	*/
	GEOL_AttributeValue mValue;
	
	/*!
	The type of the attribute, an attribute can belong to a list of "standard" types or can be a custom object
	*/
	GEOL_AttributeType mType;
	
	/*!
	Id of the attribute, used to distinguish between many attributes of the same type, must be unique
	*/
	char mAttrID[5];

};


/*!
\return
The attribute value
*/
inline GEOL_AttributeValue GEOL_Attribute::getValue() {
	return mValue;
}


/*!
Set the attribute value

\param theAttribute
New attribute value
*/
inline void GEOL_Attribute::setValue(GEOL_AttributeValue theValue) {
	mValue = theValue;
}

/*!
\return
The attribute type
*/
inline GEOL_AttributeType GEOL_Attribute::getType() {
	return mType;
}

/*!
Set the attribute type

\param theType
New attribute type
*/
inline void GEOL_Attribute::setType(GEOL_AttributeType theType) {
	mType = theType;
}


#endif
