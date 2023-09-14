#include "StdAfx.h"
#include "SMPBase.h"


void SMPBase::init(IApplicationPtr application, HMODULE hModule) {
	this->application = application;
	this->hModule = hModule;
	this->initDocumentParameters();
}

void SMPBase::initDocumentParameters() {
	this->activeDocument = this->application->ActiveDocument;
	this->kompasDocument3D = IKompasDocument3DPtr(this->activeDocument);
	this->document3D = ::ksGetActive3dDocument();
	IPartPtr p = NULL;
	if (this->document3D) {
		p = this->document3D->GetPart(Part_Type::pTop_Part);
	}
	this->part = IPartPtr(p);
	this->part7 = this->kompasDocument3D->GetTopPart();
	this->modelContainer = IModelContainerPtr(this->part7);
	this->feature = IFeature7Ptr(this->part7);
}

double SMPBase::getOuterRadius() {
	return this->outerRadiusFigure;
}

double SMPBase::getInnerRadius() {
	return this->innerRadiusFigure;
}

double SMPBase::getSide() {
	return this->sideFigure;
}

double SMPBase::getHeight() {
	return (this->height + this->height_delta);
}

double SMPBase::getBottom() {
	return this->coordinates.Z;
}

double SMPBase::getTop() {
	return (this->coordinates.Z + this->getHeight());
}

double SMPBase::getTopFinal() {
	return (this->coordinates.Z + this->height);
}

unsigned short SMPBase::getNGonVertexCount() {
	unsigned short count = 0;
	if (this->isTriangle()) {
		count = 3;
	}
	else if (this->isSquare()) {
		count = 4;
	}
	else if (this->isPentagon()) {
		count = 5;
	}
	else if (this->isHexagon()) {
		count = 6;
	}
	return count;
}

unsigned short SMPBase::getCutVertexCount() {
	unsigned short count = 0;
	if (this->isNGon()) {
		count = this->getNGonVertexCount();
	}
	else if (this->isTrigon()) {
		count = 3;
	}
	else if (this->isRhombus()) {
		count = 2;
	}
	return count;
}

double SMPBase::getRhombusAngle() {
	double angle = 0;
	if (this->isRhombus35()) {
		angle = 35;
	}
	else if (this->isRhombus55()) {
		angle = 55;
	}
	else if (this->isRhombus80()) {
		angle = 80;
	}
	return angle;
}

double SMPBase::getAngleMain() {
	double angle = 0;
	if (this->isNGon()) {
		unsigned short count = this->getNGonVertexCount();
		angle = 180.0 - (360.0 / count);
	}
	else if (this->isRhombus()) {
		angle = this->getRhombusAngle();
	}
	else if (this->isTrigon()) {
		angle = 80;
	}
	return angle;
}

double SMPBase::getAngleAdditional() {
	double angle = 0;
	if (this->isRhombus()) {
		angle = 180 - this->getRhombusAngle();
	}
	else if (this->isTrigon()) {
		angle = 160;
	}
	return angle;
}

double SMPBase::getAngleVertex() {
	double angle = 0;
	if (this->isNGon()) {
		unsigned short count = this->getNGonVertexCount();
		angle = 360.0 / count;
	}
	else if (this->isTrigon()) {
		angle = 120;
	}
	else if (this->isRhombus()) {
		angle = 180;
	}
	return angle;
}

double SMPBase::getAngleMainDirection() {
	double angle = 0;
	if (this->isNGon() || this->isRhombus()) {
		angle = this->getAngleVertex() / 2;
	}
	else if (this->isTrigon()) {
		angle = 90;
	}
	return angle;
}

double SMPBase::getAngleAdditionalDirection() {
	double angle = 0;
	if (this->isRhombus()) {
		angle = 0;
	}
	else if (this->isTrigon()) {
		angle = 30;
	}
	return angle;
}

void SMPBase::getPoint(size_t index, double* x, double* y) {
	size_t size = this->points.size();
	if ((index >= 0) && (index < size)) {
		*x = this->points[index].X;
		*y = this->points[index].Y;
	}
}

void SMPBase::getPointFirst(double* x, double* y) {
	this->getPoint(0, x, y);
}

