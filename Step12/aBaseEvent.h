/////////////////////////////////////////////////////////////////////////////
//
// EventsList - ������ �����������
// ABaseEvent - ������� ���� ��� ������������ ������� � �������������
//
////////////////////////////////////////////////////////////////////////////
#ifndef _ABASEEVENT_H
#define _ABASEEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#endif

///������� ���� ��� ����������������� ������������ �������
class ABaseEvent : public CCmdTarget
{
protected:
	static CObList& m_EventList;	/// ������ ������������ �������
	DWORD				m_dwCookie;		/// ������������� ����������
	IID					m_iidEvents;	/// IID ���������� �������
	LPCONNECTIONPOINT	m_pConnPt;		/// ����������
	LPUNKNOWN			m_pContainer;	/// �������� �������

public:
	/// �����������
	ABaseEvent(LPUNKNOWN pObject, IID iidEvents);
	/// ����������
	virtual ~ABaseEvent();

	int Advise();			/// ����������� �� ��������� �������
	void Unadvise();		/// ���������� �� ��������� �������
	void RemoveThis();		/// ������� ���� �� ������
	virtual void Clear();	/// ���������� ����������
	void Disconnect();		/// ������������

	static void TerminateEvents(void);								/// �������� ��� �������
	static void DestroyList();											/// ������� ������ �������
	static void TerminateEvents(IID iid, LPUNKNOWN container = NULL); /// ���������� �� ��������� �� ��������� ��� ����
	static bool FindEvent(IID iid, LPUNKNOWN container = NULL);		/// ����� ��������

protected:
	BEGIN_INTERFACE_PART(EventHandler, IDispatch)
		INIT_INTERFACE_PART(BaseEvent, EventHandler)
		STDMETHOD(GetTypeInfoCount)(unsigned int*);
		STDMETHOD(GetTypeInfo)(unsigned int, LCID, ITypeInfo**);
		STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, unsigned int, LCID, DISPID*);
		STDMETHOD(Invoke)(DISPID, REFIID, LCID, unsigned short, DISPPARAMS*,
			VARIANT*, EXCEPINFO*, unsigned int*);
	END_INTERFACE_PART(EventHandler)

	DECLARE_INTERFACE_MAP()
	DECLARE_EVENTSINK_MAP()
};

/// ������� ����� ��� ������������ ������� �������
class cBaseEvent : public CCmdTarget
{
protected:
	static CObList& m_EventList;		/// ������ ������������ �������
	DWORD	m_dwCookie;					/// ������������� ����������
	NotifyConnectionParam m_params;		/// ��������� ��������
	reference	refDoc;					/// �������� ��������� ��� �������

public:
	cBaseEvent(int ifType = 0, reference p = 0, long objType = 0, reference doc = 0);
	cBaseEvent(int ifType, LPUNKNOWN iContainer = NULL, long objType = 0, LPUNKNOWN iObj = NULL, reference doc = 0);
	virtual	~cBaseEvent();

	bool Advise();								/// ����������� �� ��������� �������
	void Unadvise();							/// ���������� �� ��������� �������
	void RemoveThis();							/// ������� ���� �� ������
	virtual void Clear();						/// �������
	void Disconnect();							/// ������������

	static void	TerminateEvents(void);		/// �������� ��� �������
	static void	DestroyList();					/// ������� ������
	int	GetType() { return m_params.ifType; }	/// ��� �������

	static void	TerminateEvents(long eventType, reference doc = 0, int objType = -1, LPUNKNOWN iObj = NULL);
	static cBaseEvent* FindEvents(long eventType, reference doc = 0, int objType = -1, LPUNKNOWN iObj = NULL);

	virtual	 LPUNKNOWN GetUnknown() = 0;
};
