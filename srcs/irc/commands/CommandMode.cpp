#include "CommandMode.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

CommandMode::CommandMode(CommandHandler & handler) : ACommand(handler)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

CommandMode::~CommandMode()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/


/*
						=== RFC 1459 OLD RFC ===
4.2.3 Mode message

      Command: MODE

   The MODE command is a dual-purpose command in IRC.  It allows both
   usernames and channels to have their mode changed.  The rationale for
   this choice is that one day nicknames will be obsolete and the
   equivalent property will be the channel.

   When parsing MODE messages, it is recommended that the entire message
   be parsed first and then the changes which resulted then passed on.

4.2.3.1 Channel modes

   Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
               [<ban mask>]

   The MODE command is provided so that channel operators may change the
   characteristics of `their' channel.  It is also required that servers
   be able to change channel modes so that channel operators may be
   created.

   The various modes available for channels are as follows:

           o - give/take channel operator privileges;
           p - private channel flag;
           s - secret channel flag;
           i - invite-only channel flag;
           t - topic settable by channel operator only flag;
           n - no messages to channel from clients on the outside;
           m - moderated channel;
           l - set the user limit to channel;
           b - set a ban mask to keep users out;
           v - give/take the ability to speak on a moderated channel;
           k - set a channel key (password).

   When using the 'o' and 'b' options, a restriction on a total of three
   per mode command has been imposed.  That is, any combination of 'o'
   and

4.2.3.2 User modes

   Parameters: <nickname> {[+|-]|i|w|s|o}

   The user MODEs are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.
   A user MODE command may only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.

   The available modes are as follows:

           i - marks a users as invisible;
           s - marks a user for receipt of server notices;
           w - user receives wallops;
           o - operator flag.

   Additional modes may be available later on.

   If a user attempts to make themselves an operator using the "+o"
   flag, the attempt should be ignored.  There is no restriction,
   however, on anyone `deopping' themselves (using "-o").  Numeric
   Replies:

           ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_KEYSET
           RPL_BANLIST                     RPL_ENDOFBANLIST
           ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

           ERR_USERSDONTMATCH              RPL_UMODEIS
           ERR_UMODEUNKNOWNFLAG

   Examples:

           Use of Channel Modes:

MODE #Finnish +im               ; Makes #Finnish channel moderated and
                                'invite-only'.

MODE #Finnish +o Kilroy         ; Gives 'chanop' privileges to Kilroy on

                                channel #Finnish.

MODE #Finnish +v Wiz            ; Allow WiZ to speak on #Finnish.

MODE #Fins -s                   ; Removes 'secret' flag from channel
                                #Fins.

MODE #42 +k oulu                ; Set the channel key to "oulu".

MODE #eu-opers +l 10            ; Set the limit for the number of users
                                on channel to 10.

MODE &oulu +b                   ; list ban masks set for channel.

MODE &oulu +b *!*@*             ; prevent all users from joining.

MODE &oulu +b *!*@*.edu         ; prevent any user from a hostname
                                matching *.edu from joining.

        Use of user Modes:

:MODE WiZ -w                    ; turns reception of WALLOPS messages
                                off for WiZ.

:Angel MODE Angel +i            ; Message from Angel to make themselves
                                invisible.

MODE WiZ -o                     ; WiZ 'deopping' (removing operator
                                status).  The plain reverse of this
                                command ("MODE WiZ +o") must not be
                                allowed from users since would bypass
                                the OPER command.

*/












/*
======================================================================================
										NEW RFC's
======================================================================================
*/







/*
======================================================================================
===================== RFC 2812 USER MODE MESSSAGE ====================================
======================================================================================
*/

