#ifndef NODE_H
#define NODE_H

#include <vector>

namespace rbn
{
class Network ;
class Node
{
private:
	std::vector< size_t> connections_ ;
	bool state_ ;
public:
	Node &add(const size_t v);
	Node &set(const bool b);
	
	bool get() const;
	const std::vector< size_t > & connections() const;
	
	unsigned int value( const Network & ) const;
};
}

#endif
