#ifndef _SMP_PROFILE_H
#define _SMP_PROFILE_H

#include "SMPBase.h"

#if _MSC_VER > 1000
#pragma once
#endif
#endif

/// Список типов режущей кромки
enum CutRoundingType
{
	byChamfer,	/// Фаска
	byFillet,	/// Скругление
};

/// Класс СМП по профилю режущей кромки
class SMPProfile : public SMPBase
{
public:
	/// Длина режущей кромки
	double cutLength = 0.5;
	/// Угол режущей кромки γ
	double cutAngleGamma = 75.0;
	/// Радиус режущей кромки
	double cutRadius = 0.5;
	/// Тип режущей кромки
	bool cutRoundingType = CutRoundingType::byFillet;
	/// Радиус скругления режущей кромки
	double cutFilletRadius = 0.05;
	/// Длина фаски режущей кромки
	double cutChamferLength = 0.2;
	/// Угол фаски режущей кромки
	double cutChamferAngle = 45.0;
	/// Обновление параметров меню
	bool updateParameters();
	/// Создание модели
	void createModel();

protected:
	/// Добавочная длина режущей кромки
	double cutLength_delta = 0;
	/// Длина первой стороны фаски режущей кромки
	double cutChamferLength_cat1 = 0;
	/// Длина второйстороны фаски режущей кромки
	double cutChamferLength_cat2 = 0;
	/// Получение итоговой длины режущей кромки
	double getCutLength();
	/// Проверка: тип режущей кромки - фаска
	bool isByChamfer();
	/// Проверка: тип режущей кромки - скругление
	bool isByFillet();
	/// Обновление параметров меню для основания круга
	void updateParametersCircle();
	/// Обновление параметров меню для основания правильного многоугольника
	void updateParametersNGon();
	/// Обновление параметров меню для основания тригона
	void updateParametersTrigon();
	/// Обновление параметров меню для основания ромба
	void updateParametersRhombus();
	/// Обновление параметров меню для режущей кромки
	bool updateParametersCut();
	/// Добавление режущей кромки
	void addCutEvolution();
	/// Нахождение рёбер для режущей кромки
	PArray<IModelObjectPtr>* SMPProfile::findEvolutionEdges(IFacePtr faceBase);
	/// Нахождение угла поворота координатной системы для режущей кромки 
	double findEvolutionCoordinateSystemRotationAngle(IFacePtr faceBase, point3D* CSCoord);
	/// Создание эскиза для режущей кромки
	void createEvolutionSketch(ISketchPtr cutSketch);
	/// Нахождение рёбер для скругления на режущей кромке
	PArray<IModelObjectPtr>* findEvolutionRoundingEdges();
	/// Нахождение ребра для режущей кромки с круглым основанием
	PArray<IModelObjectPtr>* findCircleTopEdge();
	/// Срез смещения итоговой модели по высоте
	void removeHeightDelta();
};
