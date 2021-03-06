#include <max-flow/solve.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <max-flow/solvers/labeling/capacity_scaling.hpp>
#include <max-flow/solvers/labeling/ford_fulkerson.hpp>
#include <max-flow/solvers/labeling/shortest_path.hpp>
#include <max-flow/solvers/preflow_push/naif.hpp>
#include <max-flow/solvers/preflow_push/fifo.hpp>
#include <max-flow/solvers/preflow_push/highest_label.hpp>
#include <max-flow/solvers/preflow_push/excess_scaling.hpp>
#include <stdexcept>
#include <vector>

using namespace MaxFlow::Graphs;

namespace MaxFlow
{

	ESolverFlags operator+(ESolverFlags _a, ESolverFlags _b)
	{
		return static_cast<ESolverFlags>(
			static_cast<std::underlying_type<ESolverFlags>::type>(_a) |
			static_cast<std::underlying_type<ESolverFlags>::type>(_b)
			);
	}

	bool operator&(ESolverFlags _a, ESolverFlags _b)
	{
		return
			static_cast<std::underlying_type<ESolverFlags>::type>(_a) &
			static_cast<std::underlying_type<ESolverFlags>::type>(_b)
			;
	}

	Solver* createSolver(Graphs::ResidualGraph& _graph, Graphs::ResidualVertex& _source, Graphs::ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix, ESolver _solver, ESolverFlags _flags)
	{
		ResidualGraph::ensureSameGraph(_graph, _source.graph(), _sink.graph());
		Solver* pSolver;
		switch (_solver)
		{
		case MaxFlow::ESolver::FordFulkerson:
		{
			auto pFFSolver = new Solvers::Labeling::FordFulkersonSolver{ _graph, _source, _sink, _capacityMatrix };
			pFFSolver->setDepthFirst(_flags & ESolverFlags::FordFulkersonDepthFirst);
			pSolver = pFFSolver;
			break;
		}
		case MaxFlow::ESolver::CapacityScalingFordFulkerson:
		case MaxFlow::ESolver::CapacityScalingShortestPath:
		{
			auto pCSSolver{ new Solvers::Labeling::CapacityScalingSolver{ _graph, _source, _sink, _capacityMatrix } };
			switch (_solver)
			{
			case MaxFlow::ESolver::CapacityScalingFordFulkerson:
				pCSSolver->setSubSolver(Solvers::Labeling::CapacityScalingSolver::ESubSolver::FordFulkerson);
				break;
			case MaxFlow::ESolver::CapacityScalingShortestPath:
				pCSSolver->setSubSolver(Solvers::Labeling::CapacityScalingSolver::ESubSolver::ShortestPath);
				break;
			}
			pCSSolver->setRemoveDeltaEdges(_flags & ESolverFlags::CapacityScalingRemoveDeltaEdges);
			pCSSolver->setMinCutDetection(_flags & ESolverFlags::ShortestPathDetectMinCut);
			pCSSolver->setDepthFirst(_flags & ESolverFlags::FordFulkersonDepthFirst);
			pSolver = pCSSolver;
			break;
		}
		case MaxFlow::ESolver::ShortestPath:
		{
			auto pSPSolver = new Solvers::Labeling::ShortestPathSolver{ _graph, _source, _sink, _capacityMatrix };
			pSPSolver->setMinCutDetection(_flags & ESolverFlags::ShortestPathDetectMinCut);
			pSolver = pSPSolver;
			break;
		}
		case MaxFlow::ESolver::NaifPreflowPush:
		{
			pSolver = new Solvers::PreflowPush::NaifPreflowPushSolver{ _graph, _source, _sink, _capacityMatrix };
			break;
		}
		case MaxFlow::ESolver::FifoPreflowPush:
		{
			pSolver = new Solvers::PreflowPush::FifoPreflowPushSolver{ _graph, _source, _sink, _capacityMatrix };
			break;
		}
		case MaxFlow::ESolver::HighestLabelPreflowPush:
		{
			pSolver = new Solvers::PreflowPush::HighestLabelPreflowPushSolver{ _graph, _source, _sink, _capacityMatrix };
			break;
		}
		case MaxFlow::ESolver::ExcessScalingPreflowPush:
		{
			pSolver = new Solvers::PreflowPush::ExcessScalingPreflowPushSolver{ _graph, _source, _sink, _capacityMatrix };
			break;
		}
		default:
			throw std::invalid_argument{ "unknown solver" };
		}
		pSolver->setRemoveZeroEdges(_flags & ESolverFlags::RemoveZeroEdgeLabels);
		return pSolver;
	}

	void solve(ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix, ESolver _solver, ESolverFlags _flags)
	{
		Solver* pSolver{ createSolver(_graph, _source, _sink, _capacityMatrix, _solver, _flags) };
		pSolver->solve();
		delete pSolver;
	}

	Solver::Solver(ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix)
		: m_graph{ _graph }, m_source{ _source }, m_sink{ _sink }, m_capacities{ _capacityMatrix }, m_areZeroEdgesRemoved{ false }
	{
		ResidualGraph::ensureSameGraph(_graph, _source.graph(), _sink.graph());
		if (_source == _sink)
		{
			throw std::logic_error{ "source == sink" };
		}
	}

	const ResidualGraph& Solver::graph() const
	{
		return m_graph;
	}

	ResidualGraph& Solver::graph()
	{
		return m_graph;
	}

	const ResidualVertex& Solver::source() const
	{
		return m_source;
	}

	ResidualVertex& Solver::source()
	{
		return m_source;
	}

	const ResidualVertex& Solver::sink() const
	{
		return m_sink;
	}

	ResidualVertex& Solver::sink()
	{
		return m_sink;
	}

	const CapacityMatrix& Solver::capacities() const
	{
		return m_capacities;
	}

	void Solver::setRemoveZeroEdges(bool _removeZeroEdges)
	{
		m_areZeroEdgesRemoved = _removeZeroEdges;
	}

	bool Solver::areZeroEdgesRemoved() const
	{
		return m_areZeroEdgesRemoved;
	}

	void Solver::solve()
	{
		m_graph.setMatrix(true);
		if (areZeroEdgesRemoved())
		{
			Algorithms::removeZeroEdges(m_graph);
		}
		/*
		else
		{
			Algorithms::removeBiZeroEdges(m_graph);
		}
		*/
		solveImpl();
	}

	flow_t CapacityMatrix::operator[](const Base::Edge& _edge) const
	{
		return capacity(_edge.from().index(), _edge.to().index());
	}

}