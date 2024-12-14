#include "Channel.hpp"
#include "Server.hpp"
#include <sstream>
#include "RPL.hpp"
#include "ERR.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

/**
 * @brief Default constructor for the Channel class.
 */
Channel::Channel()
{
}

/**
 * @brief Parameterized constructor for the Channel class.
 * @param name The name of the channel.
 * @param op The operator (owner) of the channel.
 */
Channel::Channel(const std::string &name, Client *op) : _name(name), _user_limit(-1), _invite_Only_flag(0), _creation_time(std::time(0)), _topic_restriction(0) {
    setTopic(NULL, std::string());
    addOp(op);
}

/**
 * @brief Copy constructor for the Channel class.
 * @param src The source Channel object to copy.
 */
Channel::Channel(const Channel &src)
{
    *this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

/**
 * @brief Destructor for the Channel class.
 */
Channel::~Channel()
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/**
 * @brief Assignment operator for the Channel class.
 * @param rhs The Channel object to assign from.
 * @return Reference to the updated Channel object.
 */
Channel &Channel::operator=(Channel const &rhs)
{
    if (this != &rhs)
    {
        _name = rhs.getName();
        _users = rhs.getUsers();
        _op = rhs.getOp();
        _invites = rhs.getInvites();
        _passwd = rhs.getPasswd();
        _user_limit = rhs.getUserLimit();
        _invite_Only_flag = rhs.getInviteFlag();
    }
    return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/**
 * @brief Finds a user in the channel by their nickname.
 * @param nick The nickname of the user to find.
 * @return Iterator to the user if found, otherwise end of the user set.
 */
std::set<Client *>::iterator Channel::getUserbyNick(const std::string &nick) const
{
    for (std::set<Client *>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->getNick() == nick)
            return it;
    }
    return (_users.end());
}

/**
 * @brief Finds an operator in the channel by their nickname.
 * @param nick The nickname of the operator to find.
 * @return Iterator to the operator if found, otherwise end of the operator set.
 */
std::set<Client *>::iterator Channel::getOpbyNick(const std::string &nick) const
{
    for (std::set<Client *>::iterator it = _op.begin(); it != _op.end(); it++)
    {
        if ((*it)->getNick() == nick)
            return it;
    }
    return (_op.end());
}

/**
 * @brief Removes a user from the channel.
 * @param nick The nickname of the user to remove.
 */
void Channel::delUser(const std::string &nick)
{
    std::set<Client *>::iterator user_it = getUserbyNick(nick);
    std::set<Client *>::iterator op_it = getOpbyNick(nick);

    if (user_it != _users.end())
        _users.erase(user_it);
    if (op_it != _op.end())
        _op.erase(op_it);
}

/**
 * @brief Clears the list of invited users.
 */
void Channel::clearInvitesList()
{
    _invites.clear();
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/**
 * @brief Gets the name of the channel.
 * @return The name of the channel.
 */
const std::string &Channel::getName() const
{
    return _name;
}

/**
 * @brief Gets the topic of the channel.
 * @return The topic of the channel.
 */
const std::string &Channel::getTopic() const
{
    return _topic;
}

/**
 * @brief Gets the list of users in the channel.
 * @return A set of pointers to the users.
 */
const std::set<Client *> &Channel::getUsers() const
{
    return _users;
}

/**
 * @brief Gets the list of operators in the channel.
 * @return A set of pointers to the operators.
 */
const std::set<Client *> &Channel::getOp() const
{
    return _op;
}

/**
 * @brief Gets the list of invited users.
 * @return A set of pointers to the invited users.
 */
const std::set<Client *> &Channel::getInvites() const
{
    return _invites;
}

/**
 * @brief Gets the password for the channel.
 * @return The password of the channel.
 */
const std::string &Channel::getPasswd() const
{
    return _passwd;
}

/**
 * @brief Gets the user limit for the channel.
 * @return The maximum number of users allowed in the channel.
 */
int Channel::getUserLimit() const
{
    return _user_limit;
}

/**
 * @brief Gets the invite-only flag of the channel.
 * @return The invite-only flag (1 if invite-only, 0 otherwise).
 */
int Channel::getInviteFlag() const
{
    return _invite_Only_flag;
}

/**
 * @brief Gets the creation time of the channel.
 * @return The creation time of the channel.
 */
const time_t &Channel::getCreationTime() const
{
    return _creation_time;
}

/**
 * @brief Checks if a user is invited to the channel.
 * @param client Pointer to the client to check.
 * @return True if the client is invited, false otherwise.
 */
bool Channel::checkIfUserInvited(Client *client)
{
    if (_invites.find(client) != _invites.end())
        return true;
    return false;
}
