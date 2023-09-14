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

/// ����� �������� ���������� ���������
class MainParameters
{
public:
	/// ������ Application 7
	IApplicationPtr application = NULL;
	/// �������� ������ �������
	IPropertyManagerPtr propertyManager = NULL;
	/// ����, � ������� ����� ������������ ������� ��������
	IPropertySlideBoxPtr slideBox = NULL;
	/// ��������� ������
	PropertyManagerLayout gLayout = PropertyManagerLayout::pmAlignLeft;
	/// ������ ����
	CRect* gRect = NULL;
	/// ���� �������� ������ �� ������� ������� ������
	MenuProfile* menuProfile = NULL;
	/// ���� �������� ������ �� ������� �������
	MenuConditions* menuConditions = NULL;

	/// <summary>
	/// ������������� �������� ���������� ���������
	/// </summary>
	/// <param name="hModule">�������� ����� ����������</param>
	void init(HMODULE *hModule);
	/// ������������� ���� �������
	void initPropertyManager();
	/// ������������� ������ ����
	void initModel(IPropertyControlsPtr collection);
	/// ���������� ������� �� ���������� ���� �� �������
	void buildProfileObject();
	/// ���������� ������� �� ���������� ���� �� ������� �������
	void buildConditionsObject();
	/// ��������� ��������� ����
	void getGabaritRect();
	/// ��������� ��������� ����
	void setGabaritRect();
	/// ��������� �������� ���� - �� �������
	void setMenuTypeNone();
	/// ��������� �������� ���� - �� ������� ������� ������
	void setMenuTypeProfile();
	/// ��������� �������� ���� - �� ������� �������
	void setMenuTypeConditions();
	/// ��������: ������� ���� - �� �������
	bool isMenuTypeNone();
	/// ��������: ������� ���� - �� ������� ������� ������
	bool isMenuTypeProfile();
	/// ��������: ������� ���� - �� ������� �������
	bool isMenuTypeConditions();

	/// ������ ����� ����
	enum MenuType {
		type_none,		/// ���� �� �������
		type_profile,	/// ������� ���� �� �������
		type_conditions	/// ������� ���� �� ������� �������
	};

protected:
	/// �������� ����� ����������
	HMODULE* hModule = NULL;
	/// ��� �������� ����
	enum MenuType* menuType = NULL;
};
