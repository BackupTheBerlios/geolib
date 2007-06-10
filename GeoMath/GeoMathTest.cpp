// GeoMathTest.cpp : Defines the entry point for the console application.
//

#include "stdafxt.h"

#include <float.h>
#include <math.h>



void getLogFilename(TCHAR *fileName) {
	if (!fileName)
		return;

	time_t testStartTime;
	time(&testStartTime);
	struct tm *localTime = NULL;
	localTime = localtime(&testStartTime);
	srand((unsigned int)testStartTime);
	
	//_stprintf(fileName, _T("testLog%d%d%d_%d%d.txt"), localTime -> tm_mday, localTime -> tm_mon, localTime -> tm_year, localTime -> tm_hour, localTime -> tm_min, localTime -> tm_sec);
	_stprintf(fileName, _T("testLog.txt"));
}


double getRandomDouble() {
	double ret = 0.0;

	ret = (double)(rand() - (RAND_MAX/2));
	ret = ret + ((double)rand() / (double)RAND_MAX);

	return ret;
}

double getRandom01Double() {
	double ret = 0.0;

	ret = ((double)rand() / (double)RAND_MAX);

	return ret;
}

void logMessage(TCHAR *msg) {
	if (!msg)
		return;

	if (testLogFile) {
		_ftprintf(testLogFile, msg);
	}
	if (consoleTrace) {
		_tprintf(msg);
	}
}

void logHLine() {
	if (testLogFile) {
		_ftprintf(testLogFile, _T("\n\n----------------------------------------------------------\n\n"));
	}
	if (consoleTrace) {
		_tprintf(_T("\n\n----------------------------------------------------------\n\n"));
	}
}


double getRandomAngle() {
	double ret = 0.0;

	ret = (((double)rand() - 2.0*GM_PI) / ((double)RAND_MAX)) - GM_PI;
	return ret;
}


