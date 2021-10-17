#ifndef INCLUDED_MAX_FLOW_GRAPH_BASE_VERTEX
#define INCLUDED_MAX_FLOW_GRAPH_BASE_VERTEX

#include <vector>
#include <cstddef>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/base/graph.hpp>
#include <max-flow/graph/base/edge.hpp>
#include <max-flow/graph/base/edge_iterator.hpp>

namespace MaxFlow::Graph::Base
{

	class Graph;
	class Edge;

	template<bool, bool>
	class EdgeIterator;

	using std::size_t;

	class Vertex final : private Utils::ReferenceType
	{

	private:

		// Friends

		friend class Graph;
		friend class Edge;

		// Attributes

		Edge* m_pFirstOutEdge{}, * m_pLastOutEdge{};
		std::vector<Edge*>* m_pOutVertexEdges{};
		size_t m_outEdgesCount{};
		size_t m_index;
		Graph* m_pGraph;

		// Utils

		static void ensureSameVertex (const Vertex& _a, const Vertex& _b);
		void ensureValidOrLastIndex (size_t _index);

		// Graph interface

		void vertexAdded (Vertex& _vertex);
		void vertexDestroyed (Vertex& _vertex);
		void vertexSwapped (Vertex& _a, Vertex& _b);

		void reserve (size_t _capacity);
		void shrinkToFit ();

		bool hasMatrix () const;
		void setMatrix (bool _enabled);

		void setIndex (size_t _index);

		void setGraph (Graph& _graph);

	protected:

		// Construction

		Vertex (Graph& _graph, size_t _index);

		// Edge insertion

		void addNewValidatedOutEdge (Edge& _edge);

		void ensureValidNewOutEdge (const Vertex& _to) const;
		void ensureValidNewOutEdgeBefore (const Vertex& _to, const Edge& _next) const;

	public:

		//Getters

		const Graph& graph () const;
		Graph& graph ();

		size_t index () const;

		size_t outEdgesCount () const;

		bool hasOutEdge (const Vertex& _to) const;
		bool hasOutEdge (size_t _to) const;

		const Edge& outEdge (const Vertex& _to) const;
		const Edge& outEdge (size_t _to) const;
		Edge& outEdge (Vertex& _to);
		Edge& outEdge (size_t _to);

		const Edge* outEdgeIfExists (const Vertex& _to) const;
		const Edge* outEdgeIfExists (size_t _to) const;
		Edge* outEdgeIfExists (Vertex& _to);
		Edge* outEdgeIfExists (size_t _to);

		const Edge& operator[] (const Vertex& _to) const;
		const Edge& operator[] (size_t _to) const;
		Edge& operator[] (Vertex& _to);
		Edge& operator[] (size_t _to);

		// Edge insertion

		virtual Edge& addOutEdge (Vertex& _to);
		Edge& addOutEdge (size_t _to);
		virtual Edge& addOutEdgeBefore (Vertex& _to, Edge& _next);
		Edge& addOutEdgeBefore (size_t _to, Edge& _next);

		// Destruction

		void destroyEdge (Edge& _edge);
		void destroyAllOutEdges ();
		void destroy ();

		// Iteration

		EdgeIterator<true, false> begin () const;
		EdgeIterator<false, false> begin ();
		EdgeIterator<true, true> rbegin () const;
		EdgeIterator<false, true> rbegin ();
		EdgeIterator<true, false> cbegin () const;
		EdgeIterator<true, true> crbegin () const;
		EdgeIterator<true, false> end () const;
		EdgeIterator<false, false> end ();
		EdgeIterator<true, true> rend () const;
		EdgeIterator<false, true> rend ();
		EdgeIterator<true, false> cend () const;
		EdgeIterator<true, true> crend () const;

		// Comparison

		using ReferenceType::operator==;

	};

}

#endif