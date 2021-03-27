#ifndef NETWORK_H
#define NETWORK_H

#include "Node.h"
#include <vector>
#include <string>

namespace rbn
{
class Network
{
private:
	std::vector< Node > nodes_ ;
	std::vector< bool > lookup_ ;

public:
	Network(const std::vector< Node > & nodes, const std::vector< bool > &lookup);
	Network( const size_t numberNodes , const size_t connectionsPerNode , const int seed );
	Network(const Network &);
	const std::vector< Node > & nodes() const ;
	const std::vector< bool > & lookUp() const ;
	
	const Node * get(const size_t) const ;
};
Network update( const Network & );
std::string show(const Network & ,const char ,const char);
std::string run(Network,const unsigned int steps); //copy intentional
}
#endif
