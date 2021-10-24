#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_VERTEX
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_VERTEX

#include <vector>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include <max-flow/utils/non_void.hpp>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/utils/user_data.hpp>
#include <max-flow/graphs/generic/graph.hpp>
#include <max-flow/graphs/generic/edge.hpp>
#include <max-flow/utils/iteration/doubly_linked.hpp>
#include <max-flow/graphs/base/vertex.hpp>
#include <max-flow/graphs/generic/macros.hpp>

#define MF_GG_T Vertex

namespace MaxFlow::Graphs::Generic
{

#pragma region Declaration

	MF_GG_TT_F class Vertex final : public BVertex, public Utils::UserData<TVertexData>
	{
	public:

		MF_GG_UG;
		MF_GG_UE;
		MF_GG_UI;

	private:

		// Friend classes

		friend class Graph;

		// Vertex interface

		BEdge& allocateEdge (BVertex& _to, BEdge* _pPrevious, BEdge* _pNext) override;

		// Construction

		Vertex (BGraph& _graph, size_t _index);
		MF_U_NV_SA_D (VertexData) Vertex (BGraph& _graph, size_t _index, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) Vertex (BGraph& _graph, size_t _index, TNonVoidVertexData&& _data);

	public:

		//Getters

		const Graph& graph () const;
		Graph& graph ();

		const Edge& outEdge (const BVertex& _to) const;
		Edge& outEdge (BVertex& _to);
		const Edge& outEdge (size_t _to) const;
		Edge& outEdge (size_t _to);

		const Edge* outEdgeIfExists (const BVertex& _to) const;
		Edge* outEdgeIfExists (BVertex& _to);
		const Edge* outEdgeIfExists (size_t _to) const;
		Edge* outEdgeIfExists (size_t _to);

		const Edge& operator[] (const BVertex& _to) const;
		Edge& operator[] (BVertex& _to);
		const Edge& operator[] (size_t _to) const;
		Edge& operator[] (size_t _to);

		// Edge insertion

		Edge& addOutEdge (BVertex& _to);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdge (BVertex& _to, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdge (BVertex& _to, TNonVoidEdgeData&& _data);
		Edge& addOutEdge (size_t _to);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdge (size_t _to, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdge (size_t _to, TNonVoidEdgeData&& _data);
		Edge& addOutEdgeBefore (BVertex& _to, BEdge& _next);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdgeBefore (BVertex& _to, BEdge& _next, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdgeBefore (BVertex& _to, BEdge& _next, TNonVoidEdgeData&& _data);
		Edge& addOutEdgeBefore (size_t _to, BEdge& _next);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdgeBefore (size_t _to, BEdge& _next, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) Edge& addOutEdgeBefore (size_t _to, BEdge& _next, TNonVoidEdgeData&& _data);

		// Iteration

		EdgeIteratorFC begin () const;
		EdgeIteratorFM begin ();
		EdgeIteratorRC rbegin () const;
		EdgeIteratorRM rbegin ();
		EdgeIteratorFC cbegin () const;
		EdgeIteratorRC crbegin () const;
		EdgeIteratorFC end () const;
		EdgeIteratorFM end ();
		EdgeIteratorRC rend () const;
		EdgeIteratorRM rend ();
		EdgeIteratorFC cend () const;
		EdgeIteratorRC crend () const;

		// Comparison

		using BVertex::operator==;

	};

#pragma endregion

#pragma region Implementation

	MF_GG_MS (BEdge&) allocateEdge (BVertex& _to, BEdge* _pPrevious, BEdge* _pNext) 
	{
		if constexpr (std::is_default_constructible_v<TED> || std::is_void_v<TED>)
		{
			return *new Edge{ *this, _to, _pPrevious,  _pNext };
		}
		else
		{
			throw std::runtime_error{ "non default constructible edge data" };
		}
	}

	MF_GG_PMC0 (MF_GG_A (Graph)&, graph);
	MF_GG_PMC1 (MF_GG_A (Edge)&, outEdge, BVertex&);
	MF_GG_PMC1 (MF_GG_A (Edge)&, outEdge, size_t);
	MF_GG_PMC1 (MF_GG_A (Edge)*, outEdgeIfExists, BVertex&);
	MF_GG_PMC1 (MF_GG_A (Edge)*, outEdgeIfExists, size_t);
	MF_GG_PMC1 (MF_GG_A (Edge)&, operator[], BVertex&);
	MF_GG_PMC1 (MF_GG_A (Edge)&, operator[], size_t);
	MF_GG_PM1 (MF_GG_A (Edge)&, addOutEdge, BVertex&);
	MF_GG_PM1 (MF_GG_A (Edge)&, addOutEdge, size_t);
	MF_GG_PM2 (MF_GG_A (Edge)&, addOutEdgeBefore, BVertex&, BEdge&);
	MF_GG_PM2 (MF_GG_A (Edge)&, addOutEdgeBefore, size_t, BEdge&);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorFC), begin);
	MF_GG_PM0 (MF_GG_A (EdgeIteratorFM), begin);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorRC), rbegin);
	MF_GG_PM0 (MF_GG_A (EdgeIteratorRM), rbegin);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorFC), cbegin);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorRC), crbegin);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorFC), end);
	MF_GG_PM0 (MF_GG_A (EdgeIteratorFM), end);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorRC), rend);
	MF_GG_PM0 (MF_GG_A (EdgeIteratorRM), rend);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorFC), cend);
	MF_GG_PC0 (MF_GG_A (EdgeIteratorRC), crend);

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdge (BVertex& _to, const TNonVoidED& _data)
	{
		ensureValidNewOutEdge (_to);
		Edge& edge{ *new Edge{*this, _to, last (), nullptr, _data} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdge (size_t _to, const TNonVoidED& _data)
	{
		return addOutEdge (graph ()[_to], _data);
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdge (BVertex& _to, TNonVoidED&& _data)
	{
		ensureValidNewOutEdge (_to);
		Edge& edge{ *new Edge{*this, _to, last (), nullptr, std::move (_data)} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdge (size_t _to, TNonVoidED&& _data)
	{
		return addOutEdge (graph ()[_to], std::move (_data));
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdgeBefore (BVertex& _to, BEdge& _next, const TNonVoidED& _data)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		Edge& edge{ *new Edge{*this, _to, _next.previous (), _next, _data} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdgeBefore (size_t _to, BEdge& _next, const TNonVoidED& _data)
	{
		return addOutEdgeBefore (graph ()[_to], _next, _data);
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdgeBefore (BVertex& _to, BEdge& _next, TNonVoidED&& _data)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		Edge& edge{ *new Edge{*this, _to, _next.previous (), _next, std::move (_data)} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TMS (MF_GG_A (Edge)&) addOutEdgeBefore (size_t _to, BEdge& _next, TNonVoidED&& _data)
	{
		return addOutEdgeBefore (graph ()[_to], _next, std::move (_data));
	}

	MF_GG_TT inline Vertex<TVD, TED>::Vertex (BGraph& _graph, size_t _index)
		: BVertex{ _graph, _index }, Utils::UserData<TVD> {}
	{
	}

	MF_GG_TT MF_U_NV_SA_I (VD) inline Vertex<TVD, TED>::Vertex (BGraph& _graph, size_t _index, const TNonVoidVD& _data)
		: BVertex{ _graph, _index }, Utils::UserData<TVD> {_data}
	{
	}

	MF_GG_TT MF_U_NV_SA_I (VD) inline Vertex<TVD, TED>::Vertex (BGraph& _graph, size_t _index, TNonVoidVD&& _data)
		: BVertex{ _graph, _index }, Utils::UserData<TVD> {std::move (_data)}
	{
	}

#pragma endregion

#undef MF_GG_T

}

#endif