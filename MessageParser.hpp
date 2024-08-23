#pragma once

#include <string>

class	MessageParser {
	public:
		MessageParser(void);
		~MessageParser(void);

		bool	parseMessage(const std::string::iterator& msg);
};
