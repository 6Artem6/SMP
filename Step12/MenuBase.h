#ifndef _MENU_BASE_H
#define _MENU_BASE_H

#include "StdAfx.h"
#include "Resource.h"
#include "SMPBase.h"

#if _MSC_VER > 1000
#pragma once
#endif
#endif

/// Основной класс меню параметров модели
class MenuBase
{
public:
	/// Коллекция контролов
	IPropertyControlsPtr collection = NULL;
	/// Поле "Координаты центра"
	IPropertyPoint3DPtr coordinates = NULL;
	/// Поле "Фигура основания"
	IPropertyListPtr surfaceFigure = NULL;
	/// Поле "Тип размера
	IPropertyListPtr sizeType = NULL;
	/// Поле "Величина размера"
	IPropertyEditPtr size = NULL;
	/// Поле "Высота"
	IPropertyEditPtr height = NULL;
	/// Поле "Угол наклона α"
	IPropertyEditPtr angleAlpha = NULL;
	/// Поле "Радиус скругления боковых рёбер"
	IPropertyEditPtr edgesFilletRadius = NULL;
	/// Поле "Наличие отверстия"
	IPropertyTwinSwitcherPtr hasHole = NULL;
	/// Поле "Радиус отверстия"
	IPropertyEditPtr holeRadius = NULL;
	/// Поле "Длина фаски отверстия"
	IPropertyEditPtr holeChamferLength = NULL;
	/// Создание кнопки "Применить"
	IPropertyTextButtonPtr enterButton = NULL;

	/// <summary>
	/// Получение модели объекта
	/// </summary>
	/// <returns>Модель объекта</returns>
	virtual SMPBase* getModel() = NULL;
	/// <summary>
	/// Проверка условия подключения поля "Наличия отверстия"
	/// </summary>
	void checkHasHole();

protected:
	/// <summary>
	/// Создание поля "Координаты"
	/// </summary>
	/// <param name="id">Номер элемента координат</param>
	/// <param name="id_x">Номер элемента координаты X</param>
	/// <param name="id_y">Номер элемента координаты Y</param>
	/// <param name="id_z">Номер элемента координаты Z</param>
	void initCoordinates(size_t id, size_t id_x, size_t id_y, size_t id_z);
	/// <summary>
	/// Создание поля "Тип основания"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	/// <param name="hasCircle">Добавление круга</param>
	void initSurfaceFigure(size_t id, bool hasCircle);
	/// <summary>
	/// Создание поля "Тип размера"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initSizeType(size_t id);
	/// <summary>
	/// Создание поля "Величина размера"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initSize(size_t id);
	/// <summary>
	/// Создание поля "Высота"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initHeight(size_t id);
	/// <summary>
	/// Создание поля "Наличие отверстия"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initAngleAlpha(size_t id);
	/// <summary>
	/// Создание поля "Радиус скругления боковых рёбер"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEdgesFilletRadius(size_t id);
	/// <summary>
	/// Создание поля "Угол наклона α"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initHasHole(size_t id);
	/// <summary>
	/// Создание поля "Радиус отверстия"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initHoleRadius(size_t id);
	/// <summary>
	/// Создание поля "Длина фаски отверстия"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initHoleChamferLength(size_t id);
	/// <summary>
	/// Создание кнопки "Применить"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEnterButton(size_t id);
	/// <summary>
	/// Подключение переданного поля
	/// </summary>
	/// <param name="field">Передаваемое поле</param>
	void setFieldActive(IPropertyEditPtr field);
	/// <summary>
	/// Отключение переданного поля
	/// </summary>
	/// <param name="field">Передаваемое поле</param>
	void setFieldInactive(IPropertyEditPtr field);
	/// <summary>
	/// Проверка: переключатель - активен
	/// </summary>
	/// <param name="field">Передаваемое поле</param>
	/// <returns>Логическое значение</returns>
	bool isTwinSwitcherActive(IPropertyTwinSwitcherPtr field);
	/// <summary>
	/// Получение значения для выключателя
	/// </summary>
	/// <param name="field">Передаваемое логическое значение</param>
	/// <returns>Значение для выключателя</returns>
	int getTwinSwitcherValue(bool value);
	/// <summary>
	/// Получение индекса выбранного значения из списка
	/// </summary>
	/// <param name="field">Передаваемое логическое значение</param>
	/// <returns>Индекс выбранного значения</returns>
	long getListValue(IPropertyListPtr field);
};
