////////////////////////////////////////////////////////////////////////////////
//
// ������� ����������
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "ApplicationEvent.h"
#include "MainParameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern AFX_MODULE_STATE* pModuleState;

extern MainParameters* mainParameters;

extern void UpdateSlideBox();
extern void UpdateSlideBox(reference docRef);
extern void AdviseDoc(reference docRef);
extern void ClosePropertyManager(int mes);

//-----------------------------------------------------------------------------
// �������� �� ������� ����������
// ---
cBaseEvent* NewApplicationEvent()
{
	ApplicationEvent* res = (ApplicationEvent*)cBaseEvent::FindEvents(ntKompasObjectNotify, 0);
	if (!res) {
		res = new ApplicationEvent();
		if (!res->Advise()) {
			delete res;
			res = NULL;
		}
	}
	return res;
}

////////////////////////////////////////////////////////////////////////////////
//
// ApplicationEvent	- ���������� ������� �� ����������
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
ApplicationEvent::ApplicationEvent()
	: cBaseEvent(ntKompasObjectNotify, (reference)0)
{
}

//-------------------------------------------------------------------------------
//
// ---
ApplicationEvent::~ApplicationEvent()
{
}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN ApplicationEvent::GetUnknown() {
	m_xKompasObjectNotify.AddRef();
	return &m_xKompasObjectNotify;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) ApplicationEvent::XKompasObjectNotify::Release()
{
	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) ApplicationEvent::XKompasObjectNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP ApplicationEvent::XKompasObjectNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IKompasObjectNotify))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

//-----------------------------------------------------------------------------
// �������������� �� ����������� ������� � ������� notifyType
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	return	notifyType >= koCreateDocument && notifyType <= koApplicatinDestroy && notifyType != koBeginOpenDocument; // koBeginCloseAllDocument;
}

/////////////////////////////////////////////////////////////////////////////
// ApplicationEvent message handlers
//-----------------------------------------------------------------------------
// koCreateDocument - �������� ������
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::CreateDocument(long pDoc, int docType) {
	//METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	AdviseDoc(pDoc);
	UpdateSlideBox(pDoc);
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// koOpenDocumenBegin - ������ �������� ���������
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginOpenDocument(LPSTR docName) {
	//METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// koOpenDocumen - �������� ������
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::OpenDocument(long pDoc, int docType) {
	//	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	AdviseDoc(pDoc);
	UpdateSlideBox(pDoc);
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// koActiveDocument - ������������ �� ������ �������� ��������
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::ChangeActiveDocument(long pDoc, int docType) {
	//	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	UpdateSlideBox(pDoc);
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// koApplicatinDestroy - �������� ����������
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::ApplicationDestroy() {
	ABaseEvent::TerminateEvents();
	cBaseEvent::TerminateEvents();
	ClosePropertyManager(0);
	mainParameters->application = NULL;
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// koBeginCreate - ������� �������� �����
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginCreate(long type) {
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// koBeginOpenFile - ������� �������� �����
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginOpenFile() {
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// BeginCloseAllDocument - ������� �������� ���� ����������
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginCloseAllDocument() {
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// KeyDown - ������� ����������. ( ���������� ������� )
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::KeyDown(long* key, long flags, VARIANT_BOOL sysKey)
{
	//	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// KeyUp - ������� ����������. ( ����������� ������� )
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::KeyUp(long* key, long flags, VARIANT_BOOL sysKey)
{
	//	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	return VARIANT_TRUE;
}

//-----------------------------------------------------------------------------
// KeyPress - ������� ����������. ( ������� ���������� ������� ���������� ������� )
// ---
STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::KeyPress(long* key, VARIANT_BOOL sysKey)
{
	//	METHOD_PROLOGUE_EX_(ApplicationEvent, KompasObjectNotify)
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginRequestFiles(long requestID, VARIANT* files)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginChoiceMaterial(long MaterialPropertyId)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::ChoiceMaterial(long MaterialPropertyId, LPSTR material, double density)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::IsNeedConvertToSavePrevious(long pDoc, int docType, long saveVersion, IUnknown* saveToPreviusParam, VARIANT_BOOL* needConvert)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginConvertToSavePrevious(long pDoc, int docType, long saveVersion, IUnknown* saveToPreviusParam)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::EndConvertToSavePrevious(long pDoc, int docType, long saveVersion, IUnknown* saveToPreviusParam)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::ChangeTheme(long)
{
	return VARIANT_TRUE;
}

STDMETHODIMP_(VARIANT_BOOL) ApplicationEvent::XKompasObjectNotify::BeginDragOpenFiles(VARIANT* filesList, VARIANT_BOOL insert, VARIANT_BOOL* filesListChanged)
{
	return VARIANT_TRUE;
}