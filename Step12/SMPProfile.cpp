#include "StdAfx.h"
#include "SMPProfile.h"


extern int LibMessageErrorCreateModel(LPWSTR reason);

void SMPProfile::createModel() {
	this->addEmbodiment();
	this->drawBase();
	this->addExtrusion();
	this->addEdgesFillet();
	this->addCutEvolution();
	this->removeHeightDelta();
	this->addHole();
	this->baseLocalCS->Current = false;
}

double SMPProfile::getCutLength() {
	return (this->cutLength + this->cutLength_delta);
}

bool SMPProfile::isByChamfer() {
	return (this->cutRoundingType == CutRoundingType::byChamfer);
}

bool SMPProfile::isByFillet() {
	return (this->cutRoundingType == CutRoundingType::byFillet);
}

bool SMPProfile::updateParameters() {
	this->points.clear();
	LPWSTR reason = NULL;
	if (this->angleAlpha >= 90) {
		reason = _T("неверный угол наклона пластины α выше либо равен 90 градусов.");
	} else if (this->angleAlpha < 0) {
		reason = _T("неверный угол наклона пластины α ниже 0 градусов.");
	}
	if (this->cutAngleGamma >= 90) {
		reason = _T("неверный угол γ выше либо равен 90 градусов.");
	}
	else if (this->cutAngleGamma <= 0) {
		reason = _T("неверный угол γ ниже либо равен 0 градусов.");
	}
	if (this->angleAlpha >= this->cutAngleGamma) {
		reason = _T("угол α не может быть больше γ.");
	}
	if (reason != NULL) {
		LibMessageErrorCreateModel(reason);
		return false;
	}

	this->height_delta = 0;
	this->radius_delta = 0;
	if (!this->updateParametersCut()) {
		return false;
	}
	if (this->isCircle()) {
		this->updateParametersCircle();
	}
	else if (this->isNGon()) {
		this->updateParametersNGon();
	}
	else if (this->isTrigon()) {
		this->updateParametersTrigon();
	}
	else if (this->isRhombus()) {
		this->updateParametersRhombus();
	}
	if (this->hasHole && (this->holeRadius > this->innerRadiusFigure)) {
		this->holeRadius = this->innerRadiusFigure;
	}
	return true;
}

void SMPProfile::updateParametersCircle() {
	if (!this->isCircle()) {
		return;
	}
	this->innerRadiusFigure = this->size + this->radius_delta;
	this->outerRadiusFigure = this->innerRadiusFigure;
	this->sideFigure = this->innerRadiusFigure;
	this->outerRadiusFillet = this->outerRadiusFigure;
}

