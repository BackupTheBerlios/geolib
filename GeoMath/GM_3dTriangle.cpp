/********************************************************************
* File: GM_3dTriangle.cpp											*
*********************************************************************
* Description:														*
*********************************************************************
* History:															*
* 22.4.2007 Created by : Claudio Cordara							*
*********************************************************************
*               (C) 2007 Claudio Cordara							*
********************************************************************/



#include "StdAfx.h"

#include "GM_3dPoint.h"
#include "GM_3dVector.h"
#include "GM_3dLine.h"
#include "GM_3dPlane.h"
#include "GM_3dTriangle.h"



/*!
Default constructor
*/
GM_3dTriangle::GM_3dTriangle(void) {
}



/*!
Default destructor
*/
GM_3dTriangle::~GM_3dTriangle(void) {
}



/*!
Copy constructor
*/
GM_3dTriangle::GM_3dTriangle(const GM_3dTriangle& theTriangle) {
	mEdge[0] = theTriangle.mEdge[0];
	mEdge[1] = theTriangle.mEdge[1];
	mEdge[2] = theTriangle.mEdge[2];
}



/*!
Constructor from three non-colinear vertex

\param theFirstPoint
First vertex
\param theSecondPoint
Second vertex
\param theThirdPoint
Third vertex
*/
GM_3dTriangle::GM_3dTriangle(GM_3dPoint theFirstPoint, GM_3dPoint theSecondPoint, GM_3dPoint theThirdPoint) {
	mEdge[0] = GM_3dLine(theFirstPoint, theSecondPoint);
	mEdge[1] = GM_3dLine(theSecondPoint, theThirdPoint);
	mEdge[2] = GM_3dLine(theThirdPoint, theFirstPoint);
}



/*!
Constructor from three non-parallel edges

\param theFirstEdge
First edge
\param theSecondEdge
Second edge
\param theThirdEdge
Third edge
*/
GM_3dTriangle::GM_3dTriangle(GM_3dLine theFirstEdge, GM_3dLine theSecondEdge, GM_3dLine theThirdEdge) {
	mEdge[0] = theFirstEdge;
	mEdge[1] = theSecondEdge;
	mEdge[2] = theThirdEdge;
}



/*!
\return
false if the edges are not valid, or if there is a pair of parallel edges, true otherwise
*/
bool GM_3dTriangle::isValid() const {
	if (!mEdge[0].isValid() || !mEdge[1].isValid() || !mEdge[2].isValid())
		return false;

	if ((mEdge[0]^mEdge[1]).isNull() || (mEdge[0]^mEdge[2]).isNull())
		return false;
	else
		return true;
}



/*!
Get a reference to an edge from its index

\param theIndex
Index of the edge to return by reference

\return
The reference to the edge with index theIndex
*/
GM_3dLine& GM_3dTriangle::operator[](unsigned short theIndex) {
	assert(theIndex <= 2);
	return mEdge[theIndex];
}



/*!
Get an edge from its index

\param theIndex
Index of the edge to return

\return
Th edge with index theIndex, or an invalid edge if theIndex is out of range or the triangle is not valid
*/
GM_3dLine GM_3dTriangle::operator[](unsigned short theIndex) const {
	if (theIndex <= 2)
		return mEdge[theIndex];
	else
		return GM_3dLine();
}



/*!
Invert the edges of the triangle
*/
void GM_3dTriangle::invert() {
	for (int i = 0 ; i < 3 ; i++) {
		mEdge[i].invert();	
	}
}



/*!
Sort the edges of the triangle in a way that the projection of the triangle on xy plane is a clockwise/
counterclockwise triangle basing on a parameter.
If the triangle is invalid or vertical nothing happens

\param theClockwiseFlag
If true the edges are sorted in a way that the projection of the triangle on xy plane is clockwise,
counterclockwise if false
*/
void GM_3dTriangle::setXYVersus(bool theClockwiseFlag) {
	if (isValid() && !isVertical()) {
		GM_3dVector v0(mEdge[0]);
		GM_3dVector v1(mEdge[1]);
		bool isClockwise = v0.isAtLeftOnXY(v1);
		if (isClockwise != theClockwiseFlag) {
			invert();
		}
	}
}



