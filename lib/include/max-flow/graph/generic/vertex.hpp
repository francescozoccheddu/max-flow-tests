#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_VERTEX
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_VERTEX

#include <vector>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <type_traits>
#include <max-flow/utils/macros/non_void.hpp>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/utils/user_data.hpp>
#include <max-flow/graph/generic/graph.hpp>
#include <max-flow/graph/generic/edge.hpp>
#include <max-flow/utils/iteration/list_iterator.hpp>
#include <max-flow/graph/base/vertex.hpp>
#include <max-flow/graph/generic/macros.hpp>

#define MF_GG_M_C Vertex

namespace MaxFlow::Graph::Generic
{

#pragma region Declaration

	using std::size_t;

	template<typename, typename>
	class Graph;

	template<typename, typename>
	class Edge;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	class Vertex final : public Base::Vertex, public Utils::UserData<TVertexData>
	{

	private:

		// Aliases

		using TGraph = Graph<TVertexData, TEdgeData>;
		using TEdge = Edge<TVertexData, TEdgeData>;

		template<bool constant, bool reversed>
		using TEdgeIterator = Utils::Iteration::ListIterator<TEdge, constant, reversed>;

		using TEdgeIteratorFM = TEdgeIterator<false, false>;
		using TEdgeIteratorFC = TEdgeIterator<true, false>;
		using TEdgeIteratorRM = TEdgeIterator<false, true>;
		using TEdgeIteratorRC = TEdgeIterator<true, true>;

		// Friend classes

		friend class TGraph;

		// Vertex interface

		TEdge& allocateEdge (Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext) override;

		// Construction

		Vertex (Base::Graph& _graph, size_t _index);
		MF_U_NV_SA_D (VertexData) Vertex (Base::Graph& _graph, size_t _index, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) Vertex (Base::Graph& _graph, size_t _index, TNonVoidVertexData&& _data);

	public:

		//Getters

		const TGraph& graph () const;
		TGraph& graph ();

		const TEdge& outEdge (const Base::Vertex& _to) const;
		TEdge& outEdge (Base::Vertex& _to);
		const TEdge& outEdge (size_t _to) const;
		TEdge& outEdge (size_t _to);

		const TEdge* outEdgeIfExists (const Base::Vertex& _to) const;
		TEdge* outEdgeIfExists (Base::Vertex& _to);
		const TEdge* outEdgeIfExists (size_t _to) const;
		TEdge* outEdgeIfExists (size_t _to);

		const TEdge& operator[] (const Base::Vertex& _to) const;
		TEdge& operator[] (Base::Vertex& _to);
		const TEdge& operator[] (size_t _to) const;
		TEdge& operator[] (size_t _to);

		// Edge insertion

