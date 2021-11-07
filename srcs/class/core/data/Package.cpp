# include "Package.hpp"

Package::Package( const IProtocol &protocol, const std::string &data, const SockStream *recipient )
throw(Package::SizeExeededException, Package::InvalidProtocolException) : _recipient(recipient)
{
    this->_protocol = &protocol;
    if (!data.empty())
        this->addData(data);
    else
        this->_is_invalid = true;
}

Package::Package( Package const & src )
{
    *this = src;
}

Package::~Package()
{
}

Package&		Package::operator=( Package const & rhs )
{
    if (this == &rhs)
        return (*this);
    this->_data = rhs._data;
    this->_is_invalid = rhs._is_invalid;
    this->_protocol = rhs._protocol;
    return (*this);
}

bool			Package::isInvalid( void ) const
{
    return (this->_is_invalid);
}

void			Package::flush( void )
{
    size_t pk_sz = this->_protocol->checkFormat(this->_data);
    this->nflush(pk_sz);
    this->_checksum();
}

void			Package::nflush( uint n )
{
    if (n != 0)
        this->_data = this->_data.substr(n, this->_data.size() - n);
}

bool			Package::_checksum( void )
{
    this->_is_invalid = (this->_protocol->checkFormat(this->_data) == 0);
    return (this->_is_invalid);
}

void			Package::addData( const std::string &new_data ) throw(Package::SizeExeededException)
{
    if (this->_data.size() + new_data.size() > this->_protocol->getMaximumPackageSize())
        throw Package::SizeExeededException();
    this->_data += new_data;
    this->_checksum();
}

std::string		Package::getData( void ) const
{
	return (this->_protocol->getData(this->getRawData()));
}

std::string		Package::getRawData( void ) const
{
    size_t pk_sz = this->_protocol->checkFormat(this->_data);
    if (pk_sz == 0 && !this->_data.empty())
    {
        std::cerr << "FATAL: invalid protocol" << std::endl;
        return ("error");
    }
    if (pk_sz == 0)
        return (this->_data);
    return (this->_data.substr(0, pk_sz));
}

void			Package::setRecipient( const SockStream *recipient )
{
    this->_recipient = recipient;
}

SockStream      *Package::getRecipient( void ) const
{
    return (const_cast<SockStream*>(this->_recipient));
}
