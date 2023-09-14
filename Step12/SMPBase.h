#ifndef _SMP_BASE_H
#define _SMP_BASE_H
#include "Helper.h"
#include <PArray.h>
#include <SafeArrayUtils.h>

#if _MSC_VER > 1000
#pragma once
#endif
#endif

/// Список фигур основания
enum SurfaceFigure
{
	triangle,	/// Треугольник
	square,		/// Квадрат
	pentagon,	/// Пятиугольник
	hexagon,	/// Шестиугольник
	trigon,		/// Тригон
	rhombus35,	/// Ромб с углом 35 градусов
	rhombus55,	/// Ромб с углом 55 градусов
	rhombus80,	/// Ромб с углом 80 градусов
	circle,		/// Круг
};

/// Список способов задания основного размера детали
enum SizeType
{
	byOuterRadius,	/// Размер по радиусу описанной окружности
	byInnerRadius,	/// Размер по радиусу вписанной окружности
	bySide,			/// Размер по длине стороны
};

/// Основной класс СМП
class SMPBase
{
public:
	/// Координаты центра основания
	point3D coordinates = {0.0, 0.0, 0.0};
	/// Фигура основания
	SurfaceFigure surfaceFigure = SurfaceFigure::triangle;
	/// Тип размера
	SizeType sizeType = SizeType::byOuterRadius;
	/// Основной размер
	double size = 22.0;
	/// Высота
	double height = 7.0;
	/// Наличие отверстия
	bool hasHole = true;
	/// Радиус отверстия
	double holeRadius = 2.0;
	/// Длина фаски отверстия
	double holeChamferLength = 0.5;
	/// Радиус скругления боковых рёбер
	double edgesFilletRadius = 1.2;
	/// Угол наклона пластины α
	double angleAlpha = 10.0;

	/// Получение указателя приложения
	void init(IApplicationPtr application, HMODULE hmodule);
	/// Присвоение модели параметры документа
	void initDocumentParameters();
	/// Получение радиуса описанной окружности модели
	double getOuterRadius();
	/// Получение радиуса вписанной окружности модели
	double getInnerRadius();
	/// Получение длины стороны модели
	double getSide();
	/// Получение итоговой высоты модели
	double getHeight();
	/// Получение координаты низшей точки модели
	double getBottom();
	/// Получение координаты высшей точки модели
	double getTop();
	/// Получение координаты высшей точки конечной модели
	double getTopFinal();
	/// Получение количества углов правильного многоугольника
	unsigned short getNGonVertexCount();
	/// Получение количества вершин резания
	unsigned short getCutVertexCount();
	/// Получение направления основного угла
	double getAngleMainDirection();
	/// Получение направления вспомогательного угла
	double getAngleAdditionalDirection();
	/// Получение величины угла ромба
	double getRhombusAngle();
	/// Получение значения основного угла
	double getAngleMain();
	/// Получение значения вспомогательного угла
	double getAngleAdditional();
	/// Получение угла между основными
	double getAngleVertex();
	/// Проверка: основной размер - радиус описанной окружности
	bool isByOuterRadius();
	/// Проверка: основной размер - радиус вписанной окружности
	bool isByInnerRadius();
	/// Проверка: основной размер - длины стороны
	bool isBySide();
	/// Проверка: основание - треугольник
	bool isTriangle();
	/// Проверка: основание - квадрат
	bool isSquare();
	/// Проверка: основание - пятиугольник
	bool isPentagon();
	/// Проверка: основание - шестиугольник
	bool isHexagon();
	/// Проверка: основание - ромб с углом 35 градусов
	bool isRhombus35();
	/// Проверка: основание - ромб с углом 55 градусов
	bool isRhombus55();
	/// Проверка: основание - ромб с углом 80 градусов
	bool isRhombus80();
	/// Проверка: основание - правильный многоугольник
	bool isNGon();
	/// Проверка: основание - ромб
	bool isRhombus();
	/// Проверка: основание - тригон
	bool isTrigon();
	/// Проверка: основание - круг
	bool isCircle();
	/// Проверка: основание - имеет меньший внешний радиус (ромб, тригон)
	bool hasOuterLessRadius();
	/// Проверка: необходимо скругление боковых рёбер
	bool needFillet();
	/// Проверка: необходимо отверстия
	bool needHole();

protected:
	/// Основной адрес библиотеки
	HMODULE hModule = NULL;
	/// Интерфейс компонента IApplicationPtr
	IApplicationPtr application = NULL;
	/// Интерфейс компонента IKompasDocumentPtr
	IKompasDocumentPtr activeDocument = NULL;
	/// Интерфейс компонента IKompasDocument3DPtr
	IKompasDocument3DPtr kompasDocument3D = NULL;
	/// Интерфейс компонента IDocument3D
	IDocument3DPtr document3D = NULL;
	/// Интерфейс компонента IPart
	IPartPtr part = NULL;
	/// Интерфейс компонента IPart7
	IPart7Ptr part7 = NULL;
	/// Интерфейс компонента IFeature7
	IFeature7Ptr feature = NULL;
	/// Интерфейс контейнера трехмерных объектов
	IModelContainerPtr modelContainer = NULL;
	/// Интерфейс элемента эскиза основания
	ISketchPtr baseSketch = NULL;
	/// Интерфейс элемента плоскости основания
	IPlane3DByOffsetPtr basePlane = NULL;
	/// Интерфейс элемента ЛСК основания
	ILocalCoordinateSystemPtr baseLocalCS = NULL;
	/// Радиус описанной окружности фигуры основания
	double outerRadiusFigure = 0;
	/// Меньший радиус описанной окружности модели
	double outerLessRadiusFigure = 0;
	/// Радиус вписанной окружности фигуры основания
	double innerRadiusFigure = 0;
	/// Длины стороны фигуры основания
	double sideFigure = 0;
	/// Радиус описанной окружности модели после скругления боковых рёбер
	double outerRadiusFillet = 0;
	/// Меньший радиус описанной окружности модели после скругления боковых рёбер
	double outerLessRadiusFillet = 0;
	/// Смещение итоговой модели по высоте
	double height_delta = 0;
	/// Смещение итоговой модели по радиусу внутреннему или внешнему
	double radius_delta = 0;
	/// Список координат точек фигуры основания
	vector<point2D> points;