		TEdge& addOutEdge (Base::Vertex& _to);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdge (Base::Vertex& _to, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdge (Base::Vertex& _to, TNonVoidEdgeData&& _data);
		TEdge& addOutEdge (size_t _to);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdge (size_t _to, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdge (size_t _to, TNonVoidEdgeData&& _data);
		TEdge& addOutEdgeBefore (Base::Vertex& _to, Base::Edge& _next);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdgeBefore (Base::Vertex& _to, Base::Edge& _next, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdgeBefore (Base::Vertex& _to, Base::Edge& _next, TNonVoidEdgeData&& _data);
		TEdge& addOutEdgeBefore (size_t _to, Base::Edge& _next);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdgeBefore (size_t _to, Base::Edge& _next, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) TEdge& addOutEdgeBefore (size_t _to, Base::Edge& _next, TNonVoidEdgeData&& _data);

		// Iteration

		TEdgeIteratorFC begin () const;
		TEdgeIteratorFM begin ();
		TEdgeIteratorRC rbegin () const;
		TEdgeIteratorRM rbegin ();
		TEdgeIteratorFC cbegin () const;
		TEdgeIteratorRC crbegin () const;
		TEdgeIteratorFC end () const;
		TEdgeIteratorFM end ();
		TEdgeIteratorRC rend () const;
		TEdgeIteratorRM rend ();
		TEdgeIteratorFC cend () const;
		TEdgeIteratorRC crend () const;

		// Comparison

		using Base::Vertex::operator==;

	};

#pragma endregion

#pragma region Implementation

	MF_GG_M_S (MF_GG_M_A (Edge)&) allocateEdge (Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext)
	{
		if constexpr (std::is_default_constructible_v<TED> || std::is_void_v<TED>)
		{
			return *new TEdge{ *this, _to, _pPrevious,  _pNext };
		}
		else
		{
			throw std::runtime_error{ "non default constructible edge data" };
		}
	}

	MF_GG_M_PMC0 (MF_GG_M_A (Graph)&, graph);
	MF_GG_M_PMC1 (MF_GG_M_A (Edge)&, outEdge, Base::Vertex&);
	MF_GG_M_PMC1 (MF_GG_M_A (Edge)&, outEdge, size_t);
	MF_GG_M_PMC1 (MF_GG_M_A (Edge)*, outEdgeIfExists, Base::Vertex&);
	MF_GG_M_PMC1 (MF_GG_M_A (Edge)*, outEdgeIfExists, size_t);
	MF_GG_M_PMC1 (MF_GG_M_A (Edge)&, operator[], Base::Vertex&);
	MF_GG_M_PMC1 (MF_GG_M_A (Edge)&, operator[], size_t);
	MF_GG_M_PM1 (MF_GG_M_A (Edge)&, addOutEdge, Base::Vertex&);
	MF_GG_M_PM1 (MF_GG_M_A (Edge)&, addOutEdge, size_t);
	MF_GG_M_PM2 (MF_GG_M_A (Edge)&, addOutEdgeBefore, Base::Vertex&, Base::Edge&);
	MF_GG_M_PM2 (MF_GG_M_A (Edge)&, addOutEdgeBefore, size_t, Base::Edge&);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorFC), begin);
	MF_GG_M_PM0 (MF_GG_M_A (EdgeIteratorFM), begin);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorRC), rbegin);
	MF_GG_M_PM0 (MF_GG_M_A (EdgeIteratorRM), rbegin);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorFC), cbegin);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorRC), crbegin);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorFC), end);
	MF_GG_M_PM0 (MF_GG_M_A (EdgeIteratorFM), end);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorRC), rend);
	MF_GG_M_PM0 (MF_GG_M_A (EdgeIteratorRM), rend);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorFC), cend);
	MF_GG_M_PC0 (MF_GG_M_A (EdgeIteratorRC), crend);

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdge (Base::Vertex& _to, const TNonVoidED& _data)
	{
		ensureValidNewOutEdge (_to);
		TEdge& edge{ *new TEdge{*this, _to, last (), nullptr, _data} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdge (size_t _to, const TNonVoidED& _data)
	{
		return addOutEdge (graph ()[_to], _data);
	}

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdge (Base::Vertex& _to, TNonVoidED&& _data)
	{
		ensureValidNewOutEdge (_to);
		TEdge& edge{ *new TEdge{*this, _to, last (), nullptr, std::move(_data)} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdge (size_t _to, TNonVoidED&& _data)
	{
		return addOutEdge (graph ()[_to], std::move (_data));
	}

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdgeBefore (Base::Vertex& _to, Base::Edge& _next, const TNonVoidED& _data)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		TEdge& edge{ *new TEdge{*this, _to, _next.previous(), _next, _data} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdgeBefore (size_t _to, Base::Edge& _next, const TNonVoidED& _data)
	{
		return addOutEdgeBefore (graph ()[_to], _next, _data);
	}

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdgeBefore (Base::Vertex& _to, Base::Edge& _next, TNonVoidED&& _data)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		TEdge& edge{ *new TEdge{*this, _to, _next.previous(), _next, std::move(_data)} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	MF_GG_M_TT MF_U_NV_SA_I (ED) MF_GG_M_TS (MF_GG_M_A (Edge)&) addOutEdgeBefore (size_t _to, Base::Edge& _next, TNonVoidED&& _data)
	{
		return addOutEdgeBefore (graph ()[_to], _next, std::move (_data));
	}

	MF_GG_M_TT inline Vertex<TVD, TED>::Vertex (Base::Graph& _graph, size_t _index)
		: Base::Vertex{ _graph, _index }, Utils::UserData<TVD> {}
	{
	}

	MF_GG_M_TT MF_U_NV_SA_I (VD) inline Vertex<TVD, TED>::Vertex (Base::Graph& _graph, size_t _index, const TNonVoidVD& _data)
		: Base::Vertex{ _graph, _index }, Utils::UserData<TVD> {_data}
	{
	}

	MF_GG_M_TT MF_U_NV_SA_I (VD) inline Vertex<TVD, TED>::Vertex (Base::Graph& _graph, size_t _index, TNonVoidVD&& _data)
		: Base::Vertex{ _graph, _index }, Utils::UserData<TVD> {std::move (_data)}
	{
	}

#pragma endregion

#undef MF_GG_M_C

}

#endif