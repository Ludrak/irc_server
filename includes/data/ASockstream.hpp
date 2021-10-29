#ifndef ASOCKSTREAM_HPP
# define ASOCKSTREAM_HPP

# include <iostream>
# include <string>

class ASockstream
{

	public:

		ASockstream();
		ASockstream( ASockstream const & src );
		~ASockstream();

		ASockstream &		operator=( ASockstream const & rhs );

	private:
		int						_socket;

};

std::ostream &			operator<<( std::ostream & o, ASockstream const & i );

#endif /* ***************************************************** ASOCKSTREAM_H */