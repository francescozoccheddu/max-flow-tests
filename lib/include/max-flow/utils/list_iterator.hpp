#ifndef INCLUDED_MAX_FLOW_UTILS_LIST_ITERATOR
#define INCLUDED_MAX_FLOW_UTILS_LIST_ITERATOR

#include <iterator>
#include <type_traits>
#include <cstddef>

namespace MaxFlow::Utils
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
		using reference = value_type&;
		using pointer = value_type*;
		using const_pointer = const TNode*;
		using const_reference = const TNode&;

	private:

		// Attributes

		IterableListNode* m_p;

	public:

		// Construction

		ListIterator (IterableListNode* _pCurrent);

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

		template<typename TOtherData, bool constantOther, bool reversedOther>
		bool operator== (const ListIterator<TOtherData, constantOther, reversedOther>& _other) const;

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

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed>::ListIterator (IterableListNode* _pCurrent) : m_p{ _pCurrent }
	{}

#pragma endregion

#pragma region Getters

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed>::const_reference ListIterator<TNode, constant, reversed>::operator* () const
	{
		return const_cast<ListIterator&>(*this).operator*();
	}

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed>::reference ListIterator<TNode, constant, reversed>::operator* ()
	{
		return static_cast<TNode&>(*m_p);
	}

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed>::const_pointer ListIterator<TNode, constant, reversed>::operator-> () const
	{
		return const_cast<ListIterator&>(*this).operator->();
	}

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed>::pointer ListIterator<TNode, constant, reversed>::operator-> ()
	{
		return static_cast<TNode*>(m_p);
	}

#pragma endregion

#pragma region Iteration

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed>& ListIterator<TNode, constant, reversed>::operator++ ()
	{
		if constexpr (reversed)
		{
			m_p = m_p->previous ();
		}
		else
		{
			m_p = m_p->next ();
		}
		return *this;
	}

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed>& ListIterator<TNode, constant, reversed>::operator-- ()
	{
		if constexpr (reversed)
		{
			m_p = m_p->next ();
		}
		else
		{
			m_p = m_p->previous ();
		}
		return *this;
	}

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed> ListIterator<TNode, constant, reversed>::operator++ (int)
	{
		ListIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TNode, bool constant, bool reversed>
	inline ListIterator<TNode, constant, reversed> ListIterator<TNode, constant, reversed>::operator-- (int)
	{
		ListIterator last{ *this };
		--* this;
		return last;
	}

#pragma endregion

	template<typename TNode, bool constant, bool reversed>
	template<typename TOtherData, bool constantOther, bool reversedOther>
	inline bool ListIterator<TNode, constant, reversed>::operator== (const ListIterator<TOtherData, constantOther, reversedOther>& _other) const
	{
		return _other.m_p == m_p;
	}

#pragma endregion

#pragma endregion

}

#endif