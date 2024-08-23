#pragma once

#include <string>

class MessageParser
{
public:
	MessageParser(void);
	~MessageParser(void);

	bool parseMessage(std::stringstream &msg);
	bool parseBuffer(const std::string &msg);
	bool parseParamms(std::stringstream &msg);
	bool parseCommand(std::stringstream &msg);
};
