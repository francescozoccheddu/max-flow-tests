#include <max-flow/graphs/residual.hpp>

#include <utility>

namespace MaxFlow::Graphs
{

	ResidualGraph::ResidualGraph (const UnderlyingGraph& _graph) : m_graph{_graph}
	{}

	ResidualGraph::ResidualGraph (UnderlyingGraph&& _graph) : m_graph{ std::move(_graph) }
	{}

	const ResidualGraph::UnderlyingGraph& ResidualGraph::graph () const
	{
		return m_graph;
	}

}