void SMPBase::getPointLast(double* x, double* y) {
	size_t last = this->points.size() - 1;
	this->getPoint(last, x, y);
}

void SMPBase::addPoint(double x, double y) {
	this->points.push_back({ x, y });
}

bool SMPBase::isByInnerRadius() {
	return (this->sizeType == SizeType::byInnerRadius);
}

bool SMPBase::isByOuterRadius() {
	return (this->sizeType == SizeType::byOuterRadius);
}

bool SMPBase::isBySide() {
	return (this->sizeType == SizeType::bySide);
}

bool SMPBase::isTriangle() {
	return (this->surfaceFigure == SurfaceFigure::triangle);
}

bool SMPBase::isSquare() {
	return (this->surfaceFigure == SurfaceFigure::square);
}

bool SMPBase::isPentagon() {
	return (this->surfaceFigure == SurfaceFigure::pentagon);
}

bool SMPBase::isHexagon() {
	return (this->surfaceFigure == SurfaceFigure::hexagon);
}

bool SMPBase::isRhombus35() {
	return (this->surfaceFigure == SurfaceFigure::rhombus35);
}

bool SMPBase::isRhombus55() {
	return (this->surfaceFigure == SurfaceFigure::rhombus55);
}

bool SMPBase::isRhombus80() {
	return (this->surfaceFigure == SurfaceFigure::rhombus80);
}

bool SMPBase::isTrigon() {
	return (this->surfaceFigure == SurfaceFigure::trigon);
}

bool SMPBase::isCircle() {
	return (this->surfaceFigure == SurfaceFigure::circle);
}

bool SMPBase::isNGon() {
	return (this->isTriangle() ||
		this->isSquare() ||
		this->isPentagon() ||
		this->isHexagon());
}

bool SMPBase::isRhombus() {
	return (this->isRhombus35() ||
		this->isRhombus55() ||
		this->isRhombus80());
}

bool SMPBase::hasOuterLessRadius() {
	return (this->isRhombus() || this->isTrigon());
}

bool SMPBase::needFillet() {
	return (!this->isCircle() && this->edgesFilletRadius);
}

bool SMPBase::needHole() {
	return (this->hasHole && (this->holeRadius > 0));
}

IDrawingContainerPtr SMPBase::getDrawingContainer(IFragmentDocumentPtr sketchDocument) {
	IKompasDocument2DPtr doc2d(sketchDocument);
	IViewPtr view = doc2d->GetViewsAndLayersManager()->GetViews()->GetActiveView();
	IDrawingContainerPtr drawingContainer(view);
	return drawingContainer;
}

void SMPBase::addEmbodiment() {
	IEmbodimentsManagerPtr eManager(this->part7);
	eManager->TopEmbodiment->IsCurrent = true;
	long count = eManager->GetEmbodimentCount();
	long topIndex = eManager->GetCurrentEmbodimentIndex();
	
	wchar_t temp_str[11];
	_ltow_s(count, temp_str, 10);
	BSTR embodimentNumber = SysAllocString(temp_str);
	BSTR baseMarking = _T("СМП");
	BSTR additionalNumber = _T("1");
	eManager->AddEmbodiment(
		topIndex, false, baseMarking,
		embodimentNumber, additionalNumber
	);
	this->initDocumentParameters();

	IKompasDocument1Ptr kompasDocument1(this->activeDocument);
	PArray<IModelObjectPtr> edgesArray;
	_variant_t edges = this->feature->GetModelObjects(Obj3dType::o3d_edge);
	LPDISPATCH* pEdges = NULL;
	parametersSafeArray e;
	Helper::getSafeArrayData(&edges, &pEdges, &e);
	for (long ide = 0; ide < e.count; ide++) {
		IEdgePtr edge = pEdges[ide];
		edgesArray.Add(new IModelObjectPtr(edge));
	}
	if (edgesArray.Count()) {
		_variant_t deleteObjects = CreateDispSafeArray(edgesArray);
		kompasDocument1->Delete(&deleteObjects);
	}
}

