#include "StdAfx.h"
#include "SMPConditions.h"

extern int LibMessageErrorCreateModel(LPWSTR reason);

void SMPConditions::createModel() {
	this->addEmbodiment();
	this->drawBase();
	this->addExtrusion();
	this->addEdgesFillet();
	this->addBladePlanesByMesh();
	this->addHole();
	this->baseLocalCS->Current = false;
}

bool SMPConditions::isByS() {
	return (this->parameterType == ParameterType::byS);
}

bool SMPConditions::isByT() {
	return (this->parameterType == ParameterType::byT);
}

bool SMPConditions::isByStoT() {
	return (this->parameterType == ParameterType::byStoT);
}

bool SMPConditions::isByParameters() {
	return (this->angleType == AngleType::byParameters);
}

bool SMPConditions::isByAngleEta() {
	return (this->angleType == AngleType::byAngleEta);
}

double SMPConditions::getAngleLambdaD(double S, double t) {
	return this->lambdaK + S * this->lambdaKS + this->lambdaKT * t;
}

bool SMPConditions::updateParameters() {
	this->points.clear();
	LPWSTR reason = NULL;
	if (this->angleAlpha >= 90) {
		reason = _T("неверный угол наклона пластины α выше либо равен 90 градусов.");
	} else if (this->angleAlpha < 0) {
		reason = _T("неверный угол наклона пластины α ниже 0 градусов.");
	}
	if (this->angleEta >= 90) {
		reason = _T("неверный угол η выше либо равен 90 градусов.");
	}
	else if (this->angleEta <= 0) {
		reason = _T("неверный угол η ниже либо равен 0 градусов.");
	}
	if (reason != NULL) {
		LibMessageErrorCreateModel(reason);
		return false;
	}
	if (this->cutLineCount < 5) {
		this->cutLineCount = 5;
	} else if (this->cutLineCount > 1000) {
		this->cutLineCount = 1000;
	}
	double angleMain = 0;
	if (this->isCircle()) {
		this->updateParametersCircle();
	}
	else if (this->isNGon()) {
		this->updateParametersNGon(&angleMain);
	}
	else if (this->isTrigon()) {
		this->updateParametersTrigon(&angleMain);
	}
	else if (this->isRhombus()) {
		this->updateParametersRhombus(&angleMain);
	}
	if (this->outerRadiusFillet < 1) {
		reason = _T("маленький размер радиуса описанной окружности модели.");
		LibMessageErrorCreateModel(reason);
		return false;
	}
	if (this->isByParameters()) {
		this->updateConditionsByParameters(&reason);
	}
	else if (this->isByAngleEta()) {
		this->updateConditionsByAngleEta(&reason);
	}
	if (reason != NULL) {
		LibMessageErrorCreateModel(reason);
		return false;
	}
	if (this->hasHole && (this->holeRadius > this->innerRadiusFigure)) {
		this->holeRadius = this->innerRadiusFigure;
	}
	return true;
}

void SMPConditions::updateConditionsByParameters(LPWSTR* reason) {
	if (!this->isByParameters()) {
		return;
	}
	if (this->isByS()) {
		if (this->parameterS <= 0) {
			*reason = _T("неположительный параметр подачи S.");
		}
		this->parameterT = -(this->lambdaK + this->parameterS * this->lambdaKS) / this->lambdaKT;
		if (this->parameterT <= 0) {
			*reason = _T("некорректный параметр подачи S приводит к неположительному параметру глубины t.");
		}
	}
	else if (this->isByT()) {
		if (this->parameterT <= 0) {
			*reason = _T("неположительный параметр глубины t.");
		}
		this->parameterS = -(this->lambdaK + this->lambdaKT * this->parameterT) / this->lambdaKS;
		if (this->parameterS <= 0) {
			*reason = _T("некорректный параметр глубины t приводит к неположительному параметру подачи S.");
		}
	}
	else if (this->isByStoT()) {
		if (this->parameterStoT <= 0) {
			*reason = _T("неположительный параметр отношения подачи S к глубине t.");
		}
		this->parameterT = -this->lambdaK / (this->parameterStoT * this->lambdaKS + this->lambdaKT);
		this->parameterS = this->parameterStoT * this->parameterT;
		if (this->parameterT <= 0) {
			*reason = _T("некорректный параметр отношения подачи S к глубине t приводит к неположительному параметру глубины t.");
		}
		if (this->parameterS <= 0) {
			*reason = _T("некорректный параметр отношения подачи S к глубине t приводит к неположительному параметру подачи S.");
		}
	}
}

