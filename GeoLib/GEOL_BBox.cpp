/***************************************************************************
								GEOL_BBox.cpp
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


#include <float.h>

#include "GEOL_BBox.h"



GEOL_BBox::GEOL_BBox() {
	mValid = false;
	mMinX = DBL_MAX;
	mMaxX = -DBL_MAX;
	mMinY = DBL_MAX;
	mMaxY = -DBL_MAX;
}



GEOL_BBox::GEOL_BBox(const GEOL_BBox& theBBox) {
	mValid = theBBox.mValid;
	mMinX = theBBox.mMinX;
	mMaxX = theBBox.mMaxX;
	mMinY = theBBox.mMinY;
	mMaxY = theBBox.mMaxY;	
}



GEOL_BBox::~GEOL_BBox() {
	mValid = false;
	mMinX = DBL_MAX;
	mMaxX = -DBL_MAX;
	mMinY = DBL_MAX;
	mMaxY = -DBL_MAX;
}



void GEOL_BBox::setMaxX(double theMaxX) {
	mMaxX = theMaxX;
	checkValidity();
}



void GEOL_BBox::setMaxY(double theMaxY) {
	mMaxY = theMaxY;
	checkValidity();
}



void GEOL_BBox::setMinX(double theMinX) {
	mMinX = theMinX;
	checkValidity();
}



void GEOL_BBox::setMinY(double theMinY) {
	mMinY = theMinY;
	checkValidity();
}


void GEOL_BBox::checkValidity() {
	if (mMinX <= mMaxX && mMinY <= mMaxY) {
		mValid = true;
	}
	else {
		mValid = false;
	}
}



GEOL_BBox& GEOL_BBox::operator=(const GEOL_BBox& theBBox) {
	mValid = theBBox.mValid;
	mMinX = theBBox.mMinX;
	mMaxX = theBBox.mMaxX;
	mMinY = theBBox.mMinY;
	mMaxY = theBBox.mMaxY;	

	return *this;
}
