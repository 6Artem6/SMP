#include "StdAfx.h"
#include "Helper.h"


vector2D::vector2D(point2D a, point2D b) {
	this->setCoordinates(a.X, a.Y, b.X, b.Y);
}

vector2D::vector2D(point2D a, point3D b) {
	this->setCoordinates(a.X, a.Y, b.X, b.Y);
}

vector2D::vector2D(point3D a, point2D b) {
	this->setCoordinates(a.X, a.Y, b.X, b.Y);
}

vector2D::vector2D(point3D a, point3D b) {
	this->setCoordinates(a.X, a.Y, b.X, b.Y);
}

void vector2D::setCoordinates(double x1, double y1, double x2, double y2) {
	this->X1 = x1;
	this->Y1 = y1;
	this->X2 = x2;
	this->Y2 = y2;
}

double vector2D::getCoordinateX() {
	return (this->X2 - this->X1);
}

double vector2D::getCoordinateY() {
	return (this->Y2 - this->Y1);
}

double vector2D::getLengthX() {
	return abs(this->getCoordinateX());
}

double vector2D::getLengthY() {
	return abs(this->getCoordinateY());
}

double vector2D::getLength() {
	return sqrt(pow(this->getLengthX(), 2) + pow(this->getLengthY(), 2));
}

double vector2D::getTangentAlpha() {
	return this->getCoordinateY() / this->getCoordinateX();
}

double vector2D::getAngleAlpha() {
	return Helper::atanD(this->getTangentAlpha());
}

void vector2D::incBeginK(double k) {
	this->X2 = this->X1 + k * this->getCoordinateX();
	this->Y2 = this->Y1 + k * this->getCoordinateY();
}

void vector2D::incEndK(double k) {
	this->X1 = this->X2 - k * this->getCoordinateX();
	this->Y1 = this->Y2 - k * this->getCoordinateY();
}

void vector2D::incBeginD(double d) {
	this->X2 = this->X1 + d * Helper::acosD(this->getAngleAlpha());
	this->Y2 = this->Y1 + d * Helper::asinD(this->getAngleAlpha());
}

void vector2D::incEndD(double d) {
	this->X1 = this->X2 - d * Helper::acosD(this->getAngleAlpha());
	this->Y1 = this->Y2 - d * Helper::asinD(this->getAngleAlpha());
}

vector3D::vector3D(point3D a, point3D b) {
	this->X1 = a.X;
	this->Y1 = a.Y;
	this->Z1 = a.Z;
	this->X2 = b.X;
	this->Y2 = b.Y;
	this->Z2 = b.Z;
}

double vector3D::getCoordinateX() {
	return (this->X2 - this->X1);
}

double vector3D::getCoordinateY() {
	return (this->Y2 - this->Y1);
}

double vector3D::getCoordinateZ() {
	return (this->Z2 - this->Z1);
}

double vector3D::getLengthX() {
	return abs(this->getCoordinateX());
}

double vector3D::getLengthY() {
	return abs(this->getCoordinateY());
}

double vector3D::getLengthZ() {
	return abs(this->getCoordinateZ());
}

double vector3D::getLength() {
	return sqrt(pow(this->getLengthX(), 2) +
		pow(this->getLengthY(), 2) +
		pow(this->getLengthZ(), 2));
}

double vector3D::getAngleAlpha() {
	return Helper::acosD(this->getCoordinateX() / this->getLength());
}

double vector3D::getAngleBetta() {
	return Helper::acosD(this->getCoordinateY() / this->getLength());
}

double vector3D::getAngleGamma() {
	return Helper::acosD(this->getCoordinateZ() / this->getLength());
}

void vector3D::incBeginK(double k) {
	this->X1 = this->X2 - k * this->getCoordinateX();
	this->Y1 = this->Y2 - k * this->getCoordinateY();
	this->Z1 = this->Z2 - k * this->getCoordinateZ();
}

void vector3D::incEndK(double k) {
	this->X2 = this->X1 + k * this->getCoordinateX();
	this->Y2 = this->Y1 + k * this->getCoordinateY();
	this->Z2 = this->Z1 + k * this->getCoordinateZ();
}

void vector3D::incBeginD(double d) {
	this->X1 -= d * Helper::cosD(this->getAngleAlpha());
	this->Y1 -= d * Helper::cosD(this->getAngleBetta());
	this->Z1 -= d * Helper::cosD(this->getAngleGamma());
}

void vector3D::incEndD(double d) {
	this->X2 += d * Helper::cosD(this->getAngleAlpha());
	this->Y2 += d * Helper::cosD(this->getAngleBetta());
	this->Z2 += d * Helper::cosD(this->getAngleGamma());
}


double Helper::deg2rad(double angle) {
	return (angle * M_PI / 180);
}

double Helper::rad2deg(double angle) {
	return (angle * 180 / M_PI);
}

double Helper::roundN(double value, int power) {
	return round(value * pow(10, power)) / pow(10, power);
}

double Helper::cat(double hyp, double cat) {
	return sqrt(pow(hyp, 2) - pow(cat, 2));
}

