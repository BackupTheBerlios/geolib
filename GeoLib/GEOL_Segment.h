/***************************************************************************
								GEOL_Segment.h
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

#ifndef GEOL_SEGMENT_H
#define GEOL_SEGMENT_H


#include "GEOL_Entity.h"

/*!
Straight 2dimensional segment, it points to its begin and end extremities through the pointers contained in its
superclass (GEOL_Entity), the length of the segment is manteined in a class attribute (mLength).
*/
class GEOL_Segment : public GEOL_Entity {
friend class GEOL_Context;
public:
	
	GEOL_Segment& operator=(const GEOL_Segment& theSegment);
	bool operator==(const GEOL_Segment& theSegment) const;
	bool operator!=(const GEOL_Segment& theSegment) const;
	
	GEOL_Point* begin() const;
	GEOL_Point* end() const;
	
	void begin(const GEOL_Point& theBeginPoint);
	void end(const GEOL_Point& theEndPoint);
	void begin(double theXCoord, double theYCoord);
	void end(double theXCoord, double theYCoord);
	
	double angle() const;
	
	/*!
	\return
	The length of the segment
	*/
	double length() const { return mLength; }
	double area() const;
	void direction(GEOL_Point* theDir, const GEOL_Point* thePoint) const;
	
	bool notifyDestruction(GEOL_Object *theObject, bool& theDestroyFlag);
	bool isEndPoint(const GEOL_Entity *theEntity);

	void translate(double theDX, double theDY);

	GEOL_BBox getBBox();

protected:
	GEOL_Segment();
	GEOL_Segment(GEOL_Point* theBeginPoint, GEOL_Point* theEndPoint);
	~GEOL_Segment();

	virtual bool LoadBinary(ifstream *theStream);
	virtual bool SaveBinary(ofstream *theStream);
	virtual bool LoadISO(ifstream *theStream);
	virtual bool SaveISO(ofstream *theStream);

private:
	/*!
	Length of the segment
	*/
	double mLength;
};


/*!
\return
The begin point of the segment
*/
inline GEOL_Point* GEOL_Segment::begin() const {
	const GEOL_Entity *beginPoint = getBeginEntity();
	return ((GEOL_Point*)beginPoint);
}


/*!
\return
The end point of the segment
*/
inline GEOL_Point* GEOL_Segment::end() const {
	const GEOL_Entity *endPoint = getEndEntity();
	return ((GEOL_Point*)endPoint);
}


#endif
