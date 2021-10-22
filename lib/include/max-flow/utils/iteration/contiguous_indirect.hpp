#ifndef INCLUDED_MAX_FLOW_UTILS_ITERATION_CONTIGUOUS_INDIRECT
#define INCLUDED_MAX_FLOW_UTILS_ITERATION_CONTIGUOUS_INDIRECT

#include <iterator>
#include <type_traits>
#include <cstddef>

namespace MaxFlow::Utils::Iteration
{

	template<typename TData, bool constant, bool reversed>
	class ContiguousIndirectIterator
	{

	public:

		// Tags

		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::conditional_t<constant, const TData, TData>;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		// Attributes

		pointer* m_p;

	public:

		// Construction

		ContiguousIndirectIterator (pointer* _pCurrent);

		template<typename TDataOther, bool constantOther, bool reversedOther> requires (constantOther || !constant)
		operator ContiguousIndirectIterator<TDataOther, constantOther, reversedOther> () const;

		// Getters

		reference operator*() const;

		pointer operator->() const;

		reference operator[](difference_type _diff) const;

		// Iteration

		ContiguousIndirectIterator& operator++();
		ContiguousIndirectIterator operator++(int);

		ContiguousIndirectIterator& operator--();
		ContiguousIndirectIterator operator--(int);

		ContiguousIndirectIterator operator+(difference_type _diff) const;
		ContiguousIndirectIterator operator-(difference_type _diff) const;

		ContiguousIndirectIterator& operator+=(difference_type _diff);
		ContiguousIndirectIterator& operator-=(difference_type _diff);

		// Comparison

		template<typename TDataOther, bool constantOther, bool reversedOther>
		auto operator<=>(const ContiguousIndirectIterator<TDataOther, constantOther, reversedOther>& _other) const;

		template<typename TDataOther, bool constantOther, bool reversedOther>
		bool operator==(const ContiguousIndirectIterator<TDataOther, constantOther, reversedOther>& _other) const;

	};

	// Aliases

	template<typename TData>
	using ContiguousIndirectIteratorFM = ContiguousIndirectIterator<TData, false, false>;
	template<typename TData>
	using ContiguousIndirectIteratorFC = ContiguousIndirectIterator<TData, true, false>;
	template<typename TData>
	using ContiguousIndirectIteratorRM = ContiguousIndirectIterator<TData, false, true>;
	template<typename TData>
	using ContiguousIndirectIteratorRC = ContiguousIndirectIterator<TData, true, true>;

#pragma region Implementation

#pragma region Construction

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>::ContiguousIndirectIterator (pointer* _pCurrent) : m_p{ _pCurrent }
	{}

	template<typename TD, bool c, bool r>
	template<typename TDO, bool co, bool ro> requires (co || !c)
	inline ContiguousIndirectIterator<TD, c, r>::operator ContiguousIndirectIterator<TDO, co, ro> () const
	{
		return ContiguousIndirectIterator<TDO, co, ro>{reinterpret_cast<ContiguousIndirectIterator<TDO, co, ro>::pointer*>(m_p)};
	}

#pragma endregion

#pragma region Getters

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>::reference ContiguousIndirectIterator<TD, c, r>::operator* () const 
	{
		return **m_p;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>::pointer ContiguousIndirectIterator<TD, c, r>::operator-> () const
	{
		return *m_p;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>::reference ContiguousIndirectIterator<TD, c, r>::operator[] (difference_type _diff) const
	{
		if constexpr (r)
		{
			return *m_p[-_diff];
		}
		else
		{
			return *m_p[_diff];
		}
	}

#pragma endregion

#pragma region Iteration

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>& ContiguousIndirectIterator<TD, c, r>::operator++ ()
	{
		return *this += 1;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>& ContiguousIndirectIterator<TD, c, r>::operator-- ()
	{
		return *this -= 1;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r> ContiguousIndirectIterator<TD, c, r>::operator++ (int)
	{
		ContiguousIndirectIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r> ContiguousIndirectIterator<TD, c, r>::operator-- (int)
	{
		ContiguousIndirectIterator last{ *this };
		--* this;
		return last;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r> ContiguousIndirectIterator<TD, c, r>::operator+ (difference_type _diff) const
	{
		return ContiguousIndirectIterator{ *this } += _diff;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r> ContiguousIndirectIterator<TD, c, r>::operator- (difference_type _diff) const
	{
		return ContiguousIndirectIterator{ *this } -= _diff;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>& ContiguousIndirectIterator<TD, c, r>::operator+= (difference_type _diff)
	{
		if constexpr (r)
		{
			m_p -= _diff;
		}
		else
		{
			m_p += _diff;
		}
		return *this;
	}

	template<typename TD, bool c, bool r>
	inline ContiguousIndirectIterator<TD, c, r>& ContiguousIndirectIterator<TD, c, r>::operator-= (difference_type _diff)
	{
		return *this += -_diff;
	}

#pragma endregion

	template<typename TD, bool c, bool r>
	template<typename TDO, bool co, bool ro>
	inline auto ContiguousIndirectIterator<TD, c, r>::operator<=>(const ContiguousIndirectIterator<TDO, co, ro>& _other) const
	{
		return reinterpret_cast<const void*>(m_p) <=> reinterpret_cast<const void*>(_other.m_p);
	}

	template<typename TD, bool c, bool r>
	template<typename TDO, bool co, bool ro>
	inline bool ContiguousIndirectIterator<TD, c, r>::operator==(const ContiguousIndirectIterator<TDO, co, ro>& _other) const
	{
		return reinterpret_cast<const void*>(m_p) == reinterpret_cast<const void*>(_other.m_p);
	}

#pragma endregion

}

#endif