/*
3.1.5 User mode message

      Command: MODE
   Parameters: <nickname>
               *( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )

   The user MODE's are typically changes which affect either how the
   client is seen by others or what 'extra' messages the client is sent.

   A user MODE command MUST only be accepted if both the sender of the
   message and the nickname given as a parameter are both the same.  If
   no other parameter is given, then the server will return the current
   settings for the nick.

      The available modes are as follows:

           a - user is flagged as away;
           i - marks a users as invisible;
           w - user receives wallops;
           r - restricted user connection;
           o - operator flag;
           O - local operator flag;
           s - marks a user for receipt of server notices.

   Additional modes may be available later on.

   The flag 'a' SHALL NOT be toggled by the user using the MODE command,
   instead use of the AWAY command is REQUIRED.

   If a user attempts to make themselves an operator using the "+o" or
   "+O" flag, the attempt SHOULD be ignored as users could bypass the
   authentication mechanisms of the OPER command.  There is no
   restriction, however, on anyone `deopping' themselves (using "-o" or
   "-O").

   On the other hand, if a user attempts to make themselves unrestricted
   using the "-r" flag, the attempt SHOULD be ignored.  There is no
   restriction, however, on anyone `deopping' themselves (using "+r").
   This flag is typically set by the server upon connection for
   administrative reasons.  While the restrictions imposed are left up
   to the implementation, it is typical that a restricted user not be
   allowed to change nicknames, nor make use of the channel operator
   status on channels.

   The flag 's' is obsolete but MAY still be used.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_USERSDONTMATCH
           ERR_UMODEUNKNOWNFLAG            RPL_UMODEIS

   Examples:

   MODE WiZ -w                     ; Command by WiZ to turn off
                                   reception of WALLOPS messages.

   MODE Angel +i                   ; Command from Angel to make herself
                                   invisible.

   MODE WiZ -o                     ; WiZ 'deopping' (removing operator
                                   status).

*/


/*
======================================================================================
===================== RFC 2812 CHANNEL MODE MESSSAGE =================================
======================================================================================
*/

/*

3.2.3 Channel mode message

      Command: MODE
   Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )

   The MODE command is provided so that users may query and change the
   characteristics of a channel. Note that there is a maximum limit of three (3) changes per
   command for modes that take a parameter.

   The various modes available for channels are as follows:

	O - give "channel creator" status;
	o - give/take channel operator privilege;
	v - give/take the voice privilege;

	a - toggle the anonymous channel flag;
	i - toggle the invite-only channel flag;
	m - toggle the moderated channel;
	n - toggle the no messages to channel from clients on the
		outside;
	q - toggle the quiet channel flag;
	p - toggle the private channel flag;
	s - toggle the secret channel flag;
	r - toggle the server reop channel flag;
	t - toggle the topic settable by channel operator only flag;

	k - set/remove the channel key (password);
	l - set/remove the user limit to channel;

	b - set/remove ban mask to keep users out;
	e - set/remove an exception mask to override a ban mask;
	I - set/remove an invitation mask to automatically override
		the invite-only flag;

   Note: all these modes can be manipulated by "channel operators" 

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_KEYSET
           ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE
           RPL_CHANNELMODEIS
           RPL_BANLIST                     RPL_ENDOFBANLIST
           RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
           RPL_INVITELIST                  RPL_ENDOFINVITELIST
           RPL_UNIQOPIS

   The following examples are given to help understanding the syntax of
   the MODE command, but refer to modes defined in "Internet Relay Chat:
   Channel Management" [IRC-CHAN].

   Examples:

   MODE #Finnish +imI *!*@*.fi     ; Command to make #Finnish channel
                                   moderated and 'invite-only' with user
                                   with a hostname matching *.fi
                                   automatically invited.

   MODE #Finnish +o Kilroy         ; Command to give 'chanop' privileges
                                   to Kilroy on channel #Finnish.

   MODE #Finnish +v Wiz            ; Command to allow WiZ to speak on
                                   #Finnish.

   MODE #Fins -s                   ; Command to remove 'secret' flag
                                   from channel #Fins.

   MODE #42 +k oulu                ; Command to set the channel key to
                                   "oulu".

   MODE #42 -k oulu                ; Command to remove the "oulu"
                                   channel key on channel "#42".

   MODE #eu-opers +l 10            ; Command to set the limit for the
                                   number of users on channel
                                   "#eu-opers" to 10.

   :WiZ!jto@tolsun.oulu.fi MODE #eu-opers -l
                                   ; User "WiZ" removing the limit for
                                   the number of users on channel "#eu-
                                   opers".

   MODE &oulu +b                   ; Command to list ban masks set for
                                   the channel "&oulu".

   MODE &oulu +b *!*@*             ; Command to prevent all users from
                                   joining.

   MODE &oulu +b *!*@*.edu +e *!*@*.bu.edu
                                   ; Command to prevent any user from a
                                   hostname matching *.edu from joining,
                                   except if matching *.bu.edu

   MODE #bu +be *!*@*.edu *!*@*.bu.edu
                                   ; Comment to prevent any user from a
                                   hostname matching *.edu from joining,
                                   except if matching *.bu.edu

   MODE #meditation e              ; Command to list exception masks set
                                   for the channel "#meditation".

   MODE #meditation I              ; Command to list invitations masks
                                   set for the channel "#meditation".

   MODE !12345ircd O               ; Command to ask who the channel
                                   creator for "!12345ircd" is

*/

