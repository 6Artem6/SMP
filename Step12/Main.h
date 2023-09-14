#ifndef _MAIN_H
#define _MAIN_H

#include <string>
using namespace std;

#include "Resource.h"

#ifndef	_MAIN_PARAMETERS_H
#include "MainParameters.h"
#endif

#ifndef	_CPROPMEN_H
#include "cPropMen.h"
#endif

#if _MSC_VER > 1000
#pragma once
#endif
#endif

// Функции создания подписок на события Компаса
extern cBaseEvent* NewApplicationEvent();
extern cBaseEvent* NewDocumentEvent(reference doc);
extern cBaseEvent* NewDocument2DEvent(reference doc);
extern cBaseEvent* NewDocument3DEvent(reference doc);
extern cBaseEvent* NewObj2DEvent(reference doc, int objType);
extern cBaseEvent* NewObj3DEvent(reference doc, int objType, LPUNKNOWN iObj = NULL);
extern cBaseEvent* NewSpcDocEvent(reference doc);
extern cBaseEvent* NewSpcObjectEvent(reference doc, int objType);
extern cBaseEvent* NewSpecificationEvent(reference doc);
extern cBaseEvent* NewStampEvent(reference doc);

/// <summary>
/// Получить доступ к новому API
/// </summary>
void GetApplication();
/// <summary>
/// Создание пользовательской панели и подписка
/// </summary>
void CreateAndSubscriptionPropertyManager();
/// <summary>
/// Остановить работу с панелью свойств и отписаться
/// </summary>
/// <param name="mes"></param>
void ClosePropertyManager(int mes);
/// <summary>
/// Обновление окна
/// </summary>
void UpdateSlideBox();
/// <summary>
/// Обновление окна
/// </summary>
void UpdateSlideBox(reference docRef);
/// <summary>
/// Подписка на события документа
/// </summary>
/// <param name="pDoc">Указатель на документ</param>
void AdviseDoc(long pDoc);
/// <summary>
/// Определить имя библиотеки
/// </summary>
/// <returns></returns>
int WINAPI LIBRARYID();
/// <summary>
/// Использовать новое API
/// </summary>
/// <returns>1</returns>
int WINAPI LibIsOnApplication7();
/// <summary>
/// Головная функция библиотеки
/// </summary>
/// <param name="comm"></param>
/// <returns></returns>
void WINAPI LIBRARYENTRY(unsigned int comm);
/// <summary>
/// Провести подписку библиотеки на сообщения системы
/// </summary>
/// <param name="application">Интерфейс Компаса</param>
/// <returns>0</returns>
int WINAPI LibInterfaceNotifyEntry(IDispatch* application);
/// <summary>
/// Получить идентификаторы инструментальных и компактных панелей
/// </summary>
/// <param name="barType">Тип запрашиваемой панелей 
/// ( 0 - компактная панель, 1 - простая инструментальная панель )</param>
/// <param name="index">Индекс панели</param>
int WINAPI LibToolBarId(int barType, int index);
/// <summary>
/// Загрузить строку из ресурсов
/// </summary>
/// <param name="ID"></param>
/// <returns></returns>
TCHAR* LoadStr(int ID);
/// <summary>
/// Вывод сообщения
/// </summary>
/// <param name="str"></param>
/// <param name="flags"></param>
/// <returns></returns>
int LibMessage(LPCTSTR str, int flags = MB_OK);
/// <summary>
/// Вывод сообщения
/// </summary>
/// <param name="strId"></param>
/// <param name="flags"></param>
/// <returns></returns>
int LibMessage(int strId, int flags = MB_OK);
/// <summary>
/// Вывод сообщения об ошибке построения модели
/// </summary>
/// <param name="reason"></param>
/// <returns></returns>
int LibMessageErrorCreateModel(LPWSTR reason);
