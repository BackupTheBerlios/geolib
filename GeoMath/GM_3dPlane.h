/********************************************************************
* File: GM_3dPlane.h												*
*********************************************************************
* Descrizione:														*
*********************************************************************
* History:															*
* 22.4.2007 Creato da : Cordara Claudio								*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once

class GM_3dPoint;
class GM_3dTriangle;

class GM_3dPlane {
public:
	GM_3dPlane();
	~GM_3dPlane();
	GM_3dPlane(double theCoeffArray[4]);
	GM_3dPlane(double a, double b, double c, double d);
	GM_3dPlane(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3);
	GM_3dPlane(GM_3dPoint theNormal, GM_3dPoint thePoint);
	GM_3dPlane(GM_3dTriangle theTriangle);

	double pointDistance(GM_3dPoint& thePoint, GM_3dPoint& thePointOnPlane) const;
	double pointDistance(GM_3dPoint& thePoint) const;

	bool isValid() const;
	void invalidate();
private:
	void createFromThreePoints(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3);
	void normalize();

	/*!
	Coefficenti dell' equazione caratteristica a*x + b*y + c*z + d = 0 dove
	mCoeff[0] = a
	mCoeff[1] = b
	mCoeff[2] = c
	mCoeff[3] = d
	In forma Hessiana, ovvero tale che sqrt(a*a + b*b + c*c) = 1
	*/
	double mCoeff[4];
};
