#ifndef RPL_HPP
#define RPL_HPP

#define RPL_WELCOME(nick, user, host) (":ft_irc 001 :Welcome to the FT_IRC Network, " + nick + "!" + user + "@" + host + '\n')
#define RPL_YOURHOST(nick, servername, version) (":ft_irc 002 " + nick + ":Your host is " + servername + " running, version " + version + "\n")
#define RPL_CREATED(nick, datetime) (":ft_irc 003 " + nick + ":This server was created " + datetime + "\n")
#define RPL_MYINFO(nick, servername, version, usermodes, channelmodes) (":ft_irc 004 " + nick + " " + servername + " " + version + " " + usermodes + " " + channelmodes + " \n")
#define RPL_ISUPPORT(nick) (":ft_irc 005 " + nick + " CHARSET=ascii CASEMAPPING=ascii PREFIX=(qaohv)~&@%+ CHANTYPES=# CHANLIMIT=#:10 CHANMODES=i,t,k,l :are supported by this server\n")
#define RPL_MOTD(nick, motd) (":ft_irc 372 :" + motd + "\n")
#define RPL_MOTDSTART(nick, servername) (":ft_irc 375 " + nick + " :- " + servername + " Message of the day - \n")
#define RPL_ENDOFMOTD(nick) (":ft_irc 376 " + nick + " :End of /MOTD command." + "\n")

#define RPL_JOIN(nick, user, host, channel) (":" + nick + "!~" + user + "@" + host + " JOIN :" + channel + "\r\n")
#define RPL_NAMREPLY(nick, channel, users) (":ft_irc 353 " + nick + " = " + channel + " :" + users + "\n")
#define RPL_ENDOFNAMES(nick, channel) (":ft_irc 366 " + nick + " " + channel + " " + ":End of NAMES list\n")

//WHO
#define RPL_WHOREPLY(client, channel, username, host, server, nick, flags, hopcount, realname) \
	(":ft_irc 352 " + client + " " + channel + " " + username +  " " + host + " " + server + " " + nick + " " + flags + " :" + hopcount + " " + realname + "\n")
#define RPL_ENDOFWHO(nick, mask) (nick + " " + mask + " :End of WHO list\n")

#define RPL_CREATIONTIME (nick, channel, creationtime) (":ft_irc 329 " + nick + " " channel + " " +  creationtime + "\n")
#define RPL_CHANNELMODEIS(nick, channel, modestring) ( ":ft_irc 324 " + nick + " " + channel + " " modestring + "\n")
// #define RPL_QUIT(msg) (:d!~ola@localhost "QUIT :" + msg)
#endif
