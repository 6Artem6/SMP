////////////////////////////////////////////////////////////////////////////////
//
// События документа
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _DOCUMENTEVENT_H
#include "DocumentEvent.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern AFX_MODULE_STATE* pModuleState;


extern void UpdateSlideBox();
extern void UpdateSlideBox(reference docRef);
extern void AdviseDoc(reference docRef);


//-------------------------------------------------------------------------------
// Создать обработчик событий документа
// ---
cBaseEvent* NewDocumentEvent(reference doc) {
	DocumentEvent* res = NULL;
	if (doc) {
		res = (DocumentEvent*)cBaseEvent::FindEvents(ntDocumentFileNotify, doc);
		if (!res) {
			res = new DocumentEvent(doc);
			if (!res->Advise()) {
				delete res;
				res = NULL;
			}
		}
	}
	return res;
}


//-------------------------------------------------------------------------------
// Создать обработчик событий 3D документа
// ---
cBaseEvent* NewDocument3DEvent(reference doc) {
	Document3DEvent* res = NULL;
	if (doc) {
		res = (Document3DEvent*)cBaseEvent::FindEvents(ntDocument3DNotify, doc);
		if (!res) {
			IDocument3DPtr document3D(ksGet3dDocumentFromReference(doc), false);
			if (document3D) {
				NewDocumentEvent(doc); // чтобы при закрытии документа отписаться
				res = new Document3DEvent(doc, document3D);
				if (!res->Advise()) {
					delete res;
					res = NULL;
				}
			}
		}
	}
	return res;
}


//-------------------------------------------------------------------------------
// Создать обработчик событий 2D документа
// ---
cBaseEvent* NewDocument2DEvent(reference doc) {
	Document2DEvent* res = NULL;
	if (doc) {
		if (!cBaseEvent::FindEvents(ntDocument2DNotify, doc)) {
			NewDocumentEvent(doc); // чтобы при закрытии документа отписаться
			res = new Document2DEvent(doc);
			if (!res->Advise()) {
				delete res;
				res = NULL;
			}
		}
	}
	return res;
}


////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
DocumentEvent::DocumentEvent(reference p)
	: cBaseEvent(ntDocumentFileNotify, p, 0, p)
{
}


//-------------------------------------------------------------------------------
//
// ---
DocumentEvent::~DocumentEvent()
{
}


//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN DocumentEvent::GetUnknown() {
	m_xDocumentFileNotify.AddRef();
	return &m_xDocumentFileNotify;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) DocumentEvent::XDocumentFileNotify::Release()
{
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return (ULONG)pThis->InternalRelease();
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) DocumentEvent::XDocumentFileNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return (ULONG)pThis->InternalAddRef();
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP DocumentEvent::XDocumentFileNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IDocumentFileNotify))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}


//-----------------------------------------------------------------------------
// Обрабатывается ли библиотекой событие с номером notifyType
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
	return  notifyType >= kdBeginCloseDocument && notifyType <= kdDocumentFrameOpen;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::BeginProcess(long Id) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::EndProcess(long Id, VARIANT_BOOL Success) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}


/////////////////////////////////////////////////////////////////////////////
// DocumentEvent message handlers
//-----------------------------------------------------------------------------
//kdBeginCloseDocument - Начало закрытия документа.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::BeginCloseDocument() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}


//-----------------------------------------------------------------------------
//kdCloseDocument - Документ закрыт.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::CloseDocument() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		cBaseEvent::TerminateEvents(0, pThis->refDoc);
	UpdateSlideBox(0);
	return true;
}


//-----------------------------------------------------------------------------
//kdBeginSaveDocument - Начало сохранения документа.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::BeginSaveDocument(LPSTR docName) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}


//-----------------------------------------------------------------------------
//kdSaveDocument - Документ сохранен.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::SaveDocument() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}


//-----------------------------------------------------------------------------
//kdActiveDocument - Документ активизирован.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::Activate() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		UpdateSlideBox(pThis->refDoc);
	return true;
}


//-----------------------------------------------------------------------------
//kdDeactiveDocument - Документ деактивизирован.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::Deactivate() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		UpdateSlideBox(0);
	return true;
}

//-----------------------------------------------------------------------------
//kdBeginSaveAsDocument - Процесс сохранения файла с новым именем.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::BeginSaveAsDocument() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}

//-----------------------------------------------------------------------------
//kdDocumentFrameOpen    - Окно документа открылось.
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::DocumentFrameOpen(LPUNKNOWN v) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		UpdateSlideBox();
	return true;
}

//-----------------------------------------------------------------------------
// kdProcessActivate      - Активизация процесса
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::ProcessActivate(long Id) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}

//-----------------------------------------------------------------------------
// kdProcessDeactivate    - Деактивизация процесса
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::ProcessDeactivate(long Id) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		UpdateSlideBox();
	return true;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::BeginAutoSaveDocument(LPSTR docName) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) DocumentEvent::XDocumentFileNotify::AutoSaveDocument() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}


////////////////////////////////////////////////////////////////////////////////
//
// Document2DEvent  - обработчик событий от 2D документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
Document2DEvent::Document2DEvent(reference p) :
	cBaseEvent(ntDocument2DNotify, p, 0, p)
{
}


