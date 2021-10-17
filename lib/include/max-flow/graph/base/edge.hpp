#ifndef INCLUDED_MAX_FLOW_GRAPH_BASE_EDGE
#define INCLUDED_MAX_FLOW_GRAPH_BASE_EDGE

#include <cstddef>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/base/vertex.hpp>
#include <max-flow/utils/list_iterator.hpp>

namespace MaxFlow::Graph::Base 
{

	using std::size_t;

	class Vertex;

	class Edge : private Utils::ReferenceType, public Utils::IterableListNode
	{

	private:

		// Friends

		friend class Vertex;

		// Attributes

		Vertex* const m_pFrom{},* const m_pTo{};
		Edge* m_pPrevious{},* m_pNext{};

		// Construction

		Edge (Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);

		// Vertex interface

		void detachFromList ();
		void attachToList ();

		const Edge* next () const;
		Edge* next () override;
		const Edge* previous () const;
		Edge* previous () override;

	public:

		// Getters

		const Vertex& from () const;
		Vertex& from ();
		const Vertex& to () const;
		Vertex& to ();

		bool hasAntiParallel () const;
		const Edge& antiParallel () const;
		Edge& antiParallel ();
		const Edge* antiParallelIfExists () const;
		Edge* antiParallelIfExists ();

		// Destruction

		void destroy ();

		// Comparison

		using ReferenceType::operator==;

	};

}

#endif