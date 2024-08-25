#pragma once

#include <string>
#include <vector>
#include "Client.hpp"
class MessageParser
{
	public:
		MessageParser(void);
		~MessageParser(void);

	static bool parseMessage(std::stringstream &msg, Client *client);
	static void parseBuffer(const std::string &msg, Client *client);
	static bool parseCommand(std::stringstream& msg, std::vector<std::string> &paramms);
	static bool parseParams(std::stringstream& msg, std::vector<std::string> &paramms);
	// static void execute_command(std::vector<std::string> &msg_tokens);
	// static void Pass_exec(std::vector<std::string> &msg_tokens);
	// static void User_exec(std::vector<std::string> &msg_tokens);
	// static void Nick_exec(std::vector<std::string> &msg_tokens);
};
