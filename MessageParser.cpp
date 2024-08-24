#include "MessageParser.hpp"
#include <sstream>
#include <vector>
#include <iostream>
void printVectorWithSpaces(const std::vector<std::string>& vec) {
	for (size_t i = 0; i < vec.size(); ++i) {
		std::cout << vec[i];
		if (i != vec.size() - 1) {
			std::cout << " ";
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
void skipWhiteSpaces(std::stringstream& msg) {
	char ch;
	while (msg.get(ch)) {
		if (ch == '\r' && msg.peek() == '\n') {
			msg.unget(); // Put the character back into the stream
			break;
		}
		if (!isspace(ch)) {
			msg.unget(); // Put the character back into the stream
			break;
		}
	}
}
bool	MessageParser::parseParams(std::stringstream& msg, std::vector<std::string> &message_tokens) {
	std::string token;

	while (msg >> token) {
		if (token == "\r\n") {
			std::cout << "ParseParams returning true" << std::endl;
			return true;
		}
		std::cout <<"token in params = " <<token<<std::endl;
		message_tokens.push_back(token);
	}
	std::cout << "last token = " << token << std::endl;
	std::cout << msg.rdstate() << std::endl;
	return false;
}

bool	MessageParser::parseCommand(std::stringstream& msg, std::vector<std::string> &message_tokens)  {
	std::string token;
	if (msg >> token)
	{
		std::cout << "token = " << token<< std::endl;
		message_tokens.push_back(token);
		std::cout << "ParseCommands returning true" << std::endl;
		return true;
	}
	return false;
	//parse token -> check if it's a known command and if syntax is valid;
	//otherwise ERR_UNKNOWNCOMMAND (421)
}

void	MessageParser::parseBuffer(const std::string& buff) {
	std::stringstream ss_buff(buff);
	std::cout <<"Buffer stream " <<ss_buff.str() << std::endl;
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
	std::cout << "parse message returning true" << std::endl;
	printVectorWithSpaces(message_tokens);
	std::cout << std::endl;
	return true;
}

