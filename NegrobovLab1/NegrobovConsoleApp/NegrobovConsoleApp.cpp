#include "stdafx.h"

#include "Message.h"
#include "Session.h"
#include "Writing.h"


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
				Sleep(1000 * session->sessionID);
				break;
			}
			}
		}
	}
	return 0;
}


int main()
{
    HANDLE confirmEvent = ::CreateEvent(NULL, FALSE, FALSE, "ConfirmEvent");
    HANDLE startEvent = ::CreateEvent(NULL, FALSE, FALSE, "StartEvent");
    HANDLE stopEvent = ::CreateEvent(NULL, FALSE, FALSE, "CloseProcNegrobov");
    HANDLE exitEvent = ::CreateEvent(NULL, FALSE, FALSE, "ExitProcNegrobov");

	HANDLE events[3];
	events[0] = exitEvent;
	events[1] = stopEvent;
	events[2] = startEvent;

	::SetEvent(confirmEvent);

	std::vector<HANDLE> hThreads;
	std::vector<Session*> sessions;

	while (true)
	{
		int n = ::WaitForMultipleObjects(sizeof(events) / sizeof(HANDLE), events, FALSE, INFINITE) - WAIT_OBJECT_0;
		switch (n)
		{
		case 0:
			for (int i{ 0 }; i < sessions.size(); ++i)
			{
				sessions[i]->addMessage(MT_CLOSE);
				CloseHandle(hThreads[i]);
			}
			return 0;
		case 1:

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

		case 2:

			sessions.push_back(new Session(hThreads.size() + 1));
			hThreads.push_back(::CreateThread(NULL, 0, startInThread, (LPVOID)sessions.back(), 0, NULL));
			break;

		default:

			std::cout << "There was an error" << std::endl;
			return 0;
		}
		::SetEvent(confirmEvent);
	}

	WaitForMultipleObjects((DWORD)hThreads.size(), hThreads.data(), TRUE, INFINITE);

	CloseHandle(confirmEvent);
	CloseHandle(startEvent);
	CloseHandle(stopEvent);

	return 0;
}
