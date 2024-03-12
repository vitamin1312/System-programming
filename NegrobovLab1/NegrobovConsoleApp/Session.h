#pragma once

#include "Message.h"

class Session
{
private:
	std::queue<Message> messages;
	CRITICAL_SECTION cs;
	HANDLE hEvent;
public:
	int sessionID;

	Session(int sessionID)
		: sessionID{ sessionID }
	{
		InitializeCriticalSection(&cs);
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	~Session()
	{
		DeleteCriticalSection(&cs);
		CloseHandle(hEvent);
	}

	void addMessage(Message& m);

	bool getMessage(Message& m);

	void addMessage(MessageTypes messageType, const std::string& data = "");
};
