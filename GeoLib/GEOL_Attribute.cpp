/***************************************************************************
                                GEOL_Attribute.cpp
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

#include "GEOL_Prefix.h"

#include "GEOL_Attribute.h"



/*!
Default constructor
*/
GEOL_Attribute::GEOL_Attribute() {
	pValue = NULL;
	mType = GEOL_AttrVoid;
	mAttrID[0] = 0;
	mAttrID[1] = 0;
	mAttrID[2] = 0;
	mAttrID[3] = 0;
}

/*!
Constructor with all members passed
*/
GEOL_Attribute::GEOL_Attribute(void *theAttrValue, GEOL_AttributeType theAttrType, char *theAttrID) {
	pValue = theAttrValue;
	mType = theAttrType;
	if (theAttrID) {
		mAttrID[0] = theAttrID[0];	
		mAttrID[1] = theAttrID[1];	
		mAttrID[2] = theAttrID[2];	
		mAttrID[3] = theAttrID[3];
	}
	else {
		mAttrID[0] = 0;	
		mAttrID[1] = 0;	
		mAttrID[2] = 0;	
		mAttrID[3] = 0;
	}
}

/*!
Default destructor
*/
GEOL_Attribute::~GEOL_Attribute() {
	pValue = NULL;
	mType = GEOL_AttrVoid;
	mAttrID[0] = 0;
}


/*!
Return the attribute id

\param theAttributeID
On output contains the attribute id
*/
void GEOL_Attribute::getID(char *theID) const {
	if (theID) {
		theID[0] = mAttrID[0];
		theID[1] = mAttrID[1];
		theID[2] = mAttrID[2];
		theID[3] = mAttrID[3];
	}
}


/*!
Set the attribute id

\param theAttributeID
New attribute id
*/
void GEOL_Attribute::setID(char *theID) {
	if (theID) {
		mAttrID[0] = theID[0];
		mAttrID[1] = theID[1];
		mAttrID[2] = theID[2];
		mAttrID[3] = theID[3];
	}
}


/*!
Check the equality of attributes id

\param theAttribute
Attribute to check with

\return
- true if theAttribute and this have the same id
- false otherwise 
*/
bool GEOL_Attribute::isEqualID(const GEOL_Attribute *theAttribute) {
	if (!theAttribute)
		return false;
		
	char attrId[4];
	theAttribute -> getID(attrId);
	if (attrId[0] == mAttrID[0] &&
		attrId[1] == mAttrID[1] &&
		attrId[2] == mAttrID[2] &&
		attrId[3] == mAttrID[3]) {
		
		return true;
	}
	else {
		return false;
	}
}


/*!
Check the attribute id

\param theAttributeId
id to check with

\return
- true if theAttributeId is equal to the id of this
- false otherwise 
*/
bool GEOL_Attribute::isEqualID(const char *theAttributeId) {
	if (!theAttributeId)
		return false;

	if (theAttributeId[0] == mAttrID[0] &&
		theAttributeId[1] == mAttrID[1] &&
		theAttributeId[2] == mAttrID[2] &&
		theAttributeId[3] == mAttrID[3]) {
		
		return true;
	}
	else {
		return false;
	}
}



