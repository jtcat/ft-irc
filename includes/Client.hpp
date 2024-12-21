#pragma once

#include <iostream>
#include <string>
#include <map>

class Channel;
#include "MessageParser.hpp"
#include "Channel.hpp"

#define MSG_BUFFER_MAX_SIZE 512

class Client {
	public:
		// friend class MessageParser;
		Client(int sock_fd, std::string ip_addr);
		Client(Client const &src);
		~Client(void);

		int getSockFd() const;
		const std::string &getHost() const;
		const std::string &getNick() const;
		const std::string &getUser() const;
		int getAuthenticatedFlag() const;
		int getRegisteredFlag() const;
		const std::string& getRealName(void) const;

		std::string	getSourceStr(void) const;

		//void	appendMsgToBuffer(const std::string& msgPart);

		void setAuthenticatedFlag(int flag);
		void setRegisteredFlag(int flag);
		void setUser(const std::string &username);
		void setNick(const std::string &nick);
		void setRealname(const std::string &realname);
		const std::map<std::string, Channel *> &getChannels() const;
		void addChannel(Channel *channel);
		bool isUserOnChannel(const std::string &channel) const;
		void broadcastMsg(const std::string &msg, bool broadcastToHimself) const;
		void clearMsgBuffer();
		void appendToMsgBuffer(const std::string &str);
		const std::string & getMsgBuffer();
		std::string::size_type getMsgBufferSize();
		std::string::size_type getBufferMaxSize();

		Client &operator=(Client const &rhs);
		friend std::ostream &operator<<(std::ostream &os, const Client &client);

	private:
		Client(void);
		int _sock_fd;
		std::string _host;
		std::string _nick;
		std::string _user;
		std::string _realname;
		std::string _msg_buffer;
		const std::string::size_type	_msg_buffer_max_size;
		int _authenticated;
		int _registered;
		std::map<std::string, Channel *> _channels;
};
