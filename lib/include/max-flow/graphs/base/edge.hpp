#ifndef INCLUDED_MAX_FLOW_GRAPH_BASE_EDGE
#define INCLUDED_MAX_FLOW_GRAPH_BASE_EDGE

#include <cstddef>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graphs/base/vertex.hpp>
#include <max-flow/utils/iteration/doubly_linked.hpp>

namespace MaxFlow::Graphs::Base 
{

	using std::size_t;

	class Vertex;

	class Edge : public Utils::ReferenceType, public Utils::Iteration::DoublyLinkedNode
	{

	private:

		// Friends

		friend class Vertex;

		// Attributes

		Vertex* const m_pFrom{},* const m_pTo{};
		Edge* m_pPrevious{},* m_pNext{};


		// Vertex interface

		void detachFromList ();
		void attachToList ();

	protected:

		// Construction

		Edge (Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);

		// Vertext and edge interface

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