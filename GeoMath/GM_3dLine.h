/********************************************************************
* File: GM_3dLine.h													*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once

class GM_3dPoint;

class GM_3dLine {
public:
	GM_3dLine();
	virtual ~GM_3dLine();
	GM_3dLine(const GM_3dLine& theLine);
	GM_3dLine(GM_3dPoint theBegin, GM_3dPoint theEnd);
	GM_3dLine(double theBeginXCoord, double theBeginYCoord, double theBeginZCoord, double theEndXCoord, double theEndYCoord, double theEndZCoord);

	double operator*(const GM_3dLine& theLine) const;	
	GM_3dLine operator^(const GM_3dLine& theLine) const;	
	bool operator == (const GM_3dLine& theLine) const;
	bool operator != (const GM_3dLine& theLine) const;


	/*!
	\return
	Lunghezza della proiezione sull' asse x
	*/
	double dx() const { return mEnd.x() - mBegin.x(); }
	/*!
	\return
	Lunghezza della proiezione sull' asse y
	*/
	double dy() const { return mEnd.y() - mBegin.y(); }
	/*!
	\return
	Lunghezza della proiezione sull' asse z
	*/
	double dz() const { return mEnd.z() - mBegin.z(); }

	GM_3dPoint begin() const { return mBegin; }
	GM_3dPoint end() const { return mEnd; }
	GM_3dPoint& begin() { return mBegin; }
	GM_3dPoint& end() { return mEnd; }
	GM_3dPoint center() const;
	void begin(GM_3dPoint& theBegin) { mBegin = theBegin; }
	void end(GM_3dPoint& theEnd) { mEnd = theEnd; }

	void invert();
	double length() const;
	bool isNull() const;
	bool isVertical() const;
	bool isHorizontal() const;
	double minZ() const;
	double maxZ() const;
	GM_3dPoint pointAtZ(double theZLevel) const;
	GM_3dVector normalXYVector(bool theRightSideFlag) const;
	double xyAngle() const;
	double xyAngle(GM_3dLine& theLine) const;
	bool isValid() const;
	void invalidate();

	double pointDistance(GM_3dPoint thePoint, GM_3dPoint& thePointOnLine) const;
	double pointDistance(GM_3dPoint thePoint) const;
	GM_3dPoint pointFromSection(double theSection) const;
	double sectionFromPoint(GM_3dPoint thePoint) const;
private:
	GM_3dPoint mBegin;
	GM_3dPoint mEnd;
};
