#ifndef _SMP_CONDITIONS_H
#define _SMP_CONDITIONS_H

#include "SMPBase.h"

#if _MSC_VER > 1000
#pragma once
#endif
#endif


/// Список способов задания режимов резания
enum ParameterType
{
	byS,	/// По постоянной подаче
	byT,	/// По постоянной глубине
	byStoT,	/// По постоянному отношению подачи к глубине
};

/// Список способов задания угла η
enum AngleType
{
	byParameters,	/// По параметрам резания
	byAngleEta,		/// По значению угла η
};

/// Класс СМП по условиям резания
class SMPConditions : public SMPBase
{
public:
	/// Выбор режима резания
	ParameterType parameterType = ParameterType::byS;
	/// Выбор способа задания угла η
	AngleType angleType = AngleType::byParameters;
	/// Подача S
	double parameterS = 1.5;
	/// Глубина t
	double parameterT = 4;
	/// Отношение подачи S к глубине t
	double parameterStoT = 0.1;
	/// Угол η
	double angleEta = 42.0;
	/// Количество линий режущей кромки главного лезвия
	unsigned short cutLineCount = 10;
	/// Количество линий режущей кромки вспомогательного лезвия
	unsigned short cutLineEndCount = 10;
	/// Направление режущей кромки
	bool cutDirection = false;
	/// Обновление параметров меню
	bool updateParameters();
	/// Создание модели
	void createModel();

protected:
	/// Коэффициент 1 для получения угла λд
	const double lambdaK = 13.14;
	/// Коэффициент 2 для получения угла λд - подачи S
	const double lambdaKS = 9.6;
	/// Коэффициент 3 для получения угла λд - глубины t
	const double lambdaKT = -4.46;
	/// Проверка: основной параметр - подача S
	bool isByS();
	/// Проверка: основной параметр - глубина t
	bool isByT();
	/// Проверка: основной параметр - отношение подачи S к глубине t
	bool isByStoT();
	/// Проверка: угол η - по параметрам резания
	bool isByParameters();
	/// Проверка: угол η - по значению
	bool isByAngleEta();
	/// Получение угла λд
	double getAngleLambdaD(double S, double t);
	/// Обновление режимов резания по параметрам
	void updateConditionsByParameters(LPWSTR* reason);
	/// Обновление режимов резания по углу η
	void updateConditionsByAngleEta(LPWSTR* reason);
	/// Обновление параметров меню для основания круга
	void updateParametersCircle();
	/// Обновление параметров меню для основания правильного многоугольника
	void updateParametersNGon(double* angle_main);
	/// Обновление параметров меню для основания тригона
	void updateParametersTrigon(double* angle_main);
	/// Обновление параметров меню для основания ромба
	void updateParametersRhombus(double* angle_main);
	/// Создание массива режущих кромок
	void addBladePlanesByMesh();
	/// Создание режущих кромки
	void addBladePlaneByMesh(PArray<IModelObjectPtr>* linesArray,
		bool begin, int count, double delta, double angleHalf, double angleDirection,
		point3D* planeMinCoord, point3D* planeMaxCoord,
		point2D* sCoord, point2D* tCoord, point2D maxCoord);
	/// Поиск координат точек режущей кромки
	void findBladePlanePoints(bool begin, double percent,
		double delta, double angleHalf, double angleDirection,
		point3D* pointBeginCoord, point3D* pointEndCoord,
		point2D* sCoord, point2D* tCoord, point2D maxCoord);
	/// Получение угла η по параметрам
	double getEta(double parameterValue, double delta, double angleHalf, 
		point2D* sCoord, point2D* tCoord, point2D maxCoord);
};
