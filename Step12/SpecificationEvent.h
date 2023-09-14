////////////////////////////////////////////////////////////////////////////////
//
// .h
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _SPECIFICATIONEVENT_H
#define _SPECIFICATIONEVENT_H

#ifndef _ABASEEVENT_H
#include "ABaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

////////////////////////////////////////////////////////////////////////////////
//
// SelectMngEvent	- обработчик событий от менеджера селектирования документа
//
////////////////////////////////////////////////////////////////////////////////
class SpecificationEvent : public cBaseEvent
{
	//	CString libName;

	// Attributes
public:
	SpecificationEvent(reference doc);
	virtual ~SpecificationEvent();

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

	BEGIN_INTERFACE_PART(SpecificationNotify, ISpecificationNotify)
		INIT_INTERFACE_PART(SpecificationEvent, SpecificationNotify)
		STDMETHOD_(VARIANT_BOOL, TuningSpcStyleBeginChange) (THIS_ LPSTR libName, long numb);

	STDMETHOD_(VARIANT_BOOL, TuningSpcStyleChange) (THIS_ LPSTR libName, long numb, VARIANT_BOOL isOk);

	STDMETHOD_(VARIANT_BOOL, ChangeCurrentSpcDescription) (THIS_ LPSTR libName, long numb);

	STDMETHOD_(VARIANT_BOOL, SpcDescriptionAdd) (THIS_ LPSTR libName, long numb);

	STDMETHOD_(VARIANT_BOOL, SpcDescriptionRemove) (THIS_ LPSTR libName, long numb);

	STDMETHOD_(VARIANT_BOOL, SpcDescriptionBeginEdit) (THIS_ LPSTR libName, long numb);

	STDMETHOD_(VARIANT_BOOL, SpcDescriptionEdit) (THIS_ LPSTR libName, long numb, VARIANT_BOOL isOk);

	STDMETHOD_(VARIANT_BOOL, SynchronizationBegin) (THIS_);

	STDMETHOD_(VARIANT_BOOL, Synchronization) (THIS_);

	STDMETHOD_(VARIANT_BOOL, BeginCalcPositions) (THIS_);

	STDMETHOD_(VARIANT_BOOL, CalcPositions) (THIS_);

	STDMETHOD_(VARIANT_BOOL, BeginCreateObject) (THIS_ long typeObj);
	//Обрабатывается ли библиотекой событие с номером notifyType

	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);
	END_INTERFACE_PART(SpecificationNotify)
};

cBaseEvent* NewSpecificationEvent(reference doc);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
