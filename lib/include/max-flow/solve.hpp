#ifndef INCLUDED_MAX_FLOW_SOLVE
#define INCLUDED_MAX_FLOW_SOLVE

#include <max-flow/graphs/generic/macros.hpp>
#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>
#include <type_traits>

namespace MaxFlow
{

#pragma region Declaration

	// Classes

	enum class ESolverFlags;

	ESolverFlags operator+(ESolverFlags _a, ESolverFlags _b);
	bool operator&(ESolverFlags _a, ESolverFlags _b);

	enum class ESolverFlags
	{
		None = 0,
		RemoveZeroEdgeLabels = 1 << 0,
		CapacityScalingRemoveDeltaEdges = 1 << 1,
		ShortestPathDetectMinCut = 1 << 2,
	};

	struct CapacityMatrix
	{

		virtual Graphs::flow_t capacity (size_t _from, size_t _to) const = 0;

	};

	MF_GG_TT_F class GenericCapacityMatrix : public CapacityMatrix
	{

	private:

		const Graphs::FlowGraph<TVertexData, TEdgeData>& m_graph;

	public:


		GenericCapacityMatrix (const Graphs::FlowGraph<TVertexData, TEdgeData>& _graph);

		Graphs::flow_t capacity (size_t _from, size_t _to) const override;

	};

	class Solver
	{

	private:

		Graphs::ResidualGraph& m_graph;
		Graphs::ResidualVertex& m_source, & m_sink;
		const CapacityMatrix& m_capacities;
		bool m_areZeroEdgesRemoved;

	protected:

		virtual void solveImpl () = 0;

	public:

		Solver (Graphs::ResidualGraph& _graph, Graphs::ResidualVertex& _source, Graphs::ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix);

		const Graphs::ResidualGraph& graph () const;
		Graphs::ResidualGraph& graph ();

		const Graphs::ResidualVertex& source () const;
		Graphs::ResidualVertex& source ();

		const Graphs::ResidualVertex& sink () const;
		Graphs::ResidualVertex& sink ();

		const CapacityMatrix& capacities () const;

		void setRemoveZeroEdges (bool _removeZeroEdge);
		bool areZeroEdgesRemoved () const;

		void solve ();

	};

	// Enums

	enum class ESolver
	{
		FordFulkerson, 
		CapacityScalingFordFulkerson, 
		CapacityScalingShortestPath, 
		ShortestPath, 
		NaifPreflowPush, 
		FifoPreflowPush, 
		HighestLabelPreflowPush,
		ExcessScalingPreflowPush
	};

	constexpr ESolver defaultSolver{ ESolver::FordFulkerson };

	// Functions

	MF_GG_TT_F void solve (Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver, ESolverFlags _flags = ESolverFlags::None);

	MF_GG_TT_F Graphs::FlowGraph<TVertexData, TEdgeData> solve (const Graphs::FlowGraph<TVertexData, TEdgeData>& _graph, const  Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _source, const Graphs::FlowGraphVertex<TVertexData, TEdgeData>& _sink, ESolver _solver = defaultSolver, ESolverFlags _flags = ESolverFlags::None);

	void solve (Graphs::ResidualGraph& _graph, Graphs::ResidualVertex& _source, Graphs::ResidualVertex& _sink, const CapacityMatrix& _capacityMatrix, ESolver _solver = defaultSolver, ESolverFlags _flags = ESolverFlags::None);

#pragma endregion

#pragma region Implementation

	MF_GG_TT void solve (Graphs::FlowGraph<TVD, TED>& _graph, Graphs::FlowGraphVertex<TVD, TED>& _source, Graphs::FlowGraphVertex<TVD, TED>& _sink, ESolver _solver, ESolverFlags _flags)
	{
		Graphs::Base::Graph::ensureSameGraph (_graph, _source.graph (), _sink.graph ());
		_graph.setMatrix (true);
		Graphs::ResidualGraph residualGraph{ Graphs::createResidualGraph (_graph) };
		residualGraph.setMatrix (true);
		solve (residualGraph, residualGraph[_source.index ()], residualGraph[_sink.index ()], GenericCapacityMatrix{ _graph }, _solver, _flags);
		Graphs::updateFlowsFromResidualGraph (residualGraph, _graph);
	}

	MF_GG_TT Graphs::FlowGraph<TVD, TED> solve (const Graphs::FlowGraph<TVD, TED>& _graph, const Graphs::FlowGraphVertex<TVD, TED>& _source, const Graphs::FlowGraphVertex<TVD, TED>& _sink, ESolver _solver, ESolverFlags _flags)
	{
		Graphs::Base::Graph::ensureSameGraph (_graph, _source.graph (), _sink.graph ());
		Graphs::FlowGraph<TVD, TED> copyGraph{ _graph };
		solve (copyGraph, copyGraph[_source.index ()], copyGraph[_sink.index ()], _solver, _flags);
		return copyGraph;
	}

	MF_GG_TT GenericCapacityMatrix<TVD, TED>::GenericCapacityMatrix (const Graphs::FlowGraph<TVD, TED>& _graph) : m_graph{ _graph }
	{}

	MF_GG_TT Graphs::flow_t GenericCapacityMatrix<TVD, TED>::capacity (size_t _from, size_t _to) const
	{
		const Graphs::FlowGraphVertex<TVD, TED>& vertex{ m_graph[_from] };
		const Graphs::FlowGraphEdge<TVD, TED>* pEdge{ vertex.outEdgeIfExists (_to) };
		return pEdge ? pEdge->data ().capacity () : 0;
	}

#pragma endregion

}

#endif