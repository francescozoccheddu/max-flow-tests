#ifndef INCLUDED_MAX_FLOW_GRAPH_BASE_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH_BASE_GRAPH

#include <vector>
#include <type_traits>
#include <cstddef>
#include <stdexcept>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graphs/base/vertex.hpp>
#include <max-flow/utils/iteration/contiguous_indirect.hpp>

namespace MaxFlow::Graphs::Base
{

	using std::size_t;

	class Vertex;

	template<bool constant, bool reversed>
	using VertexIterator = Utils::Iteration::ContiguousIndirectIterator<Vertex, constant, reversed>;

	class Graph : public Utils::ReferenceType
	{

	private:

		// Friends

		friend class Vertex;
		friend class Edge;

		// Attributes

		std::vector<Vertex*> m_vertices{};
		size_t m_edgesCount{};
		bool m_hasMatrix{};

		// Utils

		void updateIndices (size_t _first);
		void prepareForVertexInsertion (size_t _index, size_t _count);

		// Iteration

		static const Vertex** increasePtr (const Vertex** _p, std::ptrdiff_t _diff);
		static Vertex** increasePtr (Vertex** _p, std::ptrdiff_t _diff);

		const Vertex** firstVertex () const;
		Vertex** firstVertex ();

		const Vertex** lastVertex () const;
		Vertex** lastVertex ();

		// Vertex interface

		void edgeAdded ();
		void edgeRemoved ();

	protected:

		// Vertex insertion

		void addNewValidatedVertex (Vertex& _vertex);

		// Graph interface

		virtual Vertex& allocateVertex (size_t _index);

	public:

		// Utils

		template<typename A0, typename ...Args> static void ensureSameGraph (A0 const& _graph, Args const&... _graphs);

		static void ensureValidCount (size_t _count);
		void ensureValidVertexIndex (size_t _index) const;
		void ensureValidOrLastVertexIndex (size_t _index) const;

		// Construction

		Graph () = default;

		// Assignment

		Graph& operator=(const Graph& _moved);
		Graph& operator=(Graph&& _moved);

		// Getters

		size_t edgesCount () const;

		size_t verticesCount () const;

		const Vertex& vertex (size_t _index) const;
		Vertex& vertex (size_t _index);

		const Vertex& operator[] (size_t _index) const;
		Vertex& operator[] (size_t _index);

		// Vertex insertion

		Vertex& addVertex ();
		void addVertices (size_t _count);
		Vertex& addVertexBefore (Vertex& _next);
		Vertex& addVertexAt (size_t _index);
		void addVerticesAt (size_t _count, size_t _next);

		void swapVertices (Vertex& _a, Vertex& _b);
		void swapVertices (size_t _a, size_t _b);

		// Storage

		size_t capacity () const;
		void reserve (size_t _capacity);
		void shrinkToFit ();
		void setMatrix (bool _enabled);
		bool hasMatrix () const;

		// Destruction

		void destroyVertex (Vertex& _vertex);
		void destroyVertex (size_t _vertex);
		void destroyAllVertices ();

		~Graph ();

		// Iteration

		VertexIterator<true, false> begin () const;
		VertexIterator<false, false> begin ();
		VertexIterator<true, true> rbegin () const;
		VertexIterator<false, true> rbegin ();
		VertexIterator<true, false> cbegin () const;
		VertexIterator<true, true> crbegin () const;
		VertexIterator<true, false> end () const;
		VertexIterator<false, false> end ();
		VertexIterator<true, true> rend () const;
		VertexIterator<false, true> rend ();
		VertexIterator<true, false> cend () const;
		VertexIterator<true, true> crend () const;

	};

	template<typename A0, typename ...Args> inline void Graph::ensureSameGraph (A0 const& _graph, Args const& ..._graphs)
	{
		if (!((_graphs == _graph) && ... && true))
		{
			throw std::logic_error{ "not the same graph" };
		}
	}

}

#endif