////////////////////////////////////////////////////////////////////////////////
//
// .h
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _SPCOBJECTEVENT_H
#include "SpcObjEvent.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern AFX_MODULE_STATE* pModuleState;

extern cBaseEvent* NewDocumentEvent(reference doc);
extern void UpdateSlideBox();

//-------------------------------------------------------------------------------
// ������� ���������� ������� ��� ������� ������������
// ---
cBaseEvent* NewSpcObjectEvent(reference doc, int objType) {
	SpcObjectEvent* res = NULL;
	if (doc) {
		res = (SpcObjectEvent*)cBaseEvent::FindEvents(ntSpcObjectNotify, doc, objType);
		if (!res) {
			NewDocumentEvent(doc); // ����� ��� �������� ��������� ����������
			res = new SpcObjectEvent(doc, objType);
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
// SelectMngEvent	- ���������� ������� �� ��������� �������������� ���������
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
SpcObjectEvent::SpcObjectEvent(reference doc, reference obj)
	: cBaseEvent(ntSpcObjectNotify, doc, obj, doc)
{
}

//-------------------------------------------------------------------------------
//
// ---
SpcObjectEvent::~SpcObjectEvent()
{
}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN SpcObjectEvent::GetUnknown() {
	m_xSpcObjectNotify.AddRef();
	return &m_xSpcObjectNotify;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) SpcObjectEvent::XSpcObjectNotify::Release()
{
	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) SpcObjectEvent::XSpcObjectNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP SpcObjectEvent::XSpcObjectNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_ISpcObjectNotify))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

//-----------------------------------------------------------------------------
//�������������� �� ����������� ������� � ������� notifyType
// ---
STDMETHODIMP_(VARIANT_BOOL) SpcObjectEvent::XSpcObjectNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return	notifyType >= soBeginDelete && notifyType <= soUpdateObject;
}

/////////////////////////////////////////////////////////////////////////////
// SpcObjectEvent message handlers
//-------------------------------------------------------------------------------
// soBeginDelete - �������� ������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::BeginDelete(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// soDelete		- ������ ������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::Delete(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	UpdateSlideBox();
	return true;
}

//-------------------------------------------------------------------------------
// soCellDblClick - ������� ���� �� ������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::CellDblClick(long obj, long number) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// soCellBeginEdit - �������������� ������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::CellBeginEdit(long obj, long number) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}
//-------------------------------------------------------------------------------
// ksmUnselectAll - ��� ������� ����������������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::ChangeCurrent(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return true;
}

//-------------------------------------------------------------------------------
// ksmUnselectAll - ��� ������� ����������������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::DocumentBeginAdd(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmUnselectAll - ��� ������� ����������������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::DocumentAdd(long obj, LPSTR docName) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmUnselectAll - ��� ������� ����������������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::DocumentRemove(long obj, LPSTR docName) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmUnselectAll - ��� ������� ����������������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::BeginGeomChange(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmUnselectAll - ��� ������� ����������������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::GeomChange(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmUnselectAll - ������ ��������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::BeginProcess(long pType, long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmEndProcess - ���������� ��������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::EndProcess(long pType) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmCreateObject - �������� �������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::CreateObject(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// ksmUpdateObject - ���������� ���������� �������
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::UpdateObject(long obj) {
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	UpdateSlideBox();
	return TRUE;
}

//-----------------------------------------------------------------------------
//
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::BeginCopy(long objRef)
{
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return true;
}

//-----------------------------------------------------------------------------
//
// ---
VARIANT_BOOL SpcObjectEvent::XSpcObjectNotify::Copy(long objRef)
{
	//	METHOD_PROLOGUE_EX_(SpcObjectEvent, SpcObjectNotify)
	return true;
}