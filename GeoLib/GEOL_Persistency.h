/***************************************************************************
							 GEOL_Persistency.h
                             -------------------
    begin                : 25.03.2005
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



#ifndef GEOL_PERSISTENCY_H
#define GEOL_PERSISTENCY_H

#include <fstream>

class GEOL_Persistency {
public:
	GEOL_Persistency() {}
	virtual ~GEOL_Persistency() = 0;
	
	//virtual bool Load() = 0;
	//virtual bool Save() = 0;
	virtual bool LoadBinary(ifstream *theStream) = 0;
	virtual bool SaveBinary(ofstream *theStream) = 0;
	virtual bool LoadISO(ifstream *theStream) = 0;
private:
};



inline GEOL_Persistency::~GEOL_Persistency() {}



#endif
