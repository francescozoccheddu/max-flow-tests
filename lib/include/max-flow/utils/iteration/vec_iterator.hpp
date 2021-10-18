#ifndef INCLUDED_MAX_FLOW_UTILS_ITERATION_VEC_ITERATOR
#define INCLUDED_MAX_FLOW_UTILS_ITERATION_VEC_ITERATOR

#include <iterator>
#include <type_traits>
#include <cstddef>

namespace MaxFlow::Utils::Iteration
{

	template<typename TData, bool constant, bool reversed>
	class VecIterator
	{

	public:

		// Tags

		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::conditional_t<constant, const TData, TData>;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const TData*;
		using const_reference = const TData&;

	private:

		// Attributes

		pointer* m_p;


	public:

		// Construction

		VecIterator (pointer* _pCurrent);

		template<typename TDataOther, bool constantOther, bool reversedOther>
		operator VecIterator<TDataOther, constantOther, reversedOther> () const;

		// Getters

		const_reference operator*() const;
		reference operator*();

		const_pointer operator->() const;
		pointer operator->();

		const_reference operator[](difference_type _diff) const;
		reference operator[](difference_type _diff);

		// Iteration

		VecIterator& operator++();
		VecIterator operator++(int);

		VecIterator& operator--();
		VecIterator operator--(int);

		VecIterator operator+(difference_type _diff) const;
		VecIterator operator-(difference_type _diff) const;

		VecIterator& operator+=(difference_type _diff);
		VecIterator& operator-=(difference_type _diff);

		// Comparison

		template<typename TDataOther, bool constantOther, bool reversedOther>
		auto operator<=>(const VecIterator<TDataOther, constantOther, reversedOther>& _other) const;

		template<typename TDataOther, bool constantOther, bool reversedOther>
		bool operator==(const VecIterator<TDataOther, constantOther, reversedOther>& _other) const;

	};

	// Aliases

	template<typename TData>
	using VecIteratorFM = VecIterator<TData, false, false>;
	template<typename TData>
	using VecIteratorFC = VecIterator<TData, true, false>;
	template<typename TData>
	using VecIteratorRM = VecIterator<TData, false, true>;
	template<typename TData>
	using VecIteratorRC = VecIterator<TData, true, true>;

#pragma region Implementation

#pragma region Construction

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>::VecIterator (pointer* _pCurrent) : m_p{ _pCurrent }
	{}

	template<typename TD, bool c, bool r>
	template<typename TDO, bool co, bool ro>
	inline VecIterator<TD, c, r>::operator VecIterator<TDO, co, ro> () const
	{
		return VecIterator<TDO, co, ro>{static_cast<TDO**>(m_p)};
	}

#pragma endregion

#pragma region Getters

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>::const_reference VecIterator<TD, c, r>::operator* () const
	{
		return const_cast<VecIterator&>(*this).operator*();
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>::reference VecIterator<TD, c, r>::operator* ()
	{
		return **m_p;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>::const_pointer VecIterator<TD, c, r>::operator-> () const
	{
		return const_cast<VecIterator&>(*this).operator->();
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>::pointer VecIterator<TD, c, r>::operator-> ()
	{
		return *m_p;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>::const_reference VecIterator<TD, c, r>::operator[] (difference_type _diff) const
	{
		return const_cast<VecIterator&>(*this)[_diff];
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>::reference VecIterator<TD, c, r>::operator[] (difference_type _diff)
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
	inline VecIterator<TD, c, r>& VecIterator<TD, c, r>::operator++ ()
	{
		return *this += 1;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>& VecIterator<TD, c, r>::operator-- ()
	{
		return *this -= 1;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r> VecIterator<TD, c, r>::operator++ (int)
	{
		VecIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r> VecIterator<TD, c, r>::operator-- (int)
	{
		VecIterator last{ *this };
		--* this;
		return last;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r> VecIterator<TD, c, r>::operator+ (difference_type _diff) const
	{
		return VecIterator{ *this } += _diff;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r> VecIterator<TD, c, r>::operator- (difference_type _diff) const
	{
		return VecIterator{ *this } -= _diff;
	}

	template<typename TD, bool c, bool r>
	inline VecIterator<TD, c, r>& VecIterator<TD, c, r>::operator+= (difference_type _diff)
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
	inline VecIterator<TD, c, r>& VecIterator<TD, c, r>::operator-= (difference_type _diff)
	{
		return *this += -_diff;
	}

#pragma endregion

	template<typename TD, bool c, bool r>
	template<typename TDO, bool co, bool ro>
	inline auto VecIterator<TD, c, r>::operator<=>(const VecIterator<TDO, co, ro>& _other) const
	{
		return reinterpret_cast<const void*>(m_p) <=> reinterpret_cast<const void*>(_other.m_p);
	}

	template<typename TD, bool c, bool r>
	template<typename TDO, bool co, bool ro>
	inline bool VecIterator<TD, c, r>::operator==(const VecIterator<TDO, co, ro>& _other) const
	{
		return reinterpret_cast<const void*>(m_p) == reinterpret_cast<const void*>(_other.m_p);
	}

#pragma endregion

}

#endif