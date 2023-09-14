/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для обработчиков событий в автоматизации
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
// Список подписчиков
// ---
CObList& ABaseEvent::m_EventList = *new CObList();
CObList& cBaseEvent::m_EventList = *new CObList();

/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для автоматизационных обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
// Конструктор
// ---
ABaseEvent::ABaseEvent(LPUNKNOWN pObject,
	IID	iidEvents)
	: CCmdTarget(),
	m_dwCookie(0),
	m_pContainer(pObject),						// Источник событий
	m_iidEvents(iidEvents),						// IID
	m_pConnPt(NULL)
{
	if (m_pContainer)
		m_pContainer->AddRef();
	ASSERT(!IsEqualIID(m_iidEvents, GUID_NULL));	// Прислали тип событий
	m_EventList.AddTail(this);
}

//-------------------------------------------------------------------------------
// Деструктор
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
// Подписаться на получение событий
// ---
int ABaseEvent::Advise()
{
	ASSERT(m_dwCookie == 0); // Повторно подписываться нельзя

	// Подписаться на получение событий
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

	// Неудачная подписка на события
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
// Отсоединиться
// ---
void ABaseEvent::Disconnect()
{
	Unadvise();
	Clear();
	ExternalRelease();
}

//-------------------------------------------------------------------------------
// Отписаться от получения событий
// ---
void ABaseEvent::Unadvise()
{
	if (m_pConnPt != NULL)				// Подписка была
	{
		m_pConnPt->Unadvise(m_dwCookie);	// Отписаться от получения событий
		m_pConnPt->Release();				// Освободить
		m_pConnPt = NULL;
	}
	m_dwCookie = 0;
}

//-----------------------------------------------------------------------------
// Отписать все события
// ---
void ABaseEvent::TerminateEvents(void)
{
	while (!m_EventList.IsEmpty()) {
		ABaseEvent* headEvent = (ABaseEvent*)m_EventList.RemoveHead();
		headEvent->Disconnect();
	}
}

//-----------------------------------------------------------------------------
// Отписать все события
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
// Удалить список
// ---
void ABaseEvent::DestroyList() {
	if (&m_EventList) {
		delete& m_EventList;
	}
}

//-------------------------------------------------------------------------------
// Удалить себя из списка
// ---
void ABaseEvent::RemoveThis() {
	// Удалим себя из списка обработчиков событий
	POSITION pos = m_EventList.Find(this);
	if (pos) {
		m_EventList.RemoveAt(pos);
		Unadvise();													 // Отпишемся от получения событий
	}
}

//-------------------------------------------------------------------------------
// Карта интерфейса
// ---
BEGIN_INTERFACE_MAP(ABaseEvent, CCmdTarget)
END_INTERFACE_MAP()

//-------------------------------------------------------------------------------
// Карта сообщений интерфейса
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
// Пересылка событий
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

	// Класс параметров сообщения
	AFX_EVENT event(AFX_EVENT::event, dispidMember, lpDispparams, pexcepinfo, puArgErr);

	// Примечание: OnEvent в отсутствии обработчика возвращает FALSE, для правильной работы,
	//						 необходимо возвращать TRUE, чтобы не блокировать действия Компас, поэтому
	//						 перед OnEvent включена проверка GetEventSinkEntry

	BOOL eventHandled = TRUE; // Событие не обрабатывается, необходимо вернуть TRUE
	if (pThis->GetEventSinkEntry(1, &event) != NULL)
		eventHandled = pThis->OnEvent(1, &event, NULL); // Передача сообщения дальше

	if (pvarResult != NULL) {
		VariantClear(pvarResult);											 // Результат

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
// Базовый класс для обработчиков событий
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
// Конструктор
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
// Деструктор
// ---
cBaseEvent::~cBaseEvent()
{
	RemoveThis();	// Удалим себя из списка обработчиков событий
}

//-------------------------------------------------------------------------------
// Подписаться на получение событий
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
// Отписаться
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
// Отписаться от событий
// ---
void cBaseEvent::TerminateEvents(long eventType, reference doc, int objType, LPUNKNOWN iObj)
{
	for (int i = m_EventList.GetCount() - 1; i >= 0; i--) {
		cBaseEvent* evnt = (cBaseEvent*)m_EventList.GetAt(m_EventList.FindIndex(i));
		if (evnt && (!eventType || eventType == evnt->GetType())
			&& (!doc || doc == evnt->refDoc)
			&& (objType == -1 || objType == evnt->m_params.objType)
			&& (!iObj || iObj == evnt->m_params.iObj)) {
			evnt->Disconnect();			// в деструкторе будет удален из списка RemoveAt(pos)
		}
	}
}

//-----------------------------------------------------------------------------
// Отписаться от событий
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
			res = evnt;			// в деструкторе будет удален из списка RemoveAt(pos)
			break;
		}
	}
	return res;
}

//-----------------------------------------------------------------------------
// Отписать все события
// ---
void cBaseEvent::TerminateEvents(void)
{
	while (!m_EventList.IsEmpty()) {
		cBaseEvent* headEvent = (cBaseEvent*)m_EventList.RemoveHead();
		headEvent->Disconnect();				// в деструкторе будет удален из списка RemoveAt(pos)
	}
}

//-------------------------------------------------------------------------------
// Удалить список
// ---
void cBaseEvent::DestroyList() {
	if (&m_EventList) {
		delete& m_EventList;
	}
}

//-------------------------------------------------------------------------------
// Удалить себя из списка
// ---
void cBaseEvent::RemoveThis() {
	// Удалим себя из списка обработчиков событий
	POSITION pos = m_EventList.Find(this);
	if (pos) {
		m_EventList.RemoveAt(pos);
		Unadvise();													 // Отпишемся от получения событий
	}
}

//-----------------------------------------------------------------------------
// Очистка
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
// Отсоединение
// ---
void cBaseEvent::Disconnect()
{
	Unadvise();
	Clear();
	ExternalRelease();
}