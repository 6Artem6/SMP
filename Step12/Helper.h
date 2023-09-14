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

/// Структура 2D координат точки
struct point2D {
	/// Координата X
	double X = 0.0;
	/// Координата Y
	double Y = 0.0;
};

/// Структура 3D координат точки
struct point3D {
	/// Координата X
	double X = 0.0;
	/// Координата Y
	double Y = 0.0;
	/// Координата Z
	double Z = 0.0;
};

/// Структура 2D координат вектора
struct vector2D {
	/// Координата X начала вектора
	double X1 = 0.0;
	/// Координата Y начала вектора
	double Y1 = 0.0;
	/// Координата X конца вектора
	double X2 = 0.0;
	/// Координата Y конца вектора
	double Y2 = 0.0;

	/// Конструктор вектора по двум 2D точкам
	vector2D(point2D a, point2D b);
	/// Конструктор вектора по 2D и 3D точкам
	vector2D(point2D a, point3D b);
	/// Конструктор вектора по 3D и 2D точкам
	vector2D(point3D a, point2D b);
	/// Конструктор вектора по двум 3D точкам
	vector2D(point3D a, point3D b);
	/// Присвоение координат вектора
	void setCoordinates(double x1, double y1, double x2, double y2);
	/// Получение координаты X вектора
	double getCoordinateX();
	/// Получение координаты Y вектора
	double getCoordinateY();
	/// Получение длины по оси X вектора
	double getLengthX();
	/// Получение длины по оси Y вектора
	double getLengthY();
	/// Получение длины вектора
	double getLength();
	/// Тангенс наклона вектора
	double getTangentAlpha();
	/// Угол наклона вектора
	double getAngleAlpha();
	/// Отдаление начала вектора в k раз
	void incBeginK(double k);
	/// Отдаление конца вектора в k раз
	void incEndK(double k);
	/// Отдаление начала вектора на d
	void incBeginD(double d);
	/// Отдаление конца вектора на d
	void incEndD(double d);
};

/// Структура 3D координат вектора
struct vector3D {
	/// Координата X начала вектора
	double X1 = 0.0;
	/// Координата Y начала вектора
	double Y1 = 0.0;
	/// Координата Z начала вектора
	double Z1 = 0.0;
	/// Координата X конца вектора
	double X2 = 0.0;
	/// Координата Y конца вектора
	double Y2 = 0.0;
	/// Координата Z конца вектора
	double Z2 = 0.0;

	vector3D(point3D a, point3D b);
	/// Получение координаты X вектора
	double getCoordinateX();
	/// Получение координаты Y вектора
	double getCoordinateY();
	/// Получение координаты Z вектора
	double getCoordinateZ();
	/// Получение длины по оси X вектора
	double getLengthX();
	/// Получение длины по оси Y вектора
	double getLengthY();
	/// Получение длины по оси Z вектора
	double getLengthZ();
	/// Получение длины вектора
	double getLength();
	/// Угол наклона вектора
	double getAngleAlpha();
	/// Угол наклона вектора
	double getAngleBetta();
	/// Угол наклона вектора
	double getAngleGamma();
	/// Отдаление начала вектора в k раз
	void incBeginK(double k);
	/// Отдаление конца вектора в k раз
	void incEndK(double k);
	/// Отдаление начала вектора на длину d
	void incBeginD(double d);
	/// Отдаление конца вектора на длину d
	void incEndD(double d);
};


