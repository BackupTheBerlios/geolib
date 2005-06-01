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
Description
*/
class GEOL_PoliProfile : public GEOL_Container {
friend class GEOL_Context;
public:
protected:
	GEOL_PoliProfile();
	~GEOL_PoliProfile();

	virtual bool LoadBinary(std::ifstream *theStream);
	virtual bool SaveBinary(std::ofstream *theStream);
	virtual bool LoadISO(std::ifstream *theStream);

};



#endif
