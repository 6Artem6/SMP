////////////////////////////////////////////////////////////////////////////////
//
// События приложения
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

/// ApplicationEvent	- обработчик событий от приложения
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
		/// koCreateDocument - Документ создан
		STDMETHOD_(VARIANT_BOOL, CreateDocument)(THIS_ long pDoc, int docType);

	/// koOpenDocumenBegin - Начало открытия документа
	STDMETHOD_(VARIANT_BOOL, BeginOpenDocument)(THIS_ LPSTR docName);

	/// koOpenDocumen - Документ открыт
	STDMETHOD_(VARIANT_BOOL, OpenDocument)(THIS_ long pDoc, int docType);

	/// koActiveDocumen - Переключение на другой активный документ
	STDMETHOD_(VARIANT_BOOL, ChangeActiveDocument)(THIS_ long pDoc, int docType);

	/// koApplicatinDestroy -  Закрытие приложения
	STDMETHOD_(VARIANT_BOOL, ApplicationDestroy)(THIS_);

	/// koBeginCreate - Процесс создания файла
	STDMETHOD_(VARIANT_BOOL, BeginCreate)(THIS_ long type);

	/// koBeginOpenFile - Процесс открытия файла
	STDMETHOD_(VARIANT_BOOL, BeginOpenFile)(THIS_);

	/// koBeginCloseAllDocument - Закрытие всех файлов
	STDMETHOD_(VARIANT_BOOL, BeginCloseAllDocument)(THIS_);

	/// Обрабатывается ли библиотекой событие с номером notifyType
	STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);

	/// koKeyDown - событие клавиатуры. ( Нажимается клавиша )
	STDMETHOD_(VARIANT_BOOL, KeyDown)(THIS_ long* key, long flags, VARIANT_BOOL sysKey);
	/// koKeyUp - событие клавиатуры. ( Отпускается клавиша )
	STDMETHOD_(VARIANT_BOOL, KeyUp)(THIS_ long* key, long flags, VARIANT_BOOL sysKey);
	/// koKeyUp - событие клавиатуры. ( событие одиночного нажатия символьной клавиши )
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
