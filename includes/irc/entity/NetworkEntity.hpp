
#ifndef NETWORK_ENTITY
# define NETWORK_ENTITY

#include "AEntity.hpp"
#include "SockStream.hpp"

# define NETWORK_ENTITY_TYPE 1

class NetworkEntity : public AEntity
{
    public:
        static const int	value_type;

    public:
        NetworkEntity(SockStream &stream, const std::string &uid, const int type);
        virtual ~NetworkEntity();

        SockStream          &getStream() const;

    private:
        SockStream          &_stream;
};

const int NetworkEntity::value_type = NETWORK_ENTITY_TYPE;

#endif // NETWORK_ENTITY