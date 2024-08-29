#ifndef ERR_HPP
#define ERR_HPP

#include <iostream>
#define ERR_NOTREGISTERED() (":ft_irc 451 * :Connection not registered\n")
#define ERR_NEEDMOREPARAMS(command, nick) (":ft_irc 461 " + nick + " " + command + " :Not enough parameters" + "\n")
#define ERR_ALREADYREGISTERED(nick) (":ft_irc 462 " + nick + " :Connection already registered\n")
#define ERR_PASSWDMISMATCH() (":ft_irc 464 * :Password Mismatch\n")

// PRIVMSG
#define ERR_NOSUCHNICK(client, target) (":ft_irc 401 " + client + " " + target + " :No such nick or channel name\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) (":ft_irc 404 " + client + " " + channel + " :Cannot send to channel\n")
#define ERR_NORECIPIENT(client, cmd) (":ft_irc 411 " + client + " :No recipient given (" + cmd +")\n")
#define ERR_NOTEXTTOSEND(client) (":ft_irc 412 " + client + " :No text to send\n")

#define ERR_NONICKNAMEGIVEN() (":ft_irc 431 * :No nickname given\n")
#define ERR_ERRONEUSNICKNAME(nick) (":ft_irc 432 " + nick +" :Erroneous nickname\n")
#define ERR_NICKNAMEINUSE(nick) (":ft_irc 433 " + nick + " :Nickname is already in use\n")
#define ERR_UNKNOWNCOMMAND(command) (":ft_irc 421 " + command + " :Unknown command\n")
#define ERROR(err_msg) ("ERROR : " + err_msg + "\n")
#define ERR_BADCHANMASK(channel) (":ft_irc 476 " + channel + " :Bad Channel Mask\n")
#define ERR_BADCHANNELKEY(channel) (":ft_irc 475 " + channel + " :Cannot join channel (+k), wrong key provided\n")
#define ERR_CHANNELISFULL(channel) (":ft_irc 471 " + channel+ " :Cannot join channel (+l), channel is full\n")
#define ERR_INVITEONLYCHAN(channel) (":ft_irc 473 " + channel +" :Cannot join channel (+i), channel is invite-only\n")
#define ERR_NOSUCHCHANNEL(channel) (":ft_irc 403 "+ channel + ":No such channel\n")
#define ERR_INVALIDMODEPARAM(nick, channel, mode) (":ft_irc 696 " + nick + " " + channel + " " + mode + " :You must specify a parameter for the mode\n")
//#define ERR_NOSUCHNICK(nick, user_nick) (":ft_irc 401 " + nick + " " + user_nick + " :No such nick\n")
#define ERR_UNKNOWNMODE(nick, mode) (":ft_irc 472 " + nick + " " + mode + " :is not a recognized channel mode\n")

#endif
