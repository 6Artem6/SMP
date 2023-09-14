/////////////////////////////////////////////////////////////////////////////
//
// ������� ����� ��� ������������ ������� � �������������
//
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <afxpriv.h>

#include "abaseEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-------------------------------------------------------------------------------
// ������ �����������
// ---
CObList& ABaseEvent::m_EventList = *new CObList();
CObList& cBaseEvent::m_EventList = *new CObList();

/////////////////////////////////////////////////////////////////////////////
//
// ������� ����� ��� ����������������� ������������ �������
//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
// �����������
// ---
ABaseEvent::ABaseEvent(LPUNKNOWN pObject,
	IID	iidEvents)
	: CCmdTarget(),
	m_dwCookie(0),
	m_pContainer(pObject),						// �������� �������
	m_iidEvents(iidEvents),						// IID
	m_pConnPt(NULL)
{
	if (m_pContainer)
		m_pContainer->AddRef();
	ASSERT(!IsEqualIID(m_iidEvents, GUID_NULL));	// �������� ��� �������
	m_EventList.AddTail(this);
}

//-------------------------------------------------------------------------------
// ����������
// ---
ABaseEvent::~ABaseEvent()
{
	RemoveThis();
	if (m_pContainer)
	{
		m_pContainer->Release();
		m_pContainer = NULL;
	}
}

//-------------------------------------------------------------------------------
// ����������� �� ��������� �������
// ---
int ABaseEvent::Advise()
{
	ASSERT(m_dwCookie == 0); // �������� ������������� ������

	// ����������� �� ��������� �������
	if (m_pContainer)
	{
		LPCONNECTIONPOINTCONTAINER lpContainer = NULL;
		if (SUCCEEDED(m_pContainer->QueryInterface(IID_IConnectionPointContainer, (LPVOID*)&lpContainer)))
		{
			if (lpContainer && SUCCEEDED(lpContainer->FindConnectionPoint(m_iidEvents, &m_pConnPt)))
			{
				ASSERT(m_pConnPt != NULL);
				m_pConnPt->Advise(&m_xEventHandler, &m_dwCookie);
			}
			lpContainer->Release();
		}
	}

	// ��������� �������� �� �������
	if (!m_dwCookie)
	{
		delete this;
		return 0;
	}

	return m_dwCookie;
}

//-------------------------------------------------------------------------------
//
// ---
void ABaseEvent::Clear()
{
	if (m_pContainer)
	{
		m_pContainer->Release();
		m_pContainer = NULL;
	}
	m_iidEvents = GUID_NULL;
}

//-------------------------------------------------------------------------------
// �������������
// ---
void ABaseEvent::Disconnect()
{
	Unadvise();
	Clear();
	ExternalRelease();
}

//-------------------------------------------------------------------------------
// ���������� �� ��������� �������
// ---
void ABaseEvent::Unadvise()
{
	if (m_pConnPt != NULL)				// �������� ����
	{
		m_pConnPt->Unadvise(m_dwCookie);	// ���������� �� ��������� �������
		m_pConnPt->Release();				// ����������
		m_pConnPt = NULL;
	}
	m_dwCookie = 0;
}

//-----------------------------------------------------------------------------
// �������� ��� �������
// ---
void ABaseEvent::TerminateEvents(void)
{
	while (!m_EventList.IsEmpty()) {
		ABaseEvent* headEvent = (ABaseEvent*)m_EventList.RemoveHead();
		headEvent->Disconnect();
	}
}

//-----------------------------------------------------------------------------
// �������� ��� �������
// ---
void ABaseEvent::TerminateEvents(IID iid, LPUNKNOWN container)
{
	int count = m_EventList.GetCount();
	for (int i = count - 1; i >= 0; i--)
	{
		CObject* obj = m_EventList.GetAt(m_EventList.FindIndex(i));
		ABaseEvent* event = (ABaseEvent*)obj;
		if (event && ((!container) || event->m_pContainer == container) &&
			(IsEqualIID(iid, GUID_NULL) ||
				IsEqualIID(event->m_iidEvents, iid)))
			event->Disconnect();
	}
}

//-----------------------------------------------------------------------------
//
// ---
bool ABaseEvent::FindEvent(IID iid, LPUNKNOWN container)
{
	int count = m_EventList.GetCount();
	for (int i = count - 1; i >= 0; i--)
	{
		CObject* obj = m_EventList.GetAt(m_EventList.FindIndex(i));
		ABaseEvent* event = (ABaseEvent*)obj;
		if (event && ((!container) || event->m_pContainer == container) &&
			(IsEqualIID(iid, GUID_NULL) ||
				IsEqualIID(event->m_iidEvents, iid)))
			return true;
	}
	return false;
}

//-------------------------------------------------------------------------------
// ������� ������
// ---
void ABaseEvent::DestroyList() {
	if (&m_EventList) {
		delete& m_EventList;
	}
}

