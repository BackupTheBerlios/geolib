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


/*!
Read a text line from an ISO file

\param theStream
ISO file
\param theLine
On output contains the line readed

\return
- true if success
- false on error
*/
bool readLineFromISOFile(ifstream *theStream, std::string& theLine) {
	if (!theStream)
		return false;

	bool ret = true;
	
	theLine.clear();
	char strLine[GEOL_MAX_STRLINE];
	theStream -> getline(strLine, GEOL_MAX_STRLINE - 1);
	if (theStream -> good() || theStream -> eof()) {
		theLine = strLine;
		for (unsigned int i = 0 ; i < theLine.size() ; ) {
			if (theLine[i] == ' ') {
				theLine.erase(i, 1);
			}
			else {
				theLine[i] = toupper(theLine[i]);
				i++;
			}
		}
		ret = true;
	}
	else {
		ret = false;
	}

	return ret;
}


/*!
Extract the coordinate values from a iso text line

\param theLine
Line to parse
\param theXCoord
On output contains the x coordinate if present, DBL_MAX otherwise
\param theYCoord
On output contains the y coordinate if present, DBL_MAX otherwise
\param theZCoord
On output contains the z coordinate if present, DBL_MAX otherwise
\param theICoord
On output contains the x coordinate of the arc center if present, DBL_MAX otherwise
\param theJCoord
On output contains the y coordinate of the arc center if present, DBL_MAX otherwise

\return
- true if success
- false on error
*/
bool getCoordFromISOFileLine(const std::string& theLine, double& theXCoord, double& theYCoord, double& theZCoord, double& theICoord, double& theJCoord) {
	theXCoord = DBL_MAX;
	theYCoord = DBL_MAX;
	theZCoord = DBL_MAX;
	theICoord = DBL_MAX;
	theJCoord = DBL_MAX;

	bool ret = true;
	char coordName = 0;
	std::string strVal;
	for (unsigned int i = 0 ; ret && i <= theLine.size() ; i++) {
		if (i == theLine.size() || theLine[i] == 'X' || theLine[i] == 'Y' || theLine[i] == 'Z' || theLine[i] == 'I' || theLine[i] == 'J') {
			if (coordName != 0) {
				errno = 0;
				double val = strtod(strVal.c_str(), NULL);
				if (errno == ERANGE) {
					ret = false;
				}
				else {
					switch (coordName) {
						case 'X':
							theXCoord = val;
							break;
						case 'Y':
							theYCoord = val;
							break;
						case 'Z':
							theZCoord = val;
							break;
						case 'I':
							theICoord = val;
							break;
						case 'J':
							theJCoord = val;
							break;
						default:
							ret = false;
							break;
					}
				}
			}
			strVal.clear();
			coordName = theLine[i];
		}
		else {			
			if (coordName != 0) {
				strVal += theLine[i];
			}
		}
	}
	
	return ret;
}


