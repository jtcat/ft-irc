#include "MessageParser.hpp"
#include <sstream>
#include <vector>
#include <iostream>
void printVectorWithSpaces(const std::vector<std::string>& vec) {
	for (size_t i = 0; i < vec.size(); ++i) {
		std::cout << vec[i];
		if (i != vec.size() - 1) {
			std::cout << "---";
		}
	}
	std::cout << std::endl; // Print a newline at the end
}
void removeUntilCRLF(std::stringstream& msg) {
	char ch;
	while (msg.get(ch)) {
		if (ch == '\r' && msg.peek() == '\n') {
			msg.get(); // Consume '\n'
			break;
		}
	}
}
// NICK dav:
// :irc.local 432 yo dav: :Erroneous Nickname
//-> what to do in this case?
bool	MessageParser::parseParams(std::stringstream& msg, std::vector<std::string> &message_tokens) {
	char ch;
	int include_space = 0;
	std::stringstream ss;
	while (msg.get(ch)) {
		if (ch == '\r' && msg.peek() == '\n') {
			msg.get(); //get \n;
			if (ss.str().length() > 0)
				message_tokens.push_back(ss.str());
			return true;
		}
		if (ch == ':' && ss.str().empty()) // " : " is found
			include_space = 1;
		if (!isspace(ch) || include_space == 1) {
			ss << ch; // append the character into the string
		} else {
			if (ss.str().length() > 0) {
				message_tokens.push_back(ss.str());
				ss.str("");
				ss.clear();
			}
		}
	}
	return false;
}

bool	MessageParser::parseCommand(std::stringstream& msg, std::vector<std::string> &message_tokens)  {
	char ch;
	std::stringstream ss;
	while (msg.get(ch)) {
		if (ch == '\r' && msg.peek() == '\n') {
			msg.unget(); // Put the character back into the stream
			if (ss.str().length() > 0)
				message_tokens.push_back(ss.str());
			return true;
		}
		if (!isspace(ch)) {
			ss << ch; // append the character into the string
		} else {
			if (ss.str().length() > 0) {
				message_tokens.push_back(ss.str());
				return true;
			}
		}
	}
	return false;
}

void	MessageParser::parseBuffer(const std::string& buff) {
	std::stringstream ss_buff(buff);
	while (!ss_buff.eof())
	{
		parseMessage(ss_buff);
	}

}
bool	MessageParser::parseMessage(std::stringstream& msg) {
	std::vector<std::string> message_tokens;
	if (!parseCommand(msg, message_tokens) || !parseParams(msg, message_tokens))
	{
		removeUntilCRLF(msg);
		//print ERR_UNKNOWNERROR (400) -> can specify specific messages
		return false;
	}
	//execute command;
	printVectorWithSpaces(message_tokens);
	// std::cout <<"message size = "<<message_tokens.size() << std::endl;
	return true;
}
//check for nic:k

