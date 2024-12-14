#include "Server.hpp"
#include "Client.hpp"
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "MessageParser.hpp"
#include "RPL.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const std::string nietzscheQuotes[] = {
	"He who has a why to live can bear almost any how.",
	"That which does not kill us makes us stronger.",
	"Without music, life would be a mistake.",
	"There are no facts, only interpretations.",
	"The higher we soar, the smaller we appear to those who cannot fly.",
	"The individual has always had to struggle to keep from being overwhelmed by the tribe.",
	"Whoever fights monsters should see to it that in the process he does not become a monster.",
	"In every real man, a child is hidden that wants to play.",
	"To live is to suffer, to survive is to find some meaning in the suffering.",
	"If you gaze long enough into an abyss, the abyss will gaze back into you.",
	"We love life, not because we are used to living but because we are used to loving.",
	"Become who you are! (Be yourself!)",
	"There is always some madness in love. But there is also always some reason in madness.",
	"One must still have chaos in oneself to be able to give birth to a dancing star.",
	"The man of knowledge must be able not only to love his enemies but also to hate his friends.",
	"It is not the strength, but the duration, of great sentiments that makes great men.",
	"To deny one’s own experiences is to put one’s self on a level with those who have never lived at all.",
	"A man’s worth is determined by how much truth he can endure.",
	"The best weapon against an enemy is another enemy."
};

static std::string	calcCreateDate(void) {
	const time_t	t = time(NULL);

	return ctime(&t);
}

void	Server::createBot(void) {
	Client*	botClient = new Client(-1, "ft-irc");

	botClient->setNick("NietzscheBot");
	botClient->setRealname("NietzscheBot");
	botClient->setUser("NietzscheBot");
	botClient->setRegisteredFlag(1);
	botClient->setAuthenticatedFlag(1);
	_clients[-1] = botClient;
	_client_users.insert(std::make_pair(botClient->getNick(), botClient));
	_welcome_bot = botClient;
}

