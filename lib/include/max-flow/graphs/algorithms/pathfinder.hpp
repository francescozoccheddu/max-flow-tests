#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_PATHFINDER
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_PATHFINDER

#include <max-flow/graphs/residual.hpp>
#include <max-flow/graphs/algorithms/edge_selector.hpp>
#include <max-flow/utils/reference_equatable.hpp>
#include <vector>
#include <iterator>
#include <type_traits>

namespace MaxFlow::Graphs::Algorithms
{

#pragma region Declaration

	class Pathfinder final : public Utils::ReferenceEquatable
	{

	private:

		ResidualGraph& m_graph;
		ResidualVertex& m_source, & m_sink;
		std::vector<ResidualVertex*> m_predecessors;

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
			using pathfinder = std::conditional_t<constant, const Pathfinder, Pathfinder>;
			using vertex = std::conditional_t<constant, const ResidualVertex, ResidualVertex>;

		private:

			friend class Pathfinder;

			vertex* m_p;
			pathfinder& m_pathfinder;

			Iterator (pathfinder& _pathfinder, vertex& _current);

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

		using IteratorC = Iterator<true>;
		using IteratorM = Iterator<false>;

		Pathfinder (ResidualGraph& _graph, ResidualVertex& _source, ResidualVertex& _sink);

		void reset ();

		void calculate (EdgeSelector& _edgeSelector = EdgeSelector::all);

		void setPredecessor (ResidualVertex& _vertex, ResidualVertex& _predecessor);
		void setPredecessor (ResidualEdge& _edge);
		void resetPredecessor (ResidualVertex& _vertex);

		const ResidualVertex& operator[](const ResidualVertex& _vertex) const;
		ResidualVertex& operator[](ResidualVertex& _vertex);

		bool isLabeled (const ResidualVertex& _vertex) const;
		bool isSinkLabeled () const;
		bool isSourceLabeled () const;

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
	inline Pathfinder::Iterator<c>::Iterator (pathfinder& _pathfinder, vertex& _current) : m_pathfinder{ _pathfinder }, m_p{ &_current }
	{}

	template<bool c>
	inline Pathfinder::Iterator<c>::reference Pathfinder::Iterator<c>::operator*() const
	{
		return m_pathfinder[*m_p][*m_p];
	}

	template<bool c>
	inline Pathfinder::Iterator<c>::pointer Pathfinder::Iterator<c>::operator->() const
	{
		return &m_pathfinder[*m_p][*m_p];
	}

	template<bool c>
	inline Pathfinder::Iterator<c>& Pathfinder::Iterator<c>::operator++()
	{
		m_p = &m_pathfinder[*m_p];
		return *this;
	}

	template<bool c>
	inline Pathfinder::Iterator<c> Pathfinder::Iterator<c>::operator++(int)
	{
		Iterator last{ *this };
		++* this;
		return last;
	}

	template<bool c>
	template<bool co>
	inline bool Pathfinder::Iterator<c>::operator==(const Pathfinder::Iterator<co>& _other) const
	{
		return _other.m_pathfinder == m_pathfinder && _other.m_p == m_p;
	}

#pragma endregion


}

#endif