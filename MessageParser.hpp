#pragma once

#include <string>
#include <vector>
class MessageParser
{
public:
	MessageParser(void);
	~MessageParser(void);

	static bool parseMessage(std::stringstream &msg);
	static void parseBuffer(const std::string &msg);
	static bool parseCommand(std::stringstream& msg, std::vector<std::string> &paramms);
	static bool parseParams(std::stringstream& msg, std::vector<std::string> &paramms);
};