/*!
\return
Maximum Z value of the triangle, or -DBL_MAX if the triangle is not valid
*/
double GM_3dTriangle::maxZ() const {
	double ret = -DBL_MAX;
	if (isValid()) {
		for (int i = 0 ; i < 3 ; i++) {
			double edgeMaxZ = mEdge[i].maxZ();
			if (edgeMaxZ > ret) {
				ret = edgeMaxZ;
			}
		}
	}
	
	return ret;
}



/*!
\return
Minimum Z value of the triangle, or DBL_MAX if the triangle is not valid
*/
double GM_3dTriangle::minZ() const {
	double ret = DBL_MAX;
	if (isValid()) {
		for (int i = 0 ; i < 3 ; i++) {
			double edgeMinZ = mEdge[i].minZ();
			if (edgeMinZ < ret) {
				ret = edgeMinZ;
			}
		}
	}
	
	return ret;
}



/*!
\return
Inner angle formed by the plane that contains the triangle and the xy plane, return a value in the
interval [0 ; 2*PI], positive angles is counterclockwise, or DBL_MAX if the triangle
is not valid
*/
double GM_3dTriangle::xyAngle() const {
	GM_3dPlane trPlane(*this);
	return trPlane.xyAngle();
}



/*!
\return
Vector normal to the plane that contains the triangle, or an invalid vector if the triangle is not valid
*/
GM_3dPoint GM_3dTriangle::normalVector() const {
	GM_3dVector ret;
	if (isValid()) {
		GM_3dPlane trPlane(*this);
		ret = trPlane.normalVector();
	}

	return ret;
}



/*!
Compute a vector normal to the projection of an edge on xy plane and directed outside the triangle

\param theEdgeIndex
Index of the edge to use in normal vector computation

\return
The vector normal to the projection of the edge with index theEdgeIndex on xy plane and directed outside the
triangle, or an invalid vector if the triangle is invalid or theEdgeIndex is out of range
*/
GM_3dVector GM_3dTriangle::normalVectorOnEdge(unsigned short theEdgeIndex) const {
	GM_3dVector ret;
	if (!isValid() || theEdgeIndex > 2)
		return ret;

	int precEdgeIndex = theEdgeIndex - 1;
	if (precEdgeIndex == -1)
		precEdgeIndex = 2;

	GM_3dLine precEdge = mEdge[precEdgeIndex];
	precEdge.invert();

	// Cross product of the edge with index theEdgeIndex with that with precedent index
	GM_3dLine v1 = mEdge[theEdgeIndex]^precEdge;
	// Cross product of the edge with index theEdgeIndex with that from previous cross product, the
	// result is a vector norma to the edge theEdgeIndex directed outside the triangle
	GM_3dLine v2 = mEdge[theEdgeIndex]^v1;

	double module = ret.mod();
	if (module > GM_NULL_TOLERANCE) {
		ret.x(v2.dx() / module);
		ret.y(v2.dy() / module);
		ret.z(0.0);
	}
	else {
		ret.x(0.0);
		ret.y(0.0);
		ret.z(0.0);
	}

	return ret;
}



/*!
\return
true if the triangle is valid and horizontal, its normal vector has only the z-component not null, false
otherwise
*/
bool GM_3dTriangle::isHorizontal() const {
	if (isValid()) {
		GM_3dPoint normVect = normalVector();
		if (fabs(normVect.x()) < GM_NULL_TOLERANCE && fabs(normVect.y()) < GM_NULL_TOLERANCE && fabs(normVect.z()) > GM_NULL_TOLERANCE)
			return true;
		else
			return false;
	}
	else {
		return false;
	}
}



/*!
\return
true if the triangle is valid and vertical, its normal vector has a null z component, false otherwise
*/
bool GM_3dTriangle::isVertical() const {
	if (isValid()) {
		GM_3dPoint normVect = normalVector();
		if ((fabs(normVect.x()) > GM_NULL_TOLERANCE || fabs(normVect.y()) > GM_NULL_TOLERANCE) && fabs(normVect.z()) < GM_NULL_TOLERANCE)
			return true;
		else
			return false;
	}
	else {
		return false;
	}
}



