/***************************************************************************
							  GEOL_PoliProfile.h
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


#ifndef GEOL_POLIPROFILE_H
#define GEOL_POLIPROFILE_H


#include "GEOL_Container.h"


/*!
A complex container that can be constructed with primitive objects and with other containers (profiles or poliprofiles as well).
*/
class GEOL_PoliProfile : public GEOL_Container {
friend class GEOL_Context;
public:
	bool addProfile(GEOL_Profile *theNewProfile);
	bool removeProfile(GEOL_Profile *theProfile);
	bool detachProfile(GEOL_Profile *theProfile);

	GEOL_BBox getBBox();

protected:
	GEOL_PoliProfile();
	~GEOL_PoliProfile();

	virtual bool LoadBinary(ifstream *theStream);
	virtual bool SaveBinary(ofstream *theStream);
	virtual bool LoadISO(ifstream *theStream);
	virtual bool SaveISO(ofstream *theStream);
};


#endif
