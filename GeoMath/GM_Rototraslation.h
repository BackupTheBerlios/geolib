/********************************************************************
* File: GM_Rototraslation.h											*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once

#include "GM_3dVector.h"
#include "GM_3dTrasf.h"



/*!
Axis rotation order
*/
typedef enum {
	GM_xyz,			/// Rotation on X axes, then on Y axes, then on Z axes
	GM_zxy,			/// Rotation on Z axes, then on X axes, then on Y axes
	GM_zyx			/// Rotation on Z axes, then on Y axes, then on X axes
} GM_RotationOrder;



/*!
3D Rototraslation (affine transformation, that is a linear transformation and a traslation)
*/
class GM_Rototraslation : public GM_3dTrasf {
public:
	GM_Rototraslation();
	GM_Rototraslation(const GM_Rototraslation& theRototras);
	GM_Rototraslation(double theXAngle, double theYAngle, double theZAngle, double theXTrasl, double theYTrasl, double theZTrasl);
	virtual ~GM_Rototraslation();

	GM_Rototraslation& operator=(const GM_Rototraslation& theRototras);
	GM_Rototraslation operator*(const GM_Rototraslation& theRototras) const;

	void xRotation(double theXAngle);
	void yRotation(double theYAngle);
	void zRotation(double theZAngle);

	void xTrasl(double theXTrasl);
	void yTrasl(double theYTrasl);
	void zTrasl(double theZTrasl);

	void eulerAngles(GM_RotationOrder theRotOrder, double& theXAngle, double& theYAngle, double& theZAngle) const;
	GM_3dVector traslation() const;

private:
	GM_3dVector mTraslation;
};
