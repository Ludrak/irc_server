#ifndef CHANNELINFO_HPP
# define CHANNELINFO_HPP

# include "CommonInfo.hpp"

class ChannelInfo
{
	public:
		bool		isFull( void ) const;
		uint		getConcurrentClients( void ) const; //REVIEW rename? 
		uint		getConcurrentClientsMax( void ) const; //REVIEW rename?
		bool		incrementJoinedClients();
		bool		decrementJoinedClients();

		/* mode */
		void		toogleMode( uint modeMask );
		void		enableMode( uint modeMask );
		void		disableMode( uint modeMask );
		bool		isEnable( uint modeMask ) const;

		//TODO add inc et dec //REVIEW not here

	protected:
		uint		_mode;
		uint		_concurrentClients;
		uint		_concurrentClientsMax;
};

#endif //CHANNELINFO_HPP