	/// Создание основания
	void addEmbodiment();
	/// Создание основания
	void drawBase();
	/// Создание основания - круга
	void drawCircle(IDrawingContainerPtr drawingContainer);
	/// Создание основания - правильный многоугольник
	void drawNGon(IDrawingContainerPtr drawingContainer);
	/// Создание основания - тригон
	void drawTrigon(IDrawingContainerPtr drawingContainer);
	/// Создание основания - ромб
	void drawRhombus(IDrawingContainerPtr drawingContainer);
	/// Создание выдавливания
	void addExtrusion();
	/// Создание скругления боковых рёбер
	void addEdgesFillet();
	/// Создание скругления боковых рёбер по эскизу
	void addEdgesFilletBySketch(long count, double radiusFigure, double radiusModel,
		double angleMain, double angleDirection);
	/// Создание эскиза скругления боковых рёбер
	void addEdgesFilletSketch(ISketchPtr sketch, double radiusFigure, double radiusModel, 
		double angleMain, double angleDirection);
	/// Создание скругления боковых рёбер по хорде
	void addEdgesFilletByChord();
	/// Создание отверстия
	void addHole();
	/// <summary>
	/// Получение интерфейса контейнера объектов вида графического документа
	/// </summary>
	/// <param name="sketchDocument">Эскиз</param>
	IDrawingContainerPtr getDrawingContainer(IFragmentDocumentPtr sketchDocument);
	/// <summary>
	/// Получение координат точки основания по индексу
	/// </summary>
	/// <param name="index">Индекс точки</param>
	/// <param name="x">Координата x</param>
	/// <param name="y">Координата y</param>
	void getPoint(size_t index, double* x, double* y);
	/// Получение координат первой точки основания
	void getPointFirst(double* x, double* y);
	/// Получение координат последней точки основания
	void getPointLast(double* x, double* y);
	/// Добавление координат точки основания
	void addPoint(double x, double y);
};
