// TransportDLL.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "framework.h"
#include "TransportLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CTransportDLLApp

BEGIN_MESSAGE_MAP(CTransportLibApp, CWinApp)
END_MESSAGE_MAP()


// CTransportDLLApp construction

CTransportLibApp::CTransportLibApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTransportDLLApp object

CTransportLibApp theApp;


// CTransportDLLApp initialization

BOOL CTransportLibApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


struct header
{
	int addr;
	int size;
};

HANDLE g_mutex = CreateMutex(NULL, FALSE, NULL);


HANDLE startEvent = ::CreateEvent(NULL, FALSE, FALSE, "StartEvent");
HANDLE confirmEvent = ::CreateEvent(NULL, FALSE, FALSE, "ConfirmEvent");
HANDLE stopEvent = ::CreateEvent(NULL, FALSE, FALSE, "CloseProcNegrobov");
HANDLE sendEvent = ::CreateEvent(NULL, FALSE, FALSE, "SendEvent");
HANDLE exitEvent = ::CreateEvent(NULL, FALSE, FALSE, "ExitProcNegrobov");





extern "C"	__declspec(dllexport) void StartThread()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	::SetEvent(startEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
}

extern "C" __declspec(dllexport) void StopThread()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	::SetEvent(stopEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
}


extern "C" __declspec(dllexport) void StopAllThreads()
{
	::SetEvent(exitEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
}




extern "C" __declspec(dllexport) void SendMessageTo(int addr, const char* str)
{

	::WaitForSingleObject(g_mutex, INFINITE);

	header h = { addr, strlen(str) + 1 };
	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, h.size + sizeof(header), "MyMap");
	char* buff = (char*)MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, h.size + sizeof(header));

	memcpy(buff, &h, sizeof(header));
	memcpy(buff + sizeof(header), str, h.size);

	::ReleaseMutex(g_mutex);

	UnmapViewOfFile(buff);


	::SetEvent(sendEvent);
	::WaitForSingleObject(confirmEvent, INFINITE);
	CloseHandle(hFileMap);
}

__declspec(dllexport) std::string GetMessageFrom(header& h)
{
	::WaitForSingleObject(g_mutex, INFINITE);

	HANDLE hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(header), "MyMap");
	LPVOID buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(header));
	h = *((header*)buff);
	UnmapViewOfFile(buff);
	CloseHandle(hFileMap);

	int n = h.size + sizeof(header);
	hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, n, "MyMap");
	buff = MapViewOfFile(hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, n);

	std::string s((char*)buff + sizeof(header), h.size);

	ReleaseMutex(g_mutex);

	UnmapViewOfFile(buff);
	CloseHandle(hFileMap);
	return s;
}