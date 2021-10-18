#ifndef INCLUDED_MAX_FLOW_EDGE_FLOW
#define INCLUDED_MAX_FLOW_EDGE_FLOW

#include <max-flow/utils/user_data.hpp>
#include <max-flow/utils/graph.hpp>
#include <stdexcept>

namespace MaxFlow
{

#pragma region Declaration

	// Types

	using flow_t = unsigned int;

	// Classes

	template<typename TData=void>
	class EdgeFlow final : public Utils::UserData<TData>
	{

	private:

		// Attributes

		flow_t m_flow{}, m_capacity{};

		// Utils

		void validate () const;

	public:

		// Construction

		EdgeFlow (flow_t _capacity = 0, flow_t _flow = 0);
		MAX_FLOW_UD_VALID_T_DECL (Data) EdgeFlow (const TValidData& _data, flow_t _capacity = 0, flow_t _flow = 0);
		MAX_FLOW_UD_VALID_T_DECL (Data) EdgeFlow (TValidData&& _data, flow_t _capacity = 0, flow_t _flow = 0);

		// Getters

		flow_t flow () const;
		flow_t capacity () const;

		// Setters

		void setFlow (flow_t _flow);
		void setCapacity (flow_t _capacity);

	};

#pragma endregion

#pragma region Implementation

#pragma region Utils

	template<typename TD>
	inline void EdgeFlow<TD>::validate () const
	{
		if (m_capacity < 0)
		{
			throw std::out_of_range{ "capacity < 0" };
		}
		if (m_flow < 0)
		{
			throw std::out_of_range{ "flow < 0" };
		}
		if (m_flow > m_capacity)
		{
			throw std::out_of_range{ "flow > capacity" };
		}
	}

#pragma endregion

#pragma region Construction

	template<typename TD>
	inline EdgeFlow<TD>::EdgeFlow (flow_t _capacity, flow_t _flow) : Utils::UserData<TD>{ }, m_capacity{ _capacity }, m_flow{ _flow }
	{
		validate ();
	}

	template<typename TD>
	MAX_FLOW_UD_VALID_T_IMPL (Data) inline EdgeFlow<TD>::EdgeFlow (const TValidData& _data, flow_t _capacity, flow_t _flow) : Utils::UserData<TD>{ _data }, m_capacity{ _capacity }, m_flow{ _flow }
	{
		validate ();
	}

	template<typename TD>
	MAX_FLOW_UD_VALID_T_IMPL (Data) inline EdgeFlow<TD>::EdgeFlow (TValidData&& _data, flow_t _capacity, flow_t _flow) : Utils::UserData<TD>{ _data }, m_capacity{ _capacity }, m_flow{ _flow }
	{
		validate ();
	}

#pragma endregion

#pragma region Getters

	template<typename TD>
	inline flow_t EdgeFlow<TD>::flow () const
	{
		return m_flow;
	}

	template<typename TD>
	inline flow_t EdgeFlow<TD>::capacity () const
	{
		return m_capacity;
	}

#pragma endregion

#pragma region Setters

	template<typename TD>
	inline void EdgeFlow<TD>::setFlow (flow_t _flow)
	{
		if (_flow < 0)
		{
			throw std::out_of_range{ "flow < 0" };
		}
		if (_flow > m_capacity)
		{
			throw std::out_of_range{ "flow > capacity" };
		}
		m_flow = _flow;
	}

	template<typename TD>
	inline void EdgeFlow<TD>::setCapacity (flow_t _capacity)
	{
		if (m_capacity < 0)
		{
			throw std::out_of_range{ "capacity < 0" };
		}
		m_capacity = _capacity;
	}

#pragma endregion

#pragma endregion

}

#endif