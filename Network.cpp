#include "Network.h"
#include <random>
#include <algorithm>
#include <sstream>

namespace rbn
{
Network::Network(const std::vector< Node > &nodes, const std::vector< bool > &lookup)
	: nodes_(nodes),lookup_(lookup){}
	
Network::Network(const Network & n)
	:nodes_(n.nodes_),lookup_(n.lookup_){}
	
Network::Network( const size_t numberNodes , const size_t connectionsPerNode , const int seed )
	: nodes_( numberNodes , Node() )
{
	std::mt19937 generator(seed);
	std::bernoulli_distribution dist(0.5);
	//init lookup
	{
		const size_t lookupSize = 2 * connectionsPerNode ;
		lookup_=std::vector<bool>(lookupSize,false);
		std::generate(lookup_.begin(),lookup_.end(),[&](){return dist(generator);});
	}
	
	for( size_t it = 0 ,endIt = nodes_.size() ; it != endIt ; ++it )
	{
		Node & node = nodes_[it];
		//set it's value
		node.set(dist(generator));
		
		//randomly select other nodes to connect
		std::vector< size_t > selected ;
		{
			std::vector< size_t > indices(endIt,0);
			std::iota(indices.begin(),indices.end(),0);
			const auto end = std::stable_partition(indices.begin(),indices.end(),[=](const size_t x){ return x != it;});
			selected.reserve( connectionsPerNode );
			std::sample(indices.begin(),end,std::back_inserter(selected),connectionsPerNode,generator);
			std::shuffle(selected.begin(),selected.end(),generator);
		}
		std::for_each(selected.begin(),selected.end(),[&](const size_t x){ node.add(x); });
	}
}

const std::vector< Node > & Network::nodes() const
{
	return nodes_ ;
}

const std::vector<bool> & Network::lookUp() const
{
	return lookup_ ;
}

const Node * Network::get( const size_t index ) const
{
	return index < nodes_.size() ? &(nodes_[index]) : nullptr ;
}

Network update( const Network & old )
{
	std::vector< Node > nodes = old.nodes() ;
	
	for( auto & node : nodes )
	{
		const unsigned int value = node.value( old );
		node.set( old.lookUp()[value] );
	}
	
	return Network(nodes,old.lookUp());
}

std::string show(const Network & network,const char trueChar,const char falseChar)
{
	std::stringstream ss ;
	ss << '|';
	for( const rbn::Node & node : network.nodes() )
	{
			ss << (node.get()?trueChar:falseChar); 
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
