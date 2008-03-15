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
	mValue.GEOL_AttrVoidValue = NULL;
	mType = GEOL_AttrVoid;
	mAttrID = GEOL_ID_UNSET;
}


/*!
Constructor with all members
*/
GEOL_Attribute::GEOL_Attribute(GEOL_AttributeValue theAttrValue, GEOL_AttributeType theAttrType, int theAttrID) {
	mValue = theAttrValue;
	mType = theAttrType;
	mAttrID = theAttrID;	
}


/*!
Default destructor
*/
GEOL_Attribute::~GEOL_Attribute() {
	mValue.GEOL_AttrVoidValue = NULL;
	mType = GEOL_AttrVoid;
	mAttrID = GEOL_ID_UNSET;
}


/*!
\return
The attribute id
*/
int GEOL_Attribute::getID() const {
	return mAttrID;
}


/*!
Set the attribute id

\param theID
New attribute id
*/
void GEOL_Attribute::setID(int theID) {
	mAttrID = theID;
}


/*!
Check the equality of attributes id

\param theAttribute
Attribute to check with

\return
- true if theAttribute and this have the same id
- false otherwise 
*/
bool GEOL_Attribute::isEqualID(const GEOL_Attribute *theAttribute) const {
	if (!theAttribute)
		return false;
		
	if (mAttrID == theAttribute -> getID()) {
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
bool GEOL_Attribute::isEqualID(int theAttributeId) const {
	if (!theAttributeId)
		return false;

	if (theAttributeId == mAttrID) {
		return true;
	}
	else {
		return false;
	}
}


/*!
Load object attributes data from a strem in binary format

\param theStream
The stream to read from

\return
- true if the read operation succeed
- false otherwise
*/
bool GEOL_Attribute::LoadBinary(ifstream *theStream) {
	if (!theStream)
		return false;
		
	bool ret = !theStream -> bad();
	if (ret) {
		theStream -> read((char*)(&mAttrID), sizeof(int));
		theStream -> read((char*)(&mType), sizeof(GEOL_AttributeType));
		
		ret = !theStream -> bad();
		if (ret) {
			switch (mType) {
				case GEOL_AttrInt:
					{
						GEOL_AttributeValue value;
						value.GEOL_AttrIntValue = 0;
						theStream -> read((char*)(&value), sizeof(int));
						mValue = value;
					}
					break;
				case GEOL_AttrDouble:
					{
						GEOL_AttributeValue value;
						value.GEOL_AttrDoubleValue = 0.0;
						theStream -> read((char*)(&value), sizeof(double));
						mValue = value;
					}
					break;
				case GEOL_AttrString:
					{
						GEOL_AttributeValue value;
						value.GEOL_AttrStringValue = NULL;
						theStream -> read((char*)(&value), sizeof(char*));
						mValue = value;
					}
					break;
				case GEOL_AttrEntity:
					{
						GEOL_AttributeValue value;
						value.GEOL_AttrEntityValue = NULL;
						theStream -> read((char*)(&value), sizeof(GEOL_Entity*));
						mValue = value;
					}
					break;
				case GEOL_AttrContainer:
					{
						GEOL_AttributeValue value;
						value.GEOL_AttrContainerValue = NULL;
						theStream -> read((char*)(&value), sizeof(GEOL_Container*));
						mValue = value;
					}
					break;
				case GEOL_AttrVoid:
					{
						GEOL_AttributeValue value;
						value.GEOL_AttrVoidValue = NULL;
						theStream -> read((char*)(&value), sizeof(void*));
						mValue = value;
					}
					break;
				default:
					mValue.GEOL_AttrVoidValue = NULL;
					break;
			}
		}
	}

	return ret;
}


/*!
Save object attributes on a stream in binary mode

\param theStream
The stream to write on

\return
- true if the write operation succeed
- false otherwise
*/
bool GEOL_Attribute::SaveBinary(ofstream *theStream) {
	if (!theStream)
		return false;

	bool ret = !theStream -> bad();

	if (ret) {
		theStream -> write((char*)(&mAttrID), sizeof(int));
		theStream -> write((char*)(&mType), sizeof(GEOL_AttributeType));
		
		ret = !theStream -> bad();
		if (ret) {
			switch (mType) {
				case GEOL_AttrInt:
					{
						theStream -> write((char*)(&mValue.GEOL_AttrIntValue), sizeof(int));
					}
					break;
				case GEOL_AttrDouble:
					{
						theStream -> write((char*)(&mValue.GEOL_AttrDoubleValue), sizeof(double));
					}
					break;
				case GEOL_AttrString:
					{
						theStream -> write((char*)(&mValue.GEOL_AttrStringValue), sizeof(char*));
					}
					break;
				case GEOL_AttrEntity:
					{
						theStream -> write((char*)(&mValue.GEOL_AttrEntityValue), sizeof(GEOL_Entity*));
					}
					break;
				case GEOL_AttrContainer:
					{
						theStream -> write((char*)(&mValue.GEOL_AttrContainerValue), sizeof(GEOL_Container*));
					}
					break;
				case GEOL_AttrVoid:
					{
						theStream -> write((char*)(&mValue.GEOL_AttrVoidValue), sizeof(void*));
					}
					break;
				default:
					break;
			}
		}
	}

	return ret;
}
