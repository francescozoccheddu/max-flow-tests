#include <max-flow/graph/algorithms/equality.hpp>

namespace MaxFlow::Graph::Algorithms::Equality
{

	bool EdgeComparer::operator()(const Base::Edge&, const Base::Edge&) const
	{
		return true;
	}

	bool VertexComparer::operator()(const Base::Vertex&, const Base::Vertex&) const
	{
		return true;
	}


	bool equals (const Base::Edge& _a, const Base::Edge& _b, const EdgeComparer& _edgeComparer)
	{
		return _a.from ().index () == _b.from ().index ()
			&& _a.to ().index () == _b.to ().index ()
			&& _edgeComparer (_a, _b);
	}

	bool equals (const Base::Vertex& _a, const Base::Vertex& _b, const VertexComparer& _vertexComparer, const EdgeComparer& _edgeComparer)
	{
		if (_a.index () != _b.index () || _a.outEdgesCount () != _b.outEdgesCount () || !_vertexComparer (_a, _b))
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equals (*a, *b, _edgeComparer))
			{
				return false;
			}
		}
		return true;
	}

	bool equals (const Base::Graph& _a, const Base::Graph& _b, const VertexComparer& _vertexComparer, const EdgeComparer& _edgeComparer)
	{
		if (_a.verticesCount () != _b.verticesCount () || _a.edgesCount () != _b.edgesCount ())
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equals (*a, *b, _vertexComparer, _edgeComparer))
			{
				return false;
			}
		}
		return true;
	}
}