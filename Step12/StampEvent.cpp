////////////////////////////////////////////////////////////////////////////////
//
// .h
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _STAMPEVENT_H
#include "StampEvent.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern AFX_MODULE_STATE* pModuleState;

extern cBaseEvent* NewDocumentEvent(reference doc);
extern void UpdateSlideBox();

//-------------------------------------------------------------------------------
// Создать обработчик событий для штампа
// ---
cBaseEvent* NewStampEvent(reference doc) {
	StampEvent* res = NULL;
	if (doc) {
		int docType = ::ksGetDocumentType(doc);
		switch (docType) {
		case lt_DocSheetStandart:
		case lt_DocSheetUser:
		case lt_DocSpc:
		case lt_DocSpcUser:
		case lt_DocTxtStandart:
		case lt_DocTxtUser: {
			if (!cBaseEvent::FindEvents(ntStampNotify, doc)) {
				NewDocumentEvent(doc); // чтобы при закрытии документа отписаться
				res = new StampEvent(doc);
				if (!res->Advise()) {
					delete res;
					res = NULL;
				}
			}
		}
		}
	}
	return res;
}

////////////////////////////////////////////////////////////////////////////////
//
// StampEvent	- обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
StampEvent::StampEvent(reference doc) :
	cBaseEvent(ntStampNotify, doc, 0, doc)
{
}

//-------------------------------------------------------------------------------
//
// ---
StampEvent::~StampEvent()
{
}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN StampEvent::GetUnknown() {
	m_xStampNotify.AddRef();
	return &m_xStampNotify;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) StampEvent::XStampNotify::Release()
{
	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) StampEvent::XStampNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP StampEvent::XStampNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IStampNotify))
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
STDMETHODIMP_(VARIANT_BOOL) StampEvent::XStampNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
	return notifyType == kdEndEditStamp;// notifyType >= kdBeginEditStamp && notifyType <= kdStampCellBeginEdit;
}

/////////////////////////////////////////////////////////////////////////////
// StampEvent message handlers
//-----------------------------------------------------------------------------
// kdBeginEditStamp			- Начало работы со штампом.
// ---
STDMETHODIMP_(VARIANT_BOOL) StampEvent::XStampNotify::BeginEditStamp() {
	//	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
	return TRUE;
}

//-----------------------------------------------------------------------------
// kdEndEditStamp				 - Завершение работы со штампом.
// ---
STDMETHODIMP_(VARIANT_BOOL) StampEvent::XStampNotify::EndEditStamp(VARIANT_BOOL editResult) {
	//	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
	UpdateSlideBox();
	return TRUE;
}

//-----------------------------------------------------------------------------
//kdStampCellDblClick		- Двойной клик в ячейке штампа.
// ---
STDMETHODIMP_(VARIANT_BOOL) StampEvent::XStampNotify::StampCellDblClick(long number) {
	//	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
	return TRUE;
}

//-----------------------------------------------------------------------------
//kdStampCellBeginEdit	 - Начало редактирования в ячейке штампа.
// ---
STDMETHODIMP_(VARIANT_BOOL) StampEvent::XStampNotify::StampCellBeginEdit(long number) {
	//	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
	return TRUE;
}

//-----------------------------------------------------------------------------
//kdStampBeginClearCells	 - Начало редактирования в ячейке штампа.
// ---
STDMETHODIMP_(VARIANT_BOOL) StampEvent::XStampNotify::StampBeginClearCells(VARIANT numbers) {
	//	METHOD_PROLOGUE_EX_(StampEvent, StampNotify)
	return TRUE;
}