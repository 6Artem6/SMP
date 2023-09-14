#include "StdAfx.h"
#include <afxdllx.h>
#include "Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// ����������� ���������, ������������ � ������� ������������� DLL
static AFX_EXTENSION_MODULE DLL = { NULL, NULL };
MainParameters* mainParameters = new MainParameters();


/// ����������� ����� �����
/// ������������� � ���������� DLL
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	UNREFERENCED_PARAMETER(lpReserved);
	if (dwReason == DLL_PROCESS_ATTACH) {
		TRACE0("DLL Initializing!");

		if (!AfxInitExtensionModule(DLL, hInstance))
			return 0;

		new CDynLinkLibrary(DLL);
		mainParameters->init(&DLL.hModule);
	}
	else if (dwReason == DLL_PROCESS_DETACH) {
		// ������� �������
		ABaseEvent::TerminateEvents();
		cBaseEvent::TerminateEvents();
		// �������� ������
		ClosePropertyManager(0/* mes*/);
		// �������� ������ �������
		ABaseEvent::DestroyList();
		cBaseEvent::DestroyList();

		mainParameters->application = NULL;
		TRACE0("DLL Terminating!");
		AfxTermExtensionModule(DLL);
	}
	return 1;
}

void GetApplication() {
	if (!(IApplication*)mainParameters->application) {
		CString filename;
		if (::GetModuleFileName(NULL, filename.GetBuffer(255), 255)) {
			filename.ReleaseBuffer(255);
			CString libname;

#ifdef __LIGHT_VERSION__
			libname.LoadString(IDS_API7LT); /// klAPI7.dll
#else
			libname.LoadString(IDS_API7); /// kAPI7.dll
#endif

			CString oldname = filename.Right(filename.GetLength() - (filename.ReverseFind(_T('\\')) + 1));
			filename.Replace(oldname, libname);

			HINSTANCE hAppAuto = LoadLibrary(filename); /// ������������� kAPI7.dll
			if (hAppAuto) {
				/// ��������� �� ������� ������������ ��������� KompasApplication
				typedef LPDISPATCH(WINAPI* FCreateKompasApplication)();

				FCreateKompasApplication pCreateKompasApplication =
					(FCreateKompasApplication)GetProcAddress(hAppAuto, "CreateKompasApplication");
				if (pCreateKompasApplication)
					mainParameters->application = IDispatchPtr(pCreateKompasApplication(), false);
				FreeLibrary(hAppAuto);
			}
		}
	}
}

void CreateAndSubscriptionPropertyManager() {
	if (mainParameters->propertyManager == NULL) {
		NewApplicationEvent();
		mainParameters->initPropertyManager();		
		/// ������������� �� ������� ��������
		new PropertyManagerEvent(mainParameters->propertyManager);
		IPropertyTabPtr tab = mainParameters->propertyManager->PropertyTabs->Add(_T("�������� ���"));
		/// ��������� ���������
		IPropertyControlsPtr collection = tab->PropertyControls;
		reference docRef = ::ksGetCurrentDocument(0);
		AdviseDoc(docRef);
		mainParameters->initModel(collection);
	}
	else {
		/// ���� ������ ���� ������ �������������, �� �������� ����������� �
		mainParameters->propertyManager->Visible = true;
		if (!mainParameters->isMenuTypeNone()) {
			LibMessage(_T("������ ��� ���������"));
		}
	}
}

void ClosePropertyManager(int mes) {
	if (mainParameters->propertyManager) {
		// mainParameters->getGabaritRect();
		mainParameters->gLayout = mainParameters->propertyManager->GetLayout();
		ABaseEvent::TerminateEvents(DIID_ksPropertyManagerNotify);
		mainParameters->propertyManager->HideTabs();
		mainParameters->propertyManager = NULL;
	}
	else {
		if (mes == 1) {
			LibMessage(_T("������ ��� ���������"));
		}
	}
}

void UpdateSlideBox() {
	if (mainParameters->slideBox)
		mainParameters->slideBox->UpdateParam();
}

void UpdateSlideBox(reference docRef) {
	if (mainParameters->slideBox) {
		mainParameters->slideBox->DrawingSlide = docRef;
		mainParameters->slideBox->Value = (docRef != 0);
		mainParameters->slideBox->CheckBoxVisibility = (docRef != 0);
		mainParameters->slideBox->UpdateParam();
	}
}