void SMPConditions::updateConditionsByAngleEta(LPWSTR* reason) {
	if (!this->isByAngleEta()) {
		return;
	}
	double angleMain = this->getAngleMain();
	double angleHalf = angleMain / 2;
	double angleMax = 90 - angleHalf;
	double delta = this->outerRadiusFigure - this->outerRadiusFillet;
	int part_count = 100;
	double precision = 0.000001;
	double eta_min = 0;
	double eta_max = 0;
	double value_min = 0;
	double value_max = 0;
	point2D sCoord, tCoord, maxCoord;
	maxCoord.X = -this->edgesFilletRadius * Helper::sinD(angleMax);
	maxCoord.Y = -this->edgesFilletRadius * (1 - Helper::cosD(angleMax));
	if (this->isByS()) {
		double angle = (180 - angleMain) / 2;
		value_min = 0.01;
		value_max = -2 * maxCoord.X;
		eta_min = this->getEta(value_min, delta, angleHalf, &sCoord, &tCoord, maxCoord);
		eta_max = this->getEta(value_max, delta, angleHalf, &sCoord, &tCoord, maxCoord);
	}
	else if (this->isByT()) {
		double S_min = 0.005;
		double S_max = -2 * maxCoord.X;
		double T_min = -(this->lambdaK + S_min * this->lambdaKS) / this->lambdaKT;
		double T_max = -(this->lambdaK + S_max * this->lambdaKS) / this->lambdaKT;
		double angle = (180 - angleMain) / 2;
		value_min = T_min;
		value_max = T_max;
	}
	else if (this->isByStoT()) {
		value_min = 0.01;
		value_max = 0.3;
	}

	if (!this->isByS()) {
		double value_delta = value_max - value_min;
		bool set = false;
		for (size_t i = 0; i < part_count; i++) {
			double v = value_min + value_delta * (i + 1) / part_count;
			double e = this->getEta(v, delta, angleHalf, &sCoord, &tCoord, maxCoord);
			if (!isnan(e)) {
				if (!set) {
					eta_min = e;
					eta_max = e;
					set = true;
				}
				if (e < eta_min) {
					eta_min = e;
				}
				if (e > eta_min) {
					eta_max = e;
				}
			}
		}
	}
		
	if (this->angleEta < eta_min) {
		*reason = _T("слишком низкий угол η для заданного радиуса скругления боковых рёбер.");
	}
	else if (this->angleEta > eta_max) {
		*reason = _T("слишком большой угол η для заданного радиуса скругления боковых рёбер.");
	}
	if (*reason != NULL) {
		return;
	}
	
	double value_delta = abs(value_max - value_min);
	double value = value_min;
	double eta = 0;
	bool add = true;
	do {
		if (this->isByT() && (sCoord.X >= 0)) {
			value += value_delta;
		} 
		else if (!isnan(eta) && (value < 0)) {
			value += value_delta;
		} 
		else if (isnan(eta)) {
			if (add) {
				value -= value_delta;
			}
			else {
				value += value_delta;
			}
			add = !add;
		}
		eta = this->getEta(value, delta, angleHalf, &sCoord, &tCoord, maxCoord);
		value_delta /= 2;
		if (!isnan(eta)) {
			if (this->angleEta > eta) {
				add = true;
			} 
			else if (this->angleEta < eta) {
				add = false;
			}
		}
		if (add) {
			value += value_delta;
		}
		else {
			value -= value_delta;
		}
	} while (value_delta > precision);
			
	this->parameterS = -2 * sCoord.X;
	this->parameterT = -tCoord.Y;
	this->parameterStoT = this->parameterS/ this->parameterT;
}

