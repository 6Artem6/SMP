////////////////////////////////////////////////////////////////////////////////
//
// События документа
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _DOCUMENTEVENT_H
#define _DOCUMENTEVENT_H

#ifndef _ABASEEVENT_H
#include "aBaseEvent.h"
#endif


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



////////////////////////////////////////////////////////////////////////////////
//
// DocumentEvent  - обработчик событий от документа
//
////////////////////////////////////////////////////////////////////////////////
class DocumentEvent : public cBaseEvent
{

public:
    DocumentEvent(reference p);
    virtual ~DocumentEvent();

    // Operations
public:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(DocumentEvent)
    //}}AFX_VIRTUAL
    virtual   LPUNKNOWN GetUnknown();
    // Implementation

    BEGIN_INTERFACE_PART(DocumentFileNotify, IDocumentFileNotify)
        INIT_INTERFACE_PART(DocumentEvent, DocumentFileNotify)
        //Обрабатывается ли библиотекой событие с номером notifyType
        STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(int notifyType);

    //kdBeginCloseDocument  -  Начало закрытия документа
    STDMETHOD_(VARIANT_BOOL, BeginCloseDocument)();

    //kdCloseDocument       -  Документ закрыт
    STDMETHOD_(VARIANT_BOOL, CloseDocument)();

    //kdBeginSaveDocument   -  Начало сохранения документа
    STDMETHOD_(VARIANT_BOOL, BeginSaveDocument)(LPSTR docName);

    //kdSaveDocument        -  Документ сохранен
    STDMETHOD_(VARIANT_BOOL, SaveDocument)();

    //kdActiveDocument        -  Документ активизирован
    STDMETHOD_(VARIANT_BOOL, Activate)();

    //kdDeactiveDocument     -  Документ деактивизирован.
    STDMETHOD_(VARIANT_BOOL, Deactivate)();

    //kdBeginSaveAsDocument  - Запрос имени документа для сохранения.
    STDMETHOD_(VARIANT_BOOL, BeginSaveAsDocument)();

    //kdDocumentFrameOpen    - Окно документа открылось.
    STDMETHOD_(VARIANT_BOOL, DocumentFrameOpen)(LPUNKNOWN v);

    //kdProcessActivate      - Активизация процесса
    STDMETHOD_(VARIANT_BOOL, ProcessActivate)(long Id);

    //kdProcessDeactivate    - Деактивизация процесса
    STDMETHOD_(VARIANT_BOOL, ProcessDeactivate)(long Id);

    STDMETHOD_(VARIANT_BOOL, BeginProcess) (long Id);

    STDMETHOD_(VARIANT_BOOL, EndProcess) (long Id, VARIANT_BOOL Success);

    STDMETHOD_(VARIANT_BOOL, BeginAutoSaveDocument) (LPSTR docName);
    STDMETHOD_(VARIANT_BOOL, AutoSaveDocument) ();

    END_INTERFACE_PART(DocumentFileNotify)

};

class Document2DEvent : public cBaseEvent { // DocumentEvent {
    //	bool                      m_advise3D;     // true - сделан Advise
    //  NotifyConnectionParam     m_params3D;     // параметры подписки
    // Attributes
public:
    Document2DEvent(reference p);
    virtual ~Document2DEvent();

    // Operations
public:

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(DocumentEvent)
    //}}AFX_VIRTUAL
    virtual  LPUNKNOWN GetUnknown();
    BEGIN_INTERFACE_PART(Document2DNotify, IDocument2DNotify)
        INIT_INTERFACE_PART(Document2DEvent, Document2DNotify)

