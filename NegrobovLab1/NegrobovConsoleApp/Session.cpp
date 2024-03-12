#include "stdafx.h"
#include "Session.h"

void Session::addMessage(Message& m)
{
	EnterCriticalSection(&cs);
	messages.push(m);
	SetEvent(hEvent);
	LeaveCriticalSection(&cs);
}

bool Session::getMessage(Message& m)
{
	bool res = false;
	WaitForSingleObject(hEvent, INFINITE);
	EnterCriticalSection(&cs);
	if (!messages.empty())
	{
		res = true;
		m = messages.front();
		messages.pop();
	}
	if (messages.empty())
	{
		ResetEvent(hEvent);
	}
	LeaveCriticalSection(&cs);
	return res;
}

void Session::addMessage(MessageTypes messageType, const std::string& data)
{
	Message m(messageType, data);
	addMessage(m);
}
