#include <max-flow/graphs/algorithms/residual.hpp>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;

namespace MaxFlow::Graphs::Algorithms
{

	void augment (ResidualEdge& _edge, flow_t _amount, bool _removeZeroEdge)
	{
		*_edge -= _amount;
		ResidualEdge& antiparallel{ antiparallelEdgeOrCreate (_edge) };
		*antiparallel += _amount;
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

	ResidualEdge& antiparallelEdgeOrCreate (ResidualEdge& _edge)
	{
		return edgeOrCreate (_edge.to (), _edge.from ());
	}

	ResidualEdge& edgeOrCreate (ResidualVertex& _from, ResidualVertex& _to)
	{
		ResidualEdge* pEdge{ _from.outEdgeIfExists(_to) };
		return pEdge ? *pEdge : _from.addOutEdge (_to);
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

	void removeBiZeroEdges (ResidualGraph& _graph)
	{
		_graph.setMatrix (true);
		for (ResidualVertex& vertex : _graph)
		{
			ResidualGraph::EdgeIteratorFM it{ vertex.begin () };
			while (it != vertex.end ())
			{
				ResidualEdge& edge{ *it };
				ResidualEdge* pAntiparallel{ edge.antiParallelIfExists () };
				++it;
				if (!*edge && (!pAntiparallel || !**pAntiparallel))
				{
					edge.destroy ();
					if (pAntiparallel)
					{
						if (pAntiparallel == &*it)
						{
							++it;
						}
						pAntiparallel->destroy ();
					}
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
				antiparallelEdgeOrCreate (edge);
			}
		}
	}

	void augmentMax (const Pathfinder::IteratorM _start, const Pathfinder::IteratorM _end, bool _removeZeroEdge)
	{
		Graphs::flow_t minR{ std::numeric_limits<Graphs::flow_t>::max () };
		for (Pathfinder::IteratorM it{_start}; it != _end; ++it)
		{
			ResidualEdge& edge{ *it };
			if (*edge < minR)
			{
				minR = *edge;
			}
		}
		for (Pathfinder::IteratorM it{ _start }; it != _end; ++it)
		{
			ResidualEdge& edge{ *it };
			augment (edge, minR, _removeZeroEdge);
		}
	}

}