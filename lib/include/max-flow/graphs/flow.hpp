#ifndef INCLUDED_MAX_FLOW_GRAPHS_FLOW
#define INCLUDED_MAX_FLOW_GRAPHS_FLOW

#include <max-flow/graphs/generic/graph.hpp>

namespace MaxFlow::Graphs
{

#pragma region Declaration

	using std::size_t;

	// Types

	using flow_t = size_t;

	// Classes

	template<typename TData = void>
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
		MF_U_NV_SA_D (Data) EdgeFlow (const TNonVoidData& _data, flow_t _capacity = 0, flow_t _flow = 0);
		MF_U_NV_SA_D (Data) EdgeFlow (TNonVoidData&& _data, flow_t _capacity = 0, flow_t _flow = 0);

		// Getters

		flow_t flow () const;
		flow_t capacity () const;
		flow_t residualCapacity () const;

		// Setters

		void setFlow (flow_t _flow);
		void setCapacity (flow_t _capacity);

	};

	// Types

	MF_GG_TT_FD using FlowGraph = Generic::Graph<TVertexData, EdgeFlow<TEdgeData>>;
	MF_GG_TT_FD using FlowGraphVertex = FlowGraph<TVertexData, TEdgeData>::Vertex;
	MF_GG_TT_FD using FlowGraphEdge = FlowGraph<TVertexData, TEdgeData>::Edge;

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
	MF_U_NV_SA_I (D) inline EdgeFlow<TD>::EdgeFlow (const TNonVoidD& _data, flow_t _capacity, flow_t _flow) : Utils::UserData<TD>{ _data }, m_capacity{ _capacity }, m_flow{ _flow }
	{
		validate ();
	}

	template<typename TD>
	MF_U_NV_SA_I (D) inline EdgeFlow<TD>::EdgeFlow (TNonVoidD&& _data, flow_t _capacity, flow_t _flow) : Utils::UserData<TD>{ _data }, m_capacity{ _capacity }, m_flow{ _flow }
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

	template<typename TD>
	inline flow_t EdgeFlow<TD>::residualCapacity () const
	{
		return m_capacity - m_flow;
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