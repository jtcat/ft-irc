#ifndef ERR_HPP
#define ERR_HPP

#include <iostream>
#define ERR_NOTREGISTERED() (":ft_irc 451 * :Connection not registered")
#define ERR_NEEDMOREPARAMS(command, err_msg, nick) (":ft_irc 461 " + nick + " " + command + " :" + err_msg)
#define ERR_ALREADYREGISTERED(nick) (":ft_irc 462 " + nick + " :Connection already registered")
#define ERR_PASSWDMISMATCH() (":ft_irc 464 * :Password Mismatch")
#define ERR_NONICKNAMEGIVEN() (":ft_irc 431 * :No nickname given")
#define ERR_ERRONEUSNICKNAME(nick) (":ft_irc 432 " + nick +" :Erroneous nickname")
#define	ERR_NICKNAMEINUSE(nick) (":ft_irc 433 " + nick + " :Nickname is already in use")
#define ERR_UNKNOWNCOMMAND(command) (":ft_irc 421 " + command + " :Unknown command")
#define ERROR(err_msg) ("ERROR : " + err_msg)

#endif
