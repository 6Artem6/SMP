#ifndef _HELPER_H
#define _HELPER_H

#include "Resource.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>

using namespace std;
#define DEBUG_FILE "C:\\Users\\Public\\Documents\\filename.txt"

#if _MSC_VER > 1000
#pragma once
#endif
#endif

struct parametersSafeArray {
	long count;
	long lBound;
	long uBound;
};

/// ��������� 2D ��������� �����
struct point2D {
	/// ���������� X
	double X = 0.0;
	/// ���������� Y
	double Y = 0.0;
};

/// ��������� 3D ��������� �����
struct point3D {
	/// ���������� X
	double X = 0.0;
	/// ���������� Y
	double Y = 0.0;
	/// ���������� Z
	double Z = 0.0;
};

/// ��������� 2D ��������� �������
struct vector2D {
	/// ���������� X ������ �������
	double X1 = 0.0;
	/// ���������� Y ������ �������
	double Y1 = 0.0;
	/// ���������� X ����� �������
	double X2 = 0.0;
	/// ���������� Y ����� �������
	double Y2 = 0.0;

	/// ����������� ������� �� ���� 2D ������
	vector2D(point2D a, point2D b);
	/// ����������� ������� �� 2D � 3D ������
	vector2D(point2D a, point3D b);
	/// ����������� ������� �� 3D � 2D ������
	vector2D(point3D a, point2D b);
	/// ����������� ������� �� ���� 3D ������
	vector2D(point3D a, point3D b);
	/// ���������� ��������� �������
	void setCoordinates(double x1, double y1, double x2, double y2);
	/// ��������� ���������� X �������
	double getCoordinateX();
	/// ��������� ���������� Y �������
	double getCoordinateY();
	/// ��������� ����� �� ��� X �������
	double getLengthX();
	/// ��������� ����� �� ��� Y �������
	double getLengthY();
	/// ��������� ����� �������
	double getLength();
	/// ������� ������� �������
	double getTangentAlpha();
	/// ���� ������� �������
	double getAngleAlpha();
	/// ��������� ������ ������� � k ���
	void incBeginK(double k);
	/// ��������� ����� ������� � k ���
	void incEndK(double k);
	/// ��������� ������ ������� �� d
	void incBeginD(double d);
	/// ��������� ����� ������� �� d
	void incEndD(double d);
};

/// ��������� 3D ��������� �������
struct vector3D {
	/// ���������� X ������ �������
	double X1 = 0.0;
	/// ���������� Y ������ �������
	double Y1 = 0.0;
	/// ���������� Z ������ �������
	double Z1 = 0.0;
	/// ���������� X ����� �������
	double X2 = 0.0;
	/// ���������� Y ����� �������
	double Y2 = 0.0;
	/// ���������� Z ����� �������
	double Z2 = 0.0;

	vector3D(point3D a, point3D b);
	/// ��������� ���������� X �������
	double getCoordinateX();
	/// ��������� ���������� Y �������
	double getCoordinateY();
	/// ��������� ���������� Z �������
	double getCoordinateZ();
	/// ��������� ����� �� ��� X �������
	double getLengthX();
	/// ��������� ����� �� ��� Y �������
	double getLengthY();
	/// ��������� ����� �� ��� Z �������
	double getLengthZ();
	/// ��������� ����� �������
	double getLength();
	/// ���� ������� �������
	double getAngleAlpha();
	/// ���� ������� �������
	double getAngleBetta();
	/// ���� ������� �������
	double getAngleGamma();
	/// ��������� ������ ������� � k ���
	void incBeginK(double k);
	/// ��������� ����� ������� � k ���
	void incEndK(double k);
	/// ��������� ������ ������� �� ����� d
	void incBeginD(double d);
	/// ��������� ����� ������� �� ����� d
	void incEndD(double d);
};


