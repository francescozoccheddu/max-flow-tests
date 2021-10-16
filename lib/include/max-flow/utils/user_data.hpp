#ifndef INCLUDED_MAX_FLOW_USER_DATA
#define INCLUDED_MAX_FLOW_USER_DATA

#include <type_traits>
#include <concepts>

#define MAX_FLOW_UD_VALID_T_DECL(Type) template <std::same_as<T##Type> TValid##Type = T##Type> requires (!std::is_void_v<T##Type>)
#define MAX_FLOW_UD_VALID_T_IMPL(Type) template <std::same_as<T##Type> TValid##Type> requires (!std::is_void_v<T##Type>)

namespace MaxFlow::Utils
{

#pragma region Declaration

	template <typename TData>
	class UserData
	{

	private:

		TData m_data;

	protected:

		UserData ();
		UserData (TData&& _data);
		UserData (const TData& _data);

		virtual ~UserData () = default;

	public:

		const TData& data () const;
		TData& data ();

		const TData& operator*() const;
		TData& operator*();

		const TData* operator->() const;
		TData* operator->();


	};

	template<>
	class UserData<void>
	{};

#pragma endregion

#pragma region Implementation

#pragma region Constructors

	template<typename TData>
	inline UserData<TData>::UserData () : m_data{ }
	{}

	template<typename TData>
	inline UserData<TData>::UserData (TData&& _data) : m_data{ _data }
	{}

	template<typename TData>
	inline UserData<TData>::UserData (const TData& _data) : m_data{ _data }
	{}

#pragma endregion

#pragma region Data getters

	template <typename TData>
	inline const TData& UserData<TData>::data () const
	{
		return m_data;
	}

	template <typename TData>
	inline TData& UserData<TData>::data ()
	{
		return m_data;
	}

	template <typename TData>
	inline const TData& UserData<TData>::operator* () const
	{
		return m_data;
	}

	template <typename TData>
	inline TData& UserData<TData>::operator*  ()
	{
		return m_data;
	}

	template <typename TData>
	inline const TData* UserData<TData>::operator-> () const
	{
		return std::addressof (m_data);
	}

	template <typename TData>
	inline TData* UserData<TData>::operator->  ()
	{
		return std::addressof (m_data);
	}

#pragma endregion

#pragma endregion

}

#endif