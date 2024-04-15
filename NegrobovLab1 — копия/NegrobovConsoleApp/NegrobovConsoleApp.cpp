#include "stdafx.h"

#include "Message.h"
#include "Session.h"
#include "Writing.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct header
{
	int addr;
	int size;
};

__declspec(dllimport) std::string GetMessageFrom(header& h);

CWinApp theApp;
using namespace std;


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

int main()
{
    HANDLE confirmEvent = ::CreateEvent(NULL, FALSE, FALSE, _T("ConfirmEvent"));
    HANDLE startEvent = ::CreateEvent(NULL, FALSE, FALSE, _T("StartEvent"));
    HANDLE stopEvent = ::CreateEvent(NULL, FALSE, FALSE, _T("CloseProcNegrobov"));
	HANDLE sendEvent = ::CreateEvent(NULL, FALSE, FALSE, _T("SendEvent"));
    HANDLE exitEvent = ::CreateEvent(NULL, FALSE, FALSE, _T("ExitProcNegrobov"));

	HANDLE events[4];
	events[0] = exitEvent;
	events[1] = stopEvent;
	events[2] = startEvent;
	events[3] = sendEvent;

	::SetEvent(confirmEvent);

	std::vector<HANDLE> hThreads;
	std::vector<Session*> sessions;

	while (true)
	{
		int n = ::WaitForMultipleObjects(sizeof(events) / sizeof(HANDLE), events, FALSE, INFINITE) - WAIT_OBJECT_0;
		switch (n)
		{
		case 0:
		{
			for (int i{ 0 }; i < sessions.size(); ++i)
			{
				sessions[i]->addMessage(MT_CLOSE);
				CloseHandle(hThreads[i]);
			}
			return 0;
		}

		case 1:
		{
			if (!hThreads.size())
			{
				::SetEvent(confirmEvent);
				return 0;
			}
			sessions.back()->addMessage(MT_CLOSE);
			sessions.pop_back();

			::CloseHandle(hThreads.back());
			hThreads.pop_back();

			break;

		}

		case 2:
		{
			sessions.push_back(new Session(hThreads.size() + 1));
			hThreads.push_back(::CreateThread(NULL, 0, startInThread, (LPVOID)sessions.back(), 0, NULL));
			break;
		}

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
			std::cin;
			return 0;
		}
		::SetEvent(confirmEvent);
	}

	WaitForMultipleObjects((DWORD)hThreads.size(), hThreads.data(), TRUE, INFINITE);

	CloseHandle(confirmEvent);
	CloseHandle(startEvent);
	CloseHandle(stopEvent);
	CloseHandle(sendEvent);

	return 0;
}
