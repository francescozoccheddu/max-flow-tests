#ifndef INCLUDED_MAX_FLOW_EDGE_FLOW
#define INCLUDED_MAX_FLOW_EDGE_FLOW

#include <max-flow/utils/user_data.hpp>
#include <max-flow/utils/graph.hpp>
#include <stdexcept>

namespace MaxFlow
{

#pragma region Declaration

	using flow_t = unsigned int;

	template<typename TData=void>
	class EdgeFlow final : public Utils::UserData<TData>
	{

	private:

		flow_t m_flow{}, m_capacity{};

		void validate () const;

	public:

		EdgeFlow (flow_t _capacity = 0, flow_t _flow = 0);
		MAX_FLOW_UD_VALID_T_DECL (Data) EdgeFlow (const TValidData& _data, flow_t _capacity = 0, flow_t _flow = 0);
		MAX_FLOW_UD_VALID_T_DECL (Data) EdgeFlow (TValidData&& _data, flow_t _capacity = 0, flow_t _flow = 0);

		flow_t flow () const;
		flow_t capacity () const;

		void setFlow (flow_t _flow);
		void setCapacity (flow_t _capacity);

	};

#pragma endregion

#pragma region Implementation

	template<typename TData>
	inline void EdgeFlow<TData>::validate () const
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

	template<typename TData>
	inline EdgeFlow<TData>::EdgeFlow (flow_t _capacity, flow_t _flow) : Utils::UserData<TData>{ }, m_capacity{ _capacity }, m_flow{ _flow }
	{
		validate ();
	}

	template<typename TData>
	MAX_FLOW_UD_VALID_T_IMPL (Data) inline EdgeFlow<TData>::EdgeFlow (const TValidData& _data, flow_t _capacity, flow_t _flow) : Utils::UserData<TData>{ _data }, m_capacity{ _capacity }, m_flow{ _flow }
	{
		validate ();
	}

	template<typename TData>
	MAX_FLOW_UD_VALID_T_IMPL (Data) inline EdgeFlow<TData>::EdgeFlow (TValidData&& _data, flow_t _capacity, flow_t _flow) : Utils::UserData<TData>{ _data }, m_capacity{ _capacity }, m_flow{ _flow }
	{
		validate ();
	}

	template<typename TData>
	inline flow_t EdgeFlow<TData>::flow () const
	{
		return m_flow;
	}

	template<typename TData>
	inline flow_t EdgeFlow<TData>::capacity () const
	{
		return m_capacity;
	}

	template<typename TData>
	inline void EdgeFlow<TData>::setFlow (flow_t _flow)
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

	template<typename TData>
	inline void EdgeFlow<TData>::setCapacity (flow_t _capacity)
	{
		if (m_capacity < 0)
		{
			throw std::out_of_range{ "capacity < 0" };
		}
		m_capacity = _capacity;
	}

#pragma endregion

}

#endif