void SMPConditions::updateParametersCircle() {
	if (!this->isCircle()) {
		return;
	}
	this->innerRadiusFigure = this->size;
	this->outerRadiusFigure = this->innerRadiusFigure;
	this->sideFigure = this->innerRadiusFigure;
	this->outerRadiusFillet = this->outerRadiusFigure;
}

void SMPConditions::updateParametersNGon(double* angleMain) {
	if (!this->isNGon()) {
		return;
	}
	double angleVertex = this->getAngleVertex();
	double angleRad = (180 - angleVertex) / 2;
	*angleMain = angleRad;
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure = this->size;
	}
	else if (this->isByOuterRadius()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleRad);
	}
	else if (this->isBySide()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleRad) * Helper::sinD(angleRad) / Helper::sinD(angleVertex);
	}
	this->outerRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleRad);
	this->outerRadiusFillet = this->outerRadiusFigure;
	if (this->isByOuterRadius()) {
		double deltaOuterRadius = this->edgesFilletRadius / Helper::sinD(angleRad) - this->edgesFilletRadius;
		this->outerRadiusFigure += deltaOuterRadius;
		this->innerRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleRad);
	}
	this->sideFigure = this->outerRadiusFigure * Helper::sinD(angleVertex / 2) / Helper::sinD(angleRad);
	if (!this->isByOuterRadius()) {
		this->innerRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleRad);
		double deltaOuterRadius = this->edgesFilletRadius / Helper::sinD(angleRad) - this->edgesFilletRadius;
		this->outerRadiusFillet -= deltaOuterRadius;
	}
}

void SMPConditions::updateParametersTrigon(double* angleMain) {
	if (!this->isTrigon()) {
		return;
	}
	double angleVertex = 60;
	double angleGreater = 80;
	double angleLess = 40;
	*angleMain = angleLess;
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure = this->size;
	}
	else if (this->isByOuterRadius()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess);
	}
	else if (this->isBySide()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess) * Helper::sinD(angleGreater) / Helper::sinD(angleVertex);
	}
	this->outerRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleLess);
	this->outerRadiusFillet = this->outerRadiusFigure;
	this->outerLessRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleGreater);
	this->outerLessRadiusFillet = this->outerLessRadiusFigure;
	if (this->isByOuterRadius()) {
		double deltaOuterRadius = this->edgesFilletRadius / Helper::sinD(angleLess) - this->edgesFilletRadius;
		this->outerRadiusFigure += deltaOuterRadius;
		this->innerRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleLess);

		this->outerLessRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleLess) / Helper::sinD(angleGreater);
		this->outerLessRadiusFillet = this->outerRadiusFigure * Helper::sinD(angleLess) / Helper::sinD(angleGreater);
		double deltaOuterLessRadius = this->edgesFilletRadius / Helper::sinD(angleGreater) - this->edgesFilletRadius;
		this->outerLessRadiusFillet -= deltaOuterLessRadius;
	}
	this->sideFigure = this->outerRadiusFigure * Helper::sinD(angleVertex) / Helper::sinD(angleGreater);
	if (!this->isByOuterRadius()) {
		this->innerRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleLess);
		double deltaOuterRadius = this->edgesFilletRadius / Helper::sinD(angleLess) - this->edgesFilletRadius;
		this->outerRadiusFillet -= deltaOuterRadius;

		this->outerLessRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleLess) / Helper::sinD(angleGreater);
		this->outerLessRadiusFillet = this->outerRadiusFigure * Helper::sinD(angleLess) / Helper::sinD(angleGreater);
		double deltaOuterLessRadius = this->edgesFilletRadius / Helper::sinD(angleGreater) - this->edgesFilletRadius;
		this->outerLessRadiusFillet -= deltaOuterLessRadius;
	}
}

