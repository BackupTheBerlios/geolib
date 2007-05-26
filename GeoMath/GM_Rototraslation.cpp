/********************************************************************
* File: GM_Rototraslation.cpp										*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_Rototraslation.h"



/*!
Default constructor
*/
GM_Rototraslation::GM_Rototraslation(void) : GM_3dTrasf() {
	mTraslation.x(0.0);
	mTraslation.y(0.0);
	mTraslation.z(0.0);
}



/*!
Copy constructor
*/
GM_Rototraslation::GM_Rototraslation(const GM_Rototraslation& theRototras) : GM_3dTrasf(theRototras) {
	mTraslation = theRototras.mTraslation;
}



/*!
Constructor from rotation angles and traslation values

\param theXAngle
Rotation angle for x axis
\param theYAngle
Rotation angle for y axis
\param theZAngle
Rotation angle for z axis
\param theXTrasl
Traslation value for x axis
\param theYTrasl
Traslation value for y axis
\param theZTrasl
Traslation value for z axis
*/
GM_Rototraslation::GM_Rototraslation(double theXAngle, double theYAngle, double theZAngle, double theXTrasl, double theYTrasl, double theZTrasl) {
	setIdentity();
	xRotation(theXAngle);
	yRotation(theYAngle);
	zRotation(theZAngle);
	mTraslation.x(theXTrasl);
	mTraslation.y(theYTrasl);
	mTraslation.z(theZTrasl);
}



/*!
Default destructor
*/
GM_Rototraslation::~GM_Rototraslation(void) {
}



/*!
Assignment operator
*/
GM_Rototraslation& GM_Rototraslation::operator=(const GM_Rototraslation& theRototras) {
	if (isValid() && theRototras.isValid()) {
		*((GM_3dTrasf*)this) = *((GM_3dTrasf*)(&theRototras));
		mTraslation = theRototras.mTraslation;
	}
	return *this;
}



/*!
Add a rotation value for x axis on rotation matrix

\param theXAngle
Rotation angle for x axis
*/
void GM_Rototraslation::xRotation(double theXAngle) {
	if (isValid()) {
		GM_Matrix rMat(3, 3);
		rMat[0][0] = 1.0;
		rMat[0][1] = 0.0;
		rMat[0][2] = 0.0;
		rMat[1][0] = 0.0;
		rMat[1][1] = cos(theXAngle);
		rMat[1][2] = sin(theXAngle);
		rMat[1][0] = 0.0;
		rMat[1][1] = -sin(theXAngle);
		rMat[1][2] = cos(theXAngle);
		*((GM_Matrix*)this) = (*(GM_Matrix*)this) * ((const GM_Matrix)rMat);
	}
}



/*!
Add a rotation value for y axis on rotation matrix

\param theYAngle
Rotation angle for y axis
*/
void GM_Rototraslation::yRotation(double theYAngle) {
	if (isValid()) {
		GM_Matrix rMat(3, 3);
		rMat[0][0] = cos(theYAngle);
		rMat[0][1] = 0.0;
		rMat[0][2] = -sin(theYAngle);
		rMat[1][0] = 0.0;
		rMat[1][1] = 1.0;
		rMat[1][2] = 0.0;
		rMat[1][0] = sin(theYAngle);
		rMat[1][1] = 0.0;
		rMat[1][2] = cos(theYAngle);
		*((GM_Matrix*)this) = (*(GM_Matrix*)this) * ((const GM_Matrix)rMat);
	}
}



/*!
Add a rotation value for z axis on rotation matrix

\param theZAngle
Rotation angle for z axis
*/
void GM_Rototraslation::zRotation(double theZAngle) {
	if (isValid()) {
		GM_Matrix rMat(3, 3);
		rMat[0][0] = cos(theZAngle);
		rMat[0][1] = sin(theZAngle);
		rMat[0][2] = 0.0;
		rMat[1][0] = -sin(theZAngle);
		rMat[1][1] = cos(theZAngle);
		rMat[1][2] = 0.0;
		rMat[1][0] = 0.0;
		rMat[1][1] = 0.0;
		rMat[1][2] = 1.0;
		*((GM_Matrix*)this) = (*(GM_Matrix*)this) * ((const GM_Matrix)rMat);
	}
}



/*!
Add a traslation value for x axis to traslation vector

\param theXTrasl
Traslation value for x axis
*/
void GM_Rototraslation::xTrasl(double theXTrasl) {
	if (isValid()) {
		mTraslation.x(mTraslation.x() + theXTrasl);
	}
}