/*
User:
	Command: MODE
	Parameters: <nickname>
				*( ( "+" / "-" ) *( "i" / "w" / "o" / "O" / "r" ) )
Channel:
	Command: MODE
	Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
*/



void					CommandMode::userMode(uint nb_params, std::string params, NetworkEntity& executor)
{
	(void) executor;
	if (this->getEmitter().getFamily() != CLIENT_ENTITY_FAMILY)
			return ;
	ClientInfo *info = dynamic_cast<ClientInfo*>(&this->getEmitter());;
	/*if (this->getEmitter().getType() & Client::value_type)
		info = static_cast<ClientInfo*>(reinterpret_cast<Client*>(&this->getEmitter()));
	else
		info = static_cast<ClientInfo*>(reinterpret_cast<RelayedClient*>(&this->getEmitter()));
	*/	
	if (nb_params == 1)
	{	
		this->getServer()._sendMessage(this->getEmitter(), RPL_UMODEIS(this->getEmitter().getUID(), info->getModeString()));
		return ;
	}

	uint i = 1;
	for (; i < nb_params; i++)
	{
		std::string param = Parser::getParam(params, i);
		int			operation = 0;
		if (param[0] == '+')
			operation = +1;
		else if (param[0] == '-')
			operation = -1;
		for (std::string::iterator mode_id = param.begin(); mode_id != param.end(); ++mode_id)
		{
			if (operation == 0)
			{
				if (info->getModeBit(*mode_id) == 0 && *mode_id != '+' && *mode_id != '-')
				{
					this->getServer()._sendMessage(this->getEmitter(), ERR_UNKNOWNMODE(this->getEmitter().getUID(), *mode_id));
					continue;
				}
				std::string mode_str = info->getMode() & info->getModeBit(*mode_id) ? "+" : "-";
				mode_str.push_back(*mode_id);
				this->getServer()._sendMessage(this->getEmitter(), RPL_UMODEIS(this->getEmitter().getUID(), mode_str));
				continue;
			}
			switch (*mode_id)
			{
				//REVIEW useless since AWAY command is not implemented
				/*case 'a':
					// make away
					info->setMode(MODE_AWAY, operation);
					break;*/
				//REVIEW useless since WHO command is not implemented
				/*case 'i':
					// make invisible
					info->setMode(MODE_INVISIBLE, operation);
					break;*/
				//REVIEW useless since WALLOPS command is not implemented
				/*
				case 'w':
					//allow wallops recieving
					info->setMode(MODE_WALLOPS, operation);
					break;*/
				case 'r':
					// restrict user connection
					// REVIEW what makes a connection restricted ? (as it is implementation dependent)
					info->setMode(MODE_RESTRICTED, operation);
					break;
				case 'o':
					// global operator flag
					info->setMode(MODE_OPERATOR, operation);
					break;
				case 'O':
					// local operator flag
					// REVIEW local OP is now the same as global OP 
					info->setMode(MODE_OPERATOR, operation);
					break;
				case 's':
					// recieve server notices (deprecated)
					// REVIEW do we implement it ? (DEPRECATED ON RFC 2811)
					break;
				default:
					this->getServer()._sendMessage(this->getEmitter(), ERR_UNKNOWNMODE(this->getEmitter().getUID(), *mode_id));
					break;
			}
		}
	}
}












