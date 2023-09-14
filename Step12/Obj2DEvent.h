////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _OBJ2DEVENT_H
#define _OBJ2DEVENT_H

#ifndef _ABASEEVENT_H
#include "ABaseEvent.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#endif

////////////////////////////////////////////////////////////////////////////////
//
// Obj2DEvent	- ���������� ������� �� ���������
//
////////////////////////////////////////////////////////////////////////////////
class Obj2DEvent : public cBaseEvent
{
	//	ksObject2DNotifyResultPtr m_res;
	// Attributes
public:
	Obj2DEvent(reference p, long objType);
	virtual ~Obj2DEvent();

	// Operations
public:
	//	static TerminateEventsFromObjectType( int type );
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Obj2DEvent)
	//}}AFX_VIRTUAL
	virtual	 LPUNKNOWN GetUnknown();
	// Implementation

	BEGIN_INTERFACE_PART(Object2DNotify, IObject2DNotify)
		INIT_INTERFACE_PART(Obj2DEvent, Object2DNotify)

		//koChangeActive		// ������������ ���������� �������( ���, ����)
		STDMETHOD_(VARIANT_BOOL, ChangeActive)(THIS_ long objRef);

	//koBeginDelete		 // ������ �������� ������, false - ��������� ��������
	STDMETHOD_(VARIANT_BOOL, BeginDelete)(THIS_ long objRef);

	//koDelete					// �������� �������
	STDMETHOD_(VARIANT_BOOL, Delete)(THIS_ long objRef);

	//koBeginMove			 // ������ ������ �������, false - ��������� �����
	STDMETHOD_(VARIANT_BOOL, BeginMove)(THIS_ long objRef);

	//koMove						// ����� �������
	STDMETHOD_(VARIANT_BOOL, Move)(THIS_ long objRef);

	//koBeginRotate		 // ������ �������� �������, false - ��������� �������
	STDMETHOD_(VARIANT_BOOL, BeginRotate)(THIS_ long obgRef);

	//koRotate					// ������� �������
	STDMETHOD_(VARIANT_BOOL, Rotate)(THIS_ long objRef);

	//koBeginScale		 // ������ ��������������� �������, false - ��������� �������
	STDMETHOD_(VARIANT_BOOL, BeginScale)(THIS_ long obgRef);

	//koScale					// ��������������� �������
	STDMETHOD_(VARIANT_BOOL, Scale)(THIS_ long objRef);

	//koBeginTransform	// ������ ������������� �������, false - ��������� �������������
	STDMETHOD_(VARIANT_BOOL, BeginTransform)(THIS_ long objRef);

	//koTransform			 // ������������� �������
	STDMETHOD_(VARIANT_BOOL, Transform)(THIS_ long objRef);

	//koBeginCopy			 // ������ ����������� �������, false - ��������� �����������
	STDMETHOD_(VARIANT_BOOL, BeginCopy)(THIS_ long objRef);

	//koCopy						// ����������� �������
	STDMETHOD_(VARIANT_BOOL, Copy)(THIS_ long objRef);

	//koBeginSymmetry	 // ������ ���������	�������, false - ��������� ���������
	STDMETHOD_(VARIANT_BOOL, BeginSymmetry)(THIS_ long objRef);

	//koSymmetry				// ���������	�������
	STDMETHOD_(VARIANT_BOOL, Symmetry)(THIS_ long objRef);

	// ������ ��������������\�������� �������.false - ��������� �������.
	STDMETHOD_(VARIANT_BOOL, BeginProcess)(THIS_ long pType, long obj);

	// ����� ��������������\�������� �������
	STDMETHOD_(VARIANT_BOOL, EndProcess)(THIS_ long pType);

	// �������� �������.
	STDMETHOD_(VARIANT_BOOL, CreateObject)(THIS_ long obj);

	// �������������� �������.
	STDMETHOD_(VARIANT_BOOL, UpdateObject)(THIS_ long obj);

	// �������� �������.
	STDMETHOD_(VARIANT_BOOL, BeginDestroyObject)(THIS_ long obj);

	// �������������� �������.
	STDMETHOD_(VARIANT_BOOL, DestroyObject)(THIS_ long obj);

	//�������������� �� ����������� ������� � ������� notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);

	STDMETHOD_(VARIANT_BOOL, BeginPropertyChanged) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, PropertyChanged) (THIS_ long objRef);

	END_INTERFACE_PART(Object2DNotify)
};

cBaseEvent* NewObj2DEvent(reference doc, int objType);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