void SMPConditions::updateParametersRhombus(double* angleMain) {
	if (!this->isRhombus()) {
		return;
	}
	double angle = this->getRhombusAngle();
	double angleLess = angle / 2;
	double angleGreater = (180 - angle) / 2;
	*angleMain = angleLess;
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure = this->size;
	}
	else if (this->isByOuterRadius()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess);
	}
	else if (this->isBySide()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess) * Helper::cosD(angleLess);
	}
	this->outerRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleLess);
	this->outerRadiusFillet = this->outerRadiusFigure;
	this->outerLessRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleGreater);
	this->outerLessRadiusFillet = this->outerLessRadiusFigure;
	if (this->isByOuterRadius()) {
		double deltaOuterRadius = this->edgesFilletRadius / Helper::sinD(angleLess) - this->edgesFilletRadius;
		this->outerRadiusFigure += deltaOuterRadius;
		this->innerRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleLess);

		this->outerLessRadiusFigure = this->outerRadiusFigure * Helper::tanD(angleLess);
		this->outerLessRadiusFillet = this->outerRadiusFigure * Helper::tanD(angleLess);
		double deltaOuterLessRadius = this->edgesFilletRadius / Helper::sinD(angleGreater) - this->edgesFilletRadius;
		this->outerLessRadiusFillet -= deltaOuterLessRadius;
	}
	this->sideFigure = this->outerRadiusFigure / Helper::cosD(angleLess);
	if (!this->isByOuterRadius()) {
		this->innerRadiusFigure = this->outerRadiusFigure * Helper::sinD(angleLess);
		double deltaOuterRadius = this->edgesFilletRadius / Helper::sinD(angleLess) - this->edgesFilletRadius;
		this->outerRadiusFillet -= deltaOuterRadius;

		this->outerLessRadiusFigure = this->outerRadiusFigure * Helper::tanD(angleLess);
		this->outerLessRadiusFillet = this->outerRadiusFigure * Helper::tanD(angleLess);
		double deltaOuterLessRadius = this->edgesFilletRadius / Helper::sinD(angleGreater) - this->edgesFilletRadius;
		this->outerLessRadiusFillet -= deltaOuterLessRadius;
	}
}

