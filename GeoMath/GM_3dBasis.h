/********************************************************************
* File: GM_3dBasis.h												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara								*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once


#include "GM_3dVector.h"



/*!
3D vector space basis
*/
class GM_3dBasis {
public:
	GM_3dBasis();
	GM_3dBasis(const GM_3dBasis& theBasis);
	GM_3dBasis(GM_3dVector theGen1, GM_3dVector theGen2, GM_3dVector theGen3);
	~GM_3dBasis();

	GM_3dVector operator[](unsigned short theIndex) const;

	bool isValid() const;
	void invalidate();
	void normalize();
	bool isLinearlyInd() const;
private:
	/*!
	The three generators of the basis
	*/
	GM_3dVector mGen[3];
};