int testPoint() {
	int numErr = 0;

	logMessage(_T("TESTING  -  class GM_3dPoint ...\n\n"));

	// Default constructor, point must be invalid
	GM_3dPoint pt;
	if (pt.isValid()) {
		logMessage(_T("\tERROR - Default constructor creates valid points\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Default constructor creates invalid points\n"));
	}

	// Get/Set points coordinates
	double x = getRandomDouble();
	double y = getRandomDouble();
	double z = getRandomDouble();
	pt.x(x); pt.y(y); pt.z(z);
	if (pt.x() != x || pt.y() != y || pt.z() != z) {
		logMessage(_T("\tERROR - Get/Set not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Get/Set working\n"));
	}

	// Copy constructor
	GM_3dPoint pt1(pt);
	if (pt.isValid() != pt1.isValid() || pt1.x() != pt.x() || pt1.y() != pt.y() || pt1.z() != pt.z()) {
		logMessage(_T("\tERROR - Copy constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Copy constructor working\n"));
	}

	// Constructor (coordinates)
	x = getRandomDouble();
	y = getRandomDouble();
	z = getRandomDouble();
	GM_3dPoint pt2(x, y, z);
	if (!pt2.isValid() || pt2.x() != x || pt2.y() != y || pt2.z() != z) {
		logMessage(_T("\tERROR - Coordinate constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Coordinate constructor working\n"));
	}

	// Constructor (angle)
	double ang = getRandomAngle();
	GM_3dPoint pt3(ang);
	if (!pt2.isValid() || pt3.x() != cos(ang) || pt3.y() != sin(ang) || pt3.z() != 0.0) {
		logMessage(_T("\tERROR - XY angle constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - XY angle constructor working\n"));
	}
	
	// Invalidation
	pt.invalidate();
	if (pt.isValid()) {
		logMessage(_T("\tERROR - Invalidation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Invalidation working\n"));
	}

	// Inversion
	GM_3dPoint invPt(pt2);
	invPt.xyInvert();
	if (invPt.isValid() != pt2.isValid() || invPt.x() != -pt2.x() || invPt.y() != -pt2.y() || invPt.z() != pt2.z()) {
		logMessage(_T("\tERROR - Inversion not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Inversion working\n"));
	}

	// Origin check
	GM_3dPoint origPt(GM_NULL_TOLERANCE / 2.0, -GM_NULL_TOLERANCE / 2.0, 0.0);
	if (pt2.isOrigin() || !origPt.isOrigin()) {
		logMessage(_T("\tERROR - Origin check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Origin check working\n"));
	}

	// Distance
	GM_3dPoint startPt(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint endPt(getRandomDouble(), getRandomDouble(), getRandomDouble());
	double dx = startPt.x() - endPt.x();
	double dy = startPt.y() - endPt.y();
	double dz = startPt.z() - endPt.z();
	double dist = sqrt(dx*dx + dy*dy + dz*dz);
	if (!startPt.isValid() || !endPt.isValid() || startPt.distFrom(endPt) != dist) {
		logMessage(_T("\tERROR - Distance computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Distance computation working\n"));
	}

	// Equality
	GM_3dPoint ptEq1(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint ptEq2(ptEq1);
	GM_3dPoint ptEq3(ptEq1.x() + getRandomDouble(), ptEq1.y() + getRandomDouble(), ptEq1.z() + getRandomDouble());
	if (ptEq1 != ptEq2 || ptEq1 == ptEq3) {
		logMessage(_T("\tERROR - Equality check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Equality check working\n"));
	}

	return numErr;
}
int testVector() {
	int numErr = 0;

	logMessage(_T("TESTING  -  class GM_3dVector ...\n\n"));

	// Default constructor, vector must be invalid
	GM_3dVector v;
	if (v.isValid()) {
		logMessage(_T("\tERROR - Default constructor creates valid vector\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Default constructor creates invalid vector\n"));
	}

	// Get/Set vector coordinates
	double x = getRandomDouble();
	double y = getRandomDouble();
	double z = getRandomDouble();
	v.x(x); v.y(y); v.z(z);
	if (!v.isValid() || v.x() != x || v.y() != y || v.z() != z) {
		logMessage(_T("\tERROR - Get/Set not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Get/Set working\n"));
	}

	// Copy constructor
	GM_3dVector v1(v);
	if (v.isValid() != v1.isValid() || v1.x() != v.x() || v1.y() != v.y() || v1.z() != v.z()) {
		logMessage(_T("\tERROR - Copy constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Copy constructor working\n"));
	}

	// Constructor (point)
	GM_3dPoint pt(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dVector v2(pt);
	if (!v2.isValid() || v2.x() != pt.x() || v2.y() != pt.y() || v2.z() != pt.z()) {
		logMessage(_T("\tERROR - Constructor from point not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Constructor from point working\n"));
	}

	// Constructor (line)
	GM_3dLine line(getRandomDouble(), getRandomDouble(), getRandomDouble(), getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dVector v3(line);
	if (!v3.isValid() || v3.x() != line.end().x() - line.begin().x() || v3.y() != line.end().y() - line.begin().y() || v3.z() != line.end().z() - line.begin().z()) {
		logMessage(_T("\tERROR - Constructor from line not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Constructor from line working\n"));
	}

	// Constructor (angle)
	double ang = getRandomAngle();
	GM_3dVector v4(ang);
	if (!v4.isValid() || v4.x() != cos(ang) || v4.y() != sin(ang) || v4.z() != 0.0) {
		logMessage(_T("\tERROR - XY angle constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - XY angle constructor working\n"));
	}

	// Module
	double module = v.mod();
	if (module != sqrt(v.x()*v.x() + v.y()*v.y() + v.z()*v.z())) {
		logMessage(_T("\tERROR - Module computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Module computation working\n"));
	}

	// Normalization
	v.normalize();
	if (fabs(v.mod()-1.0) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Normalization not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Normalization working\n"));
	}

	// xy Angle
	double checkAng = v4.xyAngle();
	if (checkAng > GM_PI) {
		checkAng -= 2.0 * GM_PI;
	}
	if (fabs(checkAng - ang) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - xy angle computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - xy angle computation working\n"));
	}

	// Angle between vectors
	ang = getRandomAngle();
	GM_3dVector v5(ang);
	GM_3dVector baseVect(0.0);
	checkAng = baseVect.xyAngle(v5);
	if (checkAng > GM_PI) {
		checkAng -= 2.0 * GM_PI;
	}
	double checkAng1 = v5.xyAngle(baseVect);
	if (checkAng1 > GM_PI) {
		checkAng1 -= 2.0 * GM_PI;
	}
	if (fabs(checkAng - ang) > GM_NULL_TOLERANCE || fabs(-checkAng1 - ang) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Angle between vectors computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Angle between vectors computation working\n"));
	}

	// At left
	ang = getRandomAngle();
	if (ang < 0.0) {
		ang = 2.0 * GM_PI + ang;
	}
	double ang1 = getRandomAngle();
	if (ang1 < 0.0) {
		ang1 = 2.0 * GM_PI + ang1;
	}
	GM_3dVector v6(ang);
	GM_3dVector v7(ang1);
	bool v6AtLeftv7 = v6.isAtLeftOnXY(v7);
	bool v7AtLeftv6 = v7.isAtLeftOnXY(v6);
	double checkv6Ang = v6.xyAngle();
	double checkv7Ang = v7.xyAngle();
	bool checkv6AtLeftv7 = ang > ang1 ? true : false;
	if (fabs(ang - checkv6Ang) > GM_NULL_TOLERANCE || fabs(ang1 - checkv7Ang) > GM_NULL_TOLERANCE || checkv6AtLeftv7 != v6AtLeftv7 || v6AtLeftv7 == v7AtLeftv6) {
		logMessage(_T("\tERROR - At left not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - At left working\n"));
	}

	// Dot product
	GM_3dVector v8(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dVector v9(getRandomDouble(), getRandomDouble(), getRandomDouble());
	double dotProd = v8 * v9;
	double dotProd1 = v9 * v8;
	double checkDotProd = v8.x()*v9.x() + v8.y()*v9.y() + v8.z()*v9.z();
	if (dotProd != checkDotProd || dotProd1 != checkDotProd) {
		logMessage(_T("\tERROR - Dot product not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Dot product working\n"));
	}

	// Cross product
	GM_3dVector crossProd = v8 ^ v9;
	GM_3dVector crossProd1 = v9 ^ v8;
	double checkCrossProdX = v8.y()*v9.z() - v8.z()*v9.y();
	double checkCrossProdY = v8.z()*v9.x() - v8.x()*v9.z();
	double checkCrossProdZ = v8.x()*v9.y() - v8.y()*v9.x();
	if (crossProd.x() != checkCrossProdX || crossProd.y() != checkCrossProdY || crossProd.z() != checkCrossProdZ ||
		crossProd1.x() != -checkCrossProdX || crossProd1.y() != -checkCrossProdY || crossProd1.z() != -checkCrossProdZ) {
		logMessage(_T("\tERROR - Cross product not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Cross product working\n"));
	}

	// Scale
	double factor = getRandomDouble();
	GM_3dVector v8Scaled = v8 * factor;
	if (v8Scaled.x() != v8.x()*factor || v8Scaled.y() != v8.y()*factor || v8Scaled.z() != v8.z()*factor) {
		logMessage(_T("\tERROR - Scaling not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Scaling working\n"));
	}

	// Sum
	GM_3dVector sum = v8 + v9;
	if (sum.x() != v8.x()+v9.x() || sum.y() != v8.y()+v9.y() || sum.z() != v8.z()+v9.z()) {
		logMessage(_T("\tERROR - Sum not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Sum working\n"));
	}

	// Difference
	GM_3dVector diff = v8 - v9;
	if (diff.x() != v8.x()-v9.x() || diff.y() != v8.y()-v9.y() || diff.z() != v8.z()-v9.z()) {
		logMessage(_T("\tERROR - Difference not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Difference working\n"));
	}

	return numErr;
}
int testLine() {
	int numErr = 0;

	logMessage(_T("TESTING  -  class GM_3dLine ...\n\n"));

	// Default constructor, line must be invalid
	GM_3dLine ln;
	if (ln.isValid()) {
		logMessage(_T("\tERROR - Default constructor creates valid line\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Default constructor creates invalid line\n"));
	}

	// Get/Set line coordinates
	double xStart = getRandomDouble();
	double yStart = getRandomDouble();
	double zStart = getRandomDouble();
	double xEnd = getRandomDouble();
	double yEnd = getRandomDouble();
	double zEnd = getRandomDouble();
	ln.begin().x(xStart);
	ln.begin().y(yStart);
	ln.begin().z(zStart);
	ln.end().x(xEnd);
	ln.end().y(yEnd);
	ln.end().z(zEnd);
	if (!ln.isValid() || ln.begin().x() != xStart || ln.begin().y() != yStart || ln.begin().z() != zStart ||
				ln.end().x() != xEnd || ln.end().y() != yEnd || ln.end().z() != zEnd) {
		logMessage(_T("\tERROR - Get/Set not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Get/Set working\n"));
	}

	// Copy constructor
	GM_3dLine ln1(ln);
	if (ln1.isValid() != ln.isValid() || ln1.begin().x() != ln.begin().x() || ln1.begin().y() != ln.begin().y() || ln1.begin().z() != ln.begin().z() ||
				ln1.end().x() != ln.end().x() || ln1.end().y() != ln.end().y() || ln1.end().z() != ln.end().z()) {
		logMessage(_T("\tERROR - Copy constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Copy constructor working\n"));
	}

	// Constructor (points)
	GM_3dPoint startPt(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint endPt(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dLine ln2(startPt, endPt);
	if (!ln2.isValid() || ln2.begin().x() != startPt.x() || ln2.begin().y() != startPt.y() || ln2.begin().z() != startPt.z() ||
			ln2.end().x() != endPt.x() || ln2.end().y() != endPt.y() || ln2.end().z() != endPt.z()) {
		logMessage(_T("\tERROR - Points constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Points constructor working\n"));
	}

	// Constructor (coordinates)
	xStart = getRandomDouble();
	yStart = getRandomDouble();
	zStart = getRandomDouble();
	xEnd = getRandomDouble();
	yEnd = getRandomDouble();
	zEnd = getRandomDouble();
	GM_3dLine ln3(xStart, yStart, zStart, xEnd, yEnd, zEnd);
	if (!ln3.isValid() || ln3.begin().x() != xStart || ln3.begin().y() != yStart || ln3.begin().z() != zStart ||
			ln3.end().x() != xEnd || ln3.end().y() != yEnd || ln3.end().z() != zEnd) {
		logMessage(_T("\tERROR - Coordinate constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Coordinate constructor working\n"));
	}

	// dx, dy, dz
	double dx = ln2.dx();
	double dy = ln2.dy();
	double dz = ln2.dz();
	if (dx != ln2.end().x()-ln2.begin().x() || dy != ln2.end().y()-ln2.begin().y() || dz != ln2.end().z()-ln2.begin().z()) {
		logMessage(_T("\tERROR - dx, dy or dz not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - dx, dy or dz working\n"));
	}

	// Dot product
	double dotProduct = ln2 * ln3;
	double dxLn2 = ln2.dx();
	double dyLn2 = ln2.dy();
	double dzLn2 = ln2.dz();
	double dxLn3 = ln3.dx();
	double dyLn3 = ln3.dy();
	double dzLn3 = ln3.dz();
	double checkDotProduct = dxLn2*dxLn3 + dyLn2*dyLn3 + dzLn2*dzLn3;
	if (dotProduct != checkDotProduct) {
		logMessage(_T("\tERROR - Dot product not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Dot product working\n"));
	}

	// Cross product
	GM_3dLine crossProdLn = ln2 ^ ln3;
	double checkCrossProdX = ln2.begin().x() + (dyLn2*dzLn3 - dzLn2*dyLn3);
	double checkCrossProdY = ln2.begin().y() + (dzLn2*dxLn3 - dxLn2*dzLn3);
	double checkCrossProdZ = ln2.begin().z() + (dxLn2*dyLn3 - dyLn2*dxLn3);
	if (!crossProdLn.isValid() || crossProdLn.end().x() != checkCrossProdX || crossProdLn.end().y() != checkCrossProdY || crossProdLn.end().z() != checkCrossProdZ) {
		logMessage(_T("\tERROR - Cross product not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Cross product working\n"));
	}

	// Center
	GM_3dPoint ln2Center = ln2.center();
	if (!ln2Center.isValid() || ln2Center.x() != ln2.begin().x() + (ln2.end().x()-ln2.begin().x())/2.0 ||
								ln2Center.y() != ln2.begin().y() + (ln2.end().y()-ln2.begin().y())/2.0 ||
								ln2Center.z() != ln2.begin().z() + (ln2.end().z()-ln2.begin().z())/2.0) {
		logMessage(_T("\tERROR - Center point computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Center point computation working\n"));
	}

	// Invert
	GM_3dLine ln2Copy(ln2);
	ln2.invert();
	if (!ln2Copy.isValid() || ln2Copy.begin().x() != ln2.end().x() || ln2Copy.begin().y() != ln2.end().y() || ln2Copy.begin().z() != ln2.end().z() ||
			ln2Copy.end().x() != ln2.begin().x() || ln2Copy.end().y() != ln2.begin().y() || ln2Copy.end().z() != ln2.begin().z()) {
		logMessage(_T("\tERROR - Inversion not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Inversion working\n"));
	}

	// Length
	double checkLen = sqrt(ln2.dx()*ln2.dx() + ln2.dy()*ln2.dy() + ln2.dz()*ln2.dz());
	if (ln2.length() != checkLen) {
		logMessage(_T("\tERROR - Length computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Length computation working\n"));
	}

	// Null check
	GM_3dLine nullLn(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	bool checkNull1 = nullLn.isNull();
	nullLn.begin().x(10.0);
	bool checkNull2 = nullLn.isNull();
	if (!nullLn.isValid() || !checkNull1 || checkNull2) {
		logMessage(_T("\tERROR - Null line check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Null line check working\n"));
	}

	// Vertical line check
	double xCoord = getRandomDouble();
	double yCoord = getRandomDouble();
	GM_3dLine vertLn(xCoord, yCoord, getRandomDouble(), xCoord, yCoord, getRandomDouble());
	bool checkVert1 = vertLn.isVertical();
	vertLn.begin().x(vertLn.begin().x() + 10.0);
	bool checkVert2 = vertLn.isVertical();
	if (!vertLn.isValid() || !checkVert1 || checkVert2) {
		logMessage(_T("\tERROR - Vertical line check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Vertical line check working\n"));
	}

	// Horizontal line check
	double zCoord = getRandomDouble();
	GM_3dLine horLn(getRandomDouble(), getRandomDouble(), zCoord, getRandomDouble(), getRandomDouble(), zCoord);
	bool checkHor1 = horLn.isHorizontal();
	horLn.begin().z(horLn.begin().z() + 10.0);
	bool checkHor2 = horLn.isHorizontal();
	if (!horLn.isValid() || !checkHor1 || checkVert2) {
		logMessage(_T("\tERROR - Horizontal line check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Horizontal line check working\n"));
	}

	// Z min/max
	double minZ = ln2.minZ();
	double maxZ = ln2.maxZ();
	double checkMin = ln2.begin().z() < ln2.end().z() ? ln2.begin().z() : ln2.end().z();
	double checkMax = ln2.begin().z() > ln2.end().z() ? ln2.begin().z() : ln2.end().z();
	if (minZ != checkMin || maxZ != checkMax) {
		logMessage(_T("\tERROR - min/max Z not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - min/max Z working\n"));
	}

	// Point on line at Z
	double sec = getRandom01Double();
	double dX = ln2.dx();
	double dY = ln2.dy();
	double dZ = ln2.dz();
	double xCheck = ln2.begin().x() + (sec * dX);
	double yCheck = ln2.begin().y() + (sec * dY);
	double zCheck = ln2.begin().z() + (sec * dZ);
	GM_3dPoint pointAtZ = ln2.pointAtZ(zCheck);
	if (!pointAtZ.isValid() || fabs(pointAtZ.x()-xCheck) > GM_NULL_TOLERANCE || fabs(pointAtZ.y()-yCheck) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Point on line at Z not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Point on line at Z working\n"));
	}

	// Nomal vector on XY
	GM_3dVector normalVectorR = ln2.normalXYVector(true);
	GM_3dVector normalVectorL = ln2.normalXYVector(false);
	GM_3dVector ln2Dir(ln2);
	double dotPR = normalVectorR * ln2Dir;
	double dotPL = normalVectorL * ln2Dir;
	bool RSide = ln2Dir.isAtLeftOnXY(normalVectorR);
	bool LSide = ln2Dir.isAtLeftOnXY(normalVectorL);
	if (!normalVectorR.isValid() || !normalVectorL.isValid() || fabs(dotPR) > GM_NULL_TOLERANCE || fabs(dotPL) > GM_NULL_TOLERANCE || !RSide || LSide) {
		logMessage(_T("\tERROR - XY normal vector computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - XY normal vector computation working\n"));
	}

	// Point from section
	double section = getRandom01Double();
	GM_3dPoint sectionPoint = ln2.pointFromSection(section);
	double ln2Len = ln2.length();
	double pointSectionLen = ln2.begin().distFrom(sectionPoint);
	if (fabs((pointSectionLen / ln2Len) - section) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Point from section not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Point from section working\n"));
	}

	// Section from point
	double checkSection = ln2.sectionFromPoint(sectionPoint);
	if (fabs(checkSection - section) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Section from point not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Section from point working\n"));
	}

	// Point distance
	ln2Dir.normalize();
	GM_3dVector ln2NormDir(ln2Dir);
	section = getRandom01Double();
	sectionPoint = ln2.pointFromSection(section);
	if (fabs(ln2Dir.x()) > GM_NULL_TOLERANCE) {
		ln2NormDir.x(getRandomDouble());
		ln2NormDir.y(getRandomDouble());
		ln2NormDir.z(getRandomDouble());
		ln2NormDir.normalize();
		ln2NormDir.x(-(ln2Dir.y()*ln2NormDir.y() + ln2Dir.z()*ln2NormDir.z()) / ln2Dir.x());
	}
	else if (fabs(ln2Dir.y()) > GM_NULL_TOLERANCE) {
		ln2NormDir.x(getRandomDouble());
		ln2NormDir.y(getRandomDouble());
		ln2NormDir.z(getRandomDouble());
		ln2NormDir.normalize();
		ln2NormDir.y(-(ln2Dir.x()*ln2NormDir.x() + ln2Dir.z()*ln2NormDir.z()) / ln2Dir.y());
	}
	else if (fabs(ln2Dir.z()) > GM_NULL_TOLERANCE) {
		ln2NormDir.x(getRandomDouble());
		ln2NormDir.y(getRandomDouble());
		ln2NormDir.z(getRandomDouble());
		ln2NormDir.normalize();
		ln2NormDir.z(-(ln2Dir.x()*ln2NormDir.x() + ln2Dir.y()*ln2NormDir.y()) / ln2Dir.z());
	}
	else {
		ln2NormDir.invalidate();
	}
	double dist = 0.0;
	double checkDist = 0.0;
	GM_3dPoint pointOnLine;
	if (ln2NormDir.isValid()) {
		ln2NormDir.normalize();

		double dotProd = ln2NormDir * ln2Dir;

		dist = fabs(getRandomDouble());
		GM_3dPoint externPoint(sectionPoint);
		externPoint = (GM_3dVector)externPoint + (ln2NormDir * dist);
		checkDist = ln2.pointDistance(externPoint, pointOnLine);
	}
	if (!pointOnLine.isValid() || fabs(dist - checkDist) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Point distance not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Point distance working\n"));
	}


	return numErr;
}
int testPlane() {
	int numErr = 0;

	logMessage(_T("TESTING  -  class GM_3dPlane ...\n\n"));

	// Default constructor, plane must be invalid
	GM_3dPlane p;
	if (p.isValid()) {
		logMessage(_T("\tERROR - Default constructor creates valid plane\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Default constructor creates invalid plane\n"));
	}

	// Constructor (coeff)
	GM_3dPlane p1(getRandomDouble(), getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPlane pNull(0.0, 0.0, 0.0, getRandomDouble());
	if (!p1.isValid() || pNull.isValid()) {
		logMessage(_T("\tERROR - Coeff. constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Coeff. constructor working\n"));
	}

	// Copy constructor
	GM_3dPlane copyPlane(p1);
	if (!copyPlane.isValid() || copyPlane != p1) {
		logMessage(_T("\tERROR - Copy constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Copy constructor working\n"));
	}

	// Constructor (coeff vector)
	double pointsVect[4];
	for (int i = 0 ; i < 4 ; i++) {
		pointsVect[i] = getRandomDouble();
	}
	GM_3dPlane p2(pointsVect);
	if (!p2.isValid()) {
		logMessage(_T("\tERROR - Coeff. vector constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Coeff. vector constructor working\n"));
	}

	// Constructor (points)
	GM_3dPoint pt1(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint pt2(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint pt3(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dLine ln(getRandomDouble(), getRandomDouble(), getRandomDouble(),getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint ptLn1 = ln.begin();
	GM_3dPoint ptLn2 = ln.center();
	GM_3dPoint ptLn3 = ln.end();
	GM_3dPlane p3(pt1, pt2, pt3);
	GM_3dPlane pLine(ptLn2, ptLn2, ptLn3);
	double distPt1 = pt1.x()*p3[0] + pt1.y()*p3[1] + pt1.z()*p3[2] + p3[3];
	double distPt2 = pt2.x()*p3[0] + pt2.y()*p3[1] + pt2.z()*p3[2] + p3[3];
	double distPt3 = pt3.x()*p3[0] + pt3.y()*p3[1] + pt3.z()*p3[2] + p3[3];
	if (!p3.isValid() || pLine.isValid() || fabs(distPt1) > GM_NULL_TOLERANCE || fabs(distPt2) > GM_NULL_TOLERANCE || fabs(distPt3) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Points constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Points constructor working\n"));
	}

	// Point distance
	GM_3dPlane p4(getRandomDouble(), getRandomDouble(), getRandomDouble(),getRandomDouble());
	GM_3dPoint checkPoint(getRandomDouble(), getRandomDouble(), getRandomDouble());
	if (fabs(p4[0]) > GM_NULL_TOLERANCE) {
		checkPoint.x(-(checkPoint.y()*p4[1] + checkPoint.z()*p4[2] + p4[3]) / p4[0]);
	}
	else if (fabs(p4[1]) > GM_NULL_TOLERANCE) {
		checkPoint.y(-(checkPoint.x()*p4[0] + checkPoint.z()*p4[2] + p4[3]) / p4[1]);
	}
	else if (fabs(p4[2]) > GM_NULL_TOLERANCE) {
		checkPoint.z(-(checkPoint.x()*p4[0] + checkPoint.y()*p4[1] + p4[3]) / p4[2]);
	}
	else {
		p4.invalidate();
	}
	double checkSignedDist = getRandomDouble();
	double checkDist = fabs(checkSignedDist);
	GM_3dVector p4Norm = p4.normalVector();
	checkPoint = (GM_3dVector)checkPoint + (p4Norm * checkSignedDist);
	GM_3dPoint checkPointOnPlane1;
	GM_3dPoint checkPointOnPlane2;
	double dist = p4.pointDistance(checkPoint);
	double signedDist = p4.pointDistanceSgn(checkPoint);
	double signedDistOnPlane = p4.pointDistanceSgn(checkPoint, checkPointOnPlane1);
	double distOnPlane = p4.pointDistance(checkPoint, checkPointOnPlane2);
	
	distPt1 = checkPointOnPlane1.x()*p4[0] + checkPointOnPlane1.y()*p4[1] + checkPointOnPlane1.z()*p4[2] + p4[3];
	distPt2 = checkPointOnPlane2.x()*p4[0] + checkPointOnPlane2.y()*p4[1] + checkPointOnPlane2.z()*p4[2] + p4[3];
	
	if (!p4.isValid() || !checkPointOnPlane1.isValid() || !checkPointOnPlane2.isValid() ||
		fabs(distPt1) > GM_NULL_TOLERANCE || fabs(distPt2) > GM_NULL_TOLERANCE ||
		fabs(distOnPlane - checkDist) > GM_NULL_TOLERANCE || fabs(signedDistOnPlane - checkSignedDist) > GM_NULL_TOLERANCE ||
		fabs(dist - checkDist) > GM_NULL_TOLERANCE || fabs(signedDist - checkSignedDist) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Point distance computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Point distance computation working\n"));
	}

	// XY Angle
	double angle = ((((double)rand()) / ((double)RAND_MAX)) * GM_PI) - (GM_PI / 2.0);
	GM_3dVector normVector(angle/* + GM_HALFPI*/);
	normVector.z(normVector.y());
	normVector.y(0.0);
	GM_3dPlane angleP(normVector, GM_3dPoint(getRandomDouble(), getRandomDouble(), getRandomDouble()));
	double checkAngle = angleP.xyAngle();
	if (checkAngle > GM_PI) {
		checkAngle -= 2.0 * GM_PI;
	}
	if (!angleP.isValid() || fabs(angle - checkAngle) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - XY angle computation not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - XY angle computation working\n"));
	}


	return numErr;
}
int testTriangle() {
	int numErr = 0;

	logMessage(_T("TESTING  -  class GM_3dTriangle ...\n\n"));

	// Default constructor, triangle must be invalid
	GM_3dTriangle tr;
	if (tr.isValid()) {
		logMessage(_T("\tERROR - Default constructor creates valid triangle\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Default constructor creates invalid triangle\n"));
	}

	// Get/Set
	GM_3dPoint v1(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint v2(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dPoint v3(getRandomDouble(), getRandomDouble(), getRandomDouble());
	tr[0] = GM_3dLine(v1, v2);
	tr[1] = GM_3dLine(v2, v3);
	tr[2] = GM_3dLine(v3, v1);
	if (!tr.isValid() || tr[0].begin() != v1 || tr[0].end() != v2 || tr[1].begin() != v2 || tr[1].end() != v3
				|| tr[2].begin() != v3 || tr[2].end() != v1) {
		logMessage(_T("\tERROR - Get/Set not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Get/Set working\n"));
	}

	// Copy constructor
	GM_3dTriangle tr1(tr);
	if (!tr1.isValid() || tr[0] != tr1[0] || tr[1] != tr1[1] || tr[2] != tr1[2]) {
		logMessage(_T("\tERROR - Copy constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Copy constructor working\n"));
	}

	// Constructor (points)
	GM_3dTriangle tr2(v1, v2, v3);
	if (!tr1.isValid() || tr[0] != tr2[0] || tr[1] != tr2[1] || tr[2] != tr2[2]) {
		logMessage(_T("\tERROR - Constructor (points) not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Constructor (points) working\n"));
	}

	// Constructor (lines)
	GM_3dTriangle tr3(tr[0], tr[1], tr[2]);
	if (!tr1.isValid() || tr[0] != tr3[0] || tr[1] != tr3[1] || tr[2] != tr3[2]) {
		logMessage(_T("\tERROR - Constructor (lines) not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Constructor (lines) working\n"));
	}

	// Inversion
	tr1.invert();
	if (!tr1.isValid() || tr1[0].begin() != tr[0].end() || tr1[0].end() != tr[0].begin() || tr1[1].begin() != tr[1].end() || tr1[1].end() != tr[1].begin() ||
				tr1[2].begin() != tr[2].end() || tr1[2].end() != tr[2].begin()) {
		logMessage(_T("\tERROR - Triangle inversion not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Triangle inversion working\n"));
	}

	// Vertical check
	GM_3dPoint vPoint = GM_3dLine(v1, v2).center();
	vPoint.z(vPoint.z() + getRandomDouble());
	GM_3dTriangle VTr(v1, v2, vPoint);
	if (!VTr.isValid() || !VTr.isVertical()) {
		logMessage(_T("\tERROR - Vertical check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Vertical check working\n"));
	}

	// Horizontal check
	GM_3dTriangle HTr(tr);
	double z = getRandomDouble();
	for (unsigned int i = 0 ; i < 3 ; i++) {
		HTr[i].begin().z(z);
		HTr[i].end().z(z);
	}
	if (!HTr.isValid() || !HTr.isHorizontal()) {
		logMessage(_T("\tERROR - Horizontal check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Horizontal check working\n"));
	}

	// Connection check
	GM_3dTriangle discTr(tr);
	discTr[0].begin().x(discTr[0].begin().x() + 2.0 * GM_DIFF_TOLERANCE);
	if (!discTr.isValid() || discTr.isConnected() || !tr.isConnected()) {
		logMessage(_T("\tERROR - Connection check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Connection check working\n"));
	}

	// Max/Min z
	double maxZ = tr.maxZ();
	double minZ = tr.minZ();
	double checkMaxZ = -DBL_MAX;
	double checkMinZ = DBL_MAX;
	for (unsigned int i = 0 ; i < 3 ; i++) {
		if (tr[i].begin().z() > checkMaxZ) checkMaxZ = tr[i].begin().z();
		if (tr[i].begin().z() < checkMinZ) checkMinZ = tr[i].begin().z();
		if (tr[i].end().z() > checkMaxZ) checkMaxZ = tr[i].end().z();
		if (tr[i].end().z() < checkMinZ) checkMinZ = tr[i].end().z();
	}
	if (checkMinZ != minZ || checkMaxZ != maxZ) {
		logMessage(_T("\tERROR - Min/Max z not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Min/Max z working\n"));
	}

	return numErr;
}
int testBasis() {
	int numErr = 0;

	logMessage(_T("TESTING  -  class GM_3dBasis ...\n\n"));

	// Default constructor, basis must be invalid
	GM_3dBasis b;
	if (b.isValid()) {
		logMessage(_T("\tERROR - Default constructor creates valid basis\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Default constructor creates invalid basis\n"));
	}

	// Constructor from three vectors
	GM_3dVector v1(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dVector v2(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dVector v3(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dBasis b1(v1, v2, v3);
	if (!b1.isValid()) {
		logMessage(_T("\tERROR - Constructor from three vectors not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Constructor from three vectors working\n"));
	}

	// Copy constructor
	GM_3dBasis bCopy(b1);
	if (!bCopy.isValid() || bCopy[0] != b1[0] || bCopy[1] != b1[1] || bCopy[2] != b1[2]) {
		logMessage(_T("\tERROR - Copy constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Copy constructor working\n"));
	}

	// Linear independency check
	GM_3dBasis bIndep(GM_3dVector(getRandomDouble(), 0.0, 0.0), GM_3dVector(0.0, getRandomDouble(), 0.0), GM_3dVector(0.0, 0.0, getRandomDouble()));
	GM_3dBasis bDep(v1, v2, v1*getRandomDouble() + v2*getRandomDouble());
	double det = bIndep[0].x() * bIndep[1].y() * bIndep[2].z();
	if (!bDep.isValid() || !bIndep.isValid() || bDep.isLinearlyInd() || !bIndep.isLinearlyInd()) {
		logMessage(_T("\tERROR - Linear independency check not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Linear independency check working\n"));
	}

	
	return numErr;
}
int testMatrix() {
	int numErr = 0;

	logMessage(_T("TESTING  -  class GM_Matrix ...\n\n"));

	// Default constructor, matrix must be invalid
	GM_Matrix m;
	if (m.isValid()) {
		logMessage(_T("\tERROR - Default constructor creates valid matrix\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Default constructor creates invalid matrix\n"));
	}

	// Constructor of a non square matrix
	GM_Matrix mQuad(3, 5);
	if (!mQuad.isValid() || mQuad.isQuad() || mQuad.getNumCol() != 3 || mQuad.getNumRow() != 5) {
		logMessage(_T("\tERROR - Constructor of a non square matrix not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Constructor of a non square matrix working\n"));
	}

	// Constructor from 3d basis
	GM_3dVector v1(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dVector v2(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dVector v3(getRandomDouble(), getRandomDouble(), getRandomDouble());
	GM_3dBasis b(v1, v2, v3);
	GM_Matrix mb(b);
	if (!mb.isValid() || !mb.isQuad() || mb[0][0] != b[0].x() || mb[1][0] != b[0].y() || mb[2][0] != b[0].z() ||
		mb[0][1] != b[1].x() || mb[1][1] != b[1].y() || mb[2][1] != b[1].z() || mb[0][2] != b[2].x() || mb[1][2] != b[2].y() || mb[2][2] != b[2].z()) {
		logMessage(_T("\tERROR - Constructor from basis not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Constructor from basis working\n"));
	}

	// Copy constructor
	GM_Matrix mbCopy(mb);
	bool okCopy = true;
	for (unsigned short i = 0 ; i < 3 && okCopy ; i++) {
		for (unsigned short j = 0 ; j < 3 && okCopy ; j++) {
			if (mbCopy[i][j] != mb[i][j])
				okCopy = false;
		}
	}
	if (!mbCopy.isValid() || !okCopy) {
		logMessage(_T("\tERROR - Copy constructor not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Copy constructor working\n"));
	}

	// Get/Set
	int col = rand() % 3;
	int row = rand() % 3;
	double val = getRandomDouble();
	mb[row][col] = val;
	if (mb[row][col] != val) {
		logMessage(_T("\tERROR - Get/Set not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Get/Set working\n"));
	}

	// Equality Operators
	GM_Matrix mbCopy1(mbCopy);
	if (mbCopy != mbCopy1 || mbCopy == mb) {
		logMessage(_T("\tERROR - Equality operators not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Equality operators working\n"));
	}

	// Assignment operator
	GM_Matrix mbCopy2;
	mbCopy2 = mb;
	if (!mbCopy2.isValid() || mbCopy2 != mb) {
		logMessage(_T("\tERROR - Assignment operator not working\n"));
		numErr++;
	}
	else {
		logMessage(_T("\tOK - Assignment operator working\n"));
	}

	// Matrix product
	unsigned short numRow = 3 + rand() % 10;
	unsigned short numCol = 3 + rand() % 10;	
	unsigned short numRow1 = numCol;
	unsigned short numCol1 = 3 + rand() % 10;
	GM_Matrix m1(numCol, numRow);
	GM_Matrix m2(numCol1, numRow1);
	for (unsigned short i = 0 ; i < numRow ; i++) {
		for (unsigned short j = 0 ; j < numCol ; j++) {
			m1[i][j] = getRandomDouble();
		}
	}
	for (unsigned short i = 0 ; i < numRow1 ; i++) {
		for (unsigned short j = 0 ; j < numCol1 ; j++) {
			m2[i][j] = getRandomDouble();
		}
	}
	GM_Matrix prodM = m1 * m2;
	if (!prodM.isValid() || prodM.getNumRow() != m1.getNumRow() || prodM.getNumCol() != m2.getNumCol()) {
		logMessage(_T("\tERROR - Matrix product not working\n"));
		numErr++;
	}
	else {
		bool okProd = true;
		for (unsigned short i = 0 ; i < numRow && okProd ; i++) {
			for (unsigned short j = 0 ; j < numCol1 && okProd ; j++) {
				double val = 0.0;
				for (unsigned short j1 = 0 ; j1 < numCol ; j1++) {
					val += m1[i][j1] * m2[j1][j];
				}
				if (val != prodM[i][j]) {
					okProd = false;
				}
			}
		}
		if (!okProd) {
			logMessage(_T("\tERROR - Matrix product not working\n"));
			numErr++;
		}
		else {
			logMessage(_T("\tOK - Matrix product working\n"));
		}
	}

	// Matrix transposition
	GM_Matrix mT = mb.transpose();
	if (!mT.isValid() || mT.getNumCol() != mb.getNumRow() || mT.getNumRow() != mb.getNumCol()) {
		logMessage(_T("\tERROR - Matrix trasposition not working\n"));
		numErr++;
	}
	else {
		bool okTras = true;
		for (unsigned short i = 0 ; i < mb.getNumRow() && okTras ; i++) {
			for (unsigned short j = 0 ; j < mb.getNumCol() && okTras ; j++) {
				if (mb[i][j] != mT[j][i]) {
					okTras = false;
				}
			}
		}
		if (!okTras) {
			logMessage(_T("\tERROR - Matrix trasposition not working\n"));
			numErr++;
		}
		else {
			logMessage(_T("\tOK - Matrix trasposition working\n"));
		}
	}

	// Matrix inversion
	unsigned short numRowCol = 3;//3 + rand() % 10;
	GM_Matrix mToInv(numRowCol, numRowCol);
	for (unsigned short i = 0 ; i < numRowCol ; i++) {
		for (unsigned short j = 0 ; j < numRowCol ; j++) {
			mToInv[i][j] = getRandomDouble();
		}
	}
	double mToInvDet = mToInv.determinant();
	GM_Matrix mInv = mToInv.inverse();
	if (!mInv.isValid() && fabs(mToInvDet) > GM_NULL_TOLERANCE) {
		logMessage(_T("\tERROR - Matrix inversion not working\n"));
		numErr++;
	}
	else {
		GM_Matrix mCheck = mInv * mToInv;
		GM_Matrix mCheck1 = mToInv * mInv;
		if (!mCheck.isValid() || !mCheck.isIdentity() || !mCheck1.isValid() || !mCheck1.isIdentity()) {
			logMessage(_T("\tERROR - Matrix inversion not working\n"));
			numErr++;
		}
		else {
			logMessage(_T("\tOK - Matrix inversion working\n"));
		}
	}

	return numErr;
}
int testTrasf() {
	int numErr = 0;

	return numErr;
}
int testRototraslation() {
	int numErr = 0;

	return numErr;
}






void startTest() {
	TCHAR logFileName[1024];

	getLogFilename(logFileName);
	testLogFile = _tfopen(logFileName, _T("w"));
	if (!testLogFile)
		return;

	int pointTestErr = testPoint();
	logHLine();
	int vectorTestErr = testVector();
	logHLine();
	int lineTestErr = testLine();
	logHLine();
	int planeTestErr = testPlane();
	logHLine();
	int triangleTestErr = testTriangle();
	logHLine();
	int basisTestErr = testBasis();
	logHLine();
	int matrixTestErr = testMatrix();


	fclose(testLogFile);
	testLogFile = NULL;
}

int _tmain(int argc, _TCHAR* argv[]) {
	startTest();

	_tprintf(_T("\n\nPress any key..."));
	TCHAR ch = _gettch();
	return 0;
}

