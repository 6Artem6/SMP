////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _SPECIFICATIONEVENT_H
#include "SpecificationEvent.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern cBaseEvent* NewDocumentEvent(reference doc);
extern void UpdateSlideBox();

//-------------------------------------------------------------------------------
// ������� ���������� ������� �������������� ������������
// ---
cBaseEvent* NewSpecificationEvent(reference doc) {
	SpecificationEvent* res = NULL;
	if (doc) {
		res = (SpecificationEvent*)cBaseEvent::FindEvents(ntSpecificationNotify, doc);
		if (!res) {
			NewDocumentEvent(doc); // ����� ��� �������� ��������� ����������
			res = new SpecificationEvent(doc);
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
SpecificationEvent::SpecificationEvent(reference doc)
	: cBaseEvent(ntSpecificationNotify, doc, 0, doc)
{
}

//-------------------------------------------------------------------------------
//
// ---
SpecificationEvent::~SpecificationEvent()
{
}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN SpecificationEvent::GetUnknown() {
	m_xSpecificationNotify.AddRef();
	return &m_xSpecificationNotify;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) SpecificationEvent::XSpecificationNotify::Release()
{
	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) SpecificationEvent::XSpecificationNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP SpecificationEvent::XSpecificationNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_ISpecificationNotify))
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
STDMETHODIMP_(VARIANT_BOOL) SpecificationEvent::XSpecificationNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return	notifyType == ssTuningSpcStyleBeginChange && notifyType <= ssCalcPositions; // notifyType >= ssTuningSpcStyleBeginChange && notifyType <= ssBeginCreateObject;
}

/////////////////////////////////////////////////////////////////////////////
// SpecificationEvent message handlers
//-------------------------------------------------------------------------------
// ssTuningSpcStyleBeginChange	- ������ ��������� �������� ������������.
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::TuningSpcStyleBeginChange(LPSTR libName, long numb) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssTuningSpcStyleChange	- ��������� �������� ������������.
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::TuningSpcStyleChange(LPSTR libName, long numb, VARIANT_BOOL isOk) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssChangeCurrentSpcDescription - ��������� �������� �������� ������������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::ChangeCurrentSpcDescription(LPSTR libName, long numb) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	//	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssSpcDescriptionAdd - ���������� �������� ������������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::SpcDescriptionAdd(LPSTR libName, long numb) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	//	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssSpcDescriptionRemove - �������� �������� ������������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::SpcDescriptionRemove(LPSTR libName, long numb) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	//	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssSpcDescriptionRemove - �������������� �������� ������������
// ---
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::SpcDescriptionBeginEdit(LPSTR libName, long numb) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssSpcDescriptionEdit - �������������� �������� ������������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::SpcDescriptionEdit(LPSTR libName, long numb, VARIANT_BOOL isOk) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssSynchronizationBegin - ������ �������������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::SynchronizationBegin() {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssSynchronization - ���������� �������������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::Synchronization() {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssBeginCalcPositions - ������ �������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::BeginCalcPositions() {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssCalcPositions - ������ �������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::CalcPositions() {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// ssBeginCreateObject - �������� �������
// ---
VARIANT_BOOL SpecificationEvent::XSpecificationNotify::BeginCreateObject(long typeObj) {
	//	METHOD_PROLOGUE_EX_(SpecificationEvent, SpecificationNotify)
	return TRUE;
}