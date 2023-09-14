#include "StdAfx.h"
#include "MenuBase.h"


void MenuBase::initCoordinates(size_t id, size_t id_x, size_t id_y, size_t id_z) {
	this->coordinates = this->collection->Add(ControlTypeEnum::ksControlPoint3D);
	this->coordinates->Name = _T("Координаты центра");
	this->coordinates->Id = id;
	IPropertyEditPtr X = this->coordinates->GetCoordinate(0);
	IPropertyEditPtr Y = this->coordinates->GetCoordinate(1);
	IPropertyEditPtr Z = this->coordinates->GetCoordinate(2);
	X->Id = id_x;
	X->Value = this->getModel()->coordinates.X;
	X->CheckState = CheckStateEnum::ksCheckFixed;
	Y->Id = id_y;
	Y->Value = this->getModel()->coordinates.Y;
	Y->CheckState = CheckStateEnum::ksCheckFixed;
	Z->Id = id_z;
	Z->Value = this->getModel()->coordinates.Z;
	Z->CheckState = CheckStateEnum::ksCheckFixed;
}

void MenuBase::initSurfaceFigure(size_t id, bool hasCircle) {
	this->surfaceFigure = this->collection->Add(ControlTypeEnum::ksControlListStr);
	this->surfaceFigure->Name = _T("Фигура основания");
	this->surfaceFigure->Id = id;
	this->surfaceFigure->ReadOnly = true;
	this->surfaceFigure->Add(_T("Треугольник"));
	this->surfaceFigure->Add(_T("Квадрат"));
	this->surfaceFigure->Add(_T("Пятиугольник"));
	this->surfaceFigure->Add(_T("Шестиугольник"));
	this->surfaceFigure->Add(_T("Тригон"));
	this->surfaceFigure->Add(_T("Ромб 35"));
	this->surfaceFigure->Add(_T("Ромб 55"));
	this->surfaceFigure->Add(_T("Ромб 80"));
	if (hasCircle) {
		this->surfaceFigure->Add(_T("Круг"));
	}
	this->surfaceFigure->SetCurrentByIndex(this->getModel()->surfaceFigure);
}

void MenuBase::initSizeType(size_t id) {
	this->sizeType = this->collection->Add(ControlTypeEnum::ksControlListStr);
	this->sizeType->Name = _T("Тип размера");
	this->sizeType->Id = id;
	this->sizeType->ReadOnly = true;
	this->sizeType->Add(_T("По радиусу описанной окружности"));
	this->sizeType->Add(_T("По радиусу вписанной окружности"));
	this->sizeType->Add(_T("По длине стороны"));
	this->sizeType->SetCurrentByIndex(this->getModel()->sizeType);
}

void MenuBase::initSize(size_t id) {
	this->size = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->size->Name = _T("Величина размера");
	this->size->Id = id;
	this->size->Value = this->getModel()->size;
}

void MenuBase::initHeight(size_t id) {
	this->height = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->height->Name = _T("Высота");
	this->height->Id = id;
	this->height->Value = this->getModel()->height;
}

void MenuBase::initHasHole(size_t id) {
	this->hasHole = this->collection->Add(ControlTypeEnum::ksControlTwinSwitcher);
	this->hasHole->Name = _T("Есть отверстие");
	this->hasHole->Label2 = _T("Нет отверстия");
	this->hasHole->Id = id;
	this->hasHole->Value = this->getTwinSwitcherValue(this->getModel()->hasHole);
}

void MenuBase::initHoleRadius(size_t id) {
	this->holeRadius = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->holeRadius->Name = _T("Радиус отверстия");
	this->holeRadius->Id = id;
	this->holeRadius->Value = this->getModel()->holeRadius;
}

void MenuBase::initHoleChamferLength(size_t id) {
	this->holeChamferLength = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->holeChamferLength->Name = _T("Длина фаски отверстия");
	this->holeChamferLength->Id = id;
	this->holeChamferLength->Value = this->getModel()->holeChamferLength;
}

void MenuBase::initAngleAlpha(size_t id) {
	this->angleAlpha = this->collection->Add(ControlTypeEnum::ksControlEditAngle);
	this->angleAlpha->Name = _T("Угол наклона α");
	this->angleAlpha->Id = id;
	this->angleAlpha->Value = this->getModel()->angleAlpha;
}

void MenuBase::initEdgesFilletRadius(size_t id) {
	this->edgesFilletRadius = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->edgesFilletRadius->Name = _T("Радиус скругления боковых рёбер");
	this->edgesFilletRadius->Id = id;
	this->edgesFilletRadius->Value = this->getModel()->edgesFilletRadius;
}

void MenuBase::initEnterButton(size_t id) {
	this->enterButton = this->collection->Add(ControlTypeEnum::ksControlTextButton);
	this->enterButton->Name = _T("Применить");
	this->enterButton->Id = id;
}

bool MenuBase::isTwinSwitcherActive(IPropertyTwinSwitcherPtr field) {
	return (field->Value.intVal == 1);
}

int MenuBase::getTwinSwitcherValue(bool value) {
	return (value ? 1 : 2);
}

long MenuBase::getListValue(IPropertyListPtr field) {
	return field->Find(field->Value);
}

void MenuBase::setFieldActive(IPropertyEditPtr field) {
	field->ReadOnly = false;
	field->Enable = true;
	field->Visible = true;
}

void MenuBase::setFieldInactive(IPropertyEditPtr field) {
	field->ReadOnly = true;
	field->Enable = false;
	field->Visible = false;
}

void MenuBase::checkHasHole() {
	if (this->isTwinSwitcherActive(this->hasHole)) {
		this->setFieldActive(this->holeRadius);
		this->setFieldActive(this->holeChamferLength);
	}
	else {
		this->setFieldInactive(this->holeRadius);
		this->setFieldInactive(this->holeChamferLength);
	}
}