void SMPConditions::addBladePlanesByMesh() {
	if (this->isCircle()) {
		return;
	}
	unsigned short count = this->getCutVertexCount();
	double angleVertex = this->getAngleVertex();
	double angleDirection = this->getAngleMainDirection();
	double angleMain = this->getAngleMain();
	double angleHalf = angleMain / 2;
	double angleMax = 90 - angleHalf;
	double angleFillet = 180 - angleMain;

	point3D planeMinCoord, planeMaxCoord;
	point2D maxCoord, sCoord, tCoord;
	maxCoord.X = -this->edgesFilletRadius * Helper::sinD(angleMax);
	maxCoord.Y = -this->edgesFilletRadius * (1 - Helper::cosD(angleMax));
	double delta = this->outerRadiusFigure - this->outerRadiusFillet;

	PArray<IModelObjectPtr> linesArray;
	bool begin = true;
	int end_count = this->cutLineCount;
	this->addBladePlaneByMesh(&linesArray, begin, end_count, delta, angleHalf, angleDirection,
		&planeMinCoord, &planeMaxCoord, &sCoord, &tCoord, maxCoord);
	
	bool needAdditionalCut = true;
	if (this->isByT() && (this->parameterS < 0)) {
		needAdditionalCut = false;
	}
	if (needAdditionalCut) {
		end_count = (int)(angleFillet / this->cutLineEndCount);
		begin = false;
		this->addBladePlaneByMesh(&linesArray, begin, end_count, delta, angleHalf, angleDirection,
			&planeMinCoord, &planeMaxCoord, &sCoord, &tCoord, maxCoord);
	}

	ISurfaceContainerPtr surfaceContainer(this->part7);
	INurbsSurfaceByCurvesMeshPtr surface = surfaceContainer->GetNurbsSurfacesByCurvesMeshs()->Add();
	surface->CheckSelfIntersection = true;
	surface->UCurves = CreateDispSafeArray(linesArray);
	surface->Hidden = true;
	surface->Update();

	IFacePtr faceMesh = NULL;
	_variant_t faces = this->feature->GetModelObjects(Obj3dType::o3d_face);
	LPDISPATCH* pFaces = NULL;
	parametersSafeArray f;
	Helper::getSafeArrayData(&faces, &pFaces, &f);
	for (long idf = 0; (idf < f.count) && !faceMesh; idf++) {
		IFacePtr face = pFaces[idf];
		if (face->GetBaseSurface3DType() == ksMathSurface3DTypeEnum::ksMeshSurface) {
			point3D sMinCoord, sMaxCoord;
			face->GetMathSurface()->GetGabarit(
				&sMinCoord.X, &sMinCoord.Y, &sMinCoord.Z,
				&sMaxCoord.X, &sMaxCoord.Y, &sMaxCoord.Z);
			if (Helper::isRoundEqualCoordinates3D(sMinCoord, planeMinCoord) &&
				Helper::isRoundEqualCoordinates3D(sMaxCoord, planeMaxCoord)) {
				faceMesh = face;
			}
		}
	}

	IAxis3DPtr axis = this->baseLocalCS->GetDefaultObject(Obj3dType::o3d_axisOZ);
	ICutExtrusionPtr cutExtrusion = this->modelContainer->GetExtrusions()->Add(Obj3dType::o3d_cutExtrusion);
	cutExtrusion->Name = _T("Режущая кромка");
	cutExtrusion->Direction = ksDirectionTypeEnum::dtReverse;
	IExtrusion1Ptr extrusionSurface(cutExtrusion);
	extrusionSurface->Profile = faceMesh;
	if (this->isByStoT()) {
		cutExtrusion->SetSideParameters(
			false, ksEndTypeEnum::etThroughAll,
			0, 0, false, axis
		);
		extrusionSurface->DirectionObject = axis;
	}
	else {
		double rotationAngle = 0;
		bool revert = false;
		point2D pointFirstCoord, pointLastCoord;
		this->getPoint(0, &pointFirstCoord.X, &pointFirstCoord.Y);
		this->getPoint(1, &pointLastCoord.X, &pointLastCoord.Y);
		if (Helper::isRoundEqual(pointFirstCoord.X, pointLastCoord.X)) {
			rotationAngle = 0;
			if (pointFirstCoord.X < this->coordinates.X) {
				revert = true;
			}
		}
		else if (Helper::isRoundEqual(pointFirstCoord.Y, pointLastCoord.Y)) {
			rotationAngle = 90;
			if (pointFirstCoord.Y < this->coordinates.Y) {
				revert = true;
			}
		}
		else {
			vector2D p(pointLastCoord, pointFirstCoord);
			double angle_p = p.getAngleAlpha();
			double tangent_p = p.getTangentAlpha();
			double yc = tangent_p * this->coordinates.X + pointFirstCoord.X;
			rotationAngle = angle_p - 90;
			if (((tangent_p > 0) && (yc >= this->coordinates.Y)) ||
				((tangent_p < 0) && (yc <= this->coordinates.Y))) {
				revert = true;
			}
		}
		if (revert) {
			rotationAngle += 180;
		}
		double depth = this->outerRadiusFigure;
		IVector3DPtr v = extrusionSurface->Vector3D;
		v->ParametersType = ksVector3DParametersTypeEnum::ksVector3DCoefficients;
		IVector3DByCoefficientsParametersPtr vector_parameters = v->Parameters;
		vector_parameters->LocalCS = this->baseLocalCS;
		vector_parameters->CoefficientByX = Helper::cosD(rotationAngle);
		vector_parameters->CoefficientByY = Helper::sinD(rotationAngle);
		vector_parameters->CoefficientByZ = 1;
		cutExtrusion->SetSideParameters(
			true, ksEndTypeEnum::etBlind,
			depth, 0, false, NULL
		);
	}
	cutExtrusion->Update();

	ICircularPatternPtr circularPattern = this->modelContainer->GetFeaturePatterns()->Add(Obj3dType::o3d_circularCopy);
	circularPattern->Name = _T("Массив режущих кромок");
	circularPattern->InitialObjects = (ICutExtrusion*)cutExtrusion;
	circularPattern->BuildingType = ksCircularPatternBuildingTypeEnum::ksCPSaveAll;
	circularPattern->BasePoint = this->baseLocalCS->GetDefaultObject(Obj3dType::o3d_pointCS);
	circularPattern->Axis = axis;
	circularPattern->Step2 = 360;
	circularPattern->Count1 = 1;
	circularPattern->Count2 = count;
	circularPattern->BoundaryInstancesStepFactor2 = true;
	circularPattern->Update();
}

