#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string>

namespace rbn
{
class Network
{
private:
	std::vector< bool > nodes_ ;
	std::vector< std::vector< size_t >> connections_ ;
	std::vector< bool > lookup_ ;

public:
	Network(const std::vector< bool > & nodes,const std::vector<std::vector<size_t>> & connections, const std::vector< bool > &lookup);
	Network( const size_t numberNodes , const size_t connectionsPerNode , const int seed );
	Network(const Network &);
	const std::vector< bool > & nodes() const ;
	const std::vector<std::vector<size_t>> & connections() const;
	const std::vector< bool > & lookUp() const ;
};
Network update( const Network & );
std::string show(const Network & ,const char ,const char);
std::string run(Network,const unsigned int steps); //copy intentional
}
#endif
