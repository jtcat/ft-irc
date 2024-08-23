#include "MessageParser.hpp"
#include <sstream>
#include <vector>


bool	MessageParser::parseParamms(std::stringstream& msg, std::vector<std::string> &paramms) {
	std::string token;
	msg >> token;
	paramms.push_back(paramms);
	while (token != "\r\n")
	{
		if (msg.eof())
			return false;
		paramms.push_back(paramms);
		msg>>token;
	}
	return true;
}

bool	MessageParser::parseCommand(std::stringstream& msg, std::vector<std::string> &paramms)  {
	std::string token;
	msg >> token;
	//parse token -> check if it's a known command;
	paramms.push_back(token);
	return true;
}

bool	MessageParser::parseBuffer(const std::string& msg) {
	std::string::const_iterator it = msg.begin();
	while (it != msg.end())
	{
		parseMessage(it);
	}

}
bool	MessageParser::parseMessage(std::stringstream& msg) {
	std::vector<std::string> paramms;
	if (!parseCommand(msg, paramms) || !parseParamms(msg, paramms))
		return false;
	return true;
}