void					CommandMode::channelMode(uint nb_params, std::string params, NetworkEntity& executor)
{
	(void) executor;
	std::string chan_name = Parser::getParam(params, 0);
	if (this->getServer()._channels.count(chan_name) != 1)
	{
		Logger::critical("Channel in _enitites not in _channels list");
		return;
	}
	ClientInfo *cli_info = dynamic_cast<ClientInfo*>(&this->getEmitter());

	// TODO IMPL -> bool ClientInfo::isRegisteredOn(const std::string &channel_name) const
	int found = 0;
	ChannelInfo *info;
	Channel		*chan;
	for (std::list<Channel*>::const_iterator it = cli_info->getChannels().begin(); it != cli_info->getChannels().end(); ++it)
	{
		if ((*it)->getUID() == chan_name)
		{
			chan = *it;
			info = dynamic_cast<ChannelInfo*>(chan);
			found = 1;
			break;
		}
	}
	// TODO END-IMPL

	if (!found)
	{
		this->getServer()._sendMessage(this->getEmitter(), ERR_USERNOTINCHANNEL(this->getEmitter().getUID(), this->getEmitter().getUID(), this->getEmitter().getUID()));
		return ;
	}
	
	if (nb_params == 1)
	{	
		// TODO set mode args when implemented (key value, banmask value, etc...)
		this->getServer()._sendMessage(this->getEmitter(), RPL_CHANNELMODEIS(this->getEmitter().getUID(), chan->getUID(), info->getModeString(), ""));
		return ;	
	}

	uint i = 1;
	for (; i < nb_params; i++)
	{
		std::string param = Parser::getParam(params, i);
		int			operation = 0;
		if (param[0] == '+')
			operation = +1;
		else if (param[0] == '-')
			operation = -1;
		for (std::string::iterator mode_id = param.begin(); mode_id != param.end(); ++mode_id)
		{
			if (operation == 0)
			{
				if (info->getModeBit(*mode_id) == 0 && *mode_id != '+' && *mode_id != '-')
				{
					this->getServer()._sendMessage(this->getEmitter(), ERR_UNKNOWNMODE(this->getEmitter().getUID(), *mode_id));
					continue;
				}
				// TODO set mode args when implemented (key value, banmask value, etc...)
				std::string mode_str = info->getMode() & info->getModeBit(*mode_id) ? "+" : "-";
				mode_str.push_back(*mode_id);
				this->getServer()._sendMessage(this->getEmitter(), RPL_CHANNELMODEIS(this->getEmitter().getUID(), chan->getUID(), mode_str, ""));
				continue;
			}

			switch (*mode_id)
			{
			
			case 'O':
				//REVIEW complex mode needs flagged variable
				//info->setMode(, operation);				
				break;
			/*case 'o':
				//REVIEW complex mode needs flagged variable
				//info->setMode(, operation);				
				break;
			case 'v':
				//REVIEW complex mode needs flagged variable
				//info->setMode(, operation);
				break;
			case 'a':
				info->setMode(MODE_CHAN_ANONYMOUS, operation);
				break;
			case 'i':
				info->setMode(MODE_CHAN_INVITEONLY, operation);
				break;
			case 'm':
				info->setMode(MODE_CHAN_MODERATED, operation);
				break;
			case 'n':
				info->setMode(MODE_CHAN_LOCALONLY, operation);
				break;
			case 'q':
				info->setMode(MODE_CHAN_QUIET, operation);
				break;
			case 'p':
				info->setMode(MODE_CHAN_PRIVATE, operation);
				break;
			case 's':
				info->setMode(MODE_CHAN_SECRET, operation);
				break;
			case 'r':
				info->setMode(MODE_CHAN_SERVREOP, operation);
				break;*/
			case 't':
				info->setMode(MODE_CHAN_TOPICOPONLY, operation);
				break;
			/*case 'k':
				//REVIEW complex mode needs flagged variable
				info->setMode(MODE_CHAN_HASKEY, operation);
				break;
			case 'l':
				//REVIEW complex mode needs flagged variable
				info->setMode(MODE_CHAN_HASLIMIT, operation);
				break;
			case 'b':
				//REVIEW complex mode needs flagged variable
				info->setMode(MODE_CHAN_HASBANMASK, operation);
				break;
			case 'e':
				//REVIEW complex mode needs flagged variable
				info->setMode(MODE_CHAN_HASBANEXCEPTIONS, operation);
				break;
			case 'I':
				//REVIEW complex mode needs flagged variable
				info->setMode(MODE_CHAN_HASINVITATIONSMASK, operation);
				break;*/
			default:
				this->getServer()._sendMessage(this->getEmitter(), ERR_UNKNOWNMODE(this->getEmitter().getUID(), *mode_id));
				break;
			}
		}
	}
}