double Helper::hyp(double cat1, double cat2) {
	return sqrt(pow(cat1, 2) + pow(cat2, 2));
}

double Helper::sinD(double angle) {
	return sin(deg2rad(angle));
}

double Helper::cosD(double angle) {
	return cos(deg2rad(angle));
}

double Helper::tanD(double angle) {
	return tan(deg2rad(angle));
}

double Helper::cotD(double angle) {
	return 1 / tan(deg2rad(angle));
}

double Helper::asinD(double value) {
	return rad2deg(asin(value));
}

double Helper::acosD(double value) {
	return rad2deg(acos(value));
}

double Helper::atanD(double value) {
	return rad2deg(atan(value));
}

double Helper::acotD(double value) {
	return rad2deg(atan(1 / value));
}

bool Helper::isRoundEqual(double a, double b) {
	return (round(a) == round(b));
}

bool Helper::isRoundEqualN(double a, double b, int power) {
	return (roundN(a, power) == roundN(b, power));
}

bool Helper::isRoundNotEqual(double a, double b) {
	return (round(a) != round(b));
}

bool Helper::isEqualCoordinates2D(point2D a, point2D b) {
	return ((a.X == b.X) && (a.Y == b.Y));
}

bool Helper::isEqualCoordinates2D(point3D a, point3D b) {
	return ((a.X == b.X) && (a.Y == b.Y));
}

bool Helper::isEqualCoordinates3D(point3D a, point3D b) {
	return ((a.X == b.X) && (a.Y == b.Y) && (a.Z == b.Z));
}

bool Helper::isRoundEqualCoordinates2D(point2D a, point2D b) {
	return (isRoundEqual(a.X, b.X) && isRoundEqual(a.Y, b.Y));
}

bool Helper::isRoundEqualCoordinates2D(point3D a, point3D b) {
	return (isRoundEqual(a.X, b.X) && isRoundEqual(a.Y, b.Y));
}

bool Helper::isRoundEqualCoordinates3D(point3D a, point3D b) {
	return (isRoundEqual(a.X, b.X) && isRoundEqual(a.Y, b.Y) && isRoundEqual(a.Z, b.Z));
}

void Helper::setMinValue(bool first, double* value, double a, double b) {
	if (first) {
		*value = a > b ? b : a;
	}
	else {
		if ((*value > a) || (*value > b)) {
			*value = a > b ? b : a;
		}
	}
}

void Helper::setMaxValue(bool first, double* value, double a, double b) {
	if (first) {
		*value = a < b ? b : a;
	}
	else {
		if ((*value < a) || (*value < b)) {
			*value = a < b ? b : a;
		}
	}
}

void Helper::setMinValues2D(bool first, point2D* value, point2D a, point2D b) {
	setMinValue(first, &value->X, a.X, b.X);
	setMinValue(first, &value->Y, a.Y, b.Y);
}

void Helper::setMaxValues2D(bool first, point2D* value, point2D a, point2D b) {
	setMaxValue(first, &value->X, a.X, b.X);
	setMaxValue(first, &value->Y, a.Y, b.Y);
}

void Helper::setMinValues3D(bool first, point3D* value, point3D a, point3D b) {
	setMinValue(first, &value->X, a.X, b.X);
	setMinValue(first, &value->Y, a.Y, b.Y);
	setMinValue(first, &value->Z, a.Z, b.Z);
}

void Helper::setMaxValues3D(bool first, point3D* value, point3D a, point3D b) {
	setMaxValue(first, &value->X, a.X, b.X);
	setMaxValue(first, &value->Y, a.Y, b.Y);
	setMaxValue(first, &value->Z, a.Z, b.Z);
}

void Helper::getSafeArrayData(_variant_t* var_array, LPDISPATCH** objects, parametersSafeArray* p) {
	HRESULT hr;
	hr = SafeArrayAccessData(var_array->parray, (void**)objects);
	hr = SafeArrayUnaccessData(var_array->parray);
	hr = SafeArrayGetLBound(var_array->parray, 1, &p->lBound);
	hr = SafeArrayGetUBound(var_array->parray, 1, &p->uBound);
	p->count = p->uBound - p->lBound + 1;
}

void Helper::debug(string t) {
	ofstream f;
	f.open(DEBUG_FILE, std::ios_base::app);
	f << t << endl;
	f.close();
}

void Helper::debug(int t) {
	ofstream f;
	f.open(DEBUG_FILE, std::ios_base::app);
	f << t << endl;
	f.close();
}

void Helper::debug(double t) {
	ofstream f;
	f.open(DEBUG_FILE, std::ios_base::app);
	f << t << endl;
	f.close();
}

void Helper::debug(string name, int value) {
	ofstream f;
	f.open(DEBUG_FILE, std::ios_base::app);
	f << name << " = " << value << endl;
	f.close();
}

void Helper::debug(string name, double value) {
	ofstream f;
	f.open(DEBUG_FILE, std::ios_base::app);
	f << name << " = " << value << endl;
	f.close();
}
