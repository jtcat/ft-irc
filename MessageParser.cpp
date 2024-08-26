#include "MessageParser.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include "ERR.hpp"
#include "RPL.hpp"

Server* MessageParser::_server = NULL;
std::map<std::string, void (*)(std::vector<std::string> &, Client *)> MessageParser::command_map;
void MessageParser::setServer(Server *server)
{
	MessageParser::_server = server;
}

void MessageParser::registerClient(Client *client)
{
	client->setRegisteredFlag(1);
	std::cout << RPL_WELCOME(client->getNick(), client->getUsername(), client->getIpAddr()) << std::endl;
	//plus all the other welcome messages
}

void MessageParser::Pass_exec(std::vector<std::string> &msg_tokens, Client *client)
{
	if (msg_tokens.size() < 2)
		std::cout << ERR_NEEDMOREPARAMS("PASS", "Not enough parameters", client->getNick()) << std::endl;
	else if (client->getAuthenticatedFlag() == 1)
		std::cout << ERR_ALREADYREGISTERED(client->getNick()) << std::endl;
	else if (msg_tokens[1] != MessageParser::_server->_passwd)
	{
		std::cout << ERR_PASSWDMISMATCH() << std::endl;
		std::cout << ERROR(std::string("Closing link")) << std::endl;
		MessageParser::_server->closeClientConnection(client->getSockFd());
		MessageParser::_server->delPollFd();
	}
	else
	{
		client->setAuthenticatedFlag(1);
		if (client->getNick() != "*" && !client->getUsername().empty())
			registerClient(client);
	}
};

void MessageParser::User_exec(std::vector<std::string> &msg_tokens, Client *client)
{
	// verify USER specific syntax like username len, etc
	if (msg_tokens.size() < 5)
		std::cout << ERR_NEEDMOREPARAMS("USER", "Not enough parameters", client->getNick()) << std::endl;
	else if (client->getRegisteredFlag() == 1 || !client->getUsername().empty())
		std::cout << ERR_ALREADYREGISTERED(client->getNick()) << std::endl;
	else
	{
		client->setUsername(msg_tokens[1]);
		client->setRealname(msg_tokens[4]);
		if (client->getNick() != "*" && client->getAuthenticatedFlag() == 1)
			registerClient(client);
	}
};

void MessageParser::Nick_exec(std::vector<std::string> &msg_tokens, Client *client) {
	// verify NICK specific syntax like username len, etc
	if (msg_tokens.size() < 2)
		std::cout << ERR_NONICKNAMEGIVEN() << std::endl;
	// else if (nick already in use)
		// std::cout << ERR_NICKNAMEINUSE(client->getNick()) << std::endl;
	// else if (nick name not acoording to syntax)
	// 	std::cout << ERR_ERRONEUSNICKNAME(client->getNick()) << std::endl;
	else if (client->getRegisteredFlag() == 1)
	{
		//send reply to notify users of the nick change
		client->setNick(msg_tokens[1]);
	}
	else {
		client->setNick(msg_tokens[1]);
		if (!client->getUsername().empty() && client->getAuthenticatedFlag() == 1)
			registerClient(client);
	}
}
// not registered -> valid command -> ERR_NOTREGISTERD
// invalid command -> ignore
void MessageParser::processUnregisteredClient(std::vector<std::string> &msg_tokens, Client *client)
{
	std::map<std::string, void (*)(std::vector<std::string> &, Client *)>::iterator it = command_map.find(msg_tokens[0]);

	if (it != command_map.end()) // valid command but no registry -> ERR_NOTREGISTEREED
	{
		if (msg_tokens[0] == "PASS" || msg_tokens[0] == "NICK" || msg_tokens[0] == "USER")
			it->second(msg_tokens, client);
		else
			std::cout << ERR_NOTREGISTERED() << std::endl;
	}
	// invalid command before registry is ignored;
}

