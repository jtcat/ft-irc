#ifndef RPL_HPP
#define RPL_HPP

#define RPL_WELCOME(nick, user, host) (":ft_irc 001 :Welcome to the FT_IRC Network, " + nick + "!" + user + "@" + host + '\n')
#define RPL_JOIN(nick, user, host, channel) (":" + nick + "!" + user + "@" + host + " JOIN :" + channel + "\n")
#define RPL_NAMREPLY(nick, channel, users) (":ft_irc 353 " + nick + " = " + channel + " :" + users + "\n")
#define RPL_ENDOFNAMES(nick, channel) (":ft_irc 366 " + nick + " " + channel + " " + ":End of NAMES list\n")
#define RPL_CREATIONTIME (nick, channel, creationtime) (":ft_irc 329 " + nick + " " channel + " " +  creationtime + "\n")
#define RPL_CHANNELMODEIS(nick, channel, modestring) ( ":ft_irc 324 " + nick + " " + channel + " " modestring + "\n")
// #define RPL_QUIT(msg) (:d!~ola@localhost "QUIT :" + msg)
#endif
