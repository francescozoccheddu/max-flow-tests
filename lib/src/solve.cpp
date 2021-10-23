#include <max-flow/solve.hpp>

#include <max-flow/solvers/labeling.hpp>
#include <max-flow/solvers/capacity_scaling.hpp>
#include <max-flow/solvers/shortest_augmenting_path.hpp>
#include <max-flow/solvers/preflow_push.hpp>
#include <stdexcept>

using namespace MaxFlow::Graphs;

namespace MaxFlow
{

	constexpr void (*solverFunc (ESolver _solver)) MF_S_PL
	{
		switch (_solver)
		{
			case MaxFlow::ESolver::Labeling:
				return Solvers::labeling;
			case MaxFlow::ESolver::CapacityScaling:
				return Solvers::capacityScaling;
			case MaxFlow::ESolver::ShortestAugmentingPath:
				return Solvers::shortestAugmentingPath;
			case MaxFlow::ESolver::PreflowPush:
				return Solvers::preflowPush;
			default:
				throw std::invalid_argument{ "unknown solver" };
		}
	}

	void solve (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix, ESolver _solver)
	{
		Base::Graph::ensureSameGraph (_graph, _source.graph (), _sink.graph());
		solverFunc (_solver)(_graph, _source, _sink, _capacityMatrix);
	}

}