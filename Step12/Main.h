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

// ������� �������� �������� �� ������� �������
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
/// �������� ������ � ������ API
/// </summary>
void GetApplication();
/// <summary>
/// �������� ���������������� ������ � ��������
/// </summary>
void CreateAndSubscriptionPropertyManager();
/// <summary>
/// ���������� ������ � ������� ������� � ����������
/// </summary>
/// <param name="mes"></param>
void ClosePropertyManager(int mes);
/// <summary>
/// ���������� ����
/// </summary>
void UpdateSlideBox();
/// <summary>
/// ���������� ����
/// </summary>
void UpdateSlideBox(reference docRef);
/// <summary>
/// �������� �� ������� ���������
/// </summary>
/// <param name="pDoc">��������� �� ��������</param>
void AdviseDoc(long pDoc);
/// <summary>
/// ���������� ��� ����������
/// </summary>
/// <returns></returns>
int WINAPI LIBRARYID();
/// <summary>
/// ������������ ����� API
/// </summary>
/// <returns>1</returns>
int WINAPI LibIsOnApplication7();
/// <summary>
/// �������� ������� ����������
/// </summary>
/// <param name="comm"></param>
/// <returns></returns>
void WINAPI LIBRARYENTRY(unsigned int comm);
/// <summary>
/// �������� �������� ���������� �� ��������� �������
/// </summary>
/// <param name="application">��������� �������</param>
/// <returns>0</returns>
int WINAPI LibInterfaceNotifyEntry(IDispatch* application);
/// <summary>
/// �������� �������������� ���������������� � ���������� �������
/// </summary>
/// <param name="barType">��� ������������� ������� 
/// ( 0 - ���������� ������, 1 - ������� ���������������� ������ )</param>
/// <param name="index">������ ������</param>
int WINAPI LibToolBarId(int barType, int index);
/// <summary>
/// ��������� ������ �� ��������
/// </summary>
/// <param name="ID"></param>
/// <returns></returns>
TCHAR* LoadStr(int ID);
/// <summary>
/// ����� ���������
/// </summary>
/// <param name="str"></param>
/// <param name="flags"></param>
/// <returns></returns>
int LibMessage(LPCTSTR str, int flags = MB_OK);
/// <summary>
/// ����� ���������
/// </summary>
/// <param name="strId"></param>
/// <param name="flags"></param>
/// <returns></returns>
int LibMessage(int strId, int flags = MB_OK);
/// <summary>
/// ����� ��������� �� ������ ���������� ������
/// </summary>
/// <param name="reason"></param>
/// <returns></returns>
int LibMessageErrorCreateModel(LPWSTR reason);