void MessageParser::execute_command(std::vector<std::string> &msg_tokens, Client *client)
{
	std::map<std::string, void (*)(std::vector<std::string> &, Client *)>::iterator it = command_map.find(msg_tokens[0]);

	if (command_map.empty())
	{
		command_map["PASS"] = &MessageParser::Pass_exec;
		command_map["USER"] = &MessageParser::User_exec;
		command_map["NICK"] = &MessageParser::Nick_exec;
	}
	if (client->getRegisteredFlag() == 0)
		processUnregisteredClient(msg_tokens, client);
	else if (it != command_map.end())
		it->second(msg_tokens, client);
	else
		std::cout << ERR_UNKNOWNCOMMAND(msg_tokens[0]) << std::endl;
}

void printVectorWithSpaces(const std::vector<std::string> &vec)
{
	for (size_t i = 0; i < vec.size(); ++i)
	{
		std::cout << vec[i];
		if (i != vec.size() - 1)
		{
			std::cout << "---";
		}
	}
	std::cout << std::endl; // Print a newline at the end
}
void removeUntilCRLF(std::stringstream &msg)
{
	char ch;
	while (msg.get(ch))
	{
		if (ch == '\r' && msg.peek() == '\n')
		{
			msg.get(); // Consume '\n'
			break;
		}
	}
}
// NICK dav:
// :irc.local 432 yo dav: :Erroneous Nickname
//-> what to do in this case?
bool MessageParser::parseParams(std::stringstream &msg, std::vector<std::string> &msg_tokens)
{
	char ch;
	int include_space = 0;
	std::stringstream ss;
	while (msg.get(ch))
	{
		if (ch == '\r' && msg.peek() == '\n')
		{
			msg.get(); // get \n;
			if (ss.str().length() > 0)
				msg_tokens.push_back(ss.str());
			return true;
		}
		// should I store : together with the rest of the arguments?
		if (ch == ':' && ss.str().empty()) // " : " is found
			include_space = 1;
		else if (!isspace(ch) || include_space == 1)
		{
			ss << ch; // append the character into the string
		}
		else
		{
			if (ss.str().length() > 0)
			{
				msg_tokens.push_back(ss.str());
				ss.str("");
				ss.clear();
			}
		}
	}
	return false;
}

bool MessageParser::parseCommand(std::stringstream &msg, std::vector<std::string> &msg_tokens)
{
	char ch;
	std::stringstream ss;
	while (msg.get(ch))
	{
		if (ch == '\r' && msg.peek() == '\n')
		{
			msg.unget(); // Put the character back into the stream
			if (ss.str().length() > 0)
				msg_tokens.push_back(ss.str());
			return true;
		}
		if (!isspace(ch))
		{
			ss << ch; // append the character into the string
		}
		else
		{
			if (ss.str().length() > 0)
			{
				msg_tokens.push_back(ss.str());
				return true;
			}
		}
	}
	return false;
}

void MessageParser::parseBuffer(const std::string &buff, Client *client)
{
	std::stringstream ss_buff(buff);
	while (!ss_buff.eof())
	{
		parseMessage(ss_buff, client);
	}
}
bool MessageParser::parseMessage(std::stringstream &msg, Client *client)
{
	std::vector<std::string> msg_tokens;
	(void)client;
	if (!parseCommand(msg, msg_tokens) || !parseParams(msg, msg_tokens))
	{
		removeUntilCRLF(msg);
		// print ERR_UNKNOWNERROR (400) -> can specify specific messages
		return false;
	}
	// execute command;
	execute_command(msg_tokens, client);
	// printVectorWithSpaces(msg_tokens);

	return true;
}

// when we have quit, how do we access the values of server??
// should message parser be a friend of server?
// should we pass an instance of server to messgae parser and have an iterator point to the current client were processing?
//  check for nic:k
//  PASS must be the first command or -> ERR_NOTREGISTERED (451)
//  THEN NICK and USER or vice-versa
//  THEN WELCOME MESSAGE
