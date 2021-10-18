#ifndef INCLUDED_MAX_FLOW_UTILS_ITERATION_LIST_ITERATOR
#define INCLUDED_MAX_FLOW_UTILS_ITERATION_LIST_ITERATOR

#include <iterator>
#include <type_traits>
#include <cstddef>

namespace MaxFlow::Utils::Iteration
{

#pragma region Declaration

	class IterableListNode
	{

	private:

		template<typename, bool, bool >
		friend class ListIterator;

	protected:

		IterableListNode () = default;

		virtual const IterableListNode* next () const;
		virtual IterableListNode* next () = 0;

		virtual const IterableListNode* previous () const;
		virtual IterableListNode* previous () = 0;

	};

	template<typename TNode, bool constant, bool reversed>
	class ListIterator
	{

	public:

		// Tags

		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::conditional_t<constant, const TNode, TNode>;
		using node_pointer = std::conditional_t<constant, const IterableListNode*, IterableListNode*>;
		using reference = value_type&;
		using pointer = value_type*;
		using const_pointer = const TNode*;
		using const_reference = const TNode&;

	private:

		// Attributes

		node_pointer m_p;

	public:

		// Construction

		ListIterator (pointer _pCurrent);

		template<typename TNodeOther, bool constantOther, bool reversedOther> requires (constantOther || !constant)
		operator ListIterator<TNodeOther, constantOther, reversedOther> () const;

		// Getters

		const_reference operator*() const;
		reference operator*();

		const_pointer operator->() const;
		pointer operator->();

		// Iteration

		ListIterator& operator++();
		ListIterator operator++(int);

		ListIterator& operator--();
		ListIterator operator--(int);

		// Comparison

		template<typename TNodeOther, bool constantOther, bool reversedOther>
		bool operator== (const ListIterator<TNodeOther, constantOther, reversedOther>& _other) const;

	};

	// Aliases

	template <typename TNode>
	using ListIteratorFM = ListIterator<TNode, false, false>;
	template <typename TNode>
	using ListIteratorFC = ListIterator<TNode, true, false>;
	template <typename TNode>
	using ListIteratorRM = ListIterator<TNode, false, true>;
	template <typename TNode>
	using ListIteratorRC = ListIterator<TNode, true, true>;

#pragma endregion

#pragma region Implementation

	inline const IterableListNode* IterableListNode::next () const
	{
		return const_cast<IterableListNode&>(*this).next ();
	}

	inline const IterableListNode* IterableListNode::previous () const
	{
		return const_cast<IterableListNode&>(*this).previous ();
	}

#pragma region IterableListNode

#pragma endregion

#pragma region ListIterator

#pragma region Construction

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r>::ListIterator (pointer _pCurrent) : m_p{ static_cast<node_pointer>(_pCurrent) }
	{}

	template<typename TN, bool c, bool r>
	template<typename TNO, bool co, bool ro>  requires (co || !c)
	inline ListIterator<TN, c, r>::operator ListIterator<TNO, co, ro> () const
	{
		return ListIterator<TNO, co, ro>{static_cast<ListIterator<TNO, co, ro>::pointer>(m_p)};
	}

#pragma endregion

#pragma region Getters

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r>::const_reference ListIterator<TN, c, r>::operator* () const
	{
		return const_cast<ListIterator&>(*this).operator*();
	}

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r>::reference ListIterator<TN, c, r>::operator* ()
	{
		return static_cast<reference>(*m_p);
	}

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r>::const_pointer ListIterator<TN, c, r>::operator-> () const
	{
		return const_cast<ListIterator&>(*this).operator->();
	}

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r>::pointer ListIterator<TN, c, r>::operator-> ()
	{
		return static_cast<pointer>(m_p);
	}

#pragma endregion

#pragma region Iteration

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r>& ListIterator<TN, c, r>::operator++ ()
	{
		if constexpr (r)
		{
			m_p = m_p->previous ();
		}
		else
		{
			m_p = m_p->next ();
		}
		return *this;
	}

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r>& ListIterator<TN, c, r>::operator-- ()
	{
		if constexpr (r)
		{
			m_p = m_p->next ();
		}
		else
		{
			m_p = m_p->previous ();
		}
		return *this;
	}

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r> ListIterator<TN, c, r>::operator++ (int)
	{
		ListIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TN, bool c, bool r>
	inline ListIterator<TN, c, r> ListIterator<TN, c, r>::operator-- (int)
	{
		ListIterator last{ *this };
		--* this;
		return last;
	}

#pragma endregion

	template<typename TN, bool c, bool r>
	template<typename TNO, bool co, bool ro>
	inline bool ListIterator<TN, c, r>::operator== (const ListIterator<TNO, co, ro>& _other) const
	{
		return _other.m_p == m_p;
	}

#pragma endregion

#pragma endregion

}

#endif