/********************************************************************
* File: GM_3dTriangle.h												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once

#include "GM_3dLine.h"

class GM_3dPoint;

class GM_3dTriangle {
public:
	GM_3dTriangle();
	virtual ~GM_3dTriangle();
	GM_3dTriangle(const GM_3dTriangle& theTriangle);
	GM_3dTriangle(GM_3dPoint theFirstPoint, GM_3dPoint theSecondPoint, GM_3dPoint theThirdPoint);
	GM_3dTriangle(GM_3dLine theFirstEdge, GM_3dLine theSecondEdge, GM_3dLine theThirdEdge);

	GM_3dLine& operator[](int theIndex);
	GM_3dLine operator[](int theIndex) const;

	void invert();
	void setXYVersus(bool theClockwiseFlag);

	bool isValid() const;
	bool isHorizontal() const;
	bool isVertical() const;
	bool isConnected() const;
	double maxZ() const;
	double minZ() const;
	double xyAngle() const;
	GM_3dPoint normalVector() const;
	GM_3dPoint normalVectorOnEdge(int theEdgeIndex) const;
	GM_3dLine zSection(double theZLevel) const;
	void invalidate();

	double pointDistance(GM_3dPoint thePoint) const;
	bool isInteriorPoint(GM_3dPoint thePoint) const;

private:
	/*!
	Tre linee rappresentanti i lati del triangolo
	*/
	GM_3dLine mEdge[3];
};
