/***************************************************************************
                                GEOL_Context.h
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


#ifndef GEOL_CONTEXT_H
#define GEOL_CONTEXT_H

class GEOL_Object;
class GEOL_Point;
class GEOL_Segment;
class GEOL_Arc;
class GEOL_Profile;
class GEOL_PoliProfile;

/*!
The context is "the world" of geometrical objects, all objects belongs to a context and objects in
different contexts can't have any relationship.
Allocation and destruction of objects is the key responsability of this class.
*/
class GEOL_Context {
public:
	GEOL_Context();
	~GEOL_Context();

	//**********
	//* Creation
	//**********
	
	GEOL_Point* createPoint();
	GEOL_Point* createPoint(double theXCoord, double theYCoord);
	GEOL_Point* createPoint(const GEOL_Point& thePoint);
	
	GEOL_Segment* createSegment();
	GEOL_Segment* createSegment(const GEOL_Point& theBeginPoint, const GEOL_Point& theEndPoint);
	GEOL_Segment* createSegment(GEOL_Point* theBeginPoint, GEOL_Point* theEndPoint);
	GEOL_Segment* createSegment(const GEOL_Segment& theSegment);
	GEOL_Segment* createSegment(double theXStart, double theYStart, double theXEnd, double theYEnd);
	
	GEOL_Arc* createArc();
	GEOL_Arc* createArc(const GEOL_Point& theBeginPoint, const GEOL_Point& theEndPoint, double theRadius, GEOL_ArcVersus theVersus);
	GEOL_Arc* createArc(const GEOL_Arc& theArc);
	GEOL_Arc* createArc(double theXStart, double theYStart, double theXEnd, double theYEnd, double theRadius, GEOL_ArcVersus theVersus);
	
	GEOL_Profile* createProfile();
	GEOL_Profile* createProfile(const GEOL_Profile& theProfile);
	
	GEOL_PoliProfile* createPoliProfile();
	GEOL_PoliProfile* createPoliProfile(const GEOL_PoliProfile& thePoliProfile);

	//*************
	//* Destruction
	//*************

	bool deleteObject(GEOL_Object *theObject, bool theNotifyFlag = false);
	void removeAllObjects();
	
	//************
	//* Navigation
	//************

	GEOL_Object* getFirstObject();
	GEOL_Object* getLastObject();
	GEOL_Object* getNextObject(const GEOL_Object *theObject);
	GEOL_Object* getPrevObject(const GEOL_Object *theObject);
	void setObjectIterator(std::list<GEOL_Object*>::const_iterator &theIterator) const;
	bool isEndOfObjects(std::list<GEOL_Object*>::const_iterator &theIterator) const;
	
	//****************
	//* Interrogations
	//****************
	
	GEOL_Object* getEntityContainer(const GEOL_Object *theEntity) const;
	GEOL_Object* getParentContainer(const GEOL_Object *theContainer) const;
		
	//*************
	//* Persistency
	//*************

	bool saveContext(ofstream *theStream);
	bool loadContext(ifstream *theStream);
	bool loadBinaryObjectType(ifstream *theStream, GEOL_ObjectType& theObjectType);
	
	bool saveContextISO(ofstream *theStream);
	bool loadContextISO(ifstream *theStream);

	/*!
	\return
	Number of objects in the context
	*/
	int getSize() { return pObjectList.size(); }
	
private:
	bool addObject(GEOL_Object *theNewObject);
	bool deletePoint(GEOL_Point *thePoint, bool theNotifyFlag = false);
	bool deleteSegment(GEOL_Segment *theSegment, bool theNotifyFlag = false);
	bool deleteArc(GEOL_Arc *theArc, bool theNotifyFlag = false);
	bool deleteProfile(GEOL_Profile *theProfile, bool theNotifyFlag = false);
	bool deletePoliProfile(GEOL_PoliProfile *thePoliProfile, bool theNotifyFlag = false);
	bool notifyDestruction(GEOL_Object *theObject);

	/*!
	List of objects, entities or containers, within the context
	*/
	list<GEOL_Object*> pObjectList;

	/*!
	List of the objects remaining to delete during a delete/notify procedure, when an object is deleted
	other objects can has to be deleted as well.
	This list contains this objects
	*/
	list<GEOL_Object*> pDeletionObjList;
};



#endif