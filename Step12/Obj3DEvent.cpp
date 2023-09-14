////////////////////////////////////////////////////////////////////////////////
//
// События 3D объекта
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#ifndef _OBJ3DEVENT_H
#include "Obj3DEvent.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern AFX_MODULE_STATE* pModuleState;

extern cBaseEvent* NewDocumentEvent(reference doc);
extern void UpdateSlideBox();

//-------------------------------------------------------------------------------
// Найти контейнер для 3D объекта
// ---
LPUNKNOWN GetContainer(reference doc, int objType, LPUNKNOWN iObj) {
	IUnknownPtr container(NULL);
	if (iObj) {
		switch (objType) {
		case 0: { // тип не известен
			IPartPtr iPart(iObj);
			if (iPart)
				container = iPart->GetPart(pTop_Part);
			else {
				IEntityPtr iEntity(iObj);
				if (iEntity)
					container = iEntity->GetParent();
			}
			break;
		}

		case o3d_part: {
			IPartPtr iPart(iObj);
			if (iPart)
				container = iPart->GetPart(pTop_Part);
			break;
		}

		case o3d_feature: {
			IFeaturePtr iFeature(iObj);
			if (iFeature) {
				IUnknownPtr obj(iFeature->GetObject(), false/*AddRef*/);
				if (obj)
					container = GetContainer(0, 0, obj);
			}
			break;
		}

		default: {
			IEntityPtr iEntity(iObj);
			if (iEntity)
				container = iEntity->GetParent();
		}
		}
	}
	if (container == NULL) {
		if (doc) {
			IDocument3DPtr document3D(ksGet3dDocumentFromReference(doc), false/*AddRef*/);
			if (document3D)
				container = document3D->GetPart(pTop_Part);
		}
	}
	return container;
}

//-------------------------------------------------------------------------------
// Создать обработчик событий 3D документа
// ---
cBaseEvent* NewObj3DEvent(reference doc, int objType, LPUNKNOWN iObj) {
	Obj3DEvent* res = NULL;
	if (doc) {
		int typeObj = iObj ? 0 : objType;
		if (!cBaseEvent::FindEvents(ntObject3DNotify, doc, typeObj, iObj)) {
			LPUNKNOWN container = GetContainer(doc, objType, iObj);
			if (container) {
				NewDocumentEvent(doc); // чтобы при закрытии документа отписаться
				res = new Obj3DEvent(container, typeObj, iObj, doc);
				if (!res->Advise()) {
					delete res;
					res = NULL;
				}
				container->Release();
			}
		}
	}
	return res;
}

////////////////////////////////////////////////////////////////////////////////
//
// Obj3DEvent	- обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
//
// ---
Obj3DEvent::Obj3DEvent(LPUNKNOWN iContainer, long objType, LPUNKNOWN iObj, reference doc) :
	cBaseEvent(ntObject3DNotify, iContainer, objType, iObj, doc)
{
}

//-------------------------------------------------------------------------------
//
// ---
Obj3DEvent::~Obj3DEvent()
{
}

//-----------------------------------------------------------------------------
//
// ---
LPUNKNOWN Obj3DEvent::GetUnknown() {
	m_xObject3DNotify.AddRef();
	return &m_xObject3DNotify;
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) Obj3DEvent::XObject3DNotify::Release()
{
	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
		return (ULONG)pThis->InternalRelease();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP_(ULONG) Obj3DEvent::XObject3DNotify::AddRef()
{
	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
		return (ULONG)pThis->InternalAddRef();
}

//-----------------------------------------------------------------------------
//
// ---
STDMETHODIMP Obj3DEvent::XObject3DNotify::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)

		ASSERT(AfxIsValidAddress(ppvObj, sizeof(LPVOID), FALSE));

	if (IsEqualIID(iid, IID_IUnknown) ||
		IsEqualIID(iid, IID_IObject3DNotify))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

//-----------------------------------------------------------------------------
//Обрабатывается ли библиотекой событие с номером notifyType
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::IsNotifyProcess(int notifyType)
{
	//	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	return	notifyType >= o3BeginDelete && notifyType <= o3UpdateObject;
}

/////////////////////////////////////////////////////////////////////////////
// Obj3DEvent message handlers
//-------------------------------------------------------------------------------
// Начало удаления объекта
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::BeginDelete(LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// Oбъекты удалены
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::Delete(LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Oбъект исключен/включен в расчет
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::Excluded(LPUNKNOWN obj, VARIANT_BOOL excluded) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Oбъект скрыт/показан
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::Hidden(LPUNKNOWN obj, VARIANT_BOOL _hidden) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Начало изменения свойств объета
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::BeginPropertyChanged(LPUNKNOWN obj) {
	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
		return TRUE;
}

//-------------------------------------------------------------------------------
// Изменены свойства объета
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::PropertyChanged(LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Начало изменения положения объета
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::BeginPlacementChanged(LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// Изменено положения объета
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::PlacementChanged(LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Начало редактирования\создания объекта.false - запрещает процесс.
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::BeginProcess(long pType, LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// Конец редактирования\создания объекта
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::EndProcess(long pType) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Создание объекта.
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::CreateObject(LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Редактирование объекта.
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::UpdateObject(LPUNKNOWN obj) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}

//-------------------------------------------------------------------------------
// Редактирование объекта.
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::BeginLoadStateChange(LPUNKNOWN obj, long loadState) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	return TRUE;
}

//-------------------------------------------------------------------------------
// Редактирование объекта.
// ---
STDMETHODIMP_(VARIANT_BOOL) Obj3DEvent::XObject3DNotify::LoadStateChange(LPUNKNOWN obj, long loadState) {
	// 	METHOD_PROLOGUE_EX_(Obj3DEvent, Object3DNotify)
	UpdateSlideBox();
	return TRUE;
}