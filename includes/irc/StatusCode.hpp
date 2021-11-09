# ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

# define SUCCESS				0
# define ERR_KICK				1
# define ERR_NOSUCHNICK			401 //"<nickname> :No such nick/channel"
# define ERR_NOSUCHCHANNEL		403 //"<channel name> :No such channel" | Used to indicate the given channel name is invalid.
# define ERR_TOOMANYCHANNELS	405 //"<channel name> :You have joined too many channels"

# define ERR_NORECIPENT			411 //":No recipient given (<command>)"
# define ERR_NOTEXTTOSEND		412 //":No text to send"
# define ERR_NONICKNAMEGIVEN	431


# define ERR_ERRONEUSNICKNAME	432
# define ERR_NICKNAMEINUSE		433
# define ERR_NICKCOLLISION		436
# define ERR_NOTONCHANNEL		442 //"<channel> :You're not on that channel"
# define ERR_NEEDMOREPARAMS		461
# define ERR_ALREADYREGISTRED	462
# define ERR_CHANNELISFULL		471 //"<channel> :Cannot join channel (+l)"


#endif //endif