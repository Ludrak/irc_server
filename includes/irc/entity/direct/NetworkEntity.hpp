
#ifndef NETWORK_ENTITY
# define NETWORK_ENTITY

# include "AEntity.hpp"
# include "SockStream.hpp"

class NetworkEntity : public AEntity
{
    public:
        static const uint	value_type = NETWORK_ENTITY_TYPE;

    public:
        NetworkEntity(SockStream &stream, const std::string &uid, const uint type);
        virtual ~NetworkEntity();

        const SockStream    &getStream() const;
        SockStream          &getStream();
		bool				isCleanDisconnection( void ) const;
		void				setCleanDisconnection( const bool clean );

    private:
        SockStream          &_stream;
		bool				_cleanDisconnection;

};

#endif // NETWORK_ENTITY