/*!
Add a traslation value for y axis to traslation vector

\param theYTrasl
Traslation value for y axis
*/
void GM_Rototraslation::yTrasl(double theYTrasl) {
	if (isValid()) {
		mTraslation.y(mTraslation.y() + theYTrasl);
	}
}



/*!
Add a traslation value for z axis to traslation vector

\param theZTrasl
Traslation value for z axis
*/
void GM_Rototraslation::zTrasl(double theZTrasl) {
	if (isValid()) {
		mTraslation.z(mTraslation.z() + theZTrasl);
	}
}



/*!
\return
The traslation vector
*/
GM_3dVector GM_Rototraslation::traslation() const {
	return mTraslation;
}



/*!
Compute the euler rotation angles from rotation matrix and a specified rotation order

\param theRotOrder
Rotation order of axis for the angles computed from rotation matrix
\param theXAngle
On output contains the rotation on x axis, or DBL_MAX if the matrix is invalid
\param theYAngle
On output contains the rotation on y axis, or DBL_MAX if the matrix is invalid
\param theZAngle
On output contains the rotation on z axis, or DBL_MAX if the matrix is invalid
*/
void GM_Rototraslation::eulerAngles(GM_RotationOrder theRotOrder, double& theXAngle, double& theYAngle, double& theZAngle) const {
	theXAngle = theYAngle = theZAngle = DBL_MAX;
	if (!isValid()) {
		return;
	}

	switch(theRotOrder) {
		case GM_zxy :
			{
				theXAngle = asin(- (*this)[1][2]);
				double cosX = cos(theXAngle);
				
				if (fabs(theXAngle) < GM_NULL_TOLERANCE) {
					theZAngle = 0.0;
					double delta = atan2((*this)[0][1], (*this)[0][0]);
					if (fabs((*this)[1][2] - 1.0) < GM_NULL_TOLERANCE) {
						// Component value = -1, rotation angle on X axes is -PI/2

						theXAngle = - GM_PI / 2;
						theYAngle = delta - theZAngle;
					}
					else {
						// Component value = 1, rotation angle on X axes is PI/2
						
						theXAngle = GM_PI / 2;
						theXAngle = delta + theZAngle;
					}
				}
				else {
					theYAngle = atan2((*this)[0][2] / cosX, (*this)[2][2] / cosX);
					theZAngle = atan2((*this)[1][0] / cosX, (*this)[1][1] / cosX);
				}
			}
			break;
		case GM_zyx :
			{
				theYAngle = asin((*this)[0][2]);
				double cosY = cos(theYAngle);

				if (fabs(cosY) < GM_NULL_TOLERANCE) {
					theZAngle = 0.0;
					double delta = atan2((*this)[2][1], (*this)[2][0]);
					if (fabs((*this)[0][2] - 1.0) < GM_NULL_TOLERANCE) {
						// Component value = -1, rotation angle on Y axes is -PI/2
						
						theYAngle = - GM_PI / 2;
						theXAngle = - theZAngle + delta;
					}
					else {
						// Component value = 1, rotation angle on Y axes is PI/2
						
						theYAngle = GM_PI / 2;
						theXAngle = theZAngle + delta;
					}
				}
				else {
					theXAngle = - atan2((*this)[1][2] / cosY, (*this)[2][2] / cosY);
					theZAngle = - atan2((*this)[0][1] / cosY, (*this)[0][0] / cosY);
				}
			}
			break;
		case GM_xyz :
		default:
			{
				theYAngle = asin(- (*this)[2][0]);
				double cosY = cos(theYAngle);
				
				if (fabs(cosY) < GM_NULL_TOLERANCE) {
					theZAngle = 0.0;
					double delta = atan2((*this)[0][1], (*this)[0][2]);
					if (fabs((*this)[2][0] - 1.0) < GM_NULL_TOLERANCE) {
						// Component value = -1, rotation angle on Y axes is -PI/2
						
						theYAngle = - GM_PI / 2;
						theXAngle = - theZAngle + delta;
					}
					else {
						// Component value = 1, rotation angle on Y axes is PI/2
						
						theYAngle = GM_PI / 2;
						theXAngle = theZAngle + delta;
					}
				}
				else {
					theXAngle = atan2((*this)[2][1] / cosY, (*this)[2][2] / cosY);
					theZAngle = atan2((*this)[1][0] / cosY, (*this)[0][0] / cosY);
				}
			}
			break;
	}
}
