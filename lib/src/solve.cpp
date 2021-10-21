#include <max-flow/solve.hpp>

#include <max-flow/solvers/labeling.hpp>
#include <max-flow/solvers/capacity_scaling.hpp>
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
			default:
				throw std::invalid_argument{ "unknown solver" };
		}
	}

	void solve (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink, flow_t _maxCapacity, ESolver _solver)
	{
		Base::Graph::ensureSameGraph (_graph, _source.graph ());
		Base::Graph::ensureSameGraph (_graph, _sink.graph ());
		solverFunc (_solver)(_graph, _source, _sink, _maxCapacity);
	}

}