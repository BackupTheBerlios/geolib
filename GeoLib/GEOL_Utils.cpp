/***************************************************************************
								GEOL_Utils.cpp
                             -------------------
    begin                : 14.09.2006
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


#include "GEOL_Prefix.h"


/*!
Get the next of a given quadrant in counterclockwise versus

\param theQuadrant
Quadrant to take the next

\return
The next quadrant of theQuadrant in counterclockwise versus
*/
GEOL_Quadrant getNextQuadrant(GEOL_Quadrant theQuadrant) {
	GEOL_Quadrant ret = theQuadrant;
	switch (theQuadrant) {
		case geol_TopRight:
			ret = geol_TopLeft;
			break;
		case geol_TopLeft:
			ret = geol_BottomLeft;
			break;
		case geol_BottomLeft:
			ret = geol_BottomRight;
			break;
		case geol_BottomRight:
			ret = geol_TopRight;
			break;
		default:
			break;
	}
	
	return ret;
}



/*!
Get the prev of a given quadrant in counterclockwise versus

\param theQuadrant
Quadrant to take the prev

\return
The prev quadrant of theQuadrant in counterclockwise versus
*/
GEOL_Quadrant getPrevQuadrant(GEOL_Quadrant theQuadrant) {
	GEOL_Quadrant ret = theQuadrant;
	switch (theQuadrant) {
		case geol_TopRight:
			ret = geol_BottomRight;
			break;
		case geol_TopLeft:
			ret = geol_TopRight;
			break;
		case geol_BottomLeft:
			ret = geol_TopLeft;
			break;
		case geol_BottomRight:
			ret = geol_BottomLeft;
			break;
		default:
			break;
	}
	
	return ret;
}
