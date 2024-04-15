#pragma once 

enum MessageTypes
{
	MT_CLOSE,
	MT_DATA
};


struct MessageHeader
{
	int messageType;
	int size;
};


struct Message
{
	MessageHeader header = { 0 };
	std::string data;

	Message() = default;

	Message(MessageTypes messageType, const std::string& data = "")
		:data{ data }
	{
		header = { messageType, int(data.length()) };
	}
};