uint					CommandMode::operator()(NetworkEntity & executor, std::string params)
{
	uint nbParam = Parser::nbParam(params);
	if (nbParam == 0)
	{
		this->getServer()._sendMessage(this->getEmitter(), ERR_NEEDMOREPARAMS(this->getEmitter().getUID(), std::string("MODE")));
		return SUCCESS;
	}
	std::string uid = Parser::getParam(params, 0);
	if (this->getServer()._entities.count(uid) == 0)
	{
		this->getServer()._sendMessage(this->getEmitter(), ERR_NOSUCHNICK(this->getEmitter().getUID(), uid));
		return SUCCESS;
	}
	
	AEntity *e = this->getServer()._entities[uid];
	if (e->getUID() != this->getEmitter().getUID() && ~e->getType() & Channel::value_type)
	{
		this->getServer()._sendMessage(this->getEmitter(), ERR_USERSDONTMATCH(this->getEmitter().getUID()));
		return SUCCESS;
	}

	if (e->getType() & Channel::value_type)
		this->channelMode(nbParam, params, executor);
	else
		this->userMode(nbParam, params, executor);
	
	return (SUCCESS);




	/* OLD */
	/*
	uint nbParam = Parser::nbParam(params);
	if (nbParam == 0)
	{
		this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("MODE")));
		return SUCCESS;
	}
	std::string uid = Parser::getParam(params, 0);
	if (this->getServer()._entities.count(uid) == 0)
	{
		this->getServer()._sendMessage(executor, ERR_NOSUCHNICK(executor.getUID(), uid));
		return SUCCESS;
	}
	else if (this->getServer()._channels.count(uid) == 1)
	{
		if (nbParam < 2)
		{
			//this->getServer()._sendMessage(executor, ERR_NEEDMOREPARAMS(executor.getUID(), std::string("MODE")));
			this->getServer()._sendMessage(executor, RPL_UMODEIS(this->getEmitter().getUID(), static_cast<Client &>(this->getEmitter()).getModeString()));
			return SUCCESS;
		}		
		this->modeForChannel(nbParam, executor, params);
	}
	else
		this->modeForUser(executor, uid, Parser::getParam(params, 1));
	return SUCCESS;
	*/
}



/*
** --------------------------------- METHODS ----------------------------------
*/

bool				CommandMode::hasPermissions(AEntity & executor)
{
	if (executor.getType() & UnRegisteredConnection::value_type)
		return false;
	else if (this->getEmitter().getFamily() == SERVER_ENTITY_FAMILY)
	{
		Logger::debug("Access denied because of server type");
		return false;
	}
	return true;
}

void				CommandMode::modeForUser(NetworkEntity & executor, std::string uid, std::string mode)
{
	if (this->getEmitter().getUID() != uid)
	{
		Logger::debug("Njoin: " + this->getEmitter().getUID() + " Change mode for the other user: " + uid);
		this->getServer()._sendMessage(executor, this->getServer().getPrefix() + ERR_USERSDONTMATCH(this->getEmitter().getUID()));
		return ;
	}
	else if (mode.empty())
	{
		//Query about mode of client
		if (executor.getType() & Client::value_type)
			this->getServer()._sendMessage(executor, RPL_UMODEIS(this->getEmitter().getUID(), static_cast<Client &>(this->getEmitter()).getModeString()));
		else
			Logger::critical("Mode: unhandled type");
		return ;
	}
	else
	{
		Logger::warning("TODO handle adding a user mode");
	}
	return ;
}

void				CommandMode::modeForChannel(uint nbParam, NetworkEntity & executor, std::string params)
{
	Logger::warning("TODO mode for channel");
	//TODO Check if mode is valid
	
	std::string uid = Parser::getParam(params, 0);
	if (this->getServer()._channels.count(uid) == 0)
	{
		this->getServer()._sendMessage(executor, ERR_NOSUCHCHANNEL(executor.getUID(), uid));
		return ;
	}
	std::string mode = Parser::getParam(params, 2);
	Channel *chan = this->getServer()._channels[uid];
	if (mode.find("O") != std::string::npos && nbParam >= 3)
		chan->setCreator(this->getEmitter());
	return ;
}