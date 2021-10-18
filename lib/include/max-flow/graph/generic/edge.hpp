#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_EDGE
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_EDGE

#include <cstddef>
#include <utility>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/utils/user_data.hpp>
#include <max-flow/utils/macros/non_void.hpp>
#include <max-flow/graph/generic/vertex.hpp>
#include <max-flow/graph/base/edge.hpp>
#include <max-flow/graph/generic/macros.hpp>

#define MF_GG_M_C Edge

namespace MaxFlow::Graph::Generic
{

#pragma region Declaration

	using std::size_t;

	template<typename, typename>
	class Vertex;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	class Edge final : public Base::Edge, public Utils::UserData<TEdgeData>
	{

	private:

		// Aliases

		using TVertex = Vertex<TVertexData, TEdgeData>;
		using TEdge = Edge;

		// Friend classes

		friend class TVertex;

		// Construction

		Edge (Base::Vertex& _from, Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext);
		MF_U_NV_SA_D (EdgeData) Edge (Base::Vertex& _from, Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) Edge (Base::Vertex& _from, Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext, TNonVoidEdgeData&& _data);

	public:

		// Getters

		const TVertex& from () const;
		TVertex& from ();
		const TVertex& to () const;
		TVertex& to ();

		const Edge& antiParallel () const;
		Edge& antiParallel ();
		const Edge* antiParallelIfExists () const;
		Edge* antiParallelIfExists ();

		// Comparison

		using Base::Edge::operator==;

	};

#pragma endregion

#pragma region Implementation

	MF_GG_M_PMC0 (MF_GG_M_A (Vertex)&, from);
	MF_GG_M_PMC0 (MF_GG_M_A (Vertex)&, to);
	MF_GG_M_PMC0 (MF_GG_M_A (Edge)&, antiParallel);
	MF_GG_M_PMC0 (MF_GG_M_A (Edge)*, antiParallelIfExists);

	MF_GG_M_TT inline Edge<TVD, TED>::Edge (Base::Vertex& _from, Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext)
		: Base::Edge{ _from, _to, _pPrevious, _pNext }, Utils::UserData<TED> {}
	{}

	MF_GG_M_TT MF_U_NV_SA_I (ED) inline Edge<TVD, TED>::Edge (Base::Vertex& _from, Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext, const TNonVoidED& _data)
		: Base::Edge{ _from, _to, _pPrevious, _pNext }, Utils::UserData<TED> {_data}
	{}

	MF_GG_M_TT MF_U_NV_SA_I (ED) inline Edge<TVD, TED>::Edge (Base::Vertex& _from, Base::Vertex& _to, Base::Edge* _pPrevious, Base::Edge* _pNext, TNonVoidED&& _data)
		: Base::Edge{ _from, _to, _pPrevious, _pNext }, Utils::UserData<TED> {std::move(_data)}
	{}

#pragma endregion

}

#undef MF_GG_M_C

#endif