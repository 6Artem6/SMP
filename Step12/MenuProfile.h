#ifndef _MENU_PROFILE_H
#define _MENU_PROFILE_H

#include "MenuBase.h"
#include "SMPProfile.h"

#if _MSC_VER > 1000
#pragma once
#endif
#endif

/// Класс меню параметров модели по профилю режущей кромки
class MenuProfile : public MenuBase
{
public:
	/// Модель объекта
	SMPProfile* model = NULL;
	/// Поле "Длина режущей кромки"
	IPropertyEditPtr cutLength = NULL;
	/// Поле "Угол режущей кромки γ"
	IPropertyEditPtr cutAngleGamma = NULL;
	/// Поле "Радиус режущей кромки"
	IPropertyEditPtr cutRadius = NULL;
	/// Поле "Тип режущей кромки"
	IPropertyTwinSwitcherPtr cutRoundingType = NULL;
	/// Поле "Радиус скругления режущей кромки"
	IPropertyEditPtr cutFilletRadius = NULL;
	/// Поле "Фаска режущей кромки"
	IPropertyEditPtr cutChamferLength = NULL;
	/// Поле "Угол фаски режущей кромки"
	IPropertyEditPtr cutChamferAngle = NULL;

	MenuProfile()  { };
	~MenuProfile() { };
	/// Получение модели объекта
	SMPBase* getModel() override;
	/// <summary>
	/// Создание полей меню
	/// </summary>
	/// <param name="collection">Коллекция контролов</param>
	void init(IPropertyControlsPtr collection);
	/// Обновление параметров меню
	bool updateMenuParameters();
	/// Проверка условия подключения поля "Радиус скругления"
	void checkSurfaceFigure();
	/// Проверка условия подключения полей "Скругление режущей кромки"
	void checkCutRoundingType();

	/// Список идентификаторов полей
	enum Elements {
		e_coordinates = 1000,	/// Координаты центра основания
		e_X,					/// Координата X
		e_Y,					/// Координата Y
		e_Z,					/// Координата Z		
		e_surfaceFigure,		/// Фигура основания
		e_sizeType,				/// Тип размера
		e_size,					/// Величина размера
		e_height,				/// Высота		
		e_angleAlpha,			/// Угол наклона α
		e_edgesFilletRadius,	/// Радиус скругления боковых рёбер
		e_cutLength,			/// Длина режущей кромки
		e_cutAngleGamma,		/// Угол режущей кромки γ
		e_cutRadius,			/// Радиус режущей кромки
		e_cutRoundingType,		/// Тип режущей кромки
		e_cutFilletRadius,		/// Радиус скругления режущей кромки
		e_cutChamferLength,		/// Фаска режущей кромки
		e_cutChamferAngle,		/// Угол фаски режущей кромки		
		e_hasHole,				/// Наличие отверстия
		e_holeRadius,			/// Радиус отверстия
		e_holeChamferLength,	/// Длина фаски отверстия
		e_enterButton,			/// Кнопка "Применить"
	};

protected:
	/// <summary>
	/// Создание поля "Длина режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEvolutionLength(size_t id);
	/// <summary>
	/// Создание поля "Угол режущей кромки γ"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEvolutionAngleGamma(size_t id);
	/// <summary>
	/// Создание поля "Радиус режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEvolutionRadius(size_t id);
	/// <summary>
	/// Создание поля "Скругление режущей кромки" или "Фаска режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEvolutionRoundingType(size_t id);
	/// <summary>
	/// Создание поля "Радиус скругления режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEvolutionFilletRadius(size_t id);
	/// <summary>
	/// Создание поля "Фаска режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEvolutionChamferLength(size_t id);
	/// <summary>
	/// Создание поля "Угол фаски режущей кромки"
	/// </summary>
	/// <param name="id">Номер элемента</param>
	void initEvolutionChamferAngle(size_t id);
};
