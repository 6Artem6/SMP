#ifndef _MENU_CONDITIONS_H
#define _MENU_CONDITIONS_H

#include "MenuBase.h"
#include "SMPConditions.h"

#if _MSC_VER > 1000
#pragma once
#endif
#endif

/// Класс меню параметров модели по режимам резания
class MenuConditions : public MenuBase
{
public:
	/// Модель объекта
	SMPConditions* model = NULL;
	/// Поле "Выбор режимов резания"
	IPropertyListPtr parameterType = NULL;
	/// Поле "Подача S"
	IPropertyEditPtr parameterS = NULL;
	/// Поле "Глубина t"
	IPropertyEditPtr parameterT = NULL;
	/// Поле "Отношение S/t"
	IPropertyEditPtr parameterStoT = NULL;
	/// Поле "Выбор задания угла η"
	IPropertyListPtr angleType = NULL;
	/// Поле "Угол η"
	IPropertyEditPtr angleEta = NULL;
	/// Поле "Количество линий режущей кромки"
	IPropertyEditPtr cutLineCount = NULL;
	/// Поле "Направление режущей кромки"
	IPropertyTwinSwitcherPtr cutDirection = NULL;

	/// Получение модели объекта
	SMPBase* getModel() override;
	/// <summary>
	/// Создание полей меню
	/// </summary>
	/// <param name="collection">Коллекция контролов</param>
	void init(IPropertyControlsPtr collection);
	/// Обновление параметров меню
	bool updateMenuParameters();
	/// Проверка: выбор режима резания и угла η
	void checkParameters();
	
	/// Список идентификаторов полей
	enum Elements {
		e_coordinates = 2000,	/// Координаты центра основания
		e_X,					/// Координата X
		e_Y,					/// Координата Y
		e_Z,					/// Координата Z		
		e_surfaceFigure,		/// Фигура основания
		e_sizeType,				/// Тип размера
		e_size,					/// Величина размера
		e_height,				/// Высота		
		e_angleAlpha,			/// Угол наклона α
		e_edgesFilletRadius,	/// Радиус скругления боковых рёбер
		e_parameterType,		/// Способ задания формы
		e_parameterS,			/// Подача S
		e_parameterT,			/// Глубина t
		e_parameterStoT,		/// Отношение S/t
		e_angleType,			/// Способ задания угла η
		e_angleEta,				/// Угол η
		e_cutLineCount,			/// Количество линий режущей кромки
		e_cutDirection,			/// Направление режущей кромки
		e_cutRadius,			/// Радиус режущей кромки		
		e_hasHole,				/// Наличие отверстия
		e_holeRadius,			/// Радиус отверстия
		e_holeChamferLength,	/// Длина фаски отверстия
		e_enterButton			/// Кнопка "Применить"
	};

protected:
	/// <summary>
	/// Создание поля "Способ задания формы"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initParameterType(size_t id);
	/// <summary>
	/// Создание поля "Подача S"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initParameterS(size_t id);
	/// <summary>
	/// Создание поля "Глубина t"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initParameterT(size_t id);
	/// <summary>
	/// Создание поля "Отношение S/t"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initParameterStoT(size_t id);
	/// <summary>
	/// Создание поля "Способ задания угла η"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initAngleType(size_t id);
	/// <summary>
	/// Создание поля "Угол η"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initAngleEta(size_t id);
	/// <summary>
	/// Создание поля "Количество линий режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initCutLineCount(size_t id);
	/// <summary>
	/// Создание поля "Направление режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initCutDirection(size_t id);
};
