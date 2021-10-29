#ifndef PACKAGE_HPP
# define PACKAGE_HPP

# include <iostream>
# include <string>

template<typename T>
class Package
{

	public:

		Package();
		Package( Package const & src );
		~Package();

		Package &		operator=( Package const & rhs );

	private:

};

template<typename T>
std::ostream &			operator<<( std::ostream & o, Package<T> const & i );

#endif /* ********************************************************* PACKAGE_H */