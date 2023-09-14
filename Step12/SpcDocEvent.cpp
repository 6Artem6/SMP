////////////////////////////////////////////////////////////////////////////////
//
// ������� ��������� ������������
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _SPCDOCEVENT_H
#include "SpcDocEvent.h"
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
// ������� ���������� ������� ��������� c�����������
// ---
cBaseEvent* NewSpcDocEvent(reference doc) {
	SpcDocEvent* res = NULL;
	if (doc) {
		res = (SpcDocEvent*)cBaseEvent::FindEvents(ntSpcDocumentNotify, doc);
		if (!res) {
			NewDocumentEvent(doc); // ����� ��� �������� ��������� ����������
			res = new SpcDocEvent(doc);
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
// SpcDocEvent	- ���������� ������� �� ��������� ������������
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
SpcDocEvent::SpcDocEvent(reference doc)
	: cBaseEvent(ntSpcDocumentNotify, doc, 0, doc)
{
}

//-------------------------------------------------------------------------------
//
// ---
SpcDocEvent::~SpcDocEvent()
{
}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN SpcDocEvent::GetUnknown() {
	m_xSpcDocumentNotify.AddRef();
	return &m_xSpcDocumentNotify;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) SpcDocEvent::XSpcDocumentNotify::Release()
{
	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) SpcDocEvent::XSpcDocumentNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP SpcDocEvent::XSpcDocumentNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_ISpcDocumentNotify))
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
STDMETHODIMP_(VARIANT_BOOL) SpcDocEvent::XSpcDocumentNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
	return notifyType == sdSpcStyleChange; // notifyType >= sdDocumentBeginAdd && notifyType <= sdSpcStyleChange;
}

/////////////////////////////////////////////////////////////////////////////
// SpcDocEvent message handlers
//-------------------------------------------------------------------------------
// sdDocumentBeginAdd			- ���������� ���������
// ---
VARIANT_BOOL SpcDocEvent::XSpcDocumentNotify::DocumentBeginAdd(THIS_) {
	//	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// sdDocumentAdd		- ���������� ���������
// ---
VARIANT_BOOL SpcDocEvent::XSpcDocumentNotify::DocumentAdd(LPSTR docName) {
	//	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// sdDocumentBeginRemove - �������� ���������
// ---
VARIANT_BOOL SpcDocEvent::XSpcDocumentNotify::DocumentBeginRemove(LPSTR docName) {
	//	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// sdDocumentBeginRemove - �������� ���������
// ---
VARIANT_BOOL SpcDocEvent::XSpcDocumentNotify::DocumentRemove(LPSTR docName) {
	//	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
	return TRUE;
}
//-------------------------------------------------------------------------------
// sdSpcStyleBeginChange - ��������� �����
// ---
VARIANT_BOOL SpcDocEvent::XSpcDocumentNotify::SpcStyleBeginChange(LPSTR libName, long numb) {
	//	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// sdSpcStyleChange - ��������� �����
// ---
VARIANT_BOOL SpcDocEvent::XSpcDocumentNotify::SpcStyleChange(LPSTR libName, long numb) {
	//	METHOD_PROLOGUE_EX_(SpcDocEvent, SpcDocumentNotify)
	UpdateSlideBox();
	return TRUE;
}