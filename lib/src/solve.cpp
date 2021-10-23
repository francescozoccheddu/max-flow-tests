#include <max-flow/solve.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/solvers/labeling.hpp>
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
		std::vector<Solver*> allocatedSolvers;
		switch (_solver)
		{
			case MaxFlow::ESolver::FordFulkerson:
			{
				auto pFFSolver{ new Solvers::FordFulkersonSolver{ _graph, _source, _sink, _capacityMatrix } };
				allocatedSolvers.push_back (pFFSolver);
				pSolver = pFFSolver;
				break;
			}
			case MaxFlow::ESolver::CapacityScalingFordFulkerson:
			{
				auto pCSSolver{ new Solvers::CapacityScalingSolver{ _graph, _source, _sink, _capacityMatrix } };
				auto pCSSubSolver = new Solvers::FordFulkersonSolver{ _graph, _source, _sink, _capacityMatrix };
				pCSSubSolver->setRemoveZeroEdges (_removeZeroEdges);
				allocatedSolvers.push_back (pCSSolver);
				allocatedSolvers.push_back (pCSSubSolver);
				pCSSolver->setSolver (*pCSSubSolver);
				pSolver = pCSSolver;
				break;
			}
			case MaxFlow::ESolver::CapacityScalingShortestPath:
			{
				auto pCSSolver{ new Solvers::CapacityScalingSolver{ _graph, _source, _sink, _capacityMatrix } };
				auto pSPSubSolver = new Solvers::ShortestPathSolver{ _graph, _source, _sink, _capacityMatrix };
				pSPSubSolver->setRemoveZeroEdges (_removeZeroEdges);
				allocatedSolvers.push_back (pCSSolver);
				allocatedSolvers.push_back (pSPSubSolver);
				pCSSolver->setSolver (*pSPSubSolver);
				pSolver = pCSSolver;
				break;
			}
			case MaxFlow::ESolver::ShortestPath:
			{
				auto pSPSolver{ new Solvers::ShortestPathSolver{ _graph, _source, _sink, _capacityMatrix } };
				allocatedSolvers.push_back (pSPSolver);
				pSolver = pSPSolver;
				break;
			}
			default:
				throw std::invalid_argument{ "unknown solver" };
		}
		pSolver->setRemoveZeroEdges (_removeZeroEdges);
		for (Solver* pAllocatedSolver : allocatedSolvers)
		{
			delete pAllocatedSolver;
		}
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