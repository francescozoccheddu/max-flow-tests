#include <max-flow/solve.hpp>

#include <max-flow/solvers/labeling.hpp>
#include <stdexcept>

using namespace MaxFlow::Graphs;

namespace MaxFlow
{

	constexpr void (*solverFunc (ESolver _solver))(ResidualGraph&, ResidualVertex&, ResidualVertex&)
	{
		switch (_solver)
		{
			case MaxFlow::ESolver::Labeling:
				return Solvers::labeling;
			default:
				throw std::invalid_argument{ "unknown solver" };
		}
	}

	void solve (ResidualGraph& _residualGraph, ResidualVertex& _source, ResidualVertex& _sink, ESolver _solver)
	{
		Base::Graph::ensureSameGraph (_residualGraph, _source.graph ());
		Base::Graph::ensureSameGraph (_residualGraph, _sink.graph ());
		solverFunc (_solver)(_residualGraph, _source, _sink);
	}

}