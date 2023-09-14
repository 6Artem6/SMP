////////////////////////////////////////////////////////////////////////////////
//
// ����� ��� ������ � ������� �������
//
////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "cPropMen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int LibMessage(LPCTSTR str, int flags = MB_OK);
extern void ClosePropertyManager(int mes);				/// �������� ���������������� ������

extern MainParameters* mainParameters;

/// PropertyManagerEvent - ���������� ������� �� ���������
PropertyManagerEvent::PropertyManagerEvent(LPDISPATCH manager) :
	ABaseEvent(manager, DIID_ksPropertyManagerNotify)
{
	Advise();
}

///
PropertyManagerEvent::~PropertyManagerEvent()
{
}

/// ����� ���������
BEGIN_EVENTSINK_MAP(PropertyManagerEvent, ABaseEvent)
	/// 1 - prButtonClick - ������� ������.
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prButtonClick, ButtonClick, VTS_I4)
	/// 2 - prChangeControlValue - ������� ��������� �������� ��������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prChangeControlValue, ChangeControlValue, VTS_DISPATCH)
	/// 3 - prControlCommand - ������� ������ ��������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prControlCommand, ControlCommand, VTS_DISPATCH VTS_I4)
	/// 4 - prButtonUpdate - ���������� ��������� ������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prButtonUpdate, ButtonUpdate, VTS_I4 VTS_PI4 VTS_PBOOL)
	/// 5 - prProcessActivate - ����������� ��������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prProcessActivate, ProcessActivate, VTS_NONE)
	/// 6 - prProcessDeactivate - ������������� ��������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prProcessDeactivate, ProcessDeactivate, VTS_NONE)
	/// 7 - prCommandHelp - ����� �������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prCommandHelp, CommandHelp, VTS_I4)
	/// 8 - prSelectItem - �������������� �������� � ������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prSelectItem, SelectItem, VTS_NONE)
	/// 9 - prCheckItem - ����� �������� � ������
	ON_EVENT(PropertyManagerEvent, (unsigned int)-1, prCheckItem, CheckItem, VTS_NONE)
END_EVENTSINK_MAP()

/// prChangeControlValue - ������� ��������� �������� ��������
afx_msg BOOL PropertyManagerEvent::ChangeControlValue(LPDISPATCH iCtrl) {
	if (iCtrl) {
		IPropertyControlPtr control = iCtrl;
		long id = control->Id;
		if (id == MenuProfile::Elements::e_surfaceFigure) {
			mainParameters->menuProfile->checkSurfaceFigure();
			mainParameters->propertyManager->UpdateTabs();
		} 
		else if (id == MenuProfile::Elements::e_cutRoundingType) {
			mainParameters->menuProfile->checkCutRoundingType();
			mainParameters->propertyManager->UpdateTabs();
		}
		else if (id == MenuProfile::Elements::e_hasHole) {
			mainParameters->menuProfile->checkHasHole();
			mainParameters->propertyManager->UpdateTabs();
		}
		else if (id == MenuConditions::Elements::e_parameterType) {
			mainParameters->menuConditions->checkParameters();
			mainParameters->propertyManager->UpdateTabs();
		}
		else if (id == MenuConditions::Elements::e_angleType) {
			mainParameters->menuConditions->checkParameters();
			mainParameters->propertyManager->UpdateTabs();
		}
		else if (id == MenuConditions::Elements::e_hasHole) {
			mainParameters->menuConditions->checkHasHole();
			mainParameters->propertyManager->UpdateTabs();
		}
	}
	return TRUE;
}

/// ��������� ����� ������
void OpenHelp(int Id) {
}

/// prChangeControlValue - ������� ��������� �������� ��������
afx_msg BOOL PropertyManagerEvent::ButtonClick(long buttonID) {
	if (buttonID == pbHelp) {
		OpenHelp(1);
	}
	else if (buttonID == pbEsc) {
		ClosePropertyManager(0);
	}
	else if ((mainParameters->isMenuTypeProfile()) && (buttonID == pbEnter)){
		mainParameters->buildProfileObject();
	}
	else if ((mainParameters->isMenuTypeConditions()) && (buttonID == pbEnter)){
		mainParameters->buildConditionsObject();
	}
	else if (buttonID == MenuProfile::Elements::e_enterButton){
		mainParameters->buildProfileObject();
	}
	else if (buttonID == MenuConditions::Elements::e_enterButton){
		mainParameters->buildConditionsObject();
	}
	else {
		ClosePropertyManager(0);
	}
	return TRUE;
}

/// prControlCommand ������� ������ ��������
afx_msg BOOL PropertyManagerEvent::ControlCommand(LPDISPATCH ctrl, long buttonID) {
	if (buttonID == MenuProfile::Elements::e_enterButton) {
		mainParameters->buildProfileObject();
	}
	else if (buttonID == MenuConditions::Elements::e_enterButton) {
		mainParameters->buildConditionsObject();
	}
	return TRUE;
}

/// prButtonUpdate - ��������� ��������� ������ ����������.
afx_msg BOOL PropertyManagerEvent::ButtonUpdate(long buttonID, long* check, VARIANT_BOOL* _enable) {
	if (check)
		*check = false;
	if (_enable)
		*_enable = TRUE;

	return TRUE;
}

/// prProcessActibate - ������ ��������.
afx_msg BOOL PropertyManagerEvent::ProcessActivate() {
	return TRUE;
}

/// prProcessDeactivate - ���������� ��������.
afx_msg BOOL PropertyManagerEvent::ProcessDeactivate() {
	return TRUE;
}

/// ����� �������
afx_msg BOOL PropertyManagerEvent::CommandHelp(long buttonID) {
	OpenHelp(1);
	return TRUE;
}

/// �������������� �������� � ������
afx_msg BOOL PropertyManagerEvent::SelectItem() {
	return TRUE;
}

/// ����� �������� � ������
afx_msg BOOL PropertyManagerEvent::CheckItem() {
	return TRUE;
}