void SMPConditions::addBladePlaneByMesh(PArray<IModelObjectPtr>* linesArray, 
	bool begin, int count, double delta, double angleHalf, double angleDirection,
	point3D* planeMinCoord, point3D* planeMaxCoord,
	point2D* sCoord, point2D* tCoord, point2D maxCoord) {
	double lCount = (double)count;
	/*int begin_i;
	if (this->isByT() && this->parameterS < 0) {
		begin_i = 1;
	} 
	else if (begin) {
		begin_i = 0;
	}
	else {
		begin_i = count - 1;
	}*/
	int begin_i = begin ? 0 : count - 1;
	int end_i = this->isByT() ? 1 : 0;
	for (int i = begin_i; begin ? (i < count) : (i >= end_i); begin ? i++ : i--) {
		point3D pointBeginCoord, pointEndCoord;
		double current = (double)i;
		double percent = (lCount - current) / lCount;
		if (!begin && this->isByStoT()) {
			percent = (1.0 - percent) / this->cutLineCount;
		}
		this->findBladePlanePoints(begin, percent, delta, angleHalf, angleDirection,
			&pointBeginCoord, &pointEndCoord, sCoord, tCoord, maxCoord);

		if (!Helper::isEqualCoordinates2D(pointBeginCoord, pointEndCoord)) {
			bool first = (begin && (i == 0));
			Helper::setMinValues3D(first, planeMinCoord, pointEndCoord, pointBeginCoord);
			Helper::setMaxValues3D(first, planeMaxCoord, pointEndCoord, pointBeginCoord);
			ILineSegment3DPtr line = this->modelContainer->AddObject(Obj3dType::o3d_LineSegment3D);
			line->SetPoint(true, pointEndCoord.X, pointEndCoord.Y, pointEndCoord.Z);
			line->SetPoint(false, pointBeginCoord.X, pointBeginCoord.Y, pointBeginCoord.Z);
			line->Update();
			linesArray->Add(new IModelObjectPtr(line));
		}
	}
}

