/********************************************************************
* File: GM_3dVector.h												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once



#include "GM_3dPoint.h"
#include "GM_3dLine.h"



/*!
3D Vector is a kind of 3D point
*/
class GM_3dVector : public GM_3dPoint {
public:
	GM_3dVector(void);
	GM_3dVector(const GM_3dVector& theVect);
	GM_3dVector(const GM_3dPoint& thePoint);
	GM_3dVector(const GM_3dLine& theLine);
	GM_3dVector(double theXCoord, double theYCoord, double theZCoord);
	GM_3dVector(double theXYAngle);
	virtual ~GM_3dVector(void);

	double operator*(const GM_3dVector& theVect) const;	
	GM_3dVector operator^(const GM_3dVector& theVect) const;	
	GM_3dVector operator*(double theFactor) const;	
	GM_3dVector operator+(const GM_3dVector& theVect) const;	
	GM_3dVector operator-(const GM_3dVector& theVect) const;	
	double mod() const;
	void normalize();
	double xyAngle() const;
	double xyAngle(const GM_3dVector& theVect) const;
	bool isAtLeftOnXY(const GM_3dVector& theVect) const;
};
