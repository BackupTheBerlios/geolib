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
#include <list>
using namespace std;



/****************/
/*     Flags    */
/****************/
#define GEOL_TRACE



/****************/
/*    Include   */
/****************/
#include <math.h>




/***************/
/*    Types    */
/***************/

/*!
Type of an object
*/
typedef enum {geol_Point, geol_Segment, geol_Arc, geol_Profile, geol_PoliProfile} GEOL_ObjectType;
/*!
Versus of an arc
*/
typedef enum { GEOL_ArcClockwise, GEOL_ArcCounterClockwise } GEOL_ArcVersus;



/******************/
/*    Constants   */
/******************/
#define						GEOL_EQUAL_POINT				1.0E-5				// Point equality tolerance
#define						GEOL_EQUAL_DIST					1.0E-5				// Distance equality tolerance
#define						GEOL_PI							3.14159265359		// ¹
#define						GEOL_HPI						1.570796326795		// ¹/2
#define						GEOL_2PI						6.28318530718		// 2*¹
#define						GEOL_MAX_REF_COUNT				255					// Maximum value of refernce counters





#endif