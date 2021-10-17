#ifndef INCLUDED_MAX_FLOW_UTILS_VEC_ITERATOR
#define INCLUDED_MAX_FLOW_UTILS_VEC_ITERATOR

#include <iterator>
#include <type_traits>
#include <cstddef>

namespace MaxFlow::Utils
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

		template<typename TOtherData, bool constantOther, bool reversedOther>
		auto operator<=>(const VecIterator<TOtherData, constantOther, reversedOther>& _other) const;

		template<typename TOtherData, bool constantOther, bool reversedOther>
		bool operator==(const VecIterator<TOtherData, constantOther, reversedOther>& _other) const;

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

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>::VecIterator (pointer* _pCurrent) : m_p{ _pCurrent }
	{}

#pragma endregion

#pragma region Getters

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>::const_reference VecIterator<TData, constant, reversed>::operator* () const
	{
		return const_cast<VecIterator&>(*this).operator*();
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>::reference VecIterator<TData, constant, reversed>::operator* ()
	{
		return **m_p;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>::const_pointer VecIterator<TData, constant, reversed>::operator-> () const
	{
		return const_cast<VecIterator&>(*this).operator->();
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>::pointer VecIterator<TData, constant, reversed>::operator-> ()
	{
		return *m_p;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>::const_reference VecIterator<TData, constant, reversed>::operator[] (difference_type _diff) const
	{
		return const_cast<VecIterator&>(*this)[_diff];
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>::reference VecIterator<TData, constant, reversed>::operator[] (difference_type _diff)
	{
		if constexpr (reversed)
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

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>& VecIterator<TData, constant, reversed>::operator++ ()
	{
		return *this += 1;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>& VecIterator<TData, constant, reversed>::operator-- ()
	{
		return *this -= 1;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed> VecIterator<TData, constant, reversed>::operator++ (int)
	{
		VecIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed> VecIterator<TData, constant, reversed>::operator-- (int)
	{
		VecIterator last{ *this };
		--* this;
		return last;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed> VecIterator<TData, constant, reversed>::operator+ (difference_type _diff) const
	{
		return VecIterator{ *this } += _diff;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed> VecIterator<TData, constant, reversed>::operator- (difference_type _diff) const
	{
		return VecIterator{ *this } -= _diff;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>& VecIterator<TData, constant, reversed>::operator+= (difference_type _diff)
	{
		if constexpr (reversed)
		{
			m_p -= _diff;
		}
		else
		{
			m_p += _diff;
		}
		return *this;
	}

	template<typename TData, bool constant, bool reversed>
	inline VecIterator<TData, constant, reversed>& VecIterator<TData, constant, reversed>::operator-= (difference_type _diff)
	{
		return *this += -_diff;
	}

#pragma endregion

	template<typename TData, bool constant, bool reversed>
	template<typename TOtherData, bool constantOther, bool reversedOther>
	inline auto VecIterator<TData, constant, reversed>::operator<=>(const VecIterator<TOtherData, constantOther, reversedOther>& _other) const
	{
		return reinterpret_cast<const void*>(m_p) <=> reinterpret_cast<const void*>(_other.m_p);
	}

	template<typename TData, bool constant, bool reversed>
	template<typename TOtherData, bool constantOther, bool reversedOther>
	inline bool VecIterator<TData, constant, reversed>::operator==(const VecIterator<TOtherData, constantOther, reversedOther>& _other) const
	{
		return reinterpret_cast<const void*>(m_p) == reinterpret_cast<const void*>(_other.m_p);
	}

#pragma endregion

}

#endif