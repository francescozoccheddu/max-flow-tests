#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_VERTEX
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_VERTEX

#include <vector>
#include <cstddef>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/generic/graph.hpp>
#include <max-flow/graph/generic/edge.hpp>
#include <max-flow/utils/vec_iterator.hpp>
#include <max-flow/graph/base/vertex.hpp>

namespace MaxFlow::Graph::Generic
{

#pragma region Declaration

	using std::size_t;

	template<typename, typename>
	class Graph;

	template<typename, typename>
	class Edge;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	class Vertex final : public Base::Vertex
	{

		// Aliases

		using TGraph = Graph<TVertexData, TEdgeData>;
		using TEdge = Edge<TVertexData, TEdgeData>;

		template<bool constant, bool reversed>
		using TEdgeIterator = Utils::ListIterator<TEdge, constant, reversed>;

	public:

		//Getters

		const TGraph& graph () const;
		TGraph& graph ();

		const TEdge& outEdge (const Base::Vertex& _to) const;
		const TEdge& outEdge (size_t _to) const;
		TEdge& outEdge (Base::Vertex& _to);
		TEdge& outEdge (size_t _to);

		const TEdge* outEdgeIfExists (const Base::Vertex& _to) const;
		const TEdge* outEdgeIfExists (size_t _to) const;
		TEdge* outEdgeIfExists (Base::Vertex& _to);
		TEdge* outEdgeIfExists (size_t _to);

		const TEdge& operator[] (const Vertex& _to) const;
		const TEdge& operator[] (size_t _to) const;
		TEdge& operator[] (Vertex& _to);
		TEdge& operator[] (size_t _to);

		// Edge insertion

		virtual TEdge& addOutEdge (Base::Edge& _to);
		TEdge& addOutEdge (size_t _to);
		virtual TEdge& addOutEdgeBefore (Vertex& _to, Base::Edge& _next);
		TEdge& addOutEdgeBefore (size_t _to, Base::Edge& _next);

		// Iteration

		TEdgeIterator<true, false> begin () const;
		TEdgeIterator<false, false> begin ();
		TEdgeIterator<true, true> rbegin () const;
		TEdgeIterator<false, true> rbegin ();
		TEdgeIterator<true, false> cbegin () const;
		TEdgeIterator<true, true> crbegin () const;
		TEdgeIterator<true, false> end () const;
		TEdgeIterator<false, false> end ();
		TEdgeIterator<true, true> rend () const;
		TEdgeIterator<false, true> rend ();
		TEdgeIterator<true, false> cend () const;
		TEdgeIterator<true, true> crend () const;

	};

#pragma endregion

#pragma region Implementation

#pragma endregion

}

#endif