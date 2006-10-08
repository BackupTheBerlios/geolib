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
#include <vector>
#include <list>
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


/***************/
/*    Types    */
/***************/


/*!
Type of an object
*/
typedef enum {geol_Point, geol_Segment, geol_Arc, geol_Profile, geol_PoliProfile, geol_Undefined} GEOL_ObjectType;
/*!
Versus of an arc
*/
typedef enum { geol_ArcClockwise, geol_ArcCounterClockwise } GEOL_ArcVersus;
/*!
Quadrand of a point
*/
typedef enum { geol_TopRight, geol_TopLeft, geol_BottomLeft, geol_BottomRight } GEOL_Quadrant;


/******************/
/*    Constants   */
/******************/
#define						GEOL_EQUAL_POINT				1.0E-5				// Point equality tolerance
#define						GEOL_EQUAL_DIST					1.0E-5				// Distance equality tolerance
#define						GEOL_EQUAL_AREA					1.0E-10				// Area equality tolerance (GEOL_EQUAL_DIST*GEOL_EQUAL_DIST)
#define						GEOL_EQUAL_ANGLE				1.0E-5				// Angles equality in radians
#define						GEOL_PI							3.14159265359		// ¹
#define						GEOL_HPI						1.570796326795		// ¹/2
#define						GEOL_2PI						6.28318530718		// 2*¹
#define						GEOL_MAX_REF_COUNT				255					// Maximum value of refernce counters


/*********/
/* Utils */
/*********/
GEOL_Quadrant getNextQuadrant(GEOL_Quadrant theQuadrant);
GEOL_Quadrant getPrevQuadrant(GEOL_Quadrant theQuadrant);


#endif