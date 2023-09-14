////////////////////////////////////////////////////////////////////////////////
//
// ������� ����������
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _APPLICATIONEVENT_H
#define _APPLICATIONEVENT_H

#ifndef _ABASEEVENT_H
#include "aBaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

/// ApplicationEvent	- ���������� ������� �� ����������
class ApplicationEvent : public cBaseEvent
{
	// Attributes
public:
	ApplicationEvent();
	virtual ~ApplicationEvent();

	// Operations
public:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ApplicationEvent)
	//}}AFX_VIRTUAL
	virtual	 LPUNKNOWN GetUnknown();
	// Implementation
	BEGIN_INTERFACE_PART(KompasObjectNotify, IKompasObjectNotify)
		INIT_INTERFACE_PART(ApplicationEvent, KompasObjectNotify)
		/// koCreateDocument - �������� ������
		STDMETHOD_(VARIANT_BOOL, CreateDocument)(THIS_ long pDoc, int docType);

	/// koOpenDocumenBegin - ������ �������� ���������
	STDMETHOD_(VARIANT_BOOL, BeginOpenDocument)(THIS_ LPSTR docName);

	/// koOpenDocumen - �������� ������
	STDMETHOD_(VARIANT_BOOL, OpenDocument)(THIS_ long pDoc, int docType);

	/// koActiveDocumen - ������������ �� ������ �������� ��������
	STDMETHOD_(VARIANT_BOOL, ChangeActiveDocument)(THIS_ long pDoc, int docType);

	/// koApplicatinDestroy -  �������� ����������
	STDMETHOD_(VARIANT_BOOL, ApplicationDestroy)(THIS_);

	/// koBeginCreate - ������� �������� �����
	STDMETHOD_(VARIANT_BOOL, BeginCreate)(THIS_ long type);

	/// koBeginOpenFile - ������� �������� �����
	STDMETHOD_(VARIANT_BOOL, BeginOpenFile)(THIS_);

	/// koBeginCloseAllDocument - �������� ���� ������
	STDMETHOD_(VARIANT_BOOL, BeginCloseAllDocument)(THIS_);

	/// �������������� �� ����������� ������� � ������� notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);

	/// koKeyDown - ������� ����������. ( ���������� ������� )
	STDMETHOD_(VARIANT_BOOL, KeyDown)(THIS_ long* key, long flags, VARIANT_BOOL sysKey);
	/// koKeyUp - ������� ����������. ( ����������� ������� )
	STDMETHOD_(VARIANT_BOOL, KeyUp)(THIS_ long* key, long flags, VARIANT_BOOL sysKey);
	/// koKeyUp - ������� ����������. ( ������� ���������� ������� ���������� ������� )
	STDMETHOD_(VARIANT_BOOL, KeyPress)(THIS_ long* key, VARIANT_BOOL sysKey);

	STDMETHOD_(VARIANT_BOOL, BeginRequestFiles) (THIS_ long requestID, VARIANT* files);

	STDMETHOD_(VARIANT_BOOL, BeginChoiceMaterial)(THIS_ long MaterialPropertyId);
	STDMETHOD_(VARIANT_BOOL, ChoiceMaterial)(THIS_ long MaterialPropertyId, LPSTR material, double density);

	STDMETHOD_(VARIANT_BOOL, IsNeedConvertToSavePrevious) (long pDoc, int docType, long saveVersion, IUnknown* saveToPreviusParam, VARIANT_BOOL* needConvert);
	STDMETHOD_(VARIANT_BOOL, BeginConvertToSavePrevious) (long pDoc, int docType, long saveVersion, IUnknown* saveToPreviusParam);
	STDMETHOD_(VARIANT_BOOL, EndConvertToSavePrevious) (long pDoc, int docType, long saveVersion, IUnknown* saveToPreviusParam);
	STDMETHOD_(VARIANT_BOOL, ChangeTheme) (long newTheme);
	STDMETHOD_(VARIANT_BOOL, BeginDragOpenFiles) (VARIANT* filesList, VARIANT_BOOL insert, VARIANT_BOOL* filesListChanged);
	END_INTERFACE_PART(KompasObjectNotify)
};

cBaseEvent* NewApplicationEvent();

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