void SMPConditions::findBladePlanePoints(bool begin, double percent, 
	double delta, double angleHalf, double angleDirection,
	point3D* pointBeginCoord, point3D* pointEndCoord,
	point2D* sCoord, point2D* tCoord, point2D maxCoord) {

	pointBeginCoord->X = pointEndCoord->X = this->coordinates.X;
	pointBeginCoord->Y = pointEndCoord->Y = this->coordinates.Y;
	pointBeginCoord->Z = pointEndCoord->Z = this->getTop();
	short direction = this->cutDirection ? 1 : -1;
	short signT = -1;
	short signS = -1;
	bool endStoT = false;
	if (this->isByS()) {
		if (!begin) {
			signT = 1;
		}
		sCoord->X = -this->parameterS / 2;
		if (sCoord->X > maxCoord.X) {
			double cat2 = Helper::cat(this->edgesFilletRadius, sCoord->X);
			sCoord->Y = cat2 - this->edgesFilletRadius;
		}
		else {
			sCoord->Y = sCoord->X * Helper::cotD(angleHalf) + delta;
		}
		if (begin) {
			tCoord->Y = -this->parameterT * percent;
			if (tCoord->Y == 0) {
				tCoord->X = 0;
			} else if (tCoord->Y > maxCoord.Y) {
				double cat1 = this->edgesFilletRadius + tCoord->Y;
				tCoord->X = -Helper::cat(this->edgesFilletRadius, cat1);
			}
			else {
				tCoord->X = -(delta - tCoord->Y) * Helper::tanD(angleHalf);
			}
		}
		else {
			tCoord->Y = sCoord->Y * percent;
			if (tCoord->Y == 0) {
				tCoord->X = 0;
			}
			else {
				double cat1 = this->edgesFilletRadius + tCoord->Y;
				tCoord->X = Helper::cat(this->edgesFilletRadius, cat1);
			}
		}
	}
	else if (this->isByT()) {
		if ((this->parameterS < 0) || ((this->parameterS >= 0) && !begin)) {
			signS = 1;
		}
		tCoord->Y = -this->parameterT;
		if (tCoord->Y > maxCoord.Y) {
			double cat1 = this->edgesFilletRadius + tCoord->Y;
			tCoord->X = Helper::cat(this->edgesFilletRadius, cat1);
		}
		else {
			tCoord->X = (delta - tCoord->Y) * Helper::tanD(angleHalf);
		}
		if ((this->parameterS < 0) && begin) {
			double x_min = -this->parameterS;
			double x_max = (tCoord->X < -maxCoord.X) ? tCoord->X : -maxCoord.X;
			double x_length = x_max - x_min;
			sCoord->X = x_min + x_length * percent;
			if (sCoord->X < -maxCoord.X) {
				double cat2 = Helper::cat(this->edgesFilletRadius, sCoord->X);
				sCoord->Y = -(cat2 - this->edgesFilletRadius);
			}
			else {
				sCoord->Y = (sCoord->X * Helper::cotD(angleHalf) + delta);
			}
		}
		else if ((this->parameterS >= 0) && begin) {
			sCoord->X = -(this->parameterS / 2) * percent;
			if (sCoord->X > maxCoord.X) {
				double cat2 = Helper::cat(this->edgesFilletRadius, sCoord->X);
				sCoord->Y = cat2 - this->edgesFilletRadius;
			}
			else {
				sCoord->Y = sCoord->X * Helper::cotD(angleHalf) + delta;
			}
		}
		else if (!begin) {
			double x_max = -maxCoord.X;
			double x = (tCoord->X < x_max) ? tCoord->X : x_max;
			sCoord->X = x * percent;
			sCoord->Y = 0;
		}
	}
	else if (this->isByStoT()) {
		if (!percent) {
			double cnt = (double)(int)((180 - angleHalf * 2) / this->cutLineEndCount);
			double prc = (cnt - 1.0) / cnt;
			percent = (1.0 - prc) / this->cutLineCount;
			endStoT = true;
			signS = 1;
		}
		sCoord->X = -(this->parameterS / 2) * percent;
		tCoord->Y = -this->parameterT * percent;
		if (sCoord->X > maxCoord.X) {
			double cat2 = Helper::cat(this->edgesFilletRadius, sCoord->X);
			sCoord->Y = cat2 - this->edgesFilletRadius;
		}
		else {
			sCoord->Y = sCoord->X * Helper::cotD(angleHalf) + delta;
		}
		if (tCoord->Y > maxCoord.Y) {
			double cat1 = this->edgesFilletRadius + tCoord->Y;
			tCoord->X = -Helper::cat(this->edgesFilletRadius, cat1);
		}
		else {
			tCoord->X = -(delta - tCoord->Y) * Helper::tanD(angleHalf);
		}
		if (endStoT) {
			sCoord->X = 0;
			sCoord->Y = 0;
			tCoord->Y = 0.001 * this->edgesFilletRadius;
		}
	}
	double angleLambdaD = this->getAngleLambdaD(-2 * sCoord->X, signT * tCoord->Y);

	sCoord->X *= direction;
	tCoord->X *= direction;
	double angleBeginS = 0;
	double lengthBeginS = Helper::hyp(sCoord->X, sCoord->Y);
	if (sCoord->X) {
		int sign;
		if (this->isByStoT() && endStoT) {
			sign = -1;
		}
		else {
			sign = signS;
		}
		angleBeginS = Helper::atanD(-sCoord->Y / (sign * sCoord->X));
	}

	point2D offsetS;
	offsetS.X = lengthBeginS * Helper::sinD(angleDirection - angleBeginS);
	offsetS.Y = lengthBeginS * Helper::cosD(angleDirection - angleBeginS);
	offsetS.X *= -direction * signS;
	offsetS.Y *= direction * signS;
	
	pointBeginCoord->X += this->outerRadiusFillet * Helper::cosD(angleDirection) + offsetS.X;
	pointBeginCoord->Y += this->outerRadiusFillet * Helper::sinD(angleDirection) + offsetS.Y;
	
	double angleBeginT = 0;
	double lengthBeginT = Helper::hyp(tCoord->X, tCoord->Y);
	if (tCoord->X) {
		int sign;
		if (this->isByT()) {
			sign = 1;
		}
		else {
			sign = signT;
		}
		angleBeginT = Helper::atanD(-tCoord->Y / (sign * tCoord->X));
	}
	point2D offsetT;
	offsetT.X = lengthBeginT * Helper::sinD(angleDirection + angleBeginT);
	offsetT.Y = lengthBeginT * Helper::cosD(angleDirection + angleBeginT);
	offsetT.X *= direction * signT;
	offsetT.Y *= -direction * signT;

	pointEndCoord->X += this->outerRadiusFillet * Helper::cosD(angleDirection) + offsetT.X;
	pointEndCoord->Y += this->outerRadiusFillet * Helper::sinD(angleDirection) + offsetT.Y;

	double delta_z = 0;
	if (angleLambdaD) {
		vector2D p(*pointBeginCoord, *pointEndCoord);
		double sideA = p.getLength() * Helper::tanD(angleLambdaD);
		double angleBetta = 90 - angleLambdaD;
		double angleGamma = 180 - this->angleAlpha - angleBetta;
		double sideB = sideA * Helper::sinD(angleBetta) / Helper::sinD(angleGamma);
		delta_z = sideB * Helper::cosD(this->angleAlpha);
	}

	if (this->isByT()) {
		pointEndCoord->Z += delta_z;
	}
	else if (this->isByStoT() && endStoT) {
		pointEndCoord->Z -= delta_z ; // delta_z / 2;
	}
	else {
		pointEndCoord->Z -= delta_z;
	}

	vector3D line(*pointBeginCoord, *pointEndCoord);
	double endK = 0;
	if (this->isByS() && begin) {
		endK = 0.1 * this->parameterT;
	}
	else if (this->isByS() && !begin) {
		endK = 0.5 * (1 - percent) * this->parameterS;
	} 
	else if (this->isByStoT() && begin && !endStoT) {
		endK = 0.2 * this->edgesFilletRadius;
	}
	else if (this->isByStoT() && !begin && !endStoT) {
		endK = 0.2 * this->edgesFilletRadius;
	}
	else if (this->isByStoT() && endStoT) {
		endK = 0.15 * this->edgesFilletRadius;
	}

	if (this->isByStoT()) {
		line.incEndD(endK);
		pointEndCoord->X = line.X2;
		pointEndCoord->Y = line.Y2;
		pointEndCoord->Z = line.Z2;
	}
	if (!this->isByS()) {
		double min_delta = 0.001;
		pointBeginCoord->Z -= min_delta;
		pointEndCoord->Z -= min_delta;
	}
}