//-------------------------------------------------------------------------------
// ������� ���� �� ������
// ---
void ABaseEvent::RemoveThis() {
	// ������ ���� �� ������ ������������ �������
	POSITION pos = m_EventList.Find(this);
	if (pos) {
		m_EventList.RemoveAt(pos);
		Unadvise();													 // ��������� �� ��������� �������
	}
}

//-------------------------------------------------------------------------------
// ����� ����������
// ---
BEGIN_INTERFACE_MAP(ABaseEvent, CCmdTarget)
END_INTERFACE_MAP()

//-------------------------------------------------------------------------------
// ����� ��������� ����������
// ---
BEGIN_EVENTSINK_MAP(ABaseEvent, CCmdTarget)
END_EVENTSINK_MAP()

//-------------------------------------------------------------------------------
// AddRef
// ---
STDMETHODIMP_(ULONG) ABaseEvent::XEventHandler::AddRef()
{
	METHOD_PROLOGUE(ABaseEvent, EventHandler)
		return (ULONG)pThis->ExternalAddRef();
}

//-------------------------------------------------------------------------------
// GetIDsOfNames
// ---
STDMETHODIMP ABaseEvent::XEventHandler::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,
	unsigned int cNames, LCID lcid,
	DISPID* rgdispid)
{
	METHOD_PROLOGUE(ABaseEvent, EventHandler)
		ASSERT_VALID(pThis);

	return ResultFromScode(E_NOTIMPL);
}

//-------------------------------------------------------------------------------
// GetTypeInfo
// ---
STDMETHODIMP ABaseEvent::XEventHandler::GetTypeInfo(unsigned int itinfo, LCID lcid,
	ITypeInfo** pptinfo)
{
	METHOD_PROLOGUE(ABaseEvent, EventHandler)
		ASSERT_VALID(pThis);

	return ResultFromScode(E_NOTIMPL);
}

//-------------------------------------------------------------------------------
// GetTypeInfoCount
// ---
STDMETHODIMP ABaseEvent::XEventHandler::GetTypeInfoCount(unsigned int* pctinfo)
{
	METHOD_PROLOGUE(ABaseEvent, EventHandler)
		* pctinfo = 0;
	return NOERROR;
}

//-------------------------------------------------------------------------------
// ��������� �������
// ---
STDMETHODIMP ABaseEvent::XEventHandler::Invoke(DISPID dispidMember,
	REFIID riid,
	LCID lcid,
	unsigned short	wFlags,
	DISPPARAMS* lpDispparams,
	VARIANT* pvarResult,
	EXCEPINFO* pexcepinfo,
	unsigned int* puArgErr)
{
	METHOD_PROLOGUE(ABaseEvent, EventHandler)
		ASSERT_VALID(pThis);

	// ����� ���������� ���������
	AFX_EVENT event(AFX_EVENT::event, dispidMember, lpDispparams, pexcepinfo, puArgErr);

	// ����������: OnEvent � ���������� ����������� ���������� FALSE, ��� ���������� ������,
	//						 ���������� ���������� TRUE, ����� �� ����������� �������� ������, �������
	//						 ����� OnEvent �������� �������� GetEventSinkEntry

	BOOL eventHandled = TRUE; // ������� �� ��������������, ���������� ������� TRUE
	if (pThis->GetEventSinkEntry(1, &event) != NULL)
		eventHandled = pThis->OnEvent(1, &event, NULL); // �������� ��������� ������

	if (pvarResult != NULL) {
		VariantClear(pvarResult);											 // ���������

		V_VT(pvarResult) = VT_BOOL;
		V_BOOL(pvarResult) = eventHandled;
	}
	return event.m_hResult;
}

