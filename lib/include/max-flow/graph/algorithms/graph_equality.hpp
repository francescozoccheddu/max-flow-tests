#ifndef INCLUDED_MAX_FLOW_GRAPH_EQUALITY
#define INCLUDED_MAX_FLOW_GRAPH_EQUALITY

#include <max-flow/utils/graph.hpp>
#include <functional>
#include <type_traits>


namespace MaxFlow::Utils::GraphEquality
{

#pragma region Declaration

	template<typename TData>
	using comparer_t = std::function<bool (const TData&, const TData&)>;

	template<typename TData>
	constexpr comparer_t<TData> defaultComparer ()
	{
		return std::equal_to<TData>{};
	}

#pragma region Edge

	template<typename TVertexData, typename TEdgeData>
	bool equals (const typename Graph<TVertexData, TEdgeData>::Edge& _a, const typename Graph<TVertexData, TEdgeData>::Edge& _b, comparer_t<TEdgeData> _comparer = defaultComparer<TEdgeData> ());

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutData (const typename Graph<TVertexData, TEdgeData>::Edge& _a, const typename Graph<TVertexData, TEdgeData>::Edge& _b);

#pragma endregion

#pragma region Vertex

	template<typename TVertexData, typename TEdgeData>
	bool equals (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b, comparer_t<TVertexData> _vertexComparer = defaultComparer<TVertexData> (), comparer_t<TEdgeData> _edgeComparer = defaultComparer<TEdgeData> ());

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutEdgeData (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b, comparer_t<TVertexData> _vertexComparer = defaultComparer<TVertexData> ());

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutVertexData (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b, comparer_t<TEdgeData> _edgeComparer = defaultComparer<TEdgeData> ());

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutData (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b);

#pragma endregion

#pragma region Graph

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b, comparer_t<TVertexData> _vertexComparer = defaultComparer<TVertexData> (), comparer_t<TEdgeData> _edgeComparer = defaultComparer<TEdgeData> ());

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutEdgeData (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b, comparer_t<TVertexData> _vertexComparer = defaultComparer<TVertexData> ());

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutVertexData (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b, comparer_t<TEdgeData> _edgeComparer = defaultComparer<TEdgeData> ());

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutData (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b);

#pragma endregion

#pragma endregion

#pragma region Implementation

#pragma region Edge

	template<typename TVertexData, typename TEdgeData>
	bool equals (const typename Graph<TVertexData, TEdgeData>::Edge& _a, const typename Graph<TVertexData, TEdgeData>::Edge& _b, comparer_t<TEdgeData> _comparer)
	{
		return equalsWithoutData<TVertexData, TEdgeData> (_a, _b) && _comparer (*_a, *_b);
	}

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutData (const typename Graph<TVertexData, TEdgeData>::Edge& _a, const typename Graph<TVertexData, TEdgeData>::Edge& _b)
	{
		return _a.from ().index () == _b.from ().index ()
			&& _a.to ().index () == _b.to ().index ();
	}

#pragma endregion

#pragma region Vertex

	template<typename TVertexData, typename TEdgeData>
	bool equals (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b, comparer_t<TVertexData> _vertexComparer, comparer_t<TEdgeData> _edgeComparer)
	{
		if (_a.index () != _b.index () || _a.outEdgesCount () != _b.outEdgesCount () || !_vertexComparer (*_a, *_b))
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equals<TVertexData, TEdgeData> (*a, *b, _edgeComparer))
			{
				return false;
			}
		}
		return true;
	}

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutEdgeData (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b, comparer_t<TVertexData> _vertexComparer)
	{
		if (_a.index () != _b.index () || _a.outEdgesCount () != _b.outEdgesCount () || !_vertexComparer (*_a, *_b))
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equalsWithoutData<TVertexData, TEdgeData> (*a, *b))
			{
				return false;
			}
		}
		return true;
	}

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutVertexData (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b, comparer_t<TEdgeData> _edgeComparer)
	{
		if (_a.index () != _b.index () || _a.outEdgesCount () != _b.outEdgesCount ())
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equals<TVertexData, TEdgeData> (*a, *b, _edgeComparer))
			{
				return false;
			}
		}
		return true;
	}

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutData (const typename Graph<TVertexData, TEdgeData>::Vertex& _a, const typename Graph<TVertexData, TEdgeData>::Vertex& _b)
	{
		if (_a.index () != _b.index () || _a.outEdgesCount () != _b.outEdgesCount ())
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equalsWithoutData<TVertexData, TEdgeData> (*a, *b))
			{
				return false;
			}
		}
		return true;
	}

#pragma endregion

#pragma region Graph

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b, comparer_t<TVertexData> _vertexComparer, comparer_t<TEdgeData> _edgeComparer)
	{
		if (_a.verticesCount () != _b.verticesCount () || _a.edgesCount () != _b.edgesCount ())
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equals<TVertexData, TEdgeData> (*a, *b, _vertexComparer, _edgeComparer))
			{
				return false;
			}
		}
		return true;
	}

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutEdgeData (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b, comparer_t<TVertexData> _vertexComparer)
	{
		if (_a.verticesCount () != _b.verticesCount () || _a.edgesCount () != _b.edgesCount ())
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equalsWithoutEdgeData<TVertexData, TEdgeData> (*a, *b, _vertexComparer))
			{
				return false;
			}
		}
		return true;
	}

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutVertexData (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b, comparer_t<TEdgeData> _edgeComparer)
	{
		if (_a.verticesCount () != _b.verticesCount () || _a.edgesCount () != _b.edgesCount ())
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equalsWithoutVertexData<TVertexData, TEdgeData> (*a, *b, _edgeComparer))
			{
				return false;
			}
		}
		return true;
	}

	template<typename TVertexData, typename TEdgeData>
	bool equalsWithoutData (const Graph<TVertexData, TEdgeData>& _a, const Graph<TVertexData, TEdgeData>& _b)
	{
		if (_a.verticesCount () != _b.verticesCount () || _a.edgesCount () != _b.edgesCount ())
		{
			return false;
		}
		for (auto a{ _a.cbegin () }, b{ _b.cbegin () }; a != _a.cend () && b != _b.cend (); a++, b++)
		{
			if (!equalsWithoutData<TVertexData, TEdgeData> (*a, *b))
			{
				return false;
			}
		}
		return true;
	}

#pragma endregion

#pragma endregion

}

#endif