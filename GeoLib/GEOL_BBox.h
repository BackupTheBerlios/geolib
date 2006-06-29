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



class GEOL_BBox {
public:
	GEOL_BBox();
	GEOL_BBox(const GEOL_BBox& theBBox);
	~GEOL_BBox();
	
	bool isValid() const;
	
	double getMinX() const;
	double getMaxX() const;
	double getMinY() const;
	double getMaxY() const;
	
	void setMaxX(double theMaxX);
	void setMaxY(double theMaxY);
	void setMinX(double theMinX);
	void setMinY(double theMinY);

	GEOL_BBox& operator=(const GEOL_BBox& theBBox);
private:
	void checkValidity();

	bool mValid;
	double mMinX;
	double mMaxX;
	double mMinY;
	double mMaxY;
};



inline bool GEOL_BBox::isValid() const {
	return mValid;
}


inline double GEOL_BBox::getMinX() const {
	return mMinX;
}



inline double GEOL_BBox::getMaxX() const {
	return mMaxX;
}



inline double GEOL_BBox::getMinY() const {
	return mMinY;
}



inline double GEOL_BBox::getMaxY() const {
	return mMaxY;
}



#endif