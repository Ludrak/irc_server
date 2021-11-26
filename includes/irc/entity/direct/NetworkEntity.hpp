
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

        SockStream          &getStream() const;

    private:
        SockStream          &_stream;
};

#endif // NETWORK_ENTITY