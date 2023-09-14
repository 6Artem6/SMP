#include "StdAfx.h"
#include "MenuConditions.h"


SMPBase* MenuConditions::getModel()  { 
	return this->model; 
}

void MenuConditions::init(IPropertyControlsPtr collection) {
	this->model = new SMPConditions();
	this->collection = collection;

	this->initCoordinates(Elements::e_coordinates,
		Elements::e_X, Elements::e_Y, Elements::e_Z);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initSurfaceFigure(Elements::e_surfaceFigure, false);
	this->initSizeType(Elements::e_sizeType);
	this->initSize(Elements::e_size);
	this->initHeight(Elements::e_height);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initAngleAlpha(Elements::e_angleAlpha);
	this->initEdgesFilletRadius(Elements::e_edgesFilletRadius);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initParameterType(Elements::e_parameterType);
	this->initParameterS(Elements::e_parameterS);
	this->initParameterT(Elements::e_parameterT);
	this->initParameterStoT(Elements::e_parameterStoT);

	this->initAngleType(Elements::e_angleType);
	this->initAngleEta(Elements::e_angleEta);

	this->initCutLineCount(Elements::e_cutLineCount);
	// this->initCutDirection(Elements::e_cutDirection);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initHasHole(Elements::e_hasHole);
	this->initHoleRadius(Elements::e_holeRadius);
	this->initHoleChamferLength(Elements::e_holeChamferLength);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initEnterButton(Elements::e_enterButton);

	this->checkHasHole();
	this->checkParameters();
}

bool MenuConditions::updateMenuParameters() {
	this->model->coordinates.X = this->coordinates->GetCoordinate(0)->Value.dblVal;
	this->model->coordinates.Y = this->coordinates->GetCoordinate(1)->Value.dblVal;
	this->model->coordinates.Z = this->coordinates->GetCoordinate(2)->Value.dblVal;
	this->model->surfaceFigure = (enum SurfaceFigure)this->getListValue(this->surfaceFigure);
	this->model->sizeType = (enum SizeType)this->getListValue(this->sizeType);
	this->model->size = this->size->Value.dblVal;
	this->model->height = this->height->Value.dblVal;
	this->model->angleAlpha = this->angleAlpha->Value.dblVal;
	this->model->edgesFilletRadius = this->edgesFilletRadius->Value.dblVal;
	this->model->hasHole = this->isTwinSwitcherActive(this->hasHole);
	this->model->holeRadius = this->holeRadius->Value.dblVal;
	this->model->holeChamferLength = this->holeChamferLength->Value.dblVal;
	this->model->parameterType = (enum ParameterType)this->getListValue(this->parameterType);
	this->model->parameterS = this->parameterS->Value.dblVal;
	this->model->parameterT = this->parameterT->Value.dblVal;
	this->model->parameterStoT = this->parameterStoT->Value.dblVal;
	this->model->angleType = (enum AngleType)this->getListValue(this->angleType);
	this->model->angleEta = this->angleEta->Value.dblVal;
	this->model->cutLineCount = this->cutLineCount->Value.uiVal;
	//this->model->cutDirection = this->isTwinSwitcherActive(this->cutDirection);
	return this->model->updateParameters();
}

void MenuConditions::initParameterType(size_t id) {
	this->parameterType = this->collection->Add(ControlTypeEnum::ksControlListStr);
	this->parameterType->Name = _T("Выбор режимов резания");
	this->parameterType->Id = id;
	this->parameterType->ReadOnly = true;
	this->parameterType->Add(_T("По подаче S"));
	this->parameterType->Add(_T("По глубине t"));
	this->parameterType->Add(_T("По отношению S/t"));
	this->parameterType->SetCurrentByIndex(this->model->parameterType);
}

void MenuConditions::initParameterS(size_t id) {
	this->parameterS = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->parameterS->Name =  _T("Подача S");
	this->parameterS->Id = id;
	this->parameterS->Value = this->model->parameterS;
}

void MenuConditions::initParameterT(size_t id) {
	this->parameterT = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->parameterT->Name = _T("Глубина t");
	this->parameterT->Id = id;
	this->parameterT->Value = this->model->parameterT;
}

void MenuConditions::initParameterStoT(size_t id) {
	this->parameterStoT = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->parameterStoT->Name = _T("Отношение S/t");
	this->parameterStoT->Id = id;
	this->parameterStoT->Value = this->model->parameterStoT;
}

void MenuConditions::initAngleType(size_t id) {
	this->angleType = this->collection->Add(ControlTypeEnum::ksControlListStr);
	this->angleType->Name = _T("Выбор задания угла η");
	this->angleType->Id = id;
	this->angleType->ReadOnly = true;
	this->angleType->Add(_T("По параметру резания"));
	this->angleType->Add(_T("По углу η"));
	this->angleType->SetCurrentByIndex(this->model->angleType);
}

void MenuConditions::initAngleEta(size_t id) {
	this->angleEta = this->collection->Add(ControlTypeEnum::ksControlEditAngle);
	this->angleEta->Name = _T("Угол η");
	this->angleEta->Id = id;
	this->angleEta->Value = this->model->angleEta;
}

void MenuConditions::initCutLineCount(size_t id) {
	this->cutLineCount = this->collection->Add(ControlTypeEnum::ksControlEditInt);
	this->cutLineCount->Name = _T("Количество линий режущей кромки");
	this->cutLineCount->Id = id;
	this->cutLineCount->Value = this->model->cutLineCount;
}

void MenuConditions::initCutDirection(size_t id) {
	this->cutDirection = this->collection->Add(ControlTypeEnum::ksControlTwinSwitcher);
	this->cutDirection->Name = _T("Направление. Левое");
	this->cutDirection->Label2 = _T("Правое");
	this->cutDirection->Id = id;
	this->cutDirection->Value = this->getTwinSwitcherValue(this->model->cutDirection);
}

void MenuConditions::checkParameters() {
	ParameterType p = (enum ParameterType)this->getListValue(this->parameterType);
	AngleType a = (enum AngleType)this->getListValue(this->angleType);
	if (a == AngleType::byParameters) {
		this->setFieldInactive(this->angleEta);
		if (p == ParameterType::byS) {
			this->setFieldActive(this->parameterS);
			this->setFieldInactive(this->parameterT);
			this->setFieldInactive(this->parameterStoT);
		}
		else if (p == ParameterType::byT) {
			this->setFieldInactive(this->parameterS);
			this->setFieldActive(this->parameterT);
			this->setFieldInactive(this->parameterStoT);
		}
		else if (p == ParameterType::byStoT) {
			this->setFieldInactive(this->parameterS);
			this->setFieldInactive(this->parameterT);
			this->setFieldActive(this->parameterStoT);
		}
	}
	else if (a == AngleType::byAngleEta) {
		this->setFieldActive(this->angleEta);
		this->setFieldInactive(this->parameterS);
		this->setFieldInactive(this->parameterT);
		this->setFieldInactive(this->parameterStoT);
	}
}
