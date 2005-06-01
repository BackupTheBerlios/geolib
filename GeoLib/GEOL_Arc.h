/***************************************************************************
								  GEOL_Arc.h
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

#ifndef GEOL_ARC_H
#define GEOL_ARC_H

#include "GEOL_Entity.h"


/*!
2dimensional circle arc, it points to its begin and end extremities through the pointers contained in its
superclass (GEOL_Entity), the radius of the arc is a member variable, when the radius is positive the center of
the arc is at right of the segment connecting arc extremities, when negative the center is at left.
The length of the arc is also manteined in a class attribute (mLength).
*/
class GEOL_Arc : public GEOL_Entity {
friend class GEOL_Context;
public:
	
	GEOL_Arc& operator=(const GEOL_Arc& theArc);
	bool operator==(const GEOL_Arc& theArc) const;
	bool operator!=(const GEOL_Arc& theArc) const;

	GEOL_Point* begin() const;
	GEOL_Point* end() const;

	void begin(const GEOL_Point& theBeginPoint);
	void end(const GEOL_Point& theEndPoint);
	void begin(double theXCoord, double theYCoord);
	void end(double theXCoord, double theYCoord);

	/*!
	\return
	The radius of the arc without sign
	*/
	double radius() const { return fabs(mRadius); }
	
	/*!
	Set the radius of the arc
	
	\param theRadius
	Radius of the arc
	*/
	void radius(double theRadius);
	
	/*!
	\return
	The length of the arc
	*/
	double length() const { return mLength; }
	
	GEOL_ArcVersus versus() const;
	void versus(GEOL_ArcVersus theVersus);
	
	double angle() const;
	
	void center(GEOL_Point &theCenter) const;
	void center(double &theXCenter, double &theYCenter) const;
	
	bool isEndPoint(const GEOL_Entity *theEntity);

protected:
	GEOL_Arc();
	GEOL_Arc(GEOL_Point* theBeginPoint, GEOL_Point* theEndPoint, double theRadius, GEOL_ArcVersus theVersus);
	~GEOL_Arc();

	virtual bool LoadBinary(std::ifstream *theStream);
	virtual bool SaveBinary(std::ofstream *theStream);
	virtual bool LoadISO(std::ifstream *theStream);

private:
	double computeLength() const;

	/*!
	Radius of the arc, if positive the arc clockwise, the center of the arc is on the left of the segment
	between the start and the end of the arc
	*/
	double mRadius;

	/*!
	Length of the arc
	*/
	double mLength;
};


/*!
\return
The begin point of the arc
*/
inline GEOL_Point* GEOL_Arc::begin() const {
	const GEOL_Entity *beginPoint = getBeginEntity();
	return ((GEOL_Point*)beginPoint);
}


/*!
\return
The end point of the arc
*/
inline GEOL_Point* GEOL_Arc::end() const {
	const GEOL_Entity *endPoint = getEndEntity();
	return ((GEOL_Point*)endPoint);
}


#endif
