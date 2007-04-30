/********************************************************************
* File: GM_3dBasis.cpp												*
*********************************************************************
* Descrizione:														*
*********************************************************************
* History:															*
* 22.4.2007 Creato da : Cordara Claudio								*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_3dBasis.h"



GM_3dBasis::GM_3dBasis() {
}



GM_3dBasis::GM_3dBasis(const GM_3dBasis& theBasis) {
	mGen[0] = theBasis.mGen[0];
	mGen[1] = theBasis.mGen[1];
	mGen[2] = theBasis.mGen[2];
}



GM_3dBasis::GM_3dBasis(GM_3dVector theGen1, GM_3dVector theGen2, GM_3dVector theGen3) {
	mGen[0] = theGen1;
	mGen[1] = theGen2;
	mGen[2] = theGen3;
}



GM_3dBasis::~GM_3dBasis() {
}


bool GM_3dBasis::isValid() const {
	if (mGen[0].isValid() && mGen[1].isValid() && mGen[2].isValid())
		return true;
	else
		return false;
}


void GM_3dBasis::invalidate() {
	mGen[0].invalidate();
	mGen[1].invalidate();
	mGen[2].invalidate();
}


void GM_3dBasis::normalize() {
	mGen[0].normalize();
	mGen[1].normalize();
	mGen[2].normalize();
}


bool GM_3dBasis::isLinearlyInd() const {
	if ((mGen[0]^mGen[1]).mod() > GM_NULL_TOLERANCE && (mGen[0]^mGen[2]).mod() > GM_NULL_TOLERANCE && (mGen[1]^mGen[2]).mod() > GM_NULL_TOLERANCE)
		return true;
	else
		return false;
}
