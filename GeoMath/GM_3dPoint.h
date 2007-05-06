/********************************************************************
* File: GM_3dPoint.h												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/


#pragma once


class GM_3dVector;


class GM_3dPoint {
public:
	GM_3dPoint();
	virtual ~GM_3dPoint();
	GM_3dPoint(const GM_3dPoint& thePoint);
	GM_3dPoint(double theXCoord, double theYCoord, double theZCoord);
	GM_3dPoint(double theXYAngle);

	double x() const { return mX; }
	double y() const { return mY; }
	double z() const { return mZ; }
	void x(double theXCoord) { mX = theXCoord; }
	void y(double theYCoord) { mY = theYCoord; }
	void z(double theZCoord) { mZ = theZCoord; }

	double distFrom(const GM_3dPoint& thePoint) const;
	bool isOrigin() const;
	void xyInvert();
	bool isValid() const;
	void invalidate();

	operator GM_3dVector() const;
	bool operator == (const GM_3dPoint& thePoint) const;
	bool operator != (const GM_3dPoint& thePoint) const;

protected:
	double mX;
	double mY;
	double mZ;
};