double SMPConditions::getEta(double parameterValue, double delta, double angleHalf, 
	point2D* sCoord, point2D* tCoord, point2D maxCoord) {
	if (this->isByS()) {
		double S = parameterValue;
		double T = -(this->lambdaK + S * this->lambdaKS) / this->lambdaKT;
		sCoord->X = -S / 2;
		tCoord->Y = -T;
	} 
	else if (this->isByT()) {
		double T = parameterValue;
		double S = -(this->lambdaK + this->lambdaKT * T) / this->lambdaKS;
		sCoord->X = -S / 2;
		tCoord->Y = -T;
	} 
	else if (this->isByStoT()) {
		double StoT = parameterValue;
		double T = -this->lambdaK / (StoT * this->lambdaKS + this->lambdaKT);
		double S = StoT * T;
		sCoord->X = -(S / 2);
		tCoord->Y = -T;
	}

	if (sCoord->X > maxCoord.X) {
		double cat2 = Helper::cat(this->edgesFilletRadius, sCoord->X);
		sCoord->Y = cat2 - this->edgesFilletRadius;
	}
	else {
		sCoord->Y = sCoord->X * Helper::cotD(angleHalf) + delta;
	}
	if (tCoord->Y > maxCoord.Y) {
		double cat1 = this->edgesFilletRadius + tCoord->Y;
		tCoord->X = Helper::cat(this->edgesFilletRadius, cat1);
	}
	else {
		tCoord->X = (delta - tCoord->Y) * Helper::tanD(angleHalf);
	}
	double tangent = (tCoord->Y - sCoord->Y) / (tCoord->X - sCoord->X);
	double eta = 90 + Helper::atanD(tangent);
	return eta;
}