void SMPBase::drawBase() {
	IAuxiliaryGeomContainerPtr geomContainer(this->part7);
	this->baseLocalCS = geomContainer->GetLocalCoordinateSystems()->Add();
	this->baseLocalCS->Name = _T("ЛСК основания");
	this->baseLocalCS->X = this->coordinates.X;
	this->baseLocalCS->Y = this->coordinates.Y;
	this->baseLocalCS->Z = this->getTop();
	this->baseLocalCS->OrientationType = ksOrientationTypeEnum::ksEulerCorners;
	ILocalCSEulerParamPtr localCS_parameters = this->baseLocalCS->GetLocalCSParameters();
	localCS_parameters->NutationAngle = 0;
	localCS_parameters->PrecessionAngle = 0;
	localCS_parameters->RotationAngle = 0;
	this->baseLocalCS->Update();
	this->baseLocalCS->Current = true;

	this->baseSketch = this->modelContainer->GetSketchs()->Add();
	this->baseSketch->Name = _T("Профиль основания");
	this->baseSketch->Plane = this->baseLocalCS->GetDefaultObject(Obj3dType::o3d_planeXOY);
	this->baseSketch->Angle = 0;
	IFragmentDocumentPtr sketchDocument = this->baseSketch->BeginEdit();
	IDrawingContainerPtr drawingContainer = this->getDrawingContainer(sketchDocument);
	if (this->isCircle()) {
		this->drawCircle(drawingContainer);
	}
	else if (this->isNGon()) {
		this->drawNGon(drawingContainer);
	}
	else if (this->isTrigon()) {
		this->drawTrigon(drawingContainer);
	}
	else if (this->isRhombus()) {
		this->drawRhombus(drawingContainer);
	}
	this->baseSketch->EndEdit();
}

void SMPBase::drawCircle(IDrawingContainerPtr drawingContainer) {
	ICirclePtr figure = drawingContainer->GetCircles()->Add();
	figure->Style = ksCurveStyleEnum::ksCSNormal;
	figure->Xc = this->coordinates.X;
	figure->Yc = this->coordinates.Y;
	figure->Radius = this->innerRadiusFigure;
	figure->Update();

	this->addPoint(
		this->coordinates.X,
		this->coordinates.Y + this->innerRadiusFigure
	);
}

void SMPBase::drawNGon(IDrawingContainerPtr drawingContainer) {
	unsigned short count = this->getNGonVertexCount();
	double angleVertex = 360.0 / count;
	IRegularPolygonPtr figure = drawingContainer->GetRegularPolygons()->Add();
	figure->Count = count;
	figure->Style = ksCurveStyleEnum::ksCSNormal;
	figure->Xc = this->coordinates.X;
	figure->Yc = this->coordinates.Y;
	figure->Radius = this->innerRadiusFigure;
	figure->Describe = true;
	figure->Update();

	double angle_i;
	for (size_t i = 0; i < count; i++) {
		angle_i = angleVertex * (0.5 - i);
		this->addPoint(
			this->coordinates.X + this->outerRadiusFigure * Helper::cosD(angle_i),
			this->coordinates.Y + this->outerRadiusFigure * Helper::sinD(angle_i)
		);
	}
}

void SMPBase::drawTrigon(IDrawingContainerPtr drawingContainer) {
	double deltaMax = this->innerRadiusFigure / Helper::sinD(40);
	double deltaMin = this->innerRadiusFigure / Helper::sinD(80);
	double angleMax = 60;
	double angleMin = 30;
	this->addPoint(
		this->coordinates.X,
		this->coordinates.Y + deltaMax
	);
	this->addPoint(
		this->coordinates.X + deltaMin * Helper::sinD(angleMax),
		this->coordinates.Y + deltaMin * Helper::sinD(angleMin)
	);
	this->addPoint(
		this->coordinates.X + deltaMax * Helper::sinD(angleMax),
		this->coordinates.Y - deltaMax * Helper::sinD(angleMin)
	);
	this->addPoint(
		this->coordinates.X,
		this->coordinates.Y - deltaMin
	);
	this->addPoint(
		this->coordinates.X - deltaMax * Helper::sinD(angleMax),
		this->coordinates.Y - deltaMax * Helper::sinD(angleMin)
	);
	this->addPoint(
		this->coordinates.X - deltaMin * Helper::sinD(angleMax),
		this->coordinates.Y + deltaMin * Helper::sinD(angleMin)
	);

	IPolyLine2DPtr figure = drawingContainer->GetPolyLines2D()->Add();
	figure->Style = ksCurveStyleEnum::ksCSNormal;
	figure->Closed = true;
	point2D pointCoord;
	for (size_t i = 0; i < 6; i++) {
		this->getPoint(i, &pointCoord.X, &pointCoord.Y);
		figure->AddPoint(i, pointCoord.X, pointCoord.Y);
	}
	figure->Update();
}

