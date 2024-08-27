#include "MessageParser.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include "ERR.hpp"
#include "RPL.hpp"

#define SERV_PRIVMSG()

Server* MessageParser::_server = NULL;
std::map<std::string, void (*)(std::vector<std::string> &, Client *)> MessageParser::command_map;

void MessageParser::setServer(Server *server)
{
	MessageParser::_server = server;
}

void MessageParser::registerClient(Client *client)
{
	client->setRegisteredFlag(1);
	_server->_client_users.insert(std::make_pair(client->getNick(), client));
	Server::send(client, RPL_WELCOME(client->getNick(), client->getUsername(), client->getIpAddr()));
	//plus all the other welcome messages
}
void MessageParser::Pass_exec(std::vector<std::string> &msg_tokens, Client *client)
{
	if (msg_tokens.size() < 2)
		Server::send(client, ERR_NEEDMOREPARAMS("PASS", "Not enough parameters", client->getNick()));
	else if (client->getAuthenticatedFlag() == 1)
		Server::send(client ,ERR_ALREADYREGISTERED(client->getNick()));
	else if (msg_tokens[1] != MessageParser::_server->_passwd)
	{
		Server::send(client, ERR_PASSWDMISMATCH());
		Server::send(client, ERROR(std::string("Closing link")));
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
		Server::send(client, ERR_NEEDMOREPARAMS("USER", "Not enough parameters", client->getNick()));
	else if (client->getRegisteredFlag() == 1 || !client->getUsername().empty())
		Server::send(client, ERR_ALREADYREGISTERED(client->getNick()));
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
		Server::send(client, ERR_NONICKNAMEGIVEN());
	// else if (nick already in use)
	// 	Server::send(client, ERR_NICKNAMEINUSE(client->getNick()));
	// else if (nick name not acoording to syntax)
	// 	Server::send(client, ERR_ERRONEUSNICKNAME(client->getNick()));
	else if (client->getRegisteredFlag() == 1)
	{
		//send reply to notify users of the nick change
		_server->_client_users.erase(client->getNick());
		client->setNick(msg_tokens[1]);
		_server->_client_users.insert(std::make_pair(client->getNick(), client));
	}
	else {
		client->setNick(msg_tokens[1]);
		if (!client->getUsername().empty() && client->getAuthenticatedFlag() == 1)
			registerClient(client);
	}
}

void MessageParser::Join_exec(std::vector<std::string> &msg_tokens, Client *client) {
	//check for valid chanel names;
	//check for , separating the chanels intended to join;
	//check the chanel modes (invite only?), (key required?)
	//write function to broadcast replies to all members of a chanel;
};

void MessageParser::Quit_exec(std::vector<std::string> &msg_tokens, Client *client) {};
void MessageParser::Part_exec(std::vector<std::string> &msg_tokens, Client *client) {};

void MessageParser::Privmsg_exec(std::vector<std::string> &msg_tokens, Client *client) {
	std::map<std::string, Client*>::iterator	target_client;
	std::map<std::string, Channel*>::iterator	target_channel;

	for (std::vector<std::string>::iterator target_name = msg_tokens.begin(); target_name < msg_tokens.end() - 1; target_name++) {
		if ((target_client = _server->_client_users.find(*target_name)) != _server->_client_users.end()) {
			Server::send(client, ":" + client->getNick() + " " + "PRIVMSG" + " " + target_client->second->getNick() + " " + *(msg_tokens.end() - 1));
		}
		else if ((target_channel = _server->_channels.find(*target_name)) != _server->_channels.end()) {
			Channel *channel = target_channel->second;
			for (std::set<Client*>::iterator channel_member = channel->_users.begin(); channel_member != channel->_users.end(); channel_member++) {
				if (*channel_member != client)
					Server::send(client, ":" + client->getNick() + " " + "PRIVMSG" + " " + target_channel->first + " " + *(msg_tokens.end() - 1));
			}
		}
		else {
			Server::send(client, *target_name + " :No such nick or channel name");
		}
	}
};

void MessageParser::Oper_exec(std::vector<std::string> &msg_tokens, Client *client) {};
void MessageParser::Mode_exec(std::vector<std::string> &msg_tokens, Client *client) {};
void MessageParser::Topic_exec(std::vector<std::string> &msg_tokens, Client *client) {};
void MessageParser::Kick_exec(std::vector<std::string> &msg_tokens, Client *client) {};
void MessageParser::Invite_exec(std::vector<std::string> &msg_tokens, Client *client) {
	//how are invites gonna be handled? do they last forever ?
};

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
			Server::send(client, ERR_NOTREGISTERED());
	}
	// invalid command before registry is ignored;
}

void MessageParser::execute_command(std::vector<std::string> &msg_tokens, Client *client)
{
	if (msg_tokens.size() == 0)
		return ;
	std::map<std::string, void (*)(std::vector<std::string> &, Client *)>::iterator it = command_map.find(msg_tokens[0]);

	if (command_map.empty())
	{
		command_map["PASS"] = &MessageParser::Pass_exec;
		command_map["USER"] = &MessageParser::User_exec;
		command_map["NICK"] = &MessageParser::Nick_exec;
		command_map["JOIN"] = &MessageParser::Join_exec;
		command_map["QUIT"] = &MessageParser::Quit_exec;
		command_map["PART"] = &MessageParser::Part_exec;
		command_map["PRIVMSG"] = &MessageParser::Privmsg_exec;
		command_map["OPER"] = &MessageParser::Oper_exec;
		command_map["MODE"] = &MessageParser::Mode_exec;
		command_map["TOPIC"] = &MessageParser::Topic_exec;
		command_map["KICK"] = &MessageParser::Kick_exec;
		command_map["INVITE"] = &MessageParser::Invite_exec;
	}
	if (client->getRegisteredFlag() == 0)
		processUnregisteredClient(msg_tokens, client);
	else if (it != command_map.end())
		it->second(msg_tokens, client);
	else
		Server::send(client, ERR_UNKNOWNCOMMAND(msg_tokens[0]));
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

//handle signals
//handle freeing resources before exit(1) to clean allocated resources
//should chanel operators be stored together with users or separetely?
//when user is promoted to oper its entry gets removed from the users list and gets added to the operators
