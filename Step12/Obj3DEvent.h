////////////////////////////////////////////////////////////////////////////////
//
// ������� 3D �������
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _OBJ3DEVENT_H
#define _OBJ3DEVENT_H

#ifndef _ABASEEVENT_H
#include "ABaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

////////////////////////////////////////////////////////////////////////////////
//
// Obj3DEvent	- ���������� ������� �� ���������
//
////////////////////////////////////////////////////////////////////////////////
class Obj3DEvent : public cBaseEvent
{
	//	BaseEvent * m_parent;
	// Attributes
public:
	Obj3DEvent(LPUNKNOWN iContainer = NULL, long objType = 0, LPUNKNOWN iObj = NULL, reference doc = 0);
	virtual ~Obj3DEvent();

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Obj3DEvent)
	//}}AFX_VIRTUAL
	virtual	 LPUNKNOWN GetUnknown();

	//	void SetParent( BaseEvent * parent );
	// Implementation

	BEGIN_INTERFACE_PART(Object3DNotify, IObject3DNotify)
		INIT_INTERFACE_PART(Obj3DEvent, Object3DNotify)

		// ������ �������� �������
		STDMETHOD_(VARIANT_BOOL, BeginDelete)(THIS_ LPUNKNOWN obj);

	// O������ �������
	STDMETHOD_(VARIANT_BOOL, Delete)(THIS_ LPUNKNOWN obj);

	// O����� ��������/������� � ������
	STDMETHOD_(VARIANT_BOOL, Excluded)(THIS_ LPUNKNOWN obj, VARIANT_BOOL excluded);

	// O����� �����/�������
	STDMETHOD_(VARIANT_BOOL, Hidden)(THIS_ LPUNKNOWN obj, VARIANT_BOOL _hidden);

	// ������ ��������� ������� ������
	STDMETHOD_(VARIANT_BOOL, BeginPropertyChanged)(THIS_ LPUNKNOWN obj);

	// �������� �������� ������
	STDMETHOD_(VARIANT_BOOL, PropertyChanged)(THIS_ LPUNKNOWN obj);

	// ������ ��������� ��������� ������
	STDMETHOD_(VARIANT_BOOL, BeginPlacementChanged)(THIS_ LPUNKNOWN obj);

	// �������� ��������� ������
	STDMETHOD_(VARIANT_BOOL, PlacementChanged)(THIS_ LPUNKNOWN obj);

	// ������ ��������������\�������� �������.false - ��������� �������.
	STDMETHOD_(VARIANT_BOOL, BeginProcess)(THIS_ long pType, LPUNKNOWN obj);

	// ����� ��������������\�������� �������
	STDMETHOD_(VARIANT_BOOL, EndProcess)(THIS_ long pType);

	// �������� �������.
	STDMETHOD_(VARIANT_BOOL, CreateObject)(THIS_ LPUNKNOWN obj);

	// �������������� �������.
	STDMETHOD_(VARIANT_BOOL, UpdateObject)(THIS_ LPUNKNOWN obj);

	STDMETHOD_(VARIANT_BOOL, BeginLoadStateChange) (IUnknown* obj, long loadState);
	STDMETHOD_(VARIANT_BOOL, LoadStateChange) (IUnknown* obj, long loadState);

	//�������������� �� ����������� ������� � ������� notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);

	END_INTERFACE_PART(Object3DNotify)
};

cBaseEvent* NewObj3DEvent(reference doc, int objType, LPUNKNOWN iObj = NULL);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
