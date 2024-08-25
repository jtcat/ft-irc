#include "MessageParser.hpp"
#include <sstream>
#include <vector>
#include <iostream>

// static void Pass_exec(std::vector<std::string> &msg_tokens) {

// };
// static void User_exec(std::vector<std::string> &msg_tokens) {

// };
// static void Nick_exec(std::vector<std::string> &msg_tokens) {

// };
// void MessageParser::execute_command(std::vector<std::string> &msg_tokens) {

// 	if (msg_tokens[0] == "PASS")
// 		//exec pass
// 	else if (msg_tokens[0] == "USER")
// 		//exec user
// 	else if (msg_tokens[0] == "NICK")
// 		//exec nick
// 	else
// 		std::cout
// 		std::cout << ":ft_irc 451 * :Connection not registered" << std::endl;
// }

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
bool	MessageParser::parseParams(std::stringstream& msg, std::vector<std::string> &msg_tokens) {
	char ch;
	int include_space = 0;
	std::stringstream ss;
	while (msg.get(ch)) {
		if (ch == '\r' && msg.peek() == '\n') {
			msg.get(); //get \n;
			if (ss.str().length() > 0)
				msg_tokens.push_back(ss.str());
			return true;
		}
		//should I store : together with the rest of the arguments?
		if (ch == ':' && ss.str().empty()) // " : " is found
			include_space = 1;
		else if (!isspace(ch) || include_space == 1) {
			ss << ch; // append the character into the string
		} else {
			if (ss.str().length() > 0) {
				msg_tokens.push_back(ss.str());
				ss.str("");
				ss.clear();
			}
		}
	}
	return false;
}

bool	MessageParser::parseCommand(std::stringstream& msg, std::vector<std::string> &msg_tokens)  {
	char ch;
	std::stringstream ss;
	while (msg.get(ch)) {
		if (ch == '\r' && msg.peek() == '\n') {
			msg.unget(); // Put the character back into the stream
			if (ss.str().length() > 0)
				msg_tokens.push_back(ss.str());
			return true;
		}
		if (!isspace(ch)) {
			ss << ch; // append the character into the string
		} else {
			if (ss.str().length() > 0) {
				msg_tokens.push_back(ss.str());
				return true;
			}
		}
	}
	return false;
}

void	MessageParser::parseBuffer(const std::string& buff, Client *client)  {
	std::stringstream ss_buff(buff);
	(void)client;
	while (!ss_buff.eof())
	{
		parseMessage(ss_buff, client);
	}
}
bool	MessageParser::parseMessage(std::stringstream& msg, Client *client) {
	std::vector<std::string> msg_tokens;
	(void)client;
	if (!parseCommand(msg, msg_tokens) || !parseParams(msg, msg_tokens))
	{
		removeUntilCRLF(msg);
		//print ERR_UNKNOWNERROR (400) -> can specify specific messages
		return false;
	}

	//execute command;
	printVectorWithSpaces(msg_tokens);
	std::cout << "message size = " <<msg_tokens.size() << std::endl;
	return true;
}

//check for nic:k
//PASS must be the first command or -> ERR_NOTREGISTERED (451)
//THEN NICK and USER or vice-versa
//THEN WELCOME MESSAGE