void SMPProfile::updateParametersNGon() {
	if (!this->isNGon()) {
		return;
	}
	unsigned short count = this->getNGonVertexCount();
	double angleVertex = 360.0 / count;
	double angleRad = (180.0 - angleVertex) / 2;
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure = this->size;
	}
	else if (this->isByOuterRadius()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleRad);
	}
	else if (this->isBySide()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleRad) * Helper::sinD(angleRad) / Helper::sinD(angleVertex);
	}
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure += this->radius_delta;
	}

	this->outerRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleRad);
	this->outerRadiusFillet = this->outerRadiusFigure;
	if (this->isByOuterRadius()) {
		this->outerRadiusFigure += this->radius_delta;
		this->outerRadiusFillet += this->radius_delta;
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

void SMPProfile::updateParametersTrigon() {
	if (!this->isTrigon()) {
		return;
	}
	double angleVertex = 60;
	double angleGreater = 80;
	double angleLess = 40;
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure = this->size;
	}
	else if (this->isByOuterRadius()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess);
	}
	else if (this->isBySide()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess) * Helper::sinD(angleGreater) / Helper::sinD(angleVertex);
	}
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure += this->radius_delta;
	}

	this->outerRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleLess);
	this->outerRadiusFillet = this->outerRadiusFigure;
	this->outerLessRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleGreater);
	this->outerLessRadiusFillet = this->outerLessRadiusFigure;
	if (this->isByOuterRadius()) {
		this->outerRadiusFigure += this->radius_delta;
		this->outerRadiusFillet += this->radius_delta;
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

void SMPProfile::updateParametersRhombus() {
	if (!this->isRhombus()) {
		return;
	}
	double angle = this->getRhombusAngle();
	double angleLess = angle / 2;
	double angleGreater = (180.0 - angle) / 2;
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure = this->size;
	}
	else if (this->isByOuterRadius()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess);
	}
	else if (this->isBySide()) {
		this->innerRadiusFigure = this->size * Helper::sinD(angleLess) * Helper::cosD(angleLess);
	}
	if (this->isByInnerRadius()) {
		this->innerRadiusFigure += this->radius_delta;
	}

	this->outerRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleLess);
	this->outerRadiusFillet = this->outerRadiusFigure;
	this->outerLessRadiusFigure = this->innerRadiusFigure / Helper::sinD(angleGreater);
	this->outerLessRadiusFillet = this->outerLessRadiusFigure;
	if (this->isByOuterRadius()) {
		this->outerRadiusFigure += this->radius_delta;
		this->outerRadiusFillet += this->radius_delta;
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

bool SMPProfile::updateParametersCut() {
	double cutInclineAngle = 90 - this->cutAngleGamma;
	double cutAngle = 90 - this->angleAlpha - cutInclineAngle;
	if (this->isByFillet() && (this->cutFilletRadius > 0)) {
		double cutAngleHalf = cutAngle / 2;
		double angleRad = this->angleAlpha + cutAngleHalf;
		this->cutLength_delta = this->cutFilletRadius * Helper::cotD(cutAngleHalf);
		double length = this->cutFilletRadius / Helper::sinD(cutAngleHalf);
		double length_x = length * Helper::sinD(angleRad);
		double length_y = length * Helper::cosD(angleRad);
		this->height_delta = length_y - this->cutFilletRadius;
		if (this->isByInnerRadius()) {
			this->radius_delta = length_x - this->cutFilletRadius;
		}
		else if (this->isByOuterRadius()) {
			double angle; 
			if (this->hasOuterLessRadius()) {
				angle = 90 - this->getAngleMain() / 2;
			}
			else {
				angle = this->getAngleVertex() / 2;
			}
			this->radius_delta = (length_x - this->cutFilletRadius) / Helper::cosD(angle);
		}
	}
	else if (this->isByChamfer() && (this->cutChamferLength > 0)) {
		if (this->cutChamferAngle >= 90) {
			LibMessageErrorCreateModel(_T("неверный угол фаски режущей кромки выше либо равен 90 градусов."));
			return false;
		}
		else if (this->cutChamferAngle <= 0) {
			LibMessageErrorCreateModel(_T("неверный угол фаски режущей кромки ниже либо равен 0 градусов."));
			return false;
		}

		double alphaInclineAngle = 90 - this->angleAlpha;
		double angleChamferBetta = 180 - this->cutChamferAngle - alphaInclineAngle;
		double angleChamferGamma = 180 - cutAngle - angleChamferBetta;
		this->cutChamferLength_cat1 = this->cutChamferLength * Helper::sinD(angleChamferGamma) / Helper::sinD(cutAngle);
		this->cutChamferLength_cat2 = this->cutChamferLength * Helper::sinD(angleChamferBetta) / Helper::sinD(cutAngle);
		this->cutLength_delta = this->cutChamferLength_cat2;
		this->height_delta = this->cutChamferLength_cat2 * Helper::cosD(this->cutAngleGamma);
		if (this->isByInnerRadius()) {
			this->radius_delta = this->cutChamferLength_cat1 * Helper::sinD(this->angleAlpha);
		}
		else if (this->isByOuterRadius()) {
			double angle;
			if (this->hasOuterLessRadius()) {
				angle = 90 - this->getAngleMain() / 2;
			}
			else {
				angle = this->getAngleVertex() / 2;
			}
			this->radius_delta = (this->cutChamferLength_cat1 * Helper::sinD(this->angleAlpha)) / Helper::cosD(angle);
		}
	}
	if (this->height_delta > 0) {
		double min_height = 0.1;
		if (this->height_delta > min_height) {
			this->height_delta -= min_height;
		}
		else {
			this->height_delta *= 0.9;
		}
	}

	point2D point1Coord, point2Coord, point3Coord, pointCenterCoord;
	point1Coord.X = 0;
	point1Coord.Y = 0;
	point2Coord.X = point1Coord.X - this->getCutLength() * Helper::cosD(this->cutAngleGamma);
	point2Coord.Y = point1Coord.Y + this->getCutLength() * Helper::sinD(this->cutAngleGamma);
	pointCenterCoord.X = point2Coord.X + this->cutRadius * Helper::sinD(this->cutAngleGamma);
	pointCenterCoord.Y = point2Coord.Y + this->cutRadius * Helper::cosD(this->cutAngleGamma);
	if (pointCenterCoord.Y + this->cutRadius < point1Coord.Y) {
		LibMessageErrorCreateModel(_T("маленький радиус режущей кромки."));
		return false;
	}
	double length1Center = pointCenterCoord.X - point1Coord.X;
	double angleSide = 90 + Helper::asinD(length1Center / this->cutRadius);
	point3Coord.X = point1Coord.X;
	point3Coord.Y = pointCenterCoord.Y + this->cutRadius * Helper::sinD(angleSide);
	if (point3Coord.Y <= 0) {
		LibMessageErrorCreateModel(_T("большая режущая кромка."));
		return false;
	}
	double min_radius = point3Coord.Y + 0.01;
	if (!this->isCircle() && (this->edgesFilletRadius <= min_radius)) {
		LibMessageErrorCreateModel(_T("маленький радиус скругления боковых рёбер."));
		return false;
	}
	if (this->getHeight() <= abs(pointCenterCoord.X - this->cutRadius)) {
		LibMessageErrorCreateModel(_T("маленькая высота."));
		return false;
	}
	return true;
}

void SMPProfile::addCutEvolution() {
	double z = this->getTop();
	IFacePtr faceBase = NULL;
	_variant_t faces = this->feature->GetModelObjects(Obj3dType::o3d_face);
	LPDISPATCH* pFaces = NULL;
	parametersSafeArray f;
	Helper::getSafeArrayData(&faces, &pFaces, &f);
	for (long idf = 0; (idf < f.count) && !faceBase; idf++) {
		IFacePtr face = pFaces[idf];
		if (face->GetBaseSurface3DType() == ksMathSurface3DTypeEnum::ksPlane) {
			point3D sMinCoord, sMaxCoord;
			face->GetMathSurface()->GetGabarit(
				&sMinCoord.X, &sMinCoord.Y, &sMinCoord.Z,
				&sMaxCoord.X, &sMaxCoord.Y, &sMaxCoord.Z);
			if (Helper::isRoundEqual(sMinCoord.Z, z) &&
				Helper::isRoundEqual(sMaxCoord.Z, z)) {
				faceBase = face;
			}
		}
	}

	point3D CSCoord;
	CSCoord.X = this->coordinates.X;
	CSCoord.Y = this->coordinates.Y + this->outerRadiusFigure;
	CSCoord.Z = this->getTop();
	double nutationAngle = 0;
	double precessionAngle = 0;
	double rotationAngle = this->findEvolutionCoordinateSystemRotationAngle(faceBase, &CSCoord);

	IAuxiliaryGeomContainerPtr geomContainer(this->part7);
	ILocalCoordinateSystemPtr localCS = geomContainer->GetLocalCoordinateSystems()->Add();
	localCS->Name = _T("ЛСК профиля режущей кромки");
	localCS->X = CSCoord.X;
	localCS->Y = CSCoord.Y;
	localCS->Z = CSCoord.Z;
	localCS->OrientationType = ksOrientationTypeEnum::ksEulerCorners;
	ILocalCSEulerParamPtr localCSParameters = localCS->GetLocalCSParameters();
	localCSParameters->NutationAngle = nutationAngle;
	localCSParameters->PrecessionAngle = precessionAngle;
	localCSParameters->RotationAngle = rotationAngle;
	localCS->Update();
	
	ISketchPtr cutSketch = this->modelContainer->GetSketchs()->Add();
	cutSketch->Name = _T("Профиль режущей кромки");
	cutSketch->CoordinateSystem = localCS;
	cutSketch->Plane = localCS->GetDefaultObject(Obj3dType::o3d_planeYOZ);
	this->createEvolutionSketch(cutSketch);
	cutSketch->Update();

	PArray<IModelObjectPtr>* edgeArray = NULL;
	if (!this->isCircle()) {
		edgeArray = this->findEvolutionEdges(faceBase);
	}
	else {
		edgeArray = this->findCircleTopEdge();
	}
	if (edgeArray->Count()) {
		IEvolutionPtr cutEvolution = this->modelContainer->GetEvolutions()->Add(Obj3dType::o3d_cutEvolution);
		cutEvolution->Name = _T("Режущая кромка");
		cutEvolution->Sketch = cutSketch;
		cutEvolution->Edges = CreateDispSafeArray(*edgeArray);
		cutEvolution->Update();
	}
	
	PArray<IModelObjectPtr>* roundingObjects = NULL;
	if (!this->isCircle()) {
		roundingObjects = this->findEvolutionRoundingEdges();
	}
	else {
		roundingObjects = this->findCircleTopEdge();
	}
	if (roundingObjects->Count()) {
		if (this->isByFillet()) {
			IFilletPtr fillet = this->modelContainer->GetFillets()->Add();
			fillet->Name = _T("Скругление режущей кромки");
			fillet->BuildingType = ksFilletBuildingTypeEnum::ksFilletCircleArc;
			fillet->Radius1 = this->cutFilletRadius;
			fillet->BaseObjects = CreateDispSafeArray(*roundingObjects);
			fillet->Update();
		}
		else if (this->isByChamfer()) {
			IChamferPtr chamfer = this->modelContainer->GetChamfers()->Add();
			chamfer->Name = _T("Фаска режущей кромки");
			chamfer->BuildingType = ksChamferBuildingTypeEnum::ksChamferTwoSides;
			chamfer->Direction = true;
			chamfer->Distance1 = this->cutChamferLength_cat1;
			chamfer->Distance2 = this->cutChamferLength_cat2;
			chamfer->BaseObjects = CreateDispSafeArray(*roundingObjects);
			chamfer->Update();
		}
	}
}

PArray<IModelObjectPtr>* SMPProfile::findEvolutionEdges(IFacePtr faceBase) {
	PArray<IModelObjectPtr>* edgeArray = new PArray<IModelObjectPtr>;
	if (this->isCircle()) {
		return edgeArray;
	}
	double z = this->getTop();
	_variant_t edges = faceBase->GetLimitingEdges();	
	LPDISPATCH* pEdges = NULL;
	parametersSafeArray e;
	Helper::getSafeArrayData(&edges, &pEdges, &e);
	if (this->isTriangle() || this->isSquare() || this->isPentagon() || this->isTrigon() || this->isRhombus()) {
		for (long ide = e.count - 1; ide >= 0 ; ide--) {
			IEdgePtr edge = pEdges[ide];
			point3D cMinCoord, cMaxCoord;
			edge->GetMathCurve()->GetGabarit(
				&cMinCoord.X, &cMinCoord.Y, &cMinCoord.Z,
				&cMaxCoord.X, &cMaxCoord.Y, &cMaxCoord.Z);
			if (Helper::isRoundEqual(cMinCoord.Z, z) &&
				Helper::isRoundEqual(cMaxCoord.Z, z)) {
				edgeArray->Add(new IModelObjectPtr(edge));
			}
		}
	}
	else {
		for (long ide = 0; ide < e.count; ide++) {
			IEdgePtr edge = pEdges[ide];
			point3D cMinCoord, cMaxCoord;
			edge->GetMathCurve()->GetGabarit(
				&cMinCoord.X, &cMinCoord.Y, &cMinCoord.Z,
				&cMaxCoord.X, &cMaxCoord.Y, &cMaxCoord.Z);
			if (Helper::isRoundEqual(cMinCoord.Z, z) &&
				Helper::isRoundEqual(cMaxCoord.Z, z)) {
				edgeArray->Add(new IModelObjectPtr(edge));
			}
		}
	}
	return edgeArray;
}

double SMPProfile::findEvolutionCoordinateSystemRotationAngle(IFacePtr faceBase, point3D* CSCoord) {
	double rotationAngle = 180;
	if (this->isCircle()) {
		return rotationAngle;
	}

	point2D pointFirstCoord, pointLastCoord;
	this->getPointFirst(&pointFirstCoord.X, &pointFirstCoord.Y);
	this->getPointLast(&pointLastCoord.X, &pointLastCoord.Y);
	_variant_t edges = faceBase->GetLimitingEdges();
	LPDISPATCH* pEdges = NULL;
	parametersSafeArray e;
	Helper::getSafeArrayData(&edges, &pEdges, &e);
	bool next = true;
	bool revert = false;
	for (long ide = 0; (ide < e.count) && next; ide++) {
		IEdgePtr edge = pEdges[ide];
		if (edge->GetCurve3DType() == ksMathCurve3DTypeEnum::ksLineSegment3D) {
			point3D cMinCoord, cMaxCoord;
			IMathCurve3DPtr curve = edge->GetMathCurve();
			double paramMin = curve->GetParamMin();
			double paramMax = curve->GetParamMax();
			curve->GetPoint(paramMin, &cMinCoord.X, &cMinCoord.Y, &cMinCoord.Z);
			curve->GetPoint(paramMax, &cMaxCoord.X, &cMaxCoord.Y, &cMaxCoord.Z);

			if (Helper::isRoundEqual(cMinCoord.X, cMaxCoord.X)) {
				if (Helper::isRoundEqual(cMinCoord.X, pointFirstCoord.X)) {
					CSCoord->X = cMinCoord.X;
					CSCoord->Y = cMinCoord.Y;
					rotationAngle = 90;
					if (CSCoord->X < this->coordinates.X) {
						revert = true;
					}
					next = false;
				}
			}
			else if (Helper::isRoundEqual(cMinCoord.Y, cMaxCoord.Y)) {
				if (Helper::isRoundEqual(cMinCoord.Y, pointFirstCoord.Y)) {
					CSCoord->X = cMinCoord.X;
					CSCoord->Y = cMinCoord.Y;
					rotationAngle = 180;
					if (CSCoord->Y < this->coordinates.Y) {
						revert = true;
					}
					next = false;
				}
			}
			else {
				vector2D c(cMinCoord, cMaxCoord);
				vector2D e(cMinCoord, pointFirstCoord);
				vector2D p(pointLastCoord, pointFirstCoord);
				double angle_c = c.getAngleAlpha();
				double angle_e = e.getAngleAlpha();
				double angle_p = p.getAngleAlpha();
				double tangent_e = e.getTangentAlpha();
				int power = -1;
				if (Helper::isRoundEqualN(angle_c, angle_e, power) &&
					Helper::isRoundEqualN(angle_c, angle_p, power)) {
					CSCoord->X = cMinCoord.X;
					CSCoord->Y = cMinCoord.Y;
					rotationAngle = angle_e;
					double x_max;
					if (tangent_e > 0) {
						x_max = pointFirstCoord.X > pointLastCoord.X ? pointFirstCoord.X : pointLastCoord.X;
					}
					else {
						x_max = pointFirstCoord.X < pointLastCoord.X ? pointFirstCoord.X : pointLastCoord.X;
					}
					double length_y0 = p.getLengthY();
					double x0 = x_max - length_y0 / tangent_e;
					double yc = tangent_e * abs(this->coordinates.X - x0);
					if (Helper::isRoundNotEqual(abs(yc), 0)) {
						if ((tangent_e > 0) && (yc > 0)) {
							revert = true;
						}
						else if ((tangent_e < 0) && (yc < 0)) {
							revert = true;
						}
					}
					next = false;
				}
			}
		}
		if (revert) {
			rotationAngle += 180;
		}
	}
	return rotationAngle;
}

void SMPProfile::createEvolutionSketch(ISketchPtr cutSketch) {
	point2D point1Coord, point2Coord, point3Coord, pointCenterCoord;
	point1Coord.X = 0;
	point1Coord.Y = 0;
	point2Coord.X = point1Coord.X + this->getCutLength() * Helper::cosD(this->cutAngleGamma);
	point2Coord.Y = point1Coord.Y - this->getCutLength() * Helper::sinD(this->cutAngleGamma);
	pointCenterCoord.X = point2Coord.X - this->cutRadius * Helper::sinD(this->cutAngleGamma);
	pointCenterCoord.Y = point2Coord.Y - this->cutRadius * Helper::cosD(this->cutAngleGamma);
	double sinValue = (pointCenterCoord.X - point1Coord.X) / this->cutRadius;
	double angleArc1 = -90 - Helper::asinD(sinValue);
	double angleArc2 = 90 - this->cutAngleGamma;
	point3Coord.X = point1Coord.X;
	point3Coord.Y = pointCenterCoord.Y + this->cutRadius * Helper::sinD(angleArc1);

	IFragmentDocumentPtr sketchDocument = cutSketch->BeginEdit();
	IDrawingContainerPtr drawingContainer = this->getDrawingContainer(sketchDocument);
	ILineSegmentPtr lineHorizontal = drawingContainer->GetLineSegments()->Add();
	lineHorizontal->X1 = point1Coord.X;
	lineHorizontal->Y1 = point1Coord.Y;
	lineHorizontal->X2 = point2Coord.X;
	lineHorizontal->Y2 = point2Coord.Y;
	lineHorizontal->Style = ksCurveStyleEnum::ksCSNormal;
	lineHorizontal->Update();
	ILineSegmentPtr lineAngle = drawingContainer->GetLineSegments()->Add();
	lineAngle->X1 = point1Coord.X;
	lineAngle->Y1 = point1Coord.Y;
	lineAngle->X2 = point3Coord.X;
	lineAngle->Y2 = point3Coord.Y;
	lineAngle->Style = ksCurveStyleEnum::ksCSNormal;
	lineAngle->Update();
	IArcPtr arc = drawingContainer->GetArcs()->Add();
	arc->Xc = pointCenterCoord.X;
	arc->Yc = pointCenterCoord.Y;
	arc->Radius = this->cutRadius;
	arc->Angle1 = angleArc1;
	arc->Angle2 = angleArc2;
	arc->Direction = false;
	arc->Style = ksCurveStyleEnum::ksCSNormal;
	arc->Update();
	cutSketch->EndEdit();
}

PArray<IModelObjectPtr>* SMPProfile::findEvolutionRoundingEdges() {
	PArray<IModelObjectPtr>* edgeArray = new PArray<IModelObjectPtr>;
	if (this->isCircle()) {
		return edgeArray;
	}
	double z1 = this->getBottom();
	double z2 = this->getTop();
	_variant_t faces = this->feature->GetModelObjects(Obj3dType::o3d_face);
	LPDISPATCH* pFaces = NULL;
	parametersSafeArray f;
	Helper::getSafeArrayData(&faces, &pFaces, &f);
	for (long idf = 0; idf < f.count; idf++) {
		IFacePtr face = pFaces[idf];
		ksMathSurface3DTypeEnum type = face->GetBaseSurface3DType();
		if ((type == ksMathSurface3DTypeEnum::ksPlane) ||
			(type == ksMathSurface3DTypeEnum::ksExtrusionSurface)) {
			point3D sMinCoord, sMaxCoord;
			face->GetMathSurface()->GetGabarit(
				&sMinCoord.X, &sMinCoord.Y, &sMinCoord.Z,
				&sMaxCoord.X, &sMaxCoord.Y, &sMaxCoord.Z);
			if (Helper::isRoundEqual(sMinCoord.Z, z1) &&
				Helper::isRoundEqual(sMaxCoord.Z, z2)) {
				_variant_t edges = face->GetLimitingEdges();
				LPDISPATCH* pEdges = NULL;
				parametersSafeArray e;
				Helper::getSafeArrayData(&edges, &pEdges, &e);
				for (long ide = 0; ide < e.count; ide++) {
					IEdgePtr edge = pEdges[ide];
					point3D cMinCoord, cMaxCoord;
					edge->GetMathCurve()->GetGabarit(
						&cMinCoord.X, &cMinCoord.Y, &cMinCoord.Z,
						&cMaxCoord.X, &cMaxCoord.Y, &cMaxCoord.Z);
					if (Helper::isRoundEqual(cMinCoord.Z, z2) &&
						Helper::isRoundEqual(cMaxCoord.Z, z2)) {
						edgeArray->Add(new IModelObjectPtr(edge));
					}
				}
			}
		}
	}
	return edgeArray;
}

PArray<IModelObjectPtr>* SMPProfile::findCircleTopEdge() {
	PArray<IModelObjectPtr>* edgeArray = new PArray<IModelObjectPtr>;
	if (!this->isCircle()) {
		return edgeArray;
	}
	point3D circleMinCoord, circleMaxCoord;
	circleMinCoord.X = this->coordinates.X - this->outerRadiusFigure;
	circleMaxCoord.X = this->coordinates.X + this->outerRadiusFigure;
	circleMinCoord.Y = this->coordinates.Y - this->outerRadiusFigure;
	circleMaxCoord.Y = this->coordinates.Y + this->outerRadiusFigure;
	circleMinCoord.Z = circleMaxCoord.Z = this->getTop();
	_variant_t edges = this->feature->GetModelObjects(Obj3dType::o3d_edge);
	LPDISPATCH* pEdges = NULL;
	parametersSafeArray e;
	Helper::getSafeArrayData(&edges, &pEdges, &e);
	for (long ide = 0; ide < e.count; ide++) {
		IEdgePtr edge = pEdges[ide];
		point3D cMinCoord, cMaxCoord;
		edge->GetMathCurve()->GetGabarit(
			&cMinCoord.X, &cMinCoord.Y, &cMinCoord.Z,
			&cMaxCoord.X, &cMaxCoord.Y, &cMaxCoord.Z);
		if (Helper::isRoundEqualCoordinates3D(cMinCoord, circleMinCoord) && 
			Helper::isRoundEqualCoordinates3D(cMaxCoord, circleMaxCoord)) {
			edgeArray->Add(new IModelObjectPtr(edge));
		}
	}
	return edgeArray;
}

void SMPProfile::removeHeightDelta() {
	ICutExtrusionPtr cutExtrusion = this->modelContainer->GetExtrusions()->Add(Obj3dType::o3d_cutExtrusion);
	cutExtrusion->Name = _T("Срез верхней части");
	cutExtrusion->Direction = ksDirectionTypeEnum::dtNormal;
	cutExtrusion->Sketch = this->baseSketch;
	cutExtrusion->SetSideParameters(
		true, ksEndTypeEnum::etBlind,
		this->height_delta, 0, false, NULL
	);
	cutExtrusion->Update();
}
