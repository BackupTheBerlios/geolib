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

/*!
Types of an attribute
*/
typedef enum { GEOL_AttrInt, GEOL_AttrDouble, GEOL_AttrString, GEOL_AttrEntity, GEOL_AttrContainer, GEOL_AttrVoid } GEOL_AttributeType;

/*!
Attribute of an object, every object can have a list of attributes that characterize objects with specific
auxiliary informations not belonging to the object structure, such as markers 
*/
class GEOL_Attribute {
public:

	GEOL_Attribute();
	GEOL_Attribute(void *theAttrValue, GEOL_AttributeType theAttrType, char *theAttrID);
	~GEOL_Attribute();

	void* getValue();
	void setValue(void *theValue);

	GEOL_AttributeType getType();
	void setType(GEOL_AttributeType theType);

	void getID(char *theID) const;
	void setID(char *theID);
	
	bool isEqualID(const GEOL_Attribute *theAttribute);
	bool isEqualID(const char *theAttributeId);
	
private:

	/*!
	Value of the attribute, can be a number, a pointer to a geometrical entity, to a string or to any other
	kind of structure
	*/
	void *pValue;
	
	/*!
	The type of the attribute, an attribute can belong to a list of "standard" types or can be a custom object
	*/
	GEOL_AttributeType mType;
	
	/*!
	Id of the attribute, used to distinguish between many attributes of the same type, must be unique
	*/
	char mAttrID[4];

};


/*!
\return
The attribute value
*/
inline void* GEOL_Attribute::getValue() {
	return pValue;
}


/*!
Set the attribute value

\param theAttribute
New attribute value
*/
inline void GEOL_Attribute::setValue(void *theValue) {
	pValue = theValue;
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
