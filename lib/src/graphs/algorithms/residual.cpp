#include <max-flow/graphs/algorithms/residual.hpp>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;

namespace MaxFlow::Graphs::Algorithms
{

	void augment (ResidualEdge& _edge, flow_t _amount, bool _removeZeroEdge)
	{
		*_edge += _amount;
		ResidualEdge& antiparallel{ antiparallelEdge (_edge) };
		*antiparallel -= _amount;
		if (_removeZeroEdge)
		{
			if (!*_edge)
			{
				_edge.destroy ();
			}
			if (!*antiparallel)
			{
				antiparallel.destroy ();
			}
		}
	}

	flow_t antiparallelCapacity (const ResidualEdge& _edge)
	{
		const ResidualEdge* pAntiparallel{ _edge.antiParallelIfExists () };
		return pAntiparallel ? **pAntiparallel : 0;
	}

	ResidualEdge& antiparallelEdge (ResidualEdge& _edge)
	{
		ResidualEdge* pAntiparallel{ _edge.antiParallelIfExists () };
		return pAntiparallel ? *pAntiparallel : _edge.to ().addOutEdge (_edge.from ());
	}

	void removeZeroEdges (ResidualGraph& _graph)
	{
		for (ResidualVertex& vertex : _graph)
		{
			ResidualGraph::EdgeIteratorFM it{ vertex.begin () };
			while (it != vertex.end ())
			{
				ResidualEdge& edge{ *it };
				++it;
				if (!*edge)
				{
					edge.destroy ();
				}
			}
		}
	}

	void addZeroEdges (ResidualGraph& _graph)
	{
		_graph.setMatrix (true);
		for (ResidualVertex& vertex : _graph)
		{
			for (ResidualEdge& edge : vertex)
			{
				antiparallelEdge (edge);
			}
		}
	}

}