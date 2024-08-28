#ifndef RPL_HPP
#define RPL_HPP

#define RPL_WELCOME(nick, user, host) (":ft_irc 001 :Welcome to the FT_IRC Network, " + nick + "!" + user + "@" + host + '\n')
#define RPL_JOIN(nick, user, host, channel) (":" + nick + "!" + user + "@" + host + " JOIN :" + channel + "\n")
#define RPL_NAMREPLY(nick, channel, users) (":ft_irc 353 " + nick + " = " + channel + " :" + users + "\n")
#define RPL_ENDOFNAMES (366) (":ft_irc 366 " + nick + " " + channel + " " + ":End of NAMES list\n")


#endif
