/********************************************************************
* File: GM_3dTrasf.h												*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#pragma once



#include "GM_matrix.h"

class GM_3dBasis;
class GM_3dPoint;
class GM_3dVector;



/*!
3D basis transformation
*/
class GM_3dTrasf : public GM_Matrix {
public:
	GM_3dTrasf();
	GM_3dTrasf(const GM_3dBasis& startBase, const GM_3dBasis& endBase);
	virtual ~GM_3dTrasf();

	GM_3dVector trasf(const GM_3dVector& theVect) const;
	GM_3dPoint trasf(const GM_3dPoint& thePoint) const;
};
