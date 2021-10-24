#include <max-flow/solve.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/solvers/labeling.hpp>
#include <max-flow/solvers/capacity_scaling.hpp>
#include <max-flow/solvers/preflow_push.hpp>
#include <stdexcept>
#include <vector>

using namespace MaxFlow::Graphs;

namespace MaxFlow
{

	void solve (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix, ESolver _solver, bool _removeZeroEdges)
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph (), _sink.graph ());
		Solver* pSolver;
		switch (_solver)
		{
			case MaxFlow::ESolver::FordFulkerson:
			{
				pSolver = new Solvers::FordFulkersonSolver{ _graph, _source, _sink, _capacityMatrix };
				break;
			}
			case MaxFlow::ESolver::CapacityScalingFordFulkerson:
			{
				auto pCSSolver{ new Solvers::CapacityScalingSolver{ _graph, _source, _sink, _capacityMatrix } };
				pCSSolver->setSubSolver (Solvers::CapacityScalingSolver::ESubSolver::FordFulkerson);
				pSolver = pCSSolver;
				break;
			}
			case MaxFlow::ESolver::CapacityScalingShortestPath:
			{
				auto pCSSolver{ new Solvers::CapacityScalingSolver{ _graph, _source, _sink, _capacityMatrix } };
				pCSSolver->setSubSolver (Solvers::CapacityScalingSolver::ESubSolver::ShortestPath);
				pSolver = pCSSolver;
				break;
			}
			case MaxFlow::ESolver::ShortestPath:
			{
				pSolver = new Solvers::ShortestPathSolver{ _graph, _source, _sink, _capacityMatrix };
				break;
			}
			default:
				throw std::invalid_argument{ "unknown solver" };
		}
		pSolver->setRemoveZeroEdges (_removeZeroEdges);
		pSolver->solve ();
		delete pSolver;
	}

	Solver::Solver (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix)
		: m_graph{ _graph }, m_source{ _source }, m_sink{ _sink }, m_capacities{ _capacityMatrix }, m_areZeroEdgesRemoved{ false }
	{
		ResidualGraph::ensureSameGraph (_graph, _source.graph (), _sink.graph ());
		if (_source == _sink)
		{
			throw std::logic_error{ "source == sink" };
		}
	}

	const ResidualGraph& Solver::graph () const
	{
		return m_graph;
	}

	ResidualGraph& Solver::graph ()
	{
		return m_graph;
	}

	const ResidualVertex& Solver::source () const
	{
		return m_source;
	}

	ResidualVertex& Solver::source ()
	{
		return m_source;
	}

	const ResidualVertex& Solver::sink () const
	{
		return m_sink;
	}

	ResidualVertex& Solver::sink ()
	{
		return m_sink;
	}

	const CapacityMatrix& Solver::capacities () const
	{
		return m_capacities;
	}

	void Solver::setRemoveZeroEdges (bool _removeZeroEdges)
	{
		m_areZeroEdgesRemoved = _removeZeroEdges;
	}

	bool Solver::areZeroEdgesRemoved () const
	{
		return m_areZeroEdgesRemoved;
	}

	void Solver::solve ()
	{
		m_graph.setMatrix (true);
		if (areZeroEdgesRemoved ())
		{
			Algorithms::removeZeroEdges (m_graph);
		}
		else
		{
			Algorithms::removeBiZeroEdges (m_graph);
		}
		solveImpl ();
	}

}