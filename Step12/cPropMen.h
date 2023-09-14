////////////////////////////////////////////////////////////////////////////////
//
// PropertyManagerObject - ����� ��� ������������ ������ �������
// PropertyManagerEvent	- ����� ������� ��������� �������
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _CPROPMEN_H
#define _CPROPMEN_H

#include "Resource.h"

#ifndef _ABASEEVENT_H
#include "aBaseEvent.h"
#endif

#ifndef _MENU_PROFILE_H
#include "MenuProfile.h"
#endif

#ifndef _MENU_CONDITIONS_H
#include "MenuConditions.h"
#endif

#ifndef	_MAIN_PARAMETERS_H
#include "MainParameters.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

int LibMessage(int strId, int flags);	/// ��������� ����������� �� ��������
int LibMessage(LPCTSTR str, int flags);	/// ��������� ���������� �������

// ��� ��������� COM ����������
void DumpError(_com_error& e);			/// ����� ������ ����������
void ShowError();						/// ����� ��������� ��� ��������������� ������� HRESULT

/// ���������� ������� �� ��������� ������ �������
class PropertyManagerEvent : public ABaseEvent
{
protected:
	// Attributes
public:
	PropertyManagerEvent(LPDISPATCH manager);
	virtual ~PropertyManagerEvent();

	// Operations
public:

	/// prButtonClick - ������� ������.	"�������� OCX �������."
	afx_msg BOOL ButtonClick(long buttonID);

	/// prChangeControlValue - "��������� OCX �������."
	afx_msg BOOL ChangeControlValue(LPDISPATCH ctrl);

	/// prControlCommand  - ������� ������ ��������
	afx_msg BOOL ControlCommand(LPDISPATCH	ctrl, long buttonID);

	/// prButtonUpdate - ��������� ��������� ������ ����������.
	afx_msg BOOL ButtonUpdate(long buttonID, long* check, VARIANT_BOOL* _enable);

	/// prProcessActivate - ������ ��������
	afx_msg BOOL ProcessActivate();

	/// prProcessDeactivate	- ���������� ��������
	afx_msg BOOL ProcessDeactivate();

	/// prCommandHelp - ��������� ��������� ������ ����������.
	afx_msg BOOL CommandHelp(long buttonID);

	afx_msg BOOL SelectItem();

	afx_msg BOOL CheckItem();

	DECLARE_EVENTSINK_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
