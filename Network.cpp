#include "Network.h"
#include <random>
#include <algorithm>
#include <sstream>
#include "boost/range/irange.hpp"

namespace rbn
{
Network::Network(const std::vector< bool > &nodes,const std::vector<std::vector<size_t>>&connections, const std::vector< bool > &lookup)
	: nodes_(nodes),connections_(connections),lookup_(lookup){}

Network::Network(const Network & n)
	:nodes_(n.nodes_),connections_(n.connections_),lookup_(n.lookup_){}

static size_t power( const size_t base , const size_t index )
{
	return index == 0 ? 1 : (base * power( base , index - 1 ));
}

Network::Network( const size_t numberNodes , const size_t connectionsPerNode , const int seed )
	: nodes_( numberNodes , false ),
		connections_(numberNodes,std::vector<size_t>(connectionsPerNode,0)),
		lookup_(power(2,connectionsPerNode),false)
{
	std::mt19937 generator(seed);
	std::bernoulli_distribution dist(0.5);
	//init lookup
	{
		std::generate(lookup_.begin(),lookup_.end(),[&](){return dist(generator);});
	}

	for( size_t it = 0 ,endIt = nodes_.size() ; it != endIt ; ++it )
	{
		nodes_[it] = dist(generator);

		//randomly select other nodes to connect
		std::vector< size_t > selected ;
		{
			const auto range = boost::irange(size_t(0), nodes_.size());
			std::vector< size_t > indices(boost::begin(range),boost::end(range));
			const auto end = std::stable_partition(indices.begin(),indices.end(),[=](const size_t x){ return x != it;});
			selected.reserve( connectionsPerNode );
			std::sample(indices.begin(),end,std::back_inserter(selected),connectionsPerNode,generator);
			std::shuffle(selected.begin(),selected.end(),generator);
		}
		connections_[it]=selected;
	}
}

const std::vector< bool > & Network::nodes() const
{
	return nodes_ ;
}

const std::vector<std::vector<size_t>> &Network::connections() const
{
	return connections_;
}

const std::vector<bool> & Network::lookUp() const
{
	return lookup_ ;
}

namespace utility
{
using PowerAndAcc = std::pair< unsigned int , unsigned int > ;
static PowerAndAcc addPower( const PowerAndAcc & acc , const bool & add )
{
	return add ? PowerAndAcc( acc.first << 1 , acc.second + acc.first ) : PowerAndAcc( acc.first << 1 , acc.second );
}

static unsigned int toNum(const std::vector<bool> &state)
{
	const PowerAndAcc out = std::accumulate(state.begin(),state.end(),PowerAndAcc(1,0),addPower);
	return out.second ;
}
}

static bool newValueAt( const size_t index , const Network & old )
{
	const std::vector<size_t> & connections = old.connections()[index];
	std::vector<bool> connectionValues ;
	std::transform( connections.begin() , connections.end() ,std::back_inserter(connectionValues),[&](const size_t it){return old.nodes()[it];});
	const unsigned int value = utility::toNum(connectionValues);
	return old.lookUp()[value] ;
}

Network update( const Network & old )
{
	std::vector<bool> nodes;
	nodes.reserve(old.nodes().size());
	const auto range = boost::irange(size_t(0),old.nodes().size());
	std::transform(boost::begin(range),boost::end(range),std::back_inserter(nodes),[&](const size_t i){return newValueAt(i,old);});

	return Network(nodes,old.connections(),old.lookUp());
}

std::string show(const Network & network,const char trueChar,const char falseChar)
{
	std::stringstream ss ;
	ss << '|';
	for( const bool node : network.nodes() )
	{
			ss << (node?trueChar:falseChar);
	}
	ss << '|';
	return ss.str();
}
std::string run(Network network,const unsigned int steps)
{
	std::stringstream ss ;
	for( unsigned int i = 0 ; i < steps ; ++i )
	{
		ss << show( network , '*', ' ') << std::endl ;
		network = update(network);
	}
	return ss.str();
}
}
