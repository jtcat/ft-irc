#include "MessageParser.hpp"

bool	MessageParser::parseSource(std::string::iterator& msg) {
}

bool	MessageParser::parsePlainCommand(std::string::iterator& cursor) {
		if (cursor
	return true;
}

bool	MessageParser::parseDigitCommand(std::string::iterator& cursor) {
	return true;
}

bool	MessageParser::parseCommand(std::string::iterator& msg)  {
	if (!parsePlainCommand(msg))
		return parseDigitCommand(msg);
	return true;
}

bool	MessageParser::parseMessage(const std::string& msg) {
	std::string::iterator = msg.begin();

	parseCommand(cursor);
	parseParameters(cursor);
	parseMessageEnd(cursor);
}
