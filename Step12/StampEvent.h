////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _STAMPEVENT_H
#define _STAMPEVENT_H

#ifndef _ABASEEVENT_H
#include "ABaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

////////////////////////////////////////////////////////////////////////////////
//
// StampEvent	- ���������� ������� �� ���������
//
////////////////////////////////////////////////////////////////////////////////
class StampEvent : public cBaseEvent
{
	// Attributes
public:
	StampEvent(reference doc);
	virtual ~StampEvent();

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StampEvent)
	//}}AFX_VIRTUAL
	virtual	 LPUNKNOWN GetUnknown();
	// Implementation

	BEGIN_INTERFACE_PART(StampNotify, IStampNotify)
		INIT_INTERFACE_PART(StampEvent, StampNotify)
		// kdBeginEditStamp			// ������ ������ �� �������
		STDMETHOD_(VARIANT_BOOL, BeginEditStamp)();

	//kdEndEditStamp				 // ���������� ������ �� �������
	STDMETHOD_(VARIANT_BOOL, EndEditStamp)(VARIANT_BOOL editResult);

	//kdStampCellDblClick		// ������� ���� � ������ ������
	STDMETHOD_(VARIANT_BOOL, StampCellDblClick)(long number);

	//kdStampCellBeginEdit	 // ������ �������������� � ������ ������
	STDMETHOD_(VARIANT_BOOL, StampCellBeginEdit)(long number);

	//�������������� �� ����������� ������� � ������� notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(int notifyType);

	STDMETHOD_(VARIANT_BOOL, StampBeginClearCells) (VARIANT numbers);

	END_INTERFACE_PART(StampNotify)
};

cBaseEvent* NewStampEvent(reference doc);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
