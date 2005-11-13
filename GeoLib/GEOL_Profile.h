/***************************************************************************
								GEOL_Profile.h
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

#ifndef GEOL_PROFILE_H
#define GEOL_PROFILE_H


#include "GEOL_Container.h"

/*!
*/
class GEOL_Profile : public GEOL_Container {
friend class GEOL_Context;
public:
	bool addEntityTail(GEOL_Entity *theNewEntity);
	bool addEntityFront(GEOL_Entity *theNewEntity);
	
	bool isClosed();

	bool notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag);

protected:
	GEOL_Profile();
	~GEOL_Profile();


	virtual bool LoadBinary(std::ifstream *theStream);
	virtual bool SaveBinary(std::ofstream *theStream);
	virtual bool LoadISO(std::ifstream *theStream);

};



#endif
