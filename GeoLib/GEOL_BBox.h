/***************************************************************************
                                 GEOL_BBox.h
                             -------------------
    begin                : 25.6.2006
    copyright            : (C) 2006 by Claudio Cordara
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



#ifndef GEOL_BBOX_H
#define GEOL_BBOX_H



/*!
Bounding box of a graphical object, simple or composite.
*/
class GEOL_BBox {
public:
	GEOL_BBox();
	GEOL_BBox(double theMinX, double theMinY, double theMaxX, double theMaxY);
	GEOL_BBox(const GEOL_BBox& theBBox);
	~GEOL_BBox();
	
	bool isValid() const;
	void invalidate();
	
	double getMinX() const;
	double getMaxX() const;
	double getMinY() const;
	double getMaxY() const;
	
	void setMaxX(double theMaxX);
	void setMaxY(double theMaxY);
	void setMinX(double theMinX);
	void setMinY(double theMinY);
	
	double area() const;

	GEOL_BBox& operator=(const GEOL_BBox& theBBox);
	const GEOL_BBox operator+(const GEOL_BBox& theBBox) const;
	bool operator==(const GEOL_BBox& theBBox) const;
private:
	void checkValidity();

	/*!
	Validity flag
	*/
	bool mValid;
	/*!
	x minimum value
	*/
	double mMinX;
	/*!
	x maximum value
	*/	
	double mMaxX;
	/*!
	y minimum value	
	*/
	double mMinY;
	/*!
	y maximum value
	*/	
	double mMaxY;
};



/*!
\return
- true if the bounding box is valid
- false otherwise
*/
inline bool GEOL_BBox::isValid() const {
	return mValid;
}



/*!
\return
The x minimum value
*/
inline double GEOL_BBox::getMinX() const {
	return mMinX;
}



/*!
\return
The x maximum value
*/
inline double GEOL_BBox::getMaxX() const {
	return mMaxX;
}



/*!
\return
The y minimum value
*/
inline double GEOL_BBox::getMinY() const {
	return mMinY;
}



/*!
\return
The y maximum value
*/
inline double GEOL_BBox::getMaxY() const {
	return mMaxY;
}



#endif