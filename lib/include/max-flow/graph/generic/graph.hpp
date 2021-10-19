#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH

#include <vector>
#include <cstddef>
#include <utility>
#include <max-flow/utils/macros/non_void.hpp>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/generic/vertex.hpp>
#include <max-flow/utils/iteration/contiguous_indirect.hpp>
#include <max-flow/graph/base/graph.hpp>
#include <max-flow/graph/generic/macros.hpp>

#define MF_GG_D_T Graph

namespace MaxFlow::Graph::Generic
{

	using std::size_t;

#pragma region Declaration

	MF_GG_D_TT_F class Graph final : public BGraph
	{
	public:

		MF_GG_D_UV;
		MF_GG_D_UE;
		MF_GG_D_UI;

	private:

		// Graph interface

		Vertex& allocateVertex (size_t _index) override;

	public:

		// Construction

		using BGraph::Graph;
		Graph (const Graph& _clone);
		Graph (Graph&& _moved);

		// Assignment

		Graph& operator=(const Graph& _clone);
		Graph& operator=(Graph&& _moved);

		// Getters

		const Vertex& vertex (size_t _index) const;
		Vertex& vertex (size_t _index);

		const Vertex& operator[] (size_t _index) const;
		Vertex& operator[] (size_t _index);

		// Vertex insertion

		Vertex& addVertex ();
		MF_U_NV_SA_D (VertexData) Vertex& addVertex (const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) Vertex& addVertex (TNonVoidVertexData&& _data);
		Vertex& addVertexBefore (BVertex& _next);
		MF_U_NV_SA_D (VertexData) Vertex& addVertexBefore (BVertex& _next, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) Vertex& addVertexBefore (BVertex& _next, TNonVoidVertexData&& _data);
		Vertex& addVertexBefore (size_t _next);
		MF_U_NV_SA_D (VertexData) Vertex& addVertexBefore (size_t _next, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) Vertex& addVertexBefore (size_t _next, TNonVoidVertexData&& _data);

		// Iteration

		VertexIteratorFC begin () const;
		VertexIteratorFM begin ();
		VertexIteratorRC rbegin () const;
		VertexIteratorRM rbegin ();
		VertexIteratorFC cbegin () const;
		VertexIteratorRC crbegin () const;
		VertexIteratorFC end () const;
		VertexIteratorFM end ();
		VertexIteratorRC rend () const;
		VertexIteratorRM rend ();
		VertexIteratorFC cend () const;
		VertexIteratorRC crend () const;

	};

#pragma endregion

#pragma region Implementation

	MF_GG_D_TT inline Graph<TVD, TED>::Graph (const Graph& _graph)
	{
		*this = _graph;
	}

	MF_GG_D_TT inline Graph<TVD, TED>::Graph (Graph&& _graph)
	{
		*this = std::move (_graph);
	}

	MF_GG_D_MS (MF_GG_D_A (Vertex)&) allocateVertex (size_t _index)
	{
		if constexpr (std::is_default_constructible_v<TVD> || std::is_void_v<TVD>)
		{
			return *new Vertex{ *this, _index };
		}
		else
		{
			throw std::runtime_error{ "non default constructible vertex data" };
		}
	}


	MF_GG_D_MS (MF_GG_D_TG&) operator=(const Graph& _clone)
	{
		destroyAllVertices ();
		shrinkToFit ();
		reserve (_clone.capacity ());
		setMatrix (_clone.hasMatrix ());
		for (const Vertex& vertex : _clone)
		{
			if constexpr (std::is_void_v<TVD>)
			{
				addVertex ();
			}
			else
			{
				addVertex (*vertex);
			}
		}
		for (const Vertex& vertex : _clone)
		{
			Vertex& thisVert{ (*this)[vertex.index ()] };
			for (const Edge& edge : vertex)
			{
				if constexpr (std::is_void_v<TED>)
				{
					thisVert.addOutEdge ((*this)[edge.to ().index ()]);
				}
				else
				{
					thisVert.addOutEdge ((*this)[edge.to ().index ()], *edge);
				}
			}
		}
		return *this;
	}

	MF_GG_D_PM1 (MF_GG_D_TG&, operator=, Graph&&);
	MF_GG_D_PMC1 (MF_GG_D_A (Vertex)&, vertex, size_t);
	MF_GG_D_PMC1 (MF_GG_D_A (Vertex)&, operator[], size_t);
	MF_GG_D_PM0 (MF_GG_D_A (Vertex)&, addVertex);
	MF_GG_D_PM1 (MF_GG_D_A (Vertex)&, addVertexBefore, BVertex&);
	MF_GG_D_PM1 (MF_GG_D_A (Vertex)&, addVertexBefore, size_t);

	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorFC), begin);
	MF_GG_D_PM0 (MF_GG_D_A (VertexIteratorFM), begin);
	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorRC), rbegin);
	MF_GG_D_PM0 (MF_GG_D_A (VertexIteratorRM), rbegin);
	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorFC), cbegin);
	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorRC), crbegin);
	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorFC), end);
	MF_GG_D_PM0 (MF_GG_D_A (VertexIteratorFM), end);
	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorRC), rend);
	MF_GG_D_PM0 (MF_GG_D_A (VertexIteratorRM), rend);
	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorFC), cend);
	MF_GG_D_PC0 (MF_GG_D_A (VertexIteratorRC), crend);

	MF_GG_D_TT MF_U_NV_SA_I (VD) MF_GG_D_TMS (MF_GG_D_A (Vertex)&) addVertex (const TNonVoidVD& _data)
	{
		Vertex& vertex{ *new Vertex {*this, verticesCount (), _data} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	MF_GG_D_TT MF_U_NV_SA_I (VD) MF_GG_D_TMS (MF_GG_D_A (Vertex)&) addVertex (TNonVoidVD&& _data)
	{
		Vertex& vertex{ new Vertex {*this, verticesCount (), std::move (_data)} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	MF_GG_D_TT MF_U_NV_SA_I (VD) MF_GG_D_TMS (MF_GG_D_A (Vertex)&) addVertexBefore (BVertex& _next, const TNonVoidVD& _data)
	{
		addVertexBefore (_next.index (), _data);
	}

	MF_GG_D_TT MF_U_NV_SA_I (VD) MF_GG_D_TMS (MF_GG_D_A (Vertex)&) addVertexBefore (size_t _next, const TNonVoidVD& _data)
	{
		ensureValidVertexIndex (_next);
		Vertex& vertex{ *new Vertex {*this, _next,_data} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	MF_GG_D_TT MF_U_NV_SA_I (VD) MF_GG_D_TMS (MF_GG_D_A (Vertex)&) addVertexBefore (BVertex& _next, TNonVoidVD&& _data)
	{
		addVertexBefore (_next.index (), std::move (_data));
	}

	MF_GG_D_TT MF_U_NV_SA_I (VD) MF_GG_D_TMS (MF_GG_D_A (Vertex)&) addVertexBefore (size_t _next, TNonVoidVD&& _data)
	{
		ensureValidVertexIndex (_next);
		Vertex& vertex{ *new Vertex {*this, _next, std::move (_data)} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

#pragma endregion

}

#undef MF_GG_D_T

#endif