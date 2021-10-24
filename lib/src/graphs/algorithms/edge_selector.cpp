#include <max-flow\graphs\algorithms\edge_selector.hpp>
#include <max-flow\graphs\residual.hpp>


namespace MaxFlow::Graphs::Algorithms
{

	EdgeSelector EdgeSelector::all{};

	bool EdgeSelector::operator() (const ResidualEdge& _edge)
	{
		return true;
	}

}