Server::Server(int port, const std::string &passwd) : _name("ft_irc"), _motd(SERVER_MOTD), _create_date(calcCreateDate()), _default_kick_msg("unspecified reason"), _poll_i(0), _pfds(), _passwd(passwd), _fd_count(0)
{
	_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (_sock_fd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}
	// init the sock_address struct to pass to bind
	bzero((char *)&_serv_addr, sizeof(_serv_addr));
	_serv_addr.sin_family = AF_INET;
	_serv_addr.sin_addr.s_addr = INADDR_ANY;
	_serv_addr.sin_port = htons(port);
	// bind the socket to the port passed as input
	int yes = 1;
	if (setsockopt(_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
	{
		perror("setsockopt");
		exit(1);
	}

	createBot();
}

Server::~Server()
{
	close(_sock_fd);

	for (std::map<std::string, Channel*>::iterator	it = _channels.begin(); it != _channels.end(); it++) {
		delete it->second;
	}

	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		delete it->second;
	}
}

const std::string&	Server::getCreateDate(void) const {
	return _create_date;
}

const std::string&	Server::getMOTD(void) const {
	return _motd;
}

const std::string&	Server::getName(void) const {
	return _name;
}

void Server::bind()
{
	if (::bind(_sock_fd, reinterpret_cast<struct sockaddr *>(&_serv_addr),
			   sizeof(_serv_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}
}

const Client Server::accept()
{
	unsigned int addr_size;
	int newsock_fd;
	struct sockaddr_in client_addr;
	char ip_str[INET_ADDRSTRLEN];

	addr_size = sizeof(client_addr);
	newsock_fd = ::accept(
		_sock_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size);
	if (newsock_fd < 0)
	{
		perror("ERROR on accept");
		exit(1);
	}
	inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET6_ADDRSTRLEN);
	// std::cout << "ip str in accept = " << ip_str << std::endl;
	return (Client(newsock_fd, std::string(ip_str)));
}

int Server::send(Client *client, const std::string &msg)
{
	const char *msg_c_str = msg.c_str();
	size_t len = strlen(msg_c_str);
	size_t bytes_sent = 0;
	size_t total_bytes_sent = 0;
	while (total_bytes_sent < len)
	{
		bytes_sent = ::send(client->getSockFd(), msg_c_str + total_bytes_sent, len - total_bytes_sent, MSG_NOSIGNAL);
		if (static_cast<int>(bytes_sent) == -1)
		{
			//perror("send");
			return -1;
		}
		total_bytes_sent += bytes_sent;
	}
	return total_bytes_sent;
}

void Server::addPollFd(int client_fd)
{
	struct pollfd temp_pfds;

	temp_pfds.fd = client_fd;
	temp_pfds.events = POLLIN;
	temp_pfds.revents = 0;
	_pfds.push_back(temp_pfds);
	_fd_count++;
}

void Server::delPollFd()
{
	// close(_pfds[_poll_i].fd);
	_pfds.erase(_pfds.begin() + _poll_i);
	_fd_count--;
	_poll_i--;
};

const std::string&	Server::pickRandomQuote(void){
    // Calculate the number of quotes in the array
    static size_t numQuotes = sizeof(nietzscheQuotes) / sizeof(nietzscheQuotes[0]);

    // Seed the random number generator with the current time
    srand(time(0));

    // Generate a random index between 0 and numQuotes - 1
    int randomIndex = rand() % numQuotes;

	std::cout << "Quote: " << nietzscheQuotes[randomIndex] << std::endl;

	return nietzscheQuotes[randomIndex];
}

void Server::registerNewClient()
{
	Client *new_client = new Client(this->accept());

	int new_sock_fd = new_client->getSockFd();
	_clients[new_sock_fd] = new_client;
	_client_users.insert(std::make_pair(new_client->getNick(), new_client));
	this->addPollFd(new_sock_fd);
}

void	Server::execWelcomeBot(Client* client) const {
	Server::send(client, ":" + _welcome_bot->getNick() + "!" + _welcome_bot->getUser()  + "@" + _welcome_bot->getHost() + " " + "PRIVMSG " + client->getNick() + " :" + Server::pickRandomQuote() + "\n");
}

void Server::closeClientConnection(int client_fd)
{
	Client	*client = _clients[client_fd];

	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		it->second->delUser(client->getNick());
		it->second->delUserFromInvites(client);
		it->second->broadcastMsg(":" + client->getNick() + "!~" + client->getUser() + "@" + client->getHost() + " QUIT : Quit: Connection closed\n");
	}

	_client_users.erase(client->getNick());
	_clients.erase(client_fd);
	delete client;
}

void Server::addChannel(Channel *channel)
{
	_channels[channel->getName()] = channel;
}

bool Server::ChannelExists(const std::string &channel) const
{
	if (_channels.find(channel) != _channels.end())
		return true;
	return false;
}
bool Server::UserExists(const std::string &nick) const {
	if (_client_users.find(nick) != _client_users.end())
		return true;
	return false;
}
Client *Server::getClient(const std::string &nick) const {
	std::map<std::string, Client *>::const_iterator it = _client_users.find(nick);
	if (it != _client_users.end())
		return it->second;
	return NULL;
}

Channel *Server::getChannel(const std::string &channel) const
{
	std::map<std::string, Channel *>::const_iterator it = _channels.find(channel);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

void Server::monitorClients()
{
	char buff[512];

	MessageParser::setServer(this);
	this->addPollFd(_sock_fd);

	while (g_run)
	{
		int poll_count = poll(&_pfds[0], _fd_count, 0);
		if (poll_count == -1)
		{
			//perror("poll");
			exit(1);
		}
		_poll_i = 0;
		while (_poll_i < _pfds.size())
		{
			if (_pfds[_poll_i].revents & POLLIN)
			{
				if (_pfds[_poll_i].fd == _sock_fd)
				{
					// CHECK CONNECTION LIMIT
					registerNewClient();
				}
				else
				{
					bzero(buff, 512);
					int bytes_read = recv(_pfds[_poll_i].fd, buff, sizeof(buff), 0);
					if (bytes_read <= 0)
					{
						// Got error or connection closed by client
						if (bytes_read == 0) {
							//std::cerr << "pollserver: socket hung up" << std::endl;
						}
						else
							perror("recv");
						closeClientConnection(_pfds[_poll_i].fd);
						delPollFd();
					}
					else
					{
						std::cout << "buffer: " << buff << std::endl;
						_clients[_pfds[_poll_i].fd]->processMessage(buff);
					}
				}
			}
			_poll_i++;
		}
	}
}

void Server::listen(int n)
{
	::listen(_sock_fd, n);
}

int Server::getSockFd()
{
	return _sock_fd;
}

const std::string&	Server::getDefaultKickMsg(void) const {
	return _default_kick_msg;
}
