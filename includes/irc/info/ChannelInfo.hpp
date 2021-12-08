#ifndef CHANNELINFO_HPP
# define CHANNELINFO_HPP

# include "CommonInfo.hpp"

#define MODE_CHAN_CREATOR				(1 << 0)
#define MODE_CHAN_OP					(1 << 1)
#define MODE_CHAN_ANONYMOUS				(1 << 2)
#define MODE_CHAN_INVITEONLY			(1 << 3)
#define MODE_CHAN_MODERATED				(1 << 4)
#define MODE_CHAN_LOCALONLY				(1 << 5)
#define MODE_CHAN_QUIET					(1 << 6)
#define MODE_CHAN_PRIVATE				(1 << 7)
#define MODE_CHAN_SECRET				(1 << 8)
#define MODE_CHAN_SERVREOP				(1 << 9)
#define MODE_CHAN_TOPICOPONLY			(1 << 10)
#define MODE_CHAN_HASKEY				(1 << 11)
#define MODE_CHAN_HASLIMIT				(1 << 12)
#define MODE_CHAN_HASBANMASK			(1 << 13)
#define MODE_CHAN_HASBANEXCEPTIONS		(1 << 14)
#define MODE_CHAN_HASINVITATIONSMASK	(1 << 15)

class ChannelInfo
{
	public:
		bool		isFull( void ) const;
		uint		getConcurrentClients( void ) const; 
		uint		getConcurrentClientsMax( void ) const; 
		void		setConcurrentClientsMax( const uint new_max ); 
		bool		incrementJoinedClients();
		bool		decrementJoinedClients();

		/* mode */
		void		toogleMode( uint modeMask );
		void		enableMode( uint modeMask );
		void		disableMode( uint modeMask );
		bool		hasMode( uint modeMask ) const;
		void		setMode(uint modeMask, int set);
		uint		getMode(void) const;
		uint		getModeBit(const char c) const;
		std::string	getModeString() const;

	protected:
		uint		_mode;
		uint		_concurrentClients;
		uint		_concurrentClientsMax;
};

#endif //CHANNELINFO_HPP