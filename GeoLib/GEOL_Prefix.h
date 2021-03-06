/***************************************************************************
                                 GEOL_Prefix.h
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
 
 
#ifndef GEOL_PREFIX_H
#define GEOL_PREFIX_H


/************************/
/*    STL Definitions   */
/************************/
#undef min
#undef max
#include <string>
#include <vector>
#include <list>
#include <fstream>
using namespace std;


/****************/
/*     Flags    */
/****************/
#define GEOL_TRACE


/****************/
/*    Include   */
/****************/
#include <float.h>
#include <math.h>
#include <errno.h>


/***************/
/*    Types    */
/***************/


/*!
Quadrand of a point
*/
typedef enum { geol_TopRight, geol_TopLeft, geol_BottomLeft, geol_BottomRight } GEOL_Quadrant;


/******************/
/*    Constants   */
/******************/
#define						GEOL_NOTNULL					1,0E-10				// Not null real value
#define						GEOL_EQUAL_POINT				1.0E-5				// Point equality tolerance
#define						GEOL_EQUAL_DIST					1.0E-5				// Distance equality tolerance
#define						GEOL_EQUAL_AREA					1.0E-10				// Area equality tolerance (GEOL_EQUAL_DIST*GEOL_EQUAL_DIST)
#define						GEOL_EQUAL_ANGLE				1.0E-5				// Angles equality in radians
#define						GEOL_PI							3.14159265359		// �
#define						GEOL_HPI						1.570796326795		// �/2
#define						GEOL_2PI						6.28318530718		// 2*�
#define						GEOL_MAX_REF_COUNT				255					// Maximum value of refernce counters
#define						GEOL_MAX_STRLINE				1024				// Maxium characters in a line while reading from ISO files


/*********/
/* Utils */
/*********/
GEOL_Quadrant getNextQuadrant(GEOL_Quadrant theQuadrant);
GEOL_Quadrant getPrevQuadrant(GEOL_Quadrant theQuadrant);
bool readLineFromISOFile(ifstream *theStream, std::string& theLine);
bool getCoordFromISOFileLine(std::string& theLine, double& theXCoord, double& theYCoord, double& theZCoord, double& theICoord, double& theJCoord);

#endif