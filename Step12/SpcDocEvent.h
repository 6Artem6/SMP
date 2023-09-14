////////////////////////////////////////////////////////////////////////////////
//
// .h
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _SPCDOCEVENT_H
#define _SPCDOCEVENT_H

#ifndef _ABASEEVENT_H
#include "ABaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

////////////////////////////////////////////////////////////////////////////////
//
// SelectMngEvent	- обрабокчик событий от менеджера селектирования документа
//
////////////////////////////////////////////////////////////////////////////////
class SpcDocEvent : public cBaseEvent
{
	//	CString libName;

	// Attributes
public:
	SpcDocEvent(reference doc);
	virtual ~SpcDocEvent();

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

	BEGIN_INTERFACE_PART(SpcDocumentNotify, ISpcDocumentNotify)
		INIT_INTERFACE_PART(SpcDocEvent, SpcDocumentNotify)
		STDMETHOD_(VARIANT_BOOL, DocumentBeginAdd) (THIS_);

	STDMETHOD_(VARIANT_BOOL, DocumentAdd) (THIS_ LPSTR docName);

	STDMETHOD_(VARIANT_BOOL, DocumentBeginRemove) (THIS_	LPSTR docName);

	STDMETHOD_(VARIANT_BOOL, DocumentRemove) (THIS_ LPSTR docName);

	STDMETHOD_(VARIANT_BOOL, SpcStyleBeginChange) (THIS_ LPSTR libName, long numb);

	STDMETHOD_(VARIANT_BOOL, SpcStyleChange) (THIS_ LPSTR libName, long numb);

	//Обрабатывается ли библиотекой событие с номером notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);

	END_INTERFACE_PART(SpcDocumentNotify)
};

cBaseEvent* NewSpcDocEvent(reference doc);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
