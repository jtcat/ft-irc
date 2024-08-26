#pragma once

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include "Server.hpp"
class MessageParser
{
public:
	MessageParser(void);
	~MessageParser(void);
	// static void setPasswd(const std::string & pass);
	static bool parseMessage(std::stringstream &msg, Client *client);
	static void parseBuffer(const std::string &msg, Client *client);
	static bool parseCommand(std::stringstream &msg, std::vector<std::string> &msg_tokens);
	static bool parseParams(std::stringstream &msg, std::vector<std::string> &msg_tokens);
	static void processUnregisteredClient(std::vector<std::string> &msg_tokens, Client *client);
	static void execute_command(std::vector<std::string> &msg_token, Client *client);
	static void Pass_exec(std::vector<std::string> &msg_tokens, Client *client);
	static void User_exec(std::vector<std::string> &msg_tokens, Client *client);
	static void Nick_exec(std::vector<std::string> &msg_tokens, Client *client);
	static void registerClient(Client *client);

	static void setServer(Server *server);

private:
	static std::map<std::string, void (*)(std::vector<std::string> &, Client *)> command_map;
	static Server *_server;
};
