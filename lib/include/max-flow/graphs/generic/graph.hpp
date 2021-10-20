#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH

#include <vector>
#include <cstddef>
#include <utility>
#include <max-flow/utils/macros/non_void.hpp>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graphs/generic/vertex.hpp>
#include <max-flow/utils/iteration/contiguous_indirect.hpp>
#include <max-flow/graphs/base/graph.hpp>
#include <max-flow/graphs/generic/macros.hpp>

#define MF_GG_T Graph

namespace MaxFlow::Graphs::Generic
{

	using std::size_t;

#pragma region Declaration

	MF_GG_TT_F class Graph final : public BGraph
	{
	public:

		MF_GG_UV;
		MF_GG_UE;
		MF_GG_UI;

	private:

		// Graph interface

		BVertex& allocateVertex (size_t _index) override;

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
		Vertex& addVertexAt (size_t _next);
		MF_U_NV_SA_D (VertexData) Vertex& addVertexAt (size_t _next, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) Vertex& addVertexAt (size_t _next, TNonVoidVertexData&& _data);

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

	MF_GG_TT inline Graph<TVD, TED>::Graph (const Graph& _graph)
	{
		*this = _graph;
	}

	MF_GG_TT inline Graph<TVD, TED>::Graph (Graph&& _graph)
	{
		*this = std::move (_graph);
	}

	MF_GG_MS (BVertex&) allocateVertex (size_t _index)
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


	MF_GG_MS (MF_GG_TG&) operator=(const Graph& _clone)
	{
		setMatrix (false);
		destroyAllVertices ();
		shrinkToFit ();
		reserve (_clone.capacity ());
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

	MF_GG_MS (MF_GG_TG&) operator=(Graph&& _moved)
	{
		return static_cast<Graph&>(BGraph::operator=(std::move (_moved)));
	}

	MF_GG_PMC1 (MF_GG_A (Vertex)&, vertex, size_t);
	MF_GG_PMC1 (MF_GG_A (Vertex)&, operator[], size_t);
	MF_GG_PM0 (MF_GG_A (Vertex)&, addVertex);
	MF_GG_PM1 (MF_GG_A (Vertex)&, addVertexBefore, BVertex&);
	MF_GG_PM1 (MF_GG_A (Vertex)&, addVertexAt, size_t);

	MF_GG_PC0 (MF_GG_A (VertexIteratorFC), begin);
	MF_GG_PM0 (MF_GG_A (VertexIteratorFM), begin);
	MF_GG_PC0 (MF_GG_A (VertexIteratorRC), rbegin);
	MF_GG_PM0 (MF_GG_A (VertexIteratorRM), rbegin);
	MF_GG_PC0 (MF_GG_A (VertexIteratorFC), cbegin);
	MF_GG_PC0 (MF_GG_A (VertexIteratorRC), crbegin);
	MF_GG_PC0 (MF_GG_A (VertexIteratorFC), end);
	MF_GG_PM0 (MF_GG_A (VertexIteratorFM), end);
	MF_GG_PC0 (MF_GG_A (VertexIteratorRC), rend);
	MF_GG_PM0 (MF_GG_A (VertexIteratorRM), rend);
	MF_GG_PC0 (MF_GG_A (VertexIteratorFC), cend);
	MF_GG_PC0 (MF_GG_A (VertexIteratorRC), crend);

	MF_GG_TT MF_U_NV_SA_I (VD) MF_GG_TMS (MF_GG_A (Vertex)&) addVertex (const TNonVoidVD& _data)
	{
		return addVertexAt (verticesCount (), _data);
	}

	MF_GG_TT MF_U_NV_SA_I (VD) MF_GG_TMS (MF_GG_A (Vertex)&) addVertex (TNonVoidVD&& _data)
	{
		return addVertexAt (verticesCount (), std::move(_data));
	}

	MF_GG_TT MF_U_NV_SA_I (VD) MF_GG_TMS (MF_GG_A (Vertex)&) addVertexBefore (BVertex& _next, const TNonVoidVD& _data)
	{
		ensureSameGraph (_next.graph (), *this);
		return addVertexAt (_next.index (), _data);
	}

	MF_GG_TT MF_U_NV_SA_I (VD) MF_GG_TMS (MF_GG_A (Vertex)&) addVertexAt (size_t _next, const TNonVoidVD& _data)
	{
		ensureValidOrLastVertexIndex (_next);
		Vertex& vertex{ *new Vertex {*this, _next,_data} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	MF_GG_TT MF_U_NV_SA_I (VD) MF_GG_TMS (MF_GG_A (Vertex)&) addVertexBefore (BVertex& _next, TNonVoidVD&& _data)
	{
		ensureSameGraph (_next.graph (), *this);
		return addVertexAt (_next.index (), std::move (_data));
	}

	MF_GG_TT MF_U_NV_SA_I (VD) MF_GG_TMS (MF_GG_A (Vertex)&) addVertexAt (size_t _next, TNonVoidVD&& _data)
	{
		ensureValidOrLastVertexIndex (_next);
		Vertex& vertex{ *new Vertex {*this, _next, std::move (_data)} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

#pragma endregion

}

#undef MF_GG_T

#endif