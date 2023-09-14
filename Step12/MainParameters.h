#ifndef _MAIN_PARAMETERS_H
#define _MAIN_PARAMETERS_H

#include "Resource.h"

#ifndef _MENU_PROFILE_H
#include "MenuProfile.h"
#endif

#ifndef _MENU_CONDITIONS_H
#include "MenuConditions.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif
#endif

/// Класс основных параметров программы
class MainParameters
{
public:
	/// Компас Application 7
	IApplicationPtr application = NULL;
	/// Менеджер панели свойств
	IPropertyManagerPtr propertyManager = NULL;
	/// Окно, в котором будет отображаться текущий документ
	IPropertySlideBoxPtr slideBox = NULL;
	/// Положение панели
	PropertyManagerLayout gLayout = PropertyManagerLayout::pmAlignLeft;
	/// Размер окна
	CRect* gRect = NULL;
	/// Меню создания модели по профилю режущей кромки
	MenuProfile* menuProfile = NULL;
	/// Меню создания модели по режимам резания
	MenuConditions* menuConditions = NULL;

	/// <summary>
	/// Инициализация основных параметров программы
	/// </summary>
	/// <param name="hModule">Основной адрес библиотеки</param>
	void init(HMODULE *hModule);
	/// Инициализация меню свойств
	void initPropertyManager();
	/// Инициализация модели меню
	void initModel(IPropertyControlsPtr collection);
	/// Построение объекта по параметрам меню по профилю
	void buildProfileObject();
	/// Построение объекта по параметрам меню по режимам резания
	void buildConditionsObject();
	/// Получение габаритов окна
	void getGabaritRect();
	/// Установка габаритов окна
	void setGabaritRect();
	/// Установка текущего меню - не выбрано
	void setMenuTypeNone();
	/// Установка текущего меню - по профилю режущей кромки
	void setMenuTypeProfile();
	/// Установка текущего меню - по режимам резания
	void setMenuTypeConditions();
	/// Проверка: текущее меню - не выбрано
	bool isMenuTypeNone();
	/// Проверка: текущее меню - по профилю режущей кромки
	bool isMenuTypeProfile();
	/// Проверка: текущее меню - по режимам резания
	bool isMenuTypeConditions();

	/// Список типов меню
	enum MenuType {
		type_none,		/// Меню не выбрано
		type_profile,	/// Выбрано меню по профилю
		type_conditions	/// Выбрано меню по режимам резания
	};

protected:
	/// Основной адрес библиотеки
	HMODULE* hModule = NULL;
	/// Тип текущего меню
	enum MenuType* menuType = NULL;
};
