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
// Obj2DEvent	- обрабокчик событий от документа
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

		//koChangeActive		// Переключение активности объекта( вид, слой)
		STDMETHOD_(VARIANT_BOOL, ChangeActive)(THIS_ long objRef);

	//koBeginDelete		 // Начало удаления сдвига, false - запрещает удаление
	STDMETHOD_(VARIANT_BOOL, BeginDelete)(THIS_ long objRef);

	//koDelete					// Удаление объекта
	STDMETHOD_(VARIANT_BOOL, Delete)(THIS_ long objRef);

	//koBeginMove			 // Начало сдвига объекта, false - запрещает сдвиг
	STDMETHOD_(VARIANT_BOOL, BeginMove)(THIS_ long objRef);

	//koMove						// Сдвиг объекта
	STDMETHOD_(VARIANT_BOOL, Move)(THIS_ long objRef);

	//koBeginRotate		 // Начало поворота объекта, false - запрещает поворот
	STDMETHOD_(VARIANT_BOOL, BeginRotate)(THIS_ long obgRef);

	//koRotate					// Поворот объекта
	STDMETHOD_(VARIANT_BOOL, Rotate)(THIS_ long objRef);

	//koBeginScale		 // Начало масштабирования объекта, false - запрещает поворот
	STDMETHOD_(VARIANT_BOOL, BeginScale)(THIS_ long obgRef);

	//koScale					// Масштабирование объекта
	STDMETHOD_(VARIANT_BOOL, Scale)(THIS_ long objRef);

	//koBeginTransform	// Начало трансформации объекта, false - запрещает трансформацию
	STDMETHOD_(VARIANT_BOOL, BeginTransform)(THIS_ long objRef);

	//koTransform			 // Трансформация объекта
	STDMETHOD_(VARIANT_BOOL, Transform)(THIS_ long objRef);

	//koBeginCopy			 // Начало копирования объекта, false - запрещает копирование
	STDMETHOD_(VARIANT_BOOL, BeginCopy)(THIS_ long objRef);

	//koCopy						// Копирование объекта
	STDMETHOD_(VARIANT_BOOL, Copy)(THIS_ long objRef);

	//koBeginSymmetry	 // Начало симметрии	объекта, false - запрещает симметрию
	STDMETHOD_(VARIANT_BOOL, BeginSymmetry)(THIS_ long objRef);

	//koSymmetry				// Симметрия	объекта
	STDMETHOD_(VARIANT_BOOL, Symmetry)(THIS_ long objRef);

	// Начало редактирования\создания объекта.false - запрещает процесс.
	STDMETHOD_(VARIANT_BOOL, BeginProcess)(THIS_ long pType, long obj);

	// Конец редактирования\создания объекта
	STDMETHOD_(VARIANT_BOOL, EndProcess)(THIS_ long pType);

	// Создание объекта.
	STDMETHOD_(VARIANT_BOOL, CreateObject)(THIS_ long obj);

	// Редактирование объекта.
	STDMETHOD_(VARIANT_BOOL, UpdateObject)(THIS_ long obj);

	// Создание объекта.
	STDMETHOD_(VARIANT_BOOL, BeginDestroyObject)(THIS_ long obj);

	// Редактирование объекта.
	STDMETHOD_(VARIANT_BOOL, DestroyObject)(THIS_ long obj);

	//Обрабатывается ли библиотекой событие с номером notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);

	STDMETHOD_(VARIANT_BOOL, BeginPropertyChanged) (THIS_ long objRef);

	STDMETHOD_(VARIANT_BOOL, PropertyChanged) (THIS_ long objRef);

	END_INTERFACE_PART(Object2DNotify)
};

cBaseEvent* NewObj2DEvent(reference doc, int objType);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
