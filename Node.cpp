#include "Node.h"
#include "Network.h"

#include <numeric>
#include <algorithm>

namespace rbn
{
Node &Node::add(const size_t index)
{
	connections_.push_back(index);
	return *this;
}
	
Node &Node::set(const bool b)
{
	state_ = b;
	return *this;
}

bool Node::get() const
{
	return state_ ;
}

const std::vector< size_t > & Node::connections() const
{
	return connections_ ;
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

unsigned int Node::value( const Network & network ) const
{
	std::vector<bool> values;
	values.reserve(connections_.size());
	std::transform(connections_.begin(),connections_.end(),std::back_inserter(values),[&](const size_t index){ return network.get(index)->get();});
	return utility::toNum(values);
}
}
