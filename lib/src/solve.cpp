#include <max-flow/solve.hpp>

#include <max-flow/solvers/labeling.hpp>
#include <stdexcept>

using namespace MaxFlow::Graphs;

namespace MaxFlow
{

	constexpr void (*solverFunc (ESolver _solver))(ResidualGraph&, ResidualGraph::UnderlyingGraph::Vertex&, ResidualGraph::UnderlyingGraph::Vertex&)
	{
		switch (_solver)
		{
			case MaxFlow::ESolver::Labeling:
				return Solvers::labeling;
			default:
				throw std::invalid_argument{ "unknown solver" };
		}
	}

	void solve (ResidualGraph& _residualGraph, ResidualGraph::UnderlyingGraph::Vertex& _source, ResidualGraph::UnderlyingGraph::Vertex& _sink, ESolver _solver)
	{
		Base::Graph::ensureSameGraph (_residualGraph.graph (), _source.graph ());
		Base::Graph::ensureSameGraph (_residualGraph.graph (), _sink.graph ());
		solverFunc (_solver)(_residualGraph, _source, _sink);
	}

}