void SMPBase::drawRhombus(IDrawingContainerPtr drawingContainer) {
	double angle = this->getRhombusAngle() / 2;
	double deltaMin = this->innerRadiusFigure / Helper::cosD(angle);
	double deltaMax = this->innerRadiusFigure / Helper::sinD(angle);

	this->addPoint(
		this->coordinates.X,
		this->coordinates.Y + deltaMax
	);
	this->addPoint(
		this->coordinates.X + deltaMin,
		this->coordinates.Y
	);
	this->addPoint(
		this->coordinates.X,
		this->coordinates.Y - deltaMax
	);
	this->addPoint(
		this->coordinates.X - deltaMin,
		this->coordinates.Y
	);

	IPolyLine2DPtr figure = drawingContainer->GetPolyLines2D()->Add();
	figure->Style = ksCurveStyleEnum::ksCSNormal;
	figure->Closed = true;
	point2D pointCoord;
	for (size_t i = 0; i < 4; i++) {
		this->getPoint(i, &pointCoord.X, &pointCoord.Y);
		figure->AddPoint(i, pointCoord.X, pointCoord.Y);
	}
	figure->Update();
}

void SMPBase::addExtrusion() {
	IExtrusionPtr extrusion = this->modelContainer->GetExtrusions()->Add(Obj3dType::o3d_bossExtrusion);
	extrusion->Name = _T("Элемент выдавливания");
	extrusion->Direction = ksDirectionTypeEnum::dtReverse;
	extrusion->Sketch = this->baseSketch;
	extrusion->SetSideParameters(
		false, ksEndTypeEnum::etBlind, this->getHeight(), 
		this->angleAlpha, true, NULL
	);
	extrusion->Update();
}

void SMPBase::addEdgesFillet() {
	if (!this->needFillet()) {
		return;
	}
	long count;
	double radiusFigure, radiusModel, angleMain, angleDirection;
	count = this->getCutVertexCount();
	radiusFigure = this->outerRadiusFigure;
	radiusModel = this->outerRadiusFillet;
	angleMain = this->getAngleMain();
	angleDirection = this->getAngleMainDirection();
	this->addEdgesFilletBySketch(count, radiusFigure, radiusModel, angleMain, angleDirection);
	if (this->hasOuterLessRadius()) {
		radiusFigure = this->outerLessRadiusFigure;
		radiusModel = this->outerLessRadiusFillet;
		angleMain = this->getAngleAdditional();
		angleDirection = this->getAngleAdditionalDirection();
		this->addEdgesFilletBySketch(count, radiusFigure, radiusModel, angleMain, angleDirection);
	}
}

