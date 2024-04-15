// NegrobovApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "stdafx.h"
#include "framework.h"
#include "NegrobovApp.h"

#include "Message.h"
#include "Session.h"
#include "Writing.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
struct header
{
	int addr;
	int size;
};

__declspec(dllimport) std::string GetMessageFrom(header& h);



DWORD WINAPI startInThread(LPVOID _param)
{
	auto session = static_cast<Session*>(_param);
	SafeWriteLn("session", session->sessionID, "created");
	while (true)
	{
		Message m;
		if (session->getMessage(m))
		{
			switch (m.header.messageType)
			{
			case MT_CLOSE:
			{
				SafeWriteLn("session", session->sessionID, "closed");
				delete session;
				return 0;
			}
			case MT_DATA:
			{
				SafeWriteLn("session", session->sessionID, "data", m.data);
				break;
			}
			}
		}
	}
	return 0;
}
void start()
{
	HANDLE startEvent = ::CreateEvent(NULL, FALSE, FALSE, "StartEvent");
	HANDLE confirmEvent = ::CreateEvent(NULL, FALSE, FALSE, "ConfirmEvent");
	HANDLE stopEvent = ::CreateEvent(NULL, FALSE, FALSE, "CloseProcNegrobov");
	HANDLE sendEvent = ::CreateEvent(NULL, FALSE, FALSE, "SendEvent");
	HANDLE exitEvent = ::CreateEvent(NULL, FALSE, FALSE, "ExitProcNegrobov");

	HANDLE events[4];
	events[0] = startEvent;
	events[1] = stopEvent;
	events[2] = exitEvent;
	events[3] = sendEvent;


	std::vector<HANDLE> hThreads;
	std::vector<Session*> sessions;

	while (true)
	{
		DWORD dwWaitRes = ::WaitForMultipleObjects(sizeof(events) / sizeof(HANDLE), events, FALSE, INFINITE) - WAIT_OBJECT_0;

		switch (dwWaitRes)
		{
		case 0:
			sessions.push_back(new Session(hThreads.size() + 1));

			hThreads.push_back(AfxBeginThread((AFX_THREADPROC)startInThread, (LPVOID)sessions.back(), THREAD_PRIORITY_HIGHEST));
			break;

		case 1:

			if (!hThreads.size())
			{
				::SetEvent(confirmEvent);
				return;
			}
			sessions.back()->addMessage(MT_CLOSE);
			sessions.pop_back();

			::CloseHandle(hThreads.back());
			hThreads.pop_back();

			break;

		case 2:

			for (int i{ 0 }; i < sessions.size(); ++i)
			{
				sessions[i]->addMessage(MT_CLOSE);
				::CloseHandle(hThreads[i]);
			}
			SetEvent(confirmEvent);
			return;

		case 3:
		{

			header h;
			std::string message = GetMessageFrom(h);

			if (h.addr == -1)
			{
				SafeWriteLn("Main received data: ", message);
				std::for_each(sessions.begin(), sessions.end(), [&message](Session* s) {s->addMessage(MT_DATA, message); });
			}
			else if (h.addr == 0)
			{
				SafeWriteLn("Main received data: ", message);
			}
			else
				for (Session* s : sessions)
				{
					if (s->sessionID == h.addr)
					{
						s->addMessage(MT_DATA, message);
						break;
					}
				}
			break;
		}
		default:

			std::cout << "There was an error" << std::endl;
			return;
		}
		::SetEvent(confirmEvent);
	}

	WaitForMultipleObjects((DWORD)hThreads.size(), hThreads.data(), TRUE, INFINITE);

	CloseHandle(confirmEvent);
	CloseHandle(startEvent);
	CloseHandle(stopEvent);
	CloseHandle(sendEvent);
}


int main()
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);

	if (hModule != nullptr)
	{

		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else
		{
			start();
		}
	}
	else
	{
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}

	return nRetCode;
}