/*!
\return
true if the edges of the triangle is connected in sequence, false otherwise or if the triangle is not valid
*/
bool GM_3dTriangle::isConnected() const {
	if (isValid() && mEdge[0].end().distFrom(mEdge[1].begin()) < GM_DIFF_TOLERANCE &&
		mEdge[1].end().distFrom(mEdge[2].begin()) < GM_DIFF_TOLERANCE &&
		mEdge[2].end().distFrom(mEdge[0].begin()) < GM_DIFF_TOLERANCE)
			return true;
	else
			return false;
}



/*!
Compute a line on the triangle at the specified z

\param theZLevel
Value of z to use in computation

\return
The line on the triangle at z equal to theZLevel, return an invalid line if theZLevel is out of range or if
the triangle is invalid
*/
GM_3dLine GM_3dTriangle::zSection(double theZLevel) const {
	GM_3dLine ret;
	if (!isValid())
		return ret;

	double minZVal = minZ();
	double maxZVal = maxZ();
	if (theZLevel > minZVal + GM_DIFF_TOLERANCE && theZLevel < maxZVal - GM_DIFF_TOLERANCE) {
		bool startPointFound = false;
		bool endPointFound = false;
		for (unsigned int i = 0 ; !endPointFound && i < 3 ; i++) {
			GM_3dPoint edgePoint = mEdge[i].pointAtZ(theZLevel);
			if (edgePoint.isValid()) {
				if (startPointFound) {
					ret.end(edgePoint);
					endPointFound = true;
				}
				else {
					ret.begin(edgePoint);
					startPointFound = true;
				}
			}
		}
	}

	return ret;
}



/*!
Invalidate the triangle
*/
void GM_3dTriangle::invalidate() {
	for (int i = 0 ; i < 3 ; i++) {
		mEdge[i].invalidate();
	}
}


/*!
Determine if a point or its projection on the plane that contains the triangle is an interior point
of the triangle

\param thePoint
Point to check

\return
true if thePoint or its projection on the plane that contains the triangle is an interior point
of the triangle, false otherwise or if the triangle or thePoint are invalid
*/
bool GM_3dTriangle::isInteriorPoint(GM_3dPoint thePoint) const {
	bool ret = false;
	if (!isValid() || !thePoint.isValid())
		return ret;

	GM_3dVector bVect(mEdge[0].begin());
	GM_3dVector E0Vect(mEdge[0].end());
	GM_3dVector E1Vect;
	if (mEdge[1].begin() != mEdge[0].begin() && mEdge[1].begin() != mEdge[0].end()) {
		E1Vect = (GM_3dVector)(mEdge[1].begin());
	}
	else {
		E1Vect = (GM_3dVector)(mEdge[1].end());
	}
	GM_3dVector pointVect = (GM_3dVector)thePoint;

	GM_3dVector dVect = bVect - pointVect;
	double a = E0Vect * E0Vect;
	double b = E0Vect * E1Vect;
	double c = E1Vect * E1Vect;
	double d = E0Vect * dVect;
	double e = E1Vect * dVect;

	double det = a*c - b*b;
	double s = b*e - c*d;
	double t = b*d - a*e;

	if (s + t <= det && s >= 0.0 && t >= 0.0) {
		ret = true;
	}
	else {
		ret = false;
	}

	return ret;
}



/*!
Compute the distance of a point from the triangle

\thePoint
Point for distance computation

\return
The distance of a thePoint from the triangle, or DBL_MAX if the triangle or thePoint are invalid
*/
double GM_3dTriangle::pointDistance(GM_3dPoint thePoint) const {
	double ret = DBL_MAX;
	if (!isValid() || !thePoint.isValid())
		return ret;

	if (isInteriorPoint(thePoint)) {
		// thePoint or its projection is an interior point of the triangle, the distance is just the distrance
		// from thePoint to the plane that contains the triangle

		GM_3dPlane triPlane(*this);
		ret = triPlane.pointDistance(thePoint);
	}
	else {
		// thePoint is not an interior point of the triangle, the distance is the minimu among the distances
		// between thePoint and the three edges of the triangle

		for (unsigned int i = 0 ; i < 3  ; i++) {
			double d = mEdge[i].pointDistance(thePoint);
			if (d < ret) {
				ret = d;
			}
		}
	}

	return ret;
}
