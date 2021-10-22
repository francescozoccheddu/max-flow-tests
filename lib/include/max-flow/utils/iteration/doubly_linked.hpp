#ifndef INCLUDED_MAX_FLOW_UTILS_ITERATION_DOUBLY_LINKED
#define INCLUDED_MAX_FLOW_UTILS_ITERATION_DOUBLY_LINKED

#include <iterator>
#include <type_traits>
#include <cstddef>

namespace MaxFlow::Utils::Iteration
{

#pragma region Declaration

	class DoublyLinkedNode
	{

	private:

		template<typename, bool, bool >
		friend class DoublyLinkedIterator;

	protected:

		DoublyLinkedNode () = default;

		virtual const DoublyLinkedNode* next () const;
		virtual DoublyLinkedNode* next () = 0;

		virtual const DoublyLinkedNode* previous () const;
		virtual DoublyLinkedNode* previous () = 0;

	};

	template<typename TNode, bool constant, bool reversed>
	class DoublyLinkedIterator
	{

	public:

		// Tags

		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::conditional_t<constant, const TNode, TNode>;
		using node_pointer = std::conditional_t<constant, const DoublyLinkedNode*, DoublyLinkedNode*>;
		using reference = value_type&;
		using pointer = value_type*;

	private:

		// Attributes

		node_pointer m_p;

	public:

		// Construction

		DoublyLinkedIterator (pointer _pCurrent);

		template<typename TNodeOther, bool constantOther, bool reversedOther> requires (constantOther || !constant)
		operator DoublyLinkedIterator<TNodeOther, constantOther, reversedOther> () const;

		// Getters

		reference operator*() const;
		pointer operator->() const;

		// Iteration

		DoublyLinkedIterator& operator++();
		DoublyLinkedIterator operator++(int);

		DoublyLinkedIterator& operator--();
		DoublyLinkedIterator operator--(int);

		// Comparison

		template<typename TNodeOther, bool constantOther, bool reversedOther>
		bool operator== (const DoublyLinkedIterator<TNodeOther, constantOther, reversedOther>& _other) const;

	};

	// Aliases

	template <typename TNode>
	using DoublyLinkedIteratorFM = DoublyLinkedIterator<TNode, false, false>;
	template <typename TNode>
	using DoublyLinkedIteratorFC = DoublyLinkedIterator<TNode, true, false>;
	template <typename TNode>
	using DoublyLinkedIteratorRM = DoublyLinkedIterator<TNode, false, true>;
	template <typename TNode>
	using DoublyLinkedIteratorRC = DoublyLinkedIterator<TNode, true, true>;

#pragma endregion

#pragma region Implementation

	inline const DoublyLinkedNode* DoublyLinkedNode::next () const
	{
		return const_cast<DoublyLinkedNode&>(*this).next ();
	}

	inline const DoublyLinkedNode* DoublyLinkedNode::previous () const
	{
		return const_cast<DoublyLinkedNode&>(*this).previous ();
	}

#pragma region DoublyLinkedNode

#pragma endregion

#pragma region DoublyLinkedIterator

#pragma region Construction

	template<typename TN, bool c, bool r>
	inline DoublyLinkedIterator<TN, c, r>::DoublyLinkedIterator (pointer _pCurrent) : m_p{ static_cast<node_pointer>(_pCurrent) }
	{}

	template<typename TN, bool c, bool r>
	template<typename TNO, bool co, bool ro>  requires (co || !c)
	inline DoublyLinkedIterator<TN, c, r>::operator DoublyLinkedIterator<TNO, co, ro> () const
	{
		return DoublyLinkedIterator<TNO, co, ro>{static_cast<DoublyLinkedIterator<TNO, co, ro>::pointer>(m_p)};
	}

#pragma endregion

#pragma region Getters

	template<typename TN, bool c, bool r>
	inline DoublyLinkedIterator<TN, c, r>::reference DoublyLinkedIterator<TN, c, r>::operator* () const
	{
		return static_cast<reference>(*m_p);
	}

	template<typename TN, bool c, bool r>
	inline DoublyLinkedIterator<TN, c, r>::pointer DoublyLinkedIterator<TN, c, r>::operator-> () const
	{
		return static_cast<pointer>(m_p);
	}

#pragma endregion

#pragma region Iteration

	template<typename TN, bool c, bool r>
	inline DoublyLinkedIterator<TN, c, r>& DoublyLinkedIterator<TN, c, r>::operator++ ()
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
	inline DoublyLinkedIterator<TN, c, r>& DoublyLinkedIterator<TN, c, r>::operator-- ()
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
	inline DoublyLinkedIterator<TN, c, r> DoublyLinkedIterator<TN, c, r>::operator++ (int)
	{
		DoublyLinkedIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TN, bool c, bool r>
	inline DoublyLinkedIterator<TN, c, r> DoublyLinkedIterator<TN, c, r>::operator-- (int)
	{
		DoublyLinkedIterator last{ *this };
		--* this;
		return last;
	}

#pragma endregion

	template<typename TN, bool c, bool r>
	template<typename TNO, bool co, bool ro>
	inline bool DoublyLinkedIterator<TN, c, r>::operator== (const DoublyLinkedIterator<TNO, co, ro>& _other) const
	{
		return _other.m_p == m_p;
	}

#pragma endregion

#pragma endregion

}

#endif