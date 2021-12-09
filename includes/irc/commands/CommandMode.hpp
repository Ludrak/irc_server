#ifndef COMMANDMODE_HPP
# define COMMANDMODE_HPP

# include "ACommand.hpp"

# include "Client.hpp"
# include "Server.hpp"

# define USEROP_AWAY				0
# define USEROP_INVISIBLE			1
# define USEROP_WALLOPS				2
# define USEROP_RESTRICTED			3
# define USEROP_OPERATOR			4
# define USEROP_LOCALOPER			5
# define USEROP_RECVSERVNOTICE		6

# define CHANOP_CREATOR				0
# define CHANOP_OPERATOR			1
# define CHANOP_VOICEON				2
# define CHANOP_ANONYMOUS			3
# define CHANOP_INVITEONLY			4
# define CHANOP_MODERATED			5
# define CHANOP_LOCALONLY			6
# define CHANOP_QUIET				7
# define CHANOP_PRIVATE				8
# define CHANOP_SECRET				9
# define CHANOP_SERVERREOP			10
# define CHANOP_TOPICOPONLY			11
# define CHANOP_HASKEY 				12
# define CHANOP_HASLIMIT			13
# define CHANOP_HASBANMASK			14
# define CHANOP_HASBANEXCEPTIONS	15
# define CHANOP_HASINVITATIONMASK	16

class CommandMode : public ACommand
{

	public:
		typedef void (CommandMode::*modeOperation)(int op, ClientInfo& info, const std::string& key, const AEntity *option);

		typedef struct	s_oper {
			int				operation;
			int				action;
			std::string		key;
			AEntity			*target;
			AEntity			*option;
		}				t_oper;

		CommandMode(CommandHandler & handler);
		~CommandMode();

		uint				operator()(NetworkEntity & executor, std::string params);

		bool				hasPermissions(AEntity & executor);

	private:
		void				_userMode(uint nb_params, std::string params, NetworkEntity& executor);
		void				_channelMode(uint nb_params, std::string params, NetworkEntity& executor);

		void				_usrModeAway(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_usrModeInvisible(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_usrModeWallops(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_usrModeRestricted(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_usrModeOperator(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_usrModeLocalOperator(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_usrModeRecvServNotice(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL); // deprecated

		void				_chanModeCreator(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeOperator(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeVoiceOn(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeAnonymous(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeInviteOnly(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeModerated(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeLocalOnly(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeQuiet(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModePrivate(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeSecret(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeServerReOP(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeTopicOPOnly(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeHasKey(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeHasLimit(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeHasBanMask(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeHasBanExceptions(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);
		void				_chanModeHasInvitationMask(int operation, ClientInfo& info, const std::string& key="", const AEntity *option=NULL);

		modeOperation chanOperations[17]; 
		modeOperation userOperations[7];
};

#endif /* ***************************************************** COMMANDMODE_HPP */