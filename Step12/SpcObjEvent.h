////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _SPCOBJECTEVENT_H
#define _SPCOBJECTEVENT_H

#ifndef _ABASEEVENT_H
#include "ABaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

////////////////////////////////////////////////////////////////////////////////
//
// SpcObjEvent	- обработчик событий
//
////////////////////////////////////////////////////////////////////////////////
class SpcObjectEvent : public cBaseEvent
{
	//	CString libName;

	// Attributes
public:
	SpcObjectEvent(reference doc, reference obj);
	virtual ~SpcObjectEvent();

	// Operations
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(SelectMngEvent)
		//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SelectMngEvent)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	virtual	 LPUNKNOWN GetUnknown();
	// Implementation

	BEGIN_INTERFACE_PART(SpcObjectNotify, ISpcObjectNotify)
		INIT_INTERFACE_PART(SpcObjectEvent, SpcObjectNotify)
		STDMETHOD_(VARIANT_BOOL, BeginDelete) (THIS_	long obj);

	STDMETHOD_(VARIANT_BOOL, Delete) (THIS_ long obj);

	STDMETHOD_(VARIANT_BOOL, CellDblClick) (THIS_ long objRef, long number);

	STDMETHOD_(VARIANT_BOOL, CellBeginEdit) (THIS_ long objRef, long number);

	STDMETHOD_(VARIANT_BOOL, ChangeCurrent) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, DocumentBeginAdd) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, DocumentAdd) (THIS_ long objRef, LPSTR docName);

	STDMETHOD_(VARIANT_BOOL, DocumentRemove) (THIS_ long objRef, LPSTR docName);

	STDMETHOD_(VARIANT_BOOL, BeginGeomChange) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, GeomChange) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, BeginProcess) (THIS_ long pType, long objRef);

	STDMETHOD_(VARIANT_BOOL, EndProcess) (THIS_ long pType);

	STDMETHOD_(VARIANT_BOOL, CreateObject) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, UpdateObject) (THIS_ long objRef);

	// Обрабатывается ли библиотекой событие с номером notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(int notifyType);

	STDMETHOD_(VARIANT_BOOL, BeginCopy) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, Copy) (THIS_ long objRef);

	END_INTERFACE_PART(SpcObjectNotify)
};

cBaseEvent* NewSpcObjectEvent(reference doc, int objType);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
