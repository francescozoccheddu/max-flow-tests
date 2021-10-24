#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_EDGE
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_EDGE

#include <cstddef>
#include <utility>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/utils/user_data.hpp>
#include <max-flow/utils/non_void.hpp>
#include <max-flow/graphs/generic/vertex.hpp>
#include <max-flow/graphs/base/edge.hpp>
#include <max-flow/graphs/generic/macros.hpp>

#define MF_GG_T Edge

namespace MaxFlow::Graphs::Generic
{

#pragma region Declaration

	MF_GG_TT_F class Edge final : public BEdge, public Utils::UserData<TEdgeData>
	{

	public:

		MF_GG_UG;
		MF_GG_UV;
		MF_GG_UI;

	private:

		// Friend classes

		friend class Vertex;

		// Construction

		Edge (BVertex& _from, BVertex& _to, BEdge* _pPrevious, BEdge* _pNext);
		MF_U_NV_SA_D (EdgeData) Edge (BVertex& _from, BVertex& _to, BEdge* _pPrevious, BEdge* _pNext, const TNonVoidEdgeData& _data);
		MF_U_NV_SA_D (EdgeData) Edge (BVertex& _from, BVertex& _to, BEdge* _pPrevious, BEdge* _pNext, TNonVoidEdgeData&& _data);

	public:

		// Getters

		const Vertex& from () const;
		Vertex& from ();
		const Vertex& to () const;
		Vertex& to ();

		const Edge& antiParallel () const;
		Edge& antiParallel ();
		const Edge* antiParallelIfExists () const;
		Edge* antiParallelIfExists ();

		// Comparison

		using BEdge::operator==;

	};

#pragma endregion

#pragma region Implementation

	MF_GG_PMC0 (MF_GG_A (Vertex)&, from);
	MF_GG_PMC0 (MF_GG_A (Vertex)&, to);
	MF_GG_PMC0 (MF_GG_TG&, antiParallel);
	MF_GG_PMC0 (MF_GG_TG*, antiParallelIfExists);

	MF_GG_CS (BVertex& _from, BVertex& _to, BEdge* _pPrevious, BEdge* _pNext)
		: BEdge{ _from, _to, _pPrevious, _pNext }, Utils::UserData<TED> {}
	{
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TCS (BVertex& _from, BVertex& _to, BEdge* _pPrevious, BEdge* _pNext, const TNonVoidED& _data)
		: BEdge{ _from, _to, _pPrevious, _pNext }, Utils::UserData<TED> {_data}
	{
	}

	MF_GG_TT MF_U_NV_SA_I (ED) MF_GG_TCS (BVertex& _from, BVertex& _to, BEdge* _pPrevious, BEdge* _pNext, TNonVoidED&& _data)
		: BEdge{ _from, _to, _pPrevious, _pNext }, Utils::UserData<TED> {std::move (_data)}
	{
	}

#pragma endregion

}

#undef MF_GG_T

#endif