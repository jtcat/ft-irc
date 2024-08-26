#ifndef MACROS_HPP
#define MACROS_HPP

#include <iostream>
#define ERR_NOTREGISTERED() (":ft_irc 451 * :Connection not registered")
#define ERR_NEEDMOREPARAMS(command, err_msg, nick) (":ft_irc 461 " + nick + " " + command + " :" + err_msg)
#define ERR_ALREADYREGISTERED(nick) (":ft_irc 462 " + nick + " :Connection already registered")
#define ERR_PASSWDMISMATCH() (":ft_irc 464 * :Password Mismatch")
#endif
