#include "StdAfx.h"
#include "MenuProfile.h"


SMPBase* MenuProfile::getModel() {
	return this->model; 
}

void MenuProfile::init(IPropertyControlsPtr collection) {
	this->model = new SMPProfile();
	this->collection = collection;

	this->initCoordinates(Elements::e_coordinates,
		Elements::e_X, Elements::e_Y, Elements::e_Z);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initSurfaceFigure(Elements::e_surfaceFigure, true);
	this->initSizeType(Elements::e_sizeType);
	this->initSize(Elements::e_size);
	this->initHeight(Elements::e_height);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initAngleAlpha(Elements::e_angleAlpha);
	this->initEdgesFilletRadius(Elements::e_edgesFilletRadius);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initEvolutionLength(Elements::e_cutLength);
	this->initEvolutionAngleGamma(Elements::e_cutAngleGamma);
	this->initEvolutionRadius(Elements::e_cutRadius);
	this->initEvolutionRoundingType(Elements::e_cutRoundingType);
	this->initEvolutionFilletRadius(Elements::e_cutFilletRadius);
	this->initEvolutionChamferLength(Elements::e_cutChamferLength);
	this->initEvolutionChamferAngle(Elements::e_cutChamferAngle);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initHasHole(Elements::e_hasHole);
	this->initHoleRadius(Elements::e_holeRadius);
	this->initHoleChamferLength(Elements::e_holeChamferLength);

	this->collection->Add(ControlTypeEnum::ksControlSeparator);
	this->initEnterButton(Elements::e_enterButton);

	this->checkHasHole();
	this->checkSurfaceFigure();
	this->checkCutRoundingType();
}

bool MenuProfile::updateMenuParameters() {
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
	this->model->cutLength = this->cutLength->Value.dblVal;
	this->model->cutAngleGamma = this->cutAngleGamma->Value.dblVal;
	this->model->cutRadius = this->cutRadius->Value.dblVal;
	this->model->cutRoundingType = this->isTwinSwitcherActive(this->cutRoundingType);
	this->model->cutFilletRadius = this->cutFilletRadius->Value.dblVal;
	this->model->cutChamferLength = this->cutChamferLength->Value.dblVal;
	this->model->cutChamferAngle = this->cutChamferAngle->Value.dblVal;

	return this->model->updateParameters();
}

void MenuProfile::initEvolutionLength(size_t id) {
	this->cutLength = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->cutLength->Name = _T("Длина режущей кромки");
	this->cutLength->Id = id;
	this->cutLength->Value = this->model->cutLength;
}

void MenuProfile::initEvolutionAngleGamma(size_t id) {
	this->cutAngleGamma = this->collection->Add(ControlTypeEnum::ksControlEditAngle);
	this->cutAngleGamma->Name = _T("Угол режущей кромки γ");
	this->cutAngleGamma->Id = id;
	this->cutAngleGamma->Value = this->model->cutAngleGamma;
}

void MenuProfile::initEvolutionRadius(size_t id) {
	this->cutRadius = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->cutRadius->Name = _T("Радиус режущей кромки");
	this->cutRadius->Id = id;
	this->cutRadius->Value = this->model->cutRadius;
}

void MenuProfile::initEvolutionRoundingType(size_t id) {
	this->cutRoundingType = this->collection->Add(ControlTypeEnum::ksControlTwinSwitcher);
	this->cutRoundingType->Name = _T("Скругление режущей кромки");
	this->cutRoundingType->Label2 = _T("Фаска режущей кромки");
	this->cutRoundingType->Id = id;
	this->cutRoundingType->Value = this->getTwinSwitcherValue(this->model->cutRoundingType);
}

void MenuProfile::initEvolutionFilletRadius(size_t id) {
	this->cutFilletRadius = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->cutFilletRadius->Name = _T("Радиус скругления режущей кромки");
	this->cutFilletRadius->Id = id;
	this->cutFilletRadius->Value = this->model->cutFilletRadius;
}

void MenuProfile::initEvolutionChamferLength(size_t id) {
	this->cutChamferLength = this->collection->Add(ControlTypeEnum::ksControlEditLength);
	this->cutChamferLength->Name = _T("Фаска режущей кромки");
	this->cutChamferLength->Id = id;
	this->cutChamferLength->Value = this->model->cutChamferLength;
}

void MenuProfile::initEvolutionChamferAngle(size_t id) {
	this->cutChamferAngle = this->collection->Add(ControlTypeEnum::ksControlEditAngle);
	this->cutChamferAngle->Name = _T("Угол фаски режущей кромки");
	this->cutChamferAngle->Id = id;
	this->cutChamferAngle->Value = this->model->cutChamferAngle;
}

void MenuProfile::checkSurfaceFigure() {
	enum SurfaceFigure value = (enum SurfaceFigure)this->getListValue(this->surfaceFigure);
	if (value == SurfaceFigure::circle) {
		this->setFieldInactive(this->edgesFilletRadius);
	}
	else {
		this->setFieldActive(this->edgesFilletRadius);
	}
}

void MenuProfile::checkCutRoundingType() {
	if (this->isTwinSwitcherActive(this->cutRoundingType)) {
		this->setFieldActive(this->cutFilletRadius);
		this->setFieldInactive(this->cutChamferLength);
		this->setFieldInactive(this->cutChamferAngle);
	}
	else {
		this->setFieldInactive(this->cutFilletRadius);
		this->setFieldActive(this->cutChamferLength);
		this->setFieldActive(this->cutChamferAngle);
	}
}
