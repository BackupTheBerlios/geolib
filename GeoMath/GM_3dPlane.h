/********************************************************************
* File: GM_3dPlane.h												*
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
class GM_3dTriangle;



/*!
3D plane
*/
class GM_3dPlane {
public:
	GM_3dPlane();
	virtual ~GM_3dPlane();
	GM_3dPlane(const GM_3dPlane& thePlane);
	GM_3dPlane(double theCoeffArray[4]);
	GM_3dPlane(double a, double b, double c, double d);
	GM_3dPlane(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3);
	GM_3dPlane(GM_3dVector theNormal, GM_3dPoint thePoint);
	GM_3dPlane(GM_3dTriangle theTriangle);

	bool operator == (const GM_3dPlane& thePlane) const;
	bool operator != (const GM_3dPlane& thePlane) const;
	double operator [](unsigned int theIndex) const;

	double pointDistance(const GM_3dPoint& thePoint, GM_3dPoint& thePointOnPlane) const;
	double pointDistanceSgn(const GM_3dPoint& thePoint, GM_3dPoint& thePointOnPlane) const;
	double pointDistance(const GM_3dPoint& thePoint) const;
	double pointDistanceSgn(const GM_3dPoint& thePoint) const;
	GM_3dVector normalVector() const;
	double xyAngle() const;

	bool isValid() const;
	void invalidate();
private:
	void createFromThreePoints(GM_3dPoint thePt1, GM_3dPoint thePt2, GM_3dPoint thePt3);
	void normalize();

	/*!
	Coefficient of the general equation of a plane a*x + b*y c*z + d = 0 where
	mCoeff[0] = a
	mCoeff[1] = b
	mCoeff[2] = c
	mCoeff[3] = d
	In Hessian form that is sqrt(a*a + b*b + c*c) = 1
	*/
	double mCoeff[4];
};