/// Класс со вспомогательными функциями
class Helper
{
public:
	/// <summary>
	/// Перевод из градусов в радианы
	/// </summary>
	/// <param name="angle">Угол в градусах</param>
	/// <returns>Угол в радианах</returns>
	static double deg2rad(double angle);
	/// <summary>
	/// Перевод из радианов в градусы
	/// </summary>
	/// <param name="angle">Угол в радианах</param>
	/// <returns>Угол в градусах</returns>
	static double rad2deg(double angle);
	/// <summary>
	/// Округление числа до n знаков
	/// </summary>
	/// <param name="value">Число</param>
	/// <param name="power">Количество знаков</param>
	/// <returns>Округлённое число</returns>
	static double roundN(double value, int power);
	/// <summary>
	/// Получение катета треугольника
	/// </summary>
	/// <param name="hyp">Гипотенуза</param>
	/// <param name="cat">Первый катет</param>
	/// <returns>Длина второго катета</returns>
	static double cat(double hyp, double cat);
	/// <summary>
	/// Получение гипотенузы треугольника
	/// </summary>
	/// <param name="hyp">Первый катет</param>
	/// <param name="cat">Второй катет</param>
	/// <returns>Длина гипотенузы</returns>
	static double hyp(double cat1, double cat2);
	/// <summary>
	/// Синус угла в градусах
	/// </summary>
	/// <param name="angle">Угол в градусах</param>
	/// <returns>Результат операции</returns>
	static double sinD(double angle);
	/// <summary>
	/// Косинус угла в градусах
	/// </summary>
	/// <param name="angle">Угол в градусах</param>
	/// <returns>Результат операции</returns>
	static double cosD(double angle);
	/// <summary>
	/// Тангенс угла в градусах
	/// </summary>
	/// <param name="angle">Угол в градусах</param>
	/// <returns>Результат операции</returns>
	static double tanD(double angle);
	/// <summary>
	/// Котангенс угла в градусах
	/// </summary>
	/// <param name="angle">Угол в градусах</param>
	/// <returns>Результат операции</returns>
	static double cotD(double angle);
	/// <summary>
	/// Арксинус величины в градусах
	/// </summary>
	/// <param name="angle">Значение синуса</param>
	/// <returns>Угол в градусах</returns>
	static double asinD(double value);
	/// <summary>
	/// Арккосинус величины в градусах
	/// </summary>
	/// <param name="angle">Значение косинуса</param>
	/// <returns>Угол в градусах</returns>
	static double acosD(double value);
	/// <summary>
	/// Арктангенс величины в градусах
	/// </summary>
	/// <param name="angle">Значение тангенса</param>
	/// <returns>Угол в градусах</returns>
	static double atanD(double value);
	/// <summary>
	/// Арккотангенс величины в градусах
	/// </summary>
	/// <param name="angle">Значение котангенса</param>
	/// <returns>Угол в градусах</returns>
	static double acotD(double value);
	/// <summary>
	/// Сравнение - округлённые числа равны
	/// </summary>
	/// <param name="a">Первое число</param>
	/// <param name="b">Второе число</param>
	/// <returns>Результат сравнения</returns>
	static bool isRoundEqual(double a, double b);
	/// <summary>
	/// Сравнение - округлённые числа равны до "power" знаков
	/// </summary>
	/// <param name="a">Первое число</param>
	/// <param name="b">Второе число</param>
	/// <param name="power">Количество знаков</param>
	/// <returns>Результат сравнения</returns>
	static bool isRoundEqualN(double a, double b, int power);
	/// <summary>
	/// Сравнение - округлённые числа не равны
	/// </summary>
	/// <param name="a">Первое число</param>
	/// <param name="b">Второе число</param>
	/// <returns>Результат сравнения</returns>
	static bool isRoundNotEqual(double a, double b);
	/// <summary>
	/// Сравнение - 2D координаты равны
	/// </summary>
	/// <param name="a">Первая 2D координата</param>
	/// <param name="b">Вторая 2D координата</param>
	/// <returns>Результат сравнения</returns>
	static bool isEqualCoordinates2D(point2D a, point2D b);
	/// <summary>
	/// Сравнение - 2D координаты равны
	/// </summary>
	/// <param name="a">Первая 3D координата</param>
	/// <param name="b">Вторая 3D координата</param>
	/// <returns>Результат сравнения</returns>
	static bool isEqualCoordinates2D(point3D a, point3D b);
	/// <summary>
	/// Сравнение - 3D координаты равны
	/// </summary>
	/// <param name="a">Первая 3D координата</param>
	/// <param name="b">Вторая 3D координата</param>
	/// <returns>Результат сравнения</returns>
	static bool isEqualCoordinates3D(point3D a, point3D b);
	/// <summary>
	/// Сравнение - округлённые 2D координаты равны
	/// </summary>
	/// <param name="a">Первая 2D координата</param>
	/// <param name="b">Вторая 2D координата</param>
	/// <returns>Результат сравнения</returns>
	static bool isRoundEqualCoordinates2D(point2D a, point2D b);
	/// <summary>
	/// Сравнение - округлённые 2D координаты равны
	/// </summary>
	/// <param name="a">Первая 3D координата</param>
	/// <param name="b">Вторая 3D координата</param>
	/// <returns>Результат сравнения</returns>
	static bool isRoundEqualCoordinates2D(point3D a, point3D b);
	/// <summary>
	/// Сравнение - округлённые 3D координаты равны
	/// </summary>
	/// <param name="a">Первая 3D координата</param>
	/// <param name="b">Вторая 3D координата</param>
	/// <returns>Результат сравнения</returns>
	static bool isRoundEqualCoordinates3D(point3D a, point3D b);
	/// <summary>
	/// Присвоение минимального значения
	/// </summary>
	/// <param name="first">Признак первого элемента</param>
	/// <param name="value">Значение</param>
	/// <param name="a">Первое число</param>
	/// <param name="b">Второе число</param>
	static void setMinValue(bool first, double* value, double a, double b);
	/// <summary>
	/// Присвоение максимального значения
	/// </summary>
	/// <param name="first">Признак первого элемента</param>
	/// <param name="value">Значение</param>
	/// <param name="a">Первое число</param>
	/// <param name="b">Второе число</param>
	static void setMaxValue(bool first, double* value, double a, double b);
	/// <summary>
	/// Присвоение минимального значения 2D координаты
	/// </summary>
	/// <param name="first">Признак первого элемента</param>
	/// <param name="value">Значение</param>
	/// <param name="a">Первая 2D координата</param>
	/// <param name="b">Вторая 2D координата</param>
	static void setMinValues2D(bool first, point2D* value, point2D a, point2D b);
	/// <summary>
	/// Присвоение максимального значения 2D координаты
	/// </summary>
	/// <param name="first">Признак первого элемента</param>
	/// <param name="value">Значение</param>
	/// <param name="a">Первая 2D координата</param>
	/// <param name="b">Вторая 2D координата</param>
	static void setMaxValues2D(bool first, point2D* value, point2D a, point2D b);
	/// <summary>
	/// Присвоение минимального значения 3D координаты
	/// </summary>
	/// <param name="first">Признак первого элемента</param>
	/// <param name="value">Значение</param>
	/// <param name="a">Первая 3D координата</param>
	/// <param name="b">Вторая 3D координата</param>
	static void setMinValues3D(bool first, point3D* value, point3D a, point3D b);
	/// <summary>
	/// Присвоение максимального значения 3D координаты
	/// </summary>
	/// <param name="first">Признак первого элемента</param>
	/// <param name="value">Значение</param>
	/// <param name="a">Первая 3D координата</param>
	/// <param name="b">Вторая 3D координата</param>
	static void setMaxValues3D(bool first, point3D* value, point3D a, point3D b);
	/// <summary>
	/// Получение параметров SafeArray из _variant_t
	/// </summary>
	/// <param name="var_array">Указатель на передаваемые данные</param>
	/// <param name="objects">Указатель на массив данных</param>
	/// <param name="p">Указатель на структуру параметров границы массива</param>
	static void getSafeArrayData(_variant_t* var_array, LPDISPATCH** objects, parametersSafeArray* p);

	static void debug(string t);
	static void debug(int t);
	static void debug(double t);
	static void debug(string name, int value);
	static void debug(string name, double value);
};