void SMPBase::addEdgesFilletBySketch(long count, double radiusFigure, double radiusModel,
	double angleMain, double angleDirection) {
	if (!this->needFillet()) {
		return;
	}
	double z_min = this->getBottom();
	double z_max = this->getTop();
	point3D pointBegin;
	pointBegin.X = this->coordinates.X + radiusFigure * Helper::cosD(angleDirection);
	pointBegin.Y = this->coordinates.Y + radiusFigure * Helper::sinD(angleDirection);
	pointBegin.Z = z_max;

	IModelObjectPtr filletEdge = NULL;
	_variant_t faces = this->feature->GetModelObjects(Obj3dType::o3d_face);
	LPDISPATCH* pFaces = NULL;
	parametersSafeArray f;
	Helper::getSafeArrayData(&faces, &pFaces, &f);
	for (long idf = 0; (idf < f.count) && !filletEdge; idf++) {
		IFacePtr face = pFaces[idf];
		if (face->GetBaseSurface3DType() == ksMathSurface3DTypeEnum::ksPlane) {
			point3D sMinCoord, sMaxCoord;
			face->GetMathSurface()->GetGabarit(
				&sMinCoord.X, &sMinCoord.Y, &sMinCoord.Z,
				&sMaxCoord.X, &sMaxCoord.Y, &sMaxCoord.Z);
			if (Helper::isRoundEqual(sMinCoord.Z, z_min) &&
				Helper::isRoundEqual(sMaxCoord.Z, z_max)) {
				_variant_t edges = face->GetLimitingEdges();
				LPDISPATCH* pEdges = NULL;
				parametersSafeArray e;
				Helper::getSafeArrayData(&edges, &pEdges, &e);
				for (long ide = 0; ide < e.count; ide++) {
					IEdgePtr edge = pEdges[ide];
					point3D cMinCoord, cMaxCoord;
					IMathCurve3DPtr curve = edge->GetMathCurve();
					double paramMin = curve->GetParamMin();
					double paramMax = curve->GetParamMax();
					curve->GetPoint(paramMin, &cMinCoord.X, &cMinCoord.Y, &cMinCoord.Z);
					curve->GetPoint(paramMax, &cMaxCoord.X, &cMaxCoord.Y, &cMaxCoord.Z);
					if (Helper::isRoundNotEqual(cMinCoord.Z, cMaxCoord.Z) &&
						(Helper::isRoundEqualCoordinates3D(cMinCoord, pointBegin) ||
							Helper::isRoundEqualCoordinates3D(cMaxCoord, pointBegin))) {
						filletEdge = edge;
					}
				}
			}
		}
	}

	if (filletEdge != NULL) {
		ISketchPtr sketch = this->modelContainer->GetSketchs()->Add();
		sketch->Name = _T("Профиль скругления");
		sketch->Plane = this->baseLocalCS->GetDefaultObject(Obj3dType::o3d_planeXOY);
		this->addEdgesFilletSketch(sketch, radiusFigure, radiusModel, angleMain, angleDirection);
		sketch->Update();

		ICutExtrusionPtr cutExtrusion = this->modelContainer->GetExtrusions()->Add(Obj3dType::o3d_cutExtrusion);
		cutExtrusion->Name = _T("Скругление боковых рёбер");
		cutExtrusion->Direction = ksDirectionTypeEnum::dtNormal;
		cutExtrusion->SetSideParameters(
			true, ksEndTypeEnum::etThroughAll,
			0, 0, false, filletEdge
		);
		IExtrusion1Ptr extrusionSurface(cutExtrusion);
		extrusionSurface->Profile = sketch;
		extrusionSurface->DirectionObject = filletEdge;
		cutExtrusion->Update();

		IAxis3DPtr axis = this->baseLocalCS->GetDefaultObject(Obj3dType::o3d_axisOZ);
		ICircularPatternPtr circularPattern = this->modelContainer->GetFeaturePatterns()->Add(Obj3dType::o3d_circularCopy);
		circularPattern->Name = _T("Массив скруглений боковых рёбер");
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
}

void SMPBase::addEdgesFilletSketch(ISketchPtr sketch, double radiusFigure, double radiusModel, 
	double angleMain, double angleDirection) {
	if (!this->needFillet()) {
		return;
	}

	double angleHalf = angleMain / 2;
	double angleArc = 90 - angleHalf;
	double angleArc1 = angleDirection + angleArc;
	double angleArc2 = angleDirection - angleArc;
	double delta = radiusFigure - radiusModel;
	double radiusFillet = this->edgesFilletRadius;

	point2D maxCoord, pointAngle, pointArcCenter, pointLeft, pointRight;
	maxCoord.X = radiusFillet * Helper::cosD(angleHalf);
	maxCoord.Y = radiusFillet * (1 - Helper::sinD(angleHalf)) + delta;
	pointAngle.X = radiusFigure * Helper::cosD(angleDirection);
	pointAngle.Y = radiusFigure * Helper::sinD(angleDirection);
	pointArcCenter.X = (radiusModel - radiusFillet) * Helper::cosD(angleDirection);
	pointArcCenter.Y = (radiusModel - radiusFillet) * Helper::sinD(angleDirection);

	double angleBegin = Helper::atanD(maxCoord.Y / maxCoord.X);
	double lengthBegin = Helper::hyp(maxCoord.X, maxCoord.Y);
	point2D offsetLeft, offsetRight;
	offsetLeft.X = lengthBegin * Helper::sinD(angleDirection + angleBegin);
	offsetLeft.Y = -lengthBegin * Helper::cosD(angleDirection + angleBegin);
	offsetRight.X = -lengthBegin * Helper::sinD(angleDirection - angleBegin);
	offsetRight.Y = lengthBegin * Helper::cosD(angleDirection - angleBegin);

	pointLeft.X = pointAngle.X - offsetLeft.X;
	pointLeft.Y = pointAngle.Y - offsetLeft.Y;
	pointRight.X = pointAngle.X - offsetRight.X;
	pointRight.Y = pointAngle.Y - offsetRight.Y;

	pointAngle.X += this->coordinates.X;
	pointAngle.Y += this->coordinates.Y;
	pointArcCenter.X += this->coordinates.X;
	pointArcCenter.Y += this->coordinates.Y;
	pointLeft.X += this->coordinates.X;
	pointLeft.Y += this->coordinates.Y;
	pointRight.X += this->coordinates.X;
	pointRight.Y += this->coordinates.Y;

	IFragmentDocumentPtr sketchDocument = sketch->BeginEdit();
	IDrawingContainerPtr drawingContainer = this->getDrawingContainer(sketchDocument);
	ILineSegmentPtr lineLeft = drawingContainer->GetLineSegments()->Add();
	lineLeft->X1 = pointAngle.X;
	lineLeft->Y1 = pointAngle.Y;
	lineLeft->X2 = pointLeft.X;
	lineLeft->Y2 = pointLeft.Y;
	lineLeft->Style = ksCurveStyleEnum::ksCSNormal;
	lineLeft->Update();
	ILineSegmentPtr lineRight = drawingContainer->GetLineSegments()->Add();
	lineRight->X1 = pointAngle.X;
	lineRight->Y1 = pointAngle.Y;
	lineRight->X2 = pointRight.X;
	lineRight->Y2 = pointRight.Y;
	lineRight->Style = ksCurveStyleEnum::ksCSNormal;
	lineRight->Update();
	IArcPtr arc = drawingContainer->GetArcs()->Add();
	arc->Xc = pointArcCenter.X;
	arc->Yc = pointArcCenter.Y;
	arc->Radius = radiusFillet;
	arc->Angle1 = angleArc1;
	arc->Angle2 = angleArc2;
	arc->Direction = true;
	arc->Style = ksCurveStyleEnum::ksCSNormal;
	arc->Update();
	sketch->EndEdit();
}

void SMPBase::addEdgesFilletByChord() {
	if (!this->needFillet()) {
		return;
	}
	double z_min = this->coordinates.Z;
	double z_max = this->getTop();
	double angleMain = this->getAngleMain();
	double angleFillet = 180 - angleMain;
	double angleHalf = angleFillet / 2;
	double chord = 2 * this->edgesFilletRadius * Helper::sinD(angleHalf);
	IFilletPtr fillet = this->modelContainer->GetFillets()->Add();
	fillet->Name = _T("Скругление боковых рёбер");
	fillet->Chord = chord;
	fillet->BuildingType = ksFilletBuildingTypeEnum::ksFilletHord;
	PArray<IModelObjectPtr> filletObjects;
	
	_variant_t faces = this->feature->GetModelObjects(Obj3dType::o3d_face);
	LPDISPATCH* pFaces = NULL;
	parametersSafeArray f;
	Helper::getSafeArrayData(&faces, &pFaces, &f);
	for (long idf = 0; idf < f.count; idf++) {
		IFacePtr face = pFaces[idf];
		if (face->GetBaseSurface3DType() == ksMathSurface3DTypeEnum::ksPlane) {
			point3D sMinCoord, sMaxCoord;
			face->GetMathSurface()->GetGabarit(
				&sMinCoord.X, &sMinCoord.Y, &sMinCoord.Z, 
				&sMaxCoord.X, &sMaxCoord.Y, &sMaxCoord.Z);
			if (Helper::isRoundEqual(sMinCoord.Z, z_min) &&
				Helper::isRoundEqual(sMaxCoord.Z, z_max)) {
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
					if (Helper::isRoundNotEqual(cMinCoord.Z, cMaxCoord.Z)) {
						filletObjects.Add(new IModelObjectPtr(edge));
					}
				}
			}
		}
	}
	if (filletObjects.Count()) {
		fillet->BaseObjects = CreateDispSafeArray(filletObjects);
		fillet->Update();
	}
}

