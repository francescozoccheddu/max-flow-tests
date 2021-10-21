#include <max-flow/solvers/labeling.hpp>

using MaxFlow::Graphs::ResidualGraph;

namespace MaxFlow::Solvers
{

	void labeling (Graphs::ResidualGraph& _residualGraph, Graphs::ResidualVertex& _source, Graphs::ResidualVertex& _sink)
	{
		MaxFlow::Graphs::Base::Graph::ensureSameGraph (_residualGraph, _source.graph());
		MaxFlow::Graphs::Base::Graph::ensureSameGraph (_residualGraph, _sink.graph());

	}

}