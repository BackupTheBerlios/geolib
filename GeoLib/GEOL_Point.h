/***************************************************************************
                                 GEOL_Point.h
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

#ifndef GEOL_POINT_H
#define GEOL_POINT_H

#include "GEOL_Entity.h"

/*!
Geometrical 2dimensional point, is the fundamental brick of any other geometrical entity, x and y coordinates of
the point are storaged in the mBegin and mEnd pointers of its superclass (GEOL_Entity).
*/
class GEOL_Point : public GEOL_Entity {
friend class GEOL_Context;
public:
	
	GEOL_Point& operator=(const GEOL_Point& thePoint);
	bool operator==(const GEOL_Point& thePoint) const;
	bool operator!=(const GEOL_Point& thePoint) const;
	
	double x() const;
	double y() const;
	void x(double theXCoord);
	void y(double theYCoord);
	void xy(double theXCoord, double theYCoord);
	double pointDistance(const GEOL_Point& thePoint) const;
	double pointDistance(const GEOL_Point* thePoint) const;
	double originDistance();
	
protected:
	GEOL_Point();
	GEOL_Point(const GEOL_Point& thePoint);
	GEOL_Point(double theXCoord, double theYCoord);
	~GEOL_Point();
};


/*!
\return
The X coordinate
*/
inline double GEOL_Point::x() const {
	return mBegin;
}

/*!
\return
The Y coordinate
*/
inline double GEOL_Point::y() const {
	return mEnd;
}


/*!
Set the X coordinate

\param theXCoord
New X coordinate
*/
inline void GEOL_Point::x(double theXCoord) {
	mBegin = theXCoord;
}



/*!
Set the Y coordinate

\param theYCoord
New Y coordinate
*/
inline void GEOL_Point::y(double theYCoord) {
	mEnd = theYCoord;
}


/*!
Set the coordinates of the point

\param theXCoord
New X coordinate
\param theYCoord
New Y coordinate
*/
inline void GEOL_Point::xy(double theXCoord, double theYCoord) {
	mBegin = theXCoord;
	mEnd = theYCoord;
}



#endif