/// ����� �� ���������������� ���������
class Helper
{
public:
	/// <summary>
	/// ������� �� �������� � �������
	/// </summary>
	/// <param name="angle">���� � ��������</param>
	/// <returns>���� � ��������</returns>
	static double deg2rad(double angle);
	/// <summary>
	/// ������� �� �������� � �������
	/// </summary>
	/// <param name="angle">���� � ��������</param>
	/// <returns>���� � ��������</returns>
	static double rad2deg(double angle);
	/// <summary>
	/// ���������� ����� �� n ������
	/// </summary>
	/// <param name="value">�����</param>
	/// <param name="power">���������� ������</param>
	/// <returns>���������� �����</returns>
	static double roundN(double value, int power);
	/// <summary>
	/// ��������� ������ ������������
	/// </summary>
	/// <param name="hyp">����������</param>
	/// <param name="cat">������ �����</param>
	/// <returns>����� ������� ������</returns>
	static double cat(double hyp, double cat);
	/// <summary>
	/// ��������� ���������� ������������
	/// </summary>
	/// <param name="hyp">������ �����</param>
	/// <param name="cat">������ �����</param>
	/// <returns>����� ����������</returns>
	static double hyp(double cat1, double cat2);
	/// <summary>
	/// ����� ���� � ��������
	/// </summary>
	/// <param name="angle">���� � ��������</param>
	/// <returns>��������� ��������</returns>
	static double sinD(double angle);
	/// <summary>
	/// ������� ���� � ��������
	/// </summary>
	/// <param name="angle">���� � ��������</param>
	/// <returns>��������� ��������</returns>
	static double cosD(double angle);
	/// <summary>
	/// ������� ���� � ��������
	/// </summary>
	/// <param name="angle">���� � ��������</param>
	/// <returns>��������� ��������</returns>
	static double tanD(double angle);
	/// <summary>
	/// ��������� ���� � ��������
	/// </summary>
	/// <param name="angle">���� � ��������</param>
	/// <returns>��������� ��������</returns>
	static double cotD(double angle);
	/// <summary>
	/// �������� �������� � ��������
	/// </summary>
	/// <param name="angle">�������� ������</param>
	/// <returns>���� � ��������</returns>
	static double asinD(double value);
	/// <summary>
	/// ���������� �������� � ��������
	/// </summary>
	/// <param name="angle">�������� ��������</param>
	/// <returns>���� � ��������</returns>
	static double acosD(double value);
	/// <summary>
	/// ���������� �������� � ��������
	/// </summary>
	/// <param name="angle">�������� ��������</param>
	/// <returns>���� � ��������</returns>
	static double atanD(double value);
	/// <summary>
	/// ������������ �������� � ��������
	/// </summary>
	/// <param name="angle">�������� ����������</param>
	/// <returns>���� � ��������</returns>
	static double acotD(double value);
	/// <summary>
	/// ��������� - ���������� ����� �����
	/// </summary>
	/// <param name="a">������ �����</param>
	/// <param name="b">������ �����</param>
	/// <returns>��������� ���������</returns>
	static bool isRoundEqual(double a, double b);
	/// <summary>
	/// ��������� - ���������� ����� ����� �� "power" ������
	/// </summary>
	/// <param name="a">������ �����</param>
	/// <param name="b">������ �����</param>
	/// <param name="power">���������� ������</param>
	/// <returns>��������� ���������</returns>
	static bool isRoundEqualN(double a, double b, int power);
	/// <summary>
	/// ��������� - ���������� ����� �� �����
	/// </summary>
	/// <param name="a">������ �����</param>
	/// <param name="b">������ �����</param>
	/// <returns>��������� ���������</returns>
	static bool isRoundNotEqual(double a, double b);
	/// <summary>
	/// ��������� - 2D ���������� �����
	/// </summary>
	/// <param name="a">������ 2D ����������</param>
	/// <param name="b">������ 2D ����������</param>
	/// <returns>��������� ���������</returns>
	static bool isEqualCoordinates2D(point2D a, point2D b);
	/// <summary>
	/// ��������� - 2D ���������� �����
	/// </summary>
	/// <param name="a">������ 3D ����������</param>
	/// <param name="b">������ 3D ����������</param>
	/// <returns>��������� ���������</returns>
	static bool isEqualCoordinates2D(point3D a, point3D b);
	/// <summary>
	/// ��������� - 3D ���������� �����
	/// </summary>
	/// <param name="a">������ 3D ����������</param>
	/// <param name="b">������ 3D ����������</param>
	/// <returns>��������� ���������</returns>
	static bool isEqualCoordinates3D(point3D a, point3D b);
	/// <summary>
	/// ��������� - ���������� 2D ���������� �����
	/// </summary>
	/// <param name="a">������ 2D ����������</param>
	/// <param name="b">������ 2D ����������</param>
	/// <returns>��������� ���������</returns>
	static bool isRoundEqualCoordinates2D(point2D a, point2D b);
	/// <summary>
	/// ��������� - ���������� 2D ���������� �����
	/// </summary>
	/// <param name="a">������ 3D ����������</param>
	/// <param name="b">������ 3D ����������</param>
	/// <returns>��������� ���������</returns>
	static bool isRoundEqualCoordinates2D(point3D a, point3D b);
	/// <summary>
	/// ��������� - ���������� 3D ���������� �����
	/// </summary>
	/// <param name="a">������ 3D ����������</param>
	/// <param name="b">������ 3D ����������</param>
	/// <returns>��������� ���������</returns>
	static bool isRoundEqualCoordinates3D(point3D a, point3D b);
	/// <summary>
	/// ���������� ������������ ��������
	/// </summary>
	/// <param name="first">������� ������� ��������</param>
	/// <param name="value">��������</param>
	/// <param name="a">������ �����</param>
	/// <param name="b">������ �����</param>
	static void setMinValue(bool first, double* value, double a, double b);
	/// <summary>
	/// ���������� ������������� ��������
	/// </summary>
	/// <param name="first">������� ������� ��������</param>
	/// <param name="value">��������</param>
	/// <param name="a">������ �����</param>
	/// <param name="b">������ �����</param>
	static void setMaxValue(bool first, double* value, double a, double b);
	/// <summary>
	/// ���������� ������������ �������� 2D ����������
	/// </summary>
	/// <param name="first">������� ������� ��������</param>
	/// <param name="value">��������</param>
	/// <param name="a">������ 2D ����������</param>
	/// <param name="b">������ 2D ����������</param>
	static void setMinValues2D(bool first, point2D* value, point2D a, point2D b);
	/// <summary>
	/// ���������� ������������� �������� 2D ����������
	/// </summary>
	/// <param name="first">������� ������� ��������</param>
	/// <param name="value">��������</param>
	/// <param name="a">������ 2D ����������</param>
	/// <param name="b">������ 2D ����������</param>
	static void setMaxValues2D(bool first, point2D* value, point2D a, point2D b);
	/// <summary>
	/// ���������� ������������ �������� 3D ����������
	/// </summary>
	/// <param name="first">������� ������� ��������</param>
	/// <param name="value">��������</param>
	/// <param name="a">������ 3D ����������</param>
	/// <param name="b">������ 3D ����������</param>
	static void setMinValues3D(bool first, point3D* value, point3D a, point3D b);
	/// <summary>
	/// ���������� ������������� �������� 3D ����������
	/// </summary>
	/// <param name="first">������� ������� ��������</param>
	/// <param name="value">��������</param>
	/// <param name="a">������ 3D ����������</param>
	/// <param name="b">������ 3D ����������</param>
	static void setMaxValues3D(bool first, point3D* value, point3D a, point3D b);
	/// <summary>
	/// ��������� ���������� SafeArray �� _variant_t
	/// </summary>
	/// <param name="var_array">��������� �� ������������ ������</param>
	/// <param name="objects">��������� �� ������ ������</param>
	/// <param name="p">��������� �� ��������� ���������� ������� �������</param>
	static void getSafeArrayData(_variant_t* var_array, LPDISPATCH** objects, parametersSafeArray* p);

	static void debug(string t);
	static void debug(int t);
	static void debug(double t);
	static void debug(string name, int value);
	static void debug(string name, double value);
};