//-------------------------------------------------------------------------------
//
// ---
Document2DEvent::~Document2DEvent()
{
}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN Document2DEvent::GetUnknown() {
	m_xDocument2DNotify.AddRef();
	return &m_xDocument2DNotify;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) Document2DEvent::XDocument2DNotify::Release()
{
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) Document2DEvent::XDocument2DNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP Document2DEvent::XDocument2DNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IDocument2DNotify))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}


//-----------------------------------------------------------------------------
//Обрабатывается ли библиотекой событие с номером notifyType
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
	return  notifyType >= d2BeginRebuild && notifyType <= d2LocalFragmentEdit;
}


//-----------------------------------------------------------------------------
// d2BeginRebuild - Начало перестроения 2D документа.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::BeginRebuild() {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}


//-----------------------------------------------------------------------------
// d2Rebuild - Завершение перестроения 2D документа.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::Rebuild() {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		UpdateSlideBox();
	return true;
}


//-----------------------------------------------------------------------------
// d2BeginChoiceMaterial - Изменение материала.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::BeginChoiceMaterial() {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}


//-----------------------------------------------------------------------------
// dChoiceMaterial - Изменение материала.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::ChoiceMaterial(LPSTR material, double density) {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}


//-----------------------------------------------------------------------------
// d2BeginInsertFragment - Изменение материала.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::BeginInsertFragment() {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}


//-----------------------------------------------------------------------------
// d2LocalFragmentEdit - Редактирование локального фрагмента.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::LocalFragmentEdit(long pDoc, VARIANT_BOOL newFrw) {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::BeginChoiceProperty(long objRef, double propID) {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::ChoiceProperty(long objRef, double propID) {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}

STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::BeginDeleteProperty(long objRef, double propID) {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}

STDMETHODIMP_(VARIANT_BOOL) Document2DEvent::XDocument2DNotify::DeleteProperty(long objRef, double propID) {
	METHOD_PROLOGUE_EX_(Document2DEvent, Document2DNotify)
		return true;
}


////////////////////////////////////////////////////////////////////////////////
//
// Document3DEvent  - обработчик событий от 3D документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
Document3DEvent::Document3DEvent(reference p, LPUNKNOWN doc) :
	cBaseEvent(ntDocument3DNotify, doc, 0, 0, p)
{
}


//-------------------------------------------------------------------------------
//
// ---
Document3DEvent::~Document3DEvent()
{

}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN Document3DEvent::GetUnknown() {
	m_xDocument3DNotify.AddRef();
	return &m_xDocument3DNotify;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) Document3DEvent::XDocument3DNotify::Release()
{
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) Document3DEvent::XDocument3DNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP Document3DEvent::XDocument3DNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IDocument3DNotify))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

//-----------------------------------------------------------------------------
//Обрабатывается ли библиотекой событие с номером notifyType
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
	return  notifyType >= d3BeginRebuild && notifyType <= d3BeginCreatePartFromFile;
}

//-----------------------------------------------------------------------------
// d3BeginRebuild - Начало перестроения модели.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginRebuild() {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

//-----------------------------------------------------------------------------
// d3Rebuild - Завершение перестроения модели.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::Rebuild() {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		UpdateSlideBox();
	return true;
}

//-----------------------------------------------------------------------------
// d3BeginChoiceMaterial - Изменение материала.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginChoiceMaterial() {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

//-----------------------------------------------------------------------------
// d3ChoiceMaterial - Изменение материала.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::ChoiceMaterial(LPSTR material, double density) {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginDeleteMaterial() {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::DeleteMaterial() {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

//-----------------------------------------------------------------------------
// d3BeginChoiceMarking - Изменение обозначения.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginChoiceMarking() {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

//-----------------------------------------------------------------------------
// d3ChoiceMarking - Изменение обозначения.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::ChoiceMarking(LPSTR marking) {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

//-----------------------------------------------------------------------------
// d3BeginCreatePartFromFile - Запуск процесса вставки детали в сборку.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginSetPartFromFile() {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}

//-----------------------------------------------------------------------------
// d3BeginCreatePartFromFile - Запуск процесса вставки детали в сборку.
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginCreatePartFromFile(VARIANT_BOOL typeDoc, IEntity* plane) {
	METHOD_PROLOGUE_EX_(Document3DEvent, Document3DNotify)
		return true;
}


STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::CreateEmbodiment(LPSTR marking)
{
	return VARIANT_TRUE;
}


STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::DeleteEmbodiment(LPSTR marking)
{
	return VARIANT_TRUE;
}


STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::ChangeCurrentEmbodiment(LPSTR marking)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginChoiceProperty(IUnknown* obj, double propID)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::ChoiceProperty(IUnknown* obj, double propID)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginDeleteProperty(IUnknown* obj, double propID)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::DeleteProperty(IUnknown* obj, double propID)
{
	return VARIANT_TRUE;
}


//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BeginRollbackFeatures() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::RollbackFeatures() {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::BedinLoadCombinationChange(long index) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(VARIANT_BOOL) Document3DEvent::XDocument3DNotify::LoadCombinationChange(long index) {
	METHOD_PROLOGUE_EX_(DocumentEvent, DocumentFileNotify)
		return true;
}