//-------------------------------------------------------------------------------
// QueryInterface
// ---
STDMETHODIMP ABaseEvent::XEventHandler::QueryInterface(REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE(ABaseEvent, EventHandler)

		* ppvObj = NULL;
	if (IID_IUnknown == iid || IID_IDispatch == iid || iid == pThis->m_iidEvents)
		*ppvObj = this;

	if (NULL != *ppvObj)
	{
		((LPUNKNOWN)*ppvObj)->AddRef();
		return NOERROR;
	}

	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

//-------------------------------------------------------------------------------
// Release
// ---
STDMETHODIMP_(ULONG) ABaseEvent::XEventHandler::Release()
{
	METHOD_PROLOGUE(ABaseEvent, EventHandler)
		return (ULONG)pThis->ExternalRelease();
}

/////////////////////////////////////////////////////////////////////////////
//
// ������� ����� ��� ������������ �������
//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
cBaseEvent::cBaseEvent(int ifType, reference p, long objType, reference doc)
	: CCmdTarget(),
	m_dwCookie(0),
	refDoc(doc)
{
	::memset(&m_params, 0, sizeof(NotifyConnectionParam));
	m_params.pContainer = p;
	m_params.ifType = ifType;
	m_params.objType = objType;
	m_EventList.AddTail(this);
}

//-------------------------------------------------------------------------------
// �����������
// ---
cBaseEvent::cBaseEvent(int ifType, LPUNKNOWN iContainer, long objType, LPUNKNOWN iObj, reference doc)
	: CCmdTarget(),
	m_dwCookie(0),
	refDoc(doc)
{
	::memset(&m_params, 0, sizeof(NotifyConnectionParam));
	m_params.ifType = ifType;
	m_params.iContainer = iContainer;
	if (m_params.iContainer)
		m_params.iContainer->AddRef();
	m_params.objType = objType;
	m_params.iObj = iObj;
	if (m_params.iObj)
		m_params.iContainer->AddRef();
	m_EventList.AddTail(this);
}

//-------------------------------------------------------------------------------
// ����������
// ---
cBaseEvent::~cBaseEvent()
{
	RemoveThis();	// ������ ���� �� ������ ������������ �������
}

//-------------------------------------------------------------------------------
// ����������� �� ��������� �������
// ---
bool cBaseEvent::Advise()
{
	ASSERT(!m_dwCookie);
	if (!m_dwCookie && m_params.ifType) {
		if (ReturnResult()) {
#ifdef _DEBUG
			MessageBoxResult();
#endif
			ResultNULL();
		}
		LPUNKNOWN obj = GetUnknown();
		m_dwCookie = ::ksConnectionAdvise(&m_params, obj);
		obj->Release();
		ASSERT(m_dwCookie);
	}
	return !!m_dwCookie;
}

//-----------------------------------------------------------------------------
// ����������
// ---
void cBaseEvent::Unadvise()
{
	if (m_dwCookie)
	{
		VERIFY(::ksConnectionUnadvise(&m_params));
		m_dwCookie = 0;
	}
}

//-----------------------------------------------------------------------------
// ���������� �� �������
// ---
void cBaseEvent::TerminateEvents(long eventType, reference doc, int objType, LPUNKNOWN iObj)
{
	for (int i = m_EventList.GetCount() - 1; i >= 0; i--) {
		cBaseEvent* evnt = (cBaseEvent*)m_EventList.GetAt(m_EventList.FindIndex(i));
		if (evnt && (!eventType || eventType == evnt->GetType())
			&& (!doc || doc == evnt->refDoc)
			&& (objType == -1 || objType == evnt->m_params.objType)
			&& (!iObj || iObj == evnt->m_params.iObj)) {
			evnt->Disconnect();			// � ����������� ����� ������ �� ������ RemoveAt(pos)
		}
	}
}

//-----------------------------------------------------------------------------
// ���������� �� �������
// ---
cBaseEvent* cBaseEvent::FindEvents(long eventType, reference doc, int objType, LPUNKNOWN iObj)
{
	cBaseEvent* res = NULL;
	for (int i = m_EventList.GetCount() - 1; i >= 0; i--) {
		cBaseEvent* evnt = (cBaseEvent*)m_EventList.GetAt(m_EventList.FindIndex(i));
		if ((!eventType || eventType == evnt->GetType())
			&& ((!doc && !evnt->refDoc) || (doc == evnt->refDoc))
			&& ((objType == -1 && !evnt->m_params.objType) || (objType == evnt->m_params.objType))
			&& ((!iObj && !evnt->m_params.iObj) || (iObj == evnt->m_params.iObj)))
		{
			res = evnt;			// � ����������� ����� ������ �� ������ RemoveAt(pos)
			break;
		}
	}
	return res;
}

//-----------------------------------------------------------------------------
// �������� ��� �������
// ---
void cBaseEvent::TerminateEvents(void)
{
	while (!m_EventList.IsEmpty()) {
		cBaseEvent* headEvent = (cBaseEvent*)m_EventList.RemoveHead();
		headEvent->Disconnect();				// � ����������� ����� ������ �� ������ RemoveAt(pos)
	}
}

//-------------------------------------------------------------------------------
// ������� ������
// ---
void cBaseEvent::DestroyList() {
	if (&m_EventList) {
		delete& m_EventList;
	}
}

//-------------------------------------------------------------------------------
// ������� ���� �� ������
// ---
void cBaseEvent::RemoveThis() {
	// ������ ���� �� ������ ������������ �������
	POSITION pos = m_EventList.Find(this);
	if (pos) {
		m_EventList.RemoveAt(pos);
		Unadvise();													 // ��������� �� ��������� �������
	}
}

//-----------------------------------------------------------------------------
// �������
// ---
void cBaseEvent::Clear()
{
	if (m_params.iContainer) {
		m_params.iContainer->Release();
		m_params.iContainer = NULL;
	}

	if (m_params.iObj) {
		m_params.iObj->Release();
		m_params.iObj = NULL;
	}
	::memset(&m_params, 0, sizeof(NotifyConnectionParam));
}

//-----------------------------------------------------------------------------
// ������������
// ---
void cBaseEvent::Disconnect()
{
	Unadvise();
	Clear();
	ExternalRelease();
}