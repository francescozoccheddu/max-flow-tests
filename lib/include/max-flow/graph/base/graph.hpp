#ifndef INCLUDED_MAX_FLOW_GRAPH_BASE_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH_BASE_GRAPH

#include <vector>
#include <cstddef>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/base/vertex.hpp>
#include <max-flow/utils/iteration/vec_iterator.hpp>

namespace MaxFlow::Graph::Base
{

	using std::size_t;

	class Vertex;

	template<bool constant, bool reversed>
	using VertexIterator = Utils::Iteration::VecIterator<Vertex, constant, reversed>;

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

		// Utils

		static void ensureSameGraph (const Graph& _a, const Graph& _b);
		void ensureValidVertexIndex (size_t _index) const;

		// Vertex insertion

		void addNewValidatedVertex (Vertex& _vertex);

		// Graph interface

		virtual Vertex& allocateVertex (size_t _index);

	public:

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
		Vertex& addVertexBefore (Vertex& _next);
		Vertex& addVertexBefore (size_t _next);

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

}

#endif