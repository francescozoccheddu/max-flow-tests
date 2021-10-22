#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_LABELER
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_LABELER

#include <max-flow/graphs/residual.hpp>
#include <max-flow/utils/reference_equatable.hpp>
#include <vector>
#include <queue>
#include <functional>
#include <type_traits>

namespace MaxFlow::Graphs::Algorithms
{

#pragma region Declaration

	class Labeler final : public Utils::ReferenceEquatable
	{

	private:

		ResidualGraph& m_graph;
		ResidualVertex& m_source, & m_sink;
		std::vector<ResidualVertex*> m_predecessors;
		std::queue<ResidualVertex*> m_queue;

	public:

		template<bool constant>
		class Iterator final
		{

		public:

			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::conditional_t<constant, const ResidualEdge, ResidualEdge>;
			using reference = value_type&;
			using pointer = value_type*;
			using labeler = std::conditional_t<constant, const Labeler, Labeler>;
			using vertex = std::conditional_t<constant, const ResidualVertex, ResidualVertex>;

		private:

			friend class Labeler;

			vertex* m_p;
			labeler& m_labeler;

			Iterator (labeler& _labeler, vertex& _current);

		public:

			// Getters

			reference operator*() const;
			pointer operator->() const;

			// Iteration

			Iterator& operator++();
			Iterator operator++(int);

			// Comparison

			template<bool constantOther>
			bool operator==(const Iterator<constantOther>& _other) const;

		};

		class EdgeSelector
		{

			friend class Labeler;

		protected:

			virtual bool shouldVisit (const ResidualEdge& _edge) const;

		};

		using IteratorC = Iterator<true>;
		using IteratorM = Iterator<false>;

		Labeler (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink);

		void label (const EdgeSelector& _edgeSelector = {});

		const ResidualVertex& operator[](const ResidualVertex& _vertex) const;
		ResidualVertex& operator[](ResidualVertex& _vertex);

		bool hasPathToSink () const;

		IteratorC begin () const;
		IteratorC cbegin () const;
		IteratorM begin ();

		IteratorC end () const;
		IteratorC cend () const;
		IteratorM end ();


	};

#pragma endregion

#pragma region Implementation

	template<bool c>
	inline Labeler::Iterator<c>::Iterator (labeler& _labeler, vertex& _current) : m_labeler{ _labeler }, m_p{ &_current }
	{}

	template<bool c>
	inline Labeler::Iterator<c>::reference Labeler::Iterator<c>::operator*() const
	{
		return m_labeler[*m_p][*m_p];
	}

	template<bool c>
	inline Labeler::Iterator<c>::pointer Labeler::Iterator<c>::operator->() const
	{
		return &(operator->());
	}

	template<bool c>
	inline Labeler::Iterator<c>& Labeler::Iterator<c>::operator++()
	{
		m_p = &m_labeler[*m_p];
		return *this;
	}

	template<bool c>
	inline Labeler::Iterator<c> Labeler::Iterator<c>::operator++(int)
	{
		Iterator last{ *this };
		++* this;
		return last;
	}

	template<bool c>
	template<bool co>
	inline bool Labeler::Iterator<c>::operator==(const Labeler::Iterator<co>& _other) const
	{
		return _other.m_labeler == m_labeler && _other.m_p == m_p;
	}

#pragma endregion


}

#endif