void SMPBase::addHole() {
	if (!this->needHole()) {
		return;
	}
	ISketchPtr holeSketch = this->modelContainer->GetSketchs()->Add();
	holeSketch->Name = _T("Профиль отверстия");
	holeSketch->Plane = this->baseLocalCS->GetDefaultObject(Obj3dType::o3d_planeXOY);
	IFragmentDocumentPtr sketchDocument = holeSketch->BeginEdit();
	IDrawingContainerPtr drawingContainer = this->getDrawingContainer(sketchDocument);
	ICirclePtr figure = drawingContainer->GetCircles()->Add();
	figure->Style = ksCurveStyleEnum::ksCSNormal;
	figure->Xc = this->coordinates.X;
	figure->Yc = this->coordinates.Y;
	figure->Radius = this->holeRadius;
	figure->Update();
	holeSketch->EndEdit();
	ICutExtrusionPtr cutExtrusion = this->modelContainer->GetExtrusions()->Add(Obj3dType::o3d_cutExtrusion);
	cutExtrusion->Name = _T("Отверстие");
	cutExtrusion->Direction = ksDirectionTypeEnum::dtNormal;
	cutExtrusion->Sketch = holeSketch;
	cutExtrusion->SetSideParameters(
		true, ksEndTypeEnum::etThroughAll,
		this->getHeight(), 0, true, NULL
	);
	cutExtrusion->Update();

	point3D holeMinCoord, holeMaxCoord;
	holeMinCoord.X = this->coordinates.X - this->holeRadius;
	holeMinCoord.Y = this->coordinates.Y - this->holeRadius;
	holeMinCoord.Z = this->getBottom();
	holeMaxCoord.X = this->coordinates.X + this->holeRadius;
	holeMaxCoord.Y = this->coordinates.Y + this->holeRadius;
	holeMaxCoord.Z = this->getTopFinal();

	double chamferLength = this->holeChamferLength * Helper::sinD(45);
	double max_length = this->height / 2;
	if (chamferLength > max_length) {
		chamferLength = max_length;
	}
	IFacePtr faceCylinder = NULL;
	_variant_t faces = this->feature->GetModelObjects(Obj3dType::o3d_face);
	LPDISPATCH* pFaces = NULL;
	parametersSafeArray f;
	Helper::getSafeArrayData(&faces, &pFaces, &f);
	for (long idf = 0; (idf < f.count) && !faceCylinder; idf++) {
		IFacePtr face = pFaces[idf];
		if (face->GetBaseSurface3DType() == ksMathSurface3DTypeEnum::ksCylinderSurface) {
			point3D sMinCoord, sMaxCoord;
			face->GetMathSurface()->GetGabarit(
				&sMinCoord.X, &sMinCoord.Y, &sMinCoord.Z,
				&sMaxCoord.X, &sMaxCoord.Y, &sMaxCoord.Z);
			if (Helper::isRoundEqualCoordinates3D(sMinCoord, holeMinCoord) &&
				Helper::isRoundEqualCoordinates3D(sMaxCoord, holeMaxCoord)) {
				faceCylinder = face;
			}
		}
	}
	if (faceCylinder != NULL) {
		IChamferPtr chamfer = this->modelContainer->GetChamfers()->Add();
		chamfer->Name = _T("Фаска отверстия");
		chamfer->BuildingType = ksChamferBuildingTypeEnum::ksChamferTwoSides;
		chamfer->Direction = true;
		chamfer->Distance1 = chamferLength;
		chamfer->Distance2 = chamferLength;
		chamfer->BaseObjects = (IFace*)faceCylinder;
		chamfer->Update();
	}
}
