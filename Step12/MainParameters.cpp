#include "StdAfx.h"
#include "MainParameters.h"


void MainParameters::init(HMODULE *hModule) {
	this->hModule = hModule;
	this->gLayout = PropertyManagerLayout::pmAlignRight;
	this->gRect = new CRect(20, 20, 270, 320);
	this->menuProfile = new MenuProfile();
	this->menuConditions = new MenuConditions();
	this->menuType = new enum MenuType(MenuType::type_none);
}

void MainParameters::initPropertyManager() {
	this->propertyManager = this->application->CreatePropertyManager(true);
	this->propertyManager->Layout = PropertyManagerLayout::pmAlignLeftInGroup;
	this->propertyManager->Caption = _T("Панель СМП");
	this->propertyManager->SpecToolbar = SpecPropertyToolBarEnum::pnEnterEscCreateSaveSearchHelp;
	// this->setGabaritRect();
}

void MainParameters::initModel(IPropertyControlsPtr collection) {
	if (this->isMenuTypeProfile()) {
		this->menuProfile->init(collection);
		this->propertyManager->Caption = _T("Панель СМП по профилю режущей кромки");
	}
	else if (this->isMenuTypeConditions()) {
		this->menuConditions->init(collection);
		this->propertyManager->Caption = _T("Панель СМП по режимам резания");
	}	
	this->propertyManager->ShowTabs();
	this->propertyManager->UpdateTabs();
}

void MainParameters::buildProfileObject() {
	this->menuProfile->model->init(this->application, *this->hModule);
	if (this->menuProfile->updateMenuParameters()) {
		this->menuProfile->model->createModel();
	}
}

void MainParameters::buildConditionsObject() {
	this->menuConditions->model->init(this->application, *this->hModule);
	if (this->menuConditions->updateMenuParameters()) {
		this->menuConditions->model->createModel();
	}
}

void MainParameters::getGabaritRect() {
	this->propertyManager->GetGabaritRect(
		&this->gRect->left,
		&this->gRect->top,
		&this->gRect->right,
		&this->gRect->bottom
	);
}

void MainParameters::setGabaritRect() {
	this->propertyManager->SetGabaritRect(
		this->gRect->left,
		this->gRect->top,
		this->gRect->right,
		this->gRect->bottom
	);
}

void MainParameters::setMenuTypeNone() {
	*this->menuType = MenuType::type_none;
}

void MainParameters::setMenuTypeProfile() {
	*this->menuType = MenuType::type_profile;
}

void MainParameters::setMenuTypeConditions() {
	*this->menuType = MenuType::type_conditions;
}

bool MainParameters::isMenuTypeNone() {
	return (*this->menuType == MenuType::type_none);
}

bool MainParameters::isMenuTypeProfile() {
	return (*this->menuType == MenuType::type_profile);
}

bool MainParameters::isMenuTypeConditions() {
	return (*this->menuType == MenuType::type_conditions);
}
