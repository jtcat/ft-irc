#include "MessageParser.hpp"

bool	MessageParser::parseSource(const std::string::iterator& msg) {
}

bool	MessageParser::parsePlainCommand(const std::string::iterator& cursor) {
		if (cursor
	return true;
}

bool	MessageParser::parseDigitCommand(const std::string::iterator& cursor) {
	return true;
}

bool	MessageParser::parseCommand(const std::string::iterator& msg)  {
	if (!parsePlainCommand(msg))
		return parseDigitCommand(msg);
	return true;
}

bool	MessageParser::parseMessage(const std::string::iterator& cursor) {
	while ()
	parseCommand(cursor);
	parseParameters(cursor);
	parseMessageEnd(cursor);
}
