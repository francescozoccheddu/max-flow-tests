#ifndef INCLUDED_MAX_FLOW_UTILS_USER_DATA
#define INCLUDED_MAX_FLOW_UTILS_USER_DATA

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

		bool operator==(const UserData&) const = default;

	};

	template<>
	class UserData<void>
	{

	public:

		bool operator==(const UserData<void>&) const = default;

	};

#pragma endregion

#pragma region Implementation

#pragma region Construction

	template<typename TD>
	inline UserData<TD>::UserData () : m_data{ }
	{}

	template<typename TD>
	inline UserData<TD>::UserData (TD&& _data) : m_data{ _data }
	{}

	template<typename TD>
	inline UserData<TD>::UserData (const TD& _data) : m_data{ _data }
	{}

#pragma endregion

#pragma region Data getters

	template <typename TD>
	inline const TD& UserData<TD>::data () const
	{
		return m_data;
	}

	template <typename TD>
	inline TD& UserData<TD>::data ()
	{
		return m_data;
	}

	template <typename TD>
	inline const TD& UserData<TD>::operator* () const
	{
		return m_data;
	}

	template <typename TD>
	inline TD& UserData<TD>::operator*  ()
	{
		return m_data;
	}

	template <typename TD>
	inline const TD* UserData<TD>::operator-> () const
	{
		return std::addressof (m_data);
	}

	template <typename TD>
	inline TD* UserData<TD>::operator->  ()
	{
		return std::addressof (m_data);
	}

#pragma endregion

#pragma endregion

}

#endif