void AdviseDoc(long pDoc) {
	if (!pDoc)
		pDoc = ::ksGetCurrentDocument(0);
	else
		::SetObjParam(pDoc, NULL, 0, DOCUMENT_STATE);

	if (pDoc) {
		int docType = ::ksGetDocumentType(pDoc);
		if (NewDocumentEvent(pDoc)) {
			switch (docType) {
			case lt_DocSheetStandart:
			case lt_DocSheetUser:
				NewStampEvent(pDoc);
			case lt_DocFragment:
				NewDocument2DEvent(pDoc);
				NewObj2DEvent(pDoc, ALL_OBJ);
				break;
			case lt_DocPart3D:
			case lt_DocAssemble3D:
				NewDocument3DEvent(pDoc);
				NewObj3DEvent(pDoc, Obj3dType::o3d_unknown, NULL); /// �� ���
				break;

			case lt_DocSpc:
			case lt_DocSpcUser:
				NewSpcDocEvent(pDoc);
				NewStampEvent(pDoc);
				break;
			case lt_DocTxtStandart:
			case lt_DocTxtUser:
				NewStampEvent(pDoc);
				break;
			}

			switch (docType) {
			case lt_DocSheetStandart:
			case lt_DocSheetUser:
			case lt_DocSpc:
			case lt_DocSpcUser:
			case lt_DocAssemble3D:
				NewSpecificationEvent(pDoc);
				NewSpcObjectEvent(pDoc, SPC_BASE_OBJECT); /// �� ������� �������
				break;
			}
		}
	}
}

int WINAPI LibToolBarId(int barType, int index) {
	if (!barType)
		return !index ? 2000 : -1;
	return -1;
}

TCHAR* LoadStr(int ID) {
	static TCHAR buf[255];
	ksConvertLangStrExT(DLL.hModule, ID, buf, 255);
	return buf;
}

int LibMessage(LPCTSTR str, int flags) {
	int res = 0;

	if (str && str[0]) {						/// ������ ��������
		int enable = ::IsEnableTaskAccess();	/// �������� �������
		if (enable)								/// ���� ������ � ������ ��������
			::EnableTaskAccess(0);		/// ��������� ������

		/// ����� ��������� ��������� ���������
		res = MessageBox((HWND)GetHWindow(), str, LoadStr(IDR_LIB), flags);

		if (enable)								/// ���� ������ � ������ ��� ��������
			::EnableTaskAccess(1);		/// ��������� ������ � ������
	}

	return res;
}

int LibMessageErrorCreateModel(LPWSTR reason) {
	LPWSTR str_begin = _T("�� ������ ��������� ������.\n" "�������: ");
	wstring w_begin(str_begin);
	wstring w_reason(reason);
	wstring w_all = w_begin + w_reason;
	return LibMessage(w_all.c_str(), MB_OK);
}

int LibMessage(int strId, int flags) {
	return ::LibMessage(LoadStr(strId), flags);
}

int WINAPI LIBRARYID() {
	return IDR_LIB;
}

int WINAPI LibIsOnApplication7() {
	return 1;
}

void WINAPI LIBRARYENTRY(unsigned int comm) {
	// ������ ��������� ������������
	::GetApplication(); // ������ ���������� ���������� ������
	if ((IApplication*)mainParameters->application) {
		switch (comm)
		{
		case 1: // ������� �������� � �����������
			mainParameters->setMenuTypeProfile();
			ClosePropertyManager(0);
			CreateAndSubscriptionPropertyManager();
			break;
		case 2: // ������� �������� � �����������
			mainParameters->setMenuTypeConditions();
			ClosePropertyManager(0);
			CreateAndSubscriptionPropertyManager();
			break;
		case 3: // ����������
			ClosePropertyManager(1);
			// ���������� ��������� ������ � ����� ��
			break;
		}
	}
}

int WINAPI LibInterfaceNotifyEntry(IDispatch* application) {
	mainParameters->application = application;
	// mainParameters->CreateAndSubscriptionPropertyManager( 0/*mes*/);	// �������� ������ ����������
	return 0;
}