        STDMETHOD_(VARIANT_BOOL, BeginRebuild)();
    STDMETHOD_(VARIANT_BOOL, Rebuild)();
    STDMETHOD_(VARIANT_BOOL, BeginChoiceMaterial)();
    STDMETHOD_(VARIANT_BOOL, ChoiceMaterial)(LPSTR material, double density);
    STDMETHOD_(VARIANT_BOOL, BeginInsertFragment)();
    STDMETHOD_(VARIANT_BOOL, LocalFragmentEdit)(long pDoc, VARIANT_BOOL newFrw);
    //Обрабатывается ли библиотекой событие с номером notifyType
    STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(int notifyType);
    STDMETHOD_(VARIANT_BOOL, BeginChoiceProperty) (long objRef, double propID);
    STDMETHOD_(VARIANT_BOOL, ChoiceProperty) (long objRef, double propID);
    STDMETHOD_(VARIANT_BOOL, BeginDeleteProperty) (long objRef, double propID);
    STDMETHOD_(VARIANT_BOOL, DeleteProperty) (long objRef, double propID);

    END_INTERFACE_PART(Document2DNotify)
};

class Document3DEvent : public cBaseEvent { // DocumentEvent {
public:
    Document3DEvent(reference p, LPUNKNOWN doc = NULL);
    virtual ~Document3DEvent();

    // Operations
public:


    // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(DocumentEvent)
        //}}AFX_VIRTUAL
    virtual  LPUNKNOWN GetUnknown();
    BEGIN_INTERFACE_PART(Document3DNotify, IDocument3DNotify)
        INIT_INTERFACE_PART(Document3DEvent, Document3DNotify)

        STDMETHOD_(VARIANT_BOOL, BeginRebuild)(THIS_);
    STDMETHOD_(VARIANT_BOOL, Rebuild)(THIS_);
    STDMETHOD_(VARIANT_BOOL, BeginChoiceMaterial)(THIS_);
    STDMETHOD_(VARIANT_BOOL, ChoiceMaterial)(THIS_ LPSTR material, double density);
    STDMETHOD_(VARIANT_BOOL, BeginDeleteMaterial)(THIS_);
    STDMETHOD_(VARIANT_BOOL, DeleteMaterial)(THIS_);
    STDMETHOD_(VARIANT_BOOL, BeginChoiceMarking)(THIS_);
    STDMETHOD_(VARIANT_BOOL, ChoiceMarking)(THIS_ LPSTR marking);
    STDMETHOD_(VARIANT_BOOL, BeginSetPartFromFile)(THIS_);
    STDMETHOD_(VARIANT_BOOL, BeginCreatePartFromFile)(THIS_ VARIANT_BOOL typeDoc, IEntity* plane);
    //Обрабатывается ли библиотекой событие с номером notifyType
    STDMETHOD_(VARIANT_BOOL, IsNotifyProcess)(THIS_ int notifyType);
    STDMETHOD_(VARIANT_BOOL, CreateEmbodiment)(THIS_ LPSTR marking);
    STDMETHOD_(VARIANT_BOOL, DeleteEmbodiment)(THIS_ LPSTR marking);
    STDMETHOD_(VARIANT_BOOL, ChangeCurrentEmbodiment)(THIS_ LPSTR marking);
    STDMETHOD_(VARIANT_BOOL, BeginChoiceProperty) (THIS_ IUnknown* obj, double propID);
    STDMETHOD_(VARIANT_BOOL, ChoiceProperty) (THIS_ IUnknown* obj, double propID);
    STDMETHOD_(VARIANT_BOOL, BeginDeleteProperty) (THIS_ IUnknown* obj, double propID);
    STDMETHOD_(VARIANT_BOOL, DeleteProperty) (THIS_ IUnknown* obj, double propID);
    STDMETHOD_(VARIANT_BOOL, BeginRollbackFeatures) (THIS_);
    STDMETHOD_(VARIANT_BOOL, RollbackFeatures)  (THIS_);
    STDMETHOD_(VARIANT_BOOL, BedinLoadCombinationChange) (THIS_ long index);
    STDMETHOD_(VARIANT_BOOL, LoadCombinationChange)     (THIS_ long index);

    END_INTERFACE_PART(Document3DNotify)
};


cBaseEvent* NewDocumentEvent(reference doc);
cBaseEvent* NewDocument2DEvent(reference doc);
cBaseEvent* NewDocument3DEvent(reference doc);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
