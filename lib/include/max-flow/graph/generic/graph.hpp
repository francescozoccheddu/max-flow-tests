#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH

#include <vector>
#include <cstddef>
#include <utility>
#include <max-flow/utils/macros/non_void.hpp>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/generic/vertex.hpp>
#include <max-flow/utils/iteration/vec_iterator.hpp>
#include <max-flow/graph/base/graph.hpp>
#include <max-flow/graph/generic/macros.hpp>

#define MF_GG_M_C Graph

namespace MaxFlow::Graph::Generic
{

#pragma region Declaration

	using std::size_t;

	template<typename, typename>
	class Vertex;

	template<typename, typename>
	class Edge;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	class Graph final : public Base::Graph
	{

	private:

		// Aliases

		using TGraph = Graph;
		using TVertex = Vertex<TVertexData, TEdgeData>;
		using TEdge = Edge<TVertexData, TEdgeData>;

		template<bool constant, bool reversed>
		using TVertexIterator = Utils::Iteration::VecIterator<TVertex, constant, reversed>;

		using TVertexIteratorFM = TVertexIterator<false, false>;
		using TVertexIteratorFC = TVertexIterator<true, false>;
		using TVertexIteratorRM = TVertexIterator<false, true>;
		using TVertexIteratorRC = TVertexIterator<true, true>;

		// Graph interface

		TVertex& allocateVertex (size_t _index) override;

	public:

		// Construction

		using Base::Graph::Graph;
		Graph (const Graph& _clone);
		Graph (Graph&& _moved);

		// Assignment

		Graph& operator=(const Graph& _clone);
		Graph& operator=(Graph&& _moved);

		// Getters

		const TVertex& vertex (size_t _index) const;
		TVertex& vertex (size_t _index);

		const TVertex& operator[] (size_t _index) const;
		TVertex& operator[] (size_t _index);

		// Vertex insertion

		TVertex& addVertex ();
		MF_U_NV_SA_D (VertexData) TVertex& addVertex (const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) TVertex& addVertex (TNonVoidVertexData&& _data);
		TVertex& addVertexBefore (Base::Vertex& _next);
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (Base::Vertex& _next, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (Base::Vertex& _next, TNonVoidVertexData&& _data);
		TVertex& addVertexBefore (size_t _next);
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (size_t _next, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (size_t _next, TNonVoidVertexData&& _data);

		// Iteration

		TVertexIteratorFC begin () const;
		TVertexIteratorFM begin ();
		TVertexIteratorRC rbegin () const;
		TVertexIteratorRM rbegin ();
		TVertexIteratorFC cbegin () const;
		TVertexIteratorRC crbegin () const;
		TVertexIteratorFC end () const;
		TVertexIteratorFM end ();
		TVertexIteratorRC rend () const;
		TVertexIteratorRM rend ();
		TVertexIteratorFC cend () const;
		TVertexIteratorRC crend () const;

	};

#pragma endregion

#pragma region Implementation

	MF_GG_M_TT inline Graph<TVD, TED>::Graph (const Graph& _graph)
	{
		*this = _graph;
	}

	MF_GG_M_TT inline Graph<TVD, TED>::Graph (Graph&& _graph)
	{
		*this = std::move (_graph);
	}

	MF_GG_M_S (MF_GG_M_A (Vertex)&) allocateVertex (size_t _index)
	{
		if constexpr (std::is_default_constructible_v<TVD> || std::is_void_v<TVD>)
		{
			return *new TVertex{ *this, _index };
		}
		else
		{
			throw std::runtime_error{ "non default constructible vertex data" };
		}
	}


	MF_GG_M_S (MF_GG_M_A (Graph)&) operator=(const Graph& _clone)
	{
		destroyAllVertices ();
		shrinkToFit ();
		reserve (_clone.capacity ());
		setMatrix (_clone.hasMatrix ());
		for (const TVertex& vertex : _clone)
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
		for (const TVertex& vertex : _clone)
		{
			TVertex& thisVert{ (*this)[vertex.index ()] };
			for (const TEdge& edge : vertex)
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

	MF_GG_M_PM1 (MF_GG_M_A (Graph)&, operator=, Graph&&);
	MF_GG_M_PMC1 (MF_GG_M_A (Vertex)&, vertex, size_t);
	MF_GG_M_PMC1 (MF_GG_M_A (Vertex)&, operator[], size_t);
	MF_GG_M_PM0 (MF_GG_M_A (Vertex)&, addVertex);
	MF_GG_M_PM1 (MF_GG_M_A (Vertex)&, addVertexBefore, Base::Vertex&);
	MF_GG_M_PM1 (MF_GG_M_A (Vertex)&, addVertexBefore, size_t);

	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorFC), begin);
	MF_GG_M_PM0 (MF_GG_M_A (VertexIteratorFM), begin);
	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorRC), rbegin);
	MF_GG_M_PM0 (MF_GG_M_A (VertexIteratorRM), rbegin);
	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorFC), cbegin);
	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorRC), crbegin);
	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorFC), end);
	MF_GG_M_PM0 (MF_GG_M_A (VertexIteratorFM), end);
	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorRC), rend);
	MF_GG_M_PM0 (MF_GG_M_A (VertexIteratorRM), rend);
	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorFC), cend);
	MF_GG_M_PC0 (MF_GG_M_A (VertexIteratorRC), crend);

	MF_GG_M_TT MF_U_NV_SA_I (VD) MF_GG_M_TS (MF_GG_M_A (Vertex)&) addVertex (const TNonVoidVD& _data)
	{
		TVertex& vertex{ *new TVertex {*this, verticesCount (), _data} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	MF_GG_M_TT MF_U_NV_SA_I (VD) MF_GG_M_TS (MF_GG_M_A (Vertex)&) addVertex (TNonVoidVD&& _data)
	{
		TVertex& vertex{ new TVertex {*this, verticesCount (), std::move (_data)} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	MF_GG_M_TT MF_U_NV_SA_I (VD) MF_GG_M_TS (MF_GG_M_A (Vertex)&) addVertexBefore (Base::Vertex& _next, const TNonVoidVD& _data)
	{
		addVertexBefore (_next.index (), _data);
	}

	MF_GG_M_TT MF_U_NV_SA_I (VD) MF_GG_M_TS (MF_GG_M_A (Vertex)&) addVertexBefore (size_t _next, const TNonVoidVD& _data)
	{
		ensureValidVertexIndex (_next);
		TVertex& vertex{ *new TVertex {*this, _next,_data} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	MF_GG_M_TT MF_U_NV_SA_I (VD) MF_GG_M_TS (MF_GG_M_A (Vertex)&) addVertexBefore (Base::Vertex& _next, TNonVoidVD&& _data)
	{
		addVertexBefore (_next.index (), std::move (_data));
	}

	MF_GG_M_TT MF_U_NV_SA_I (VD) MF_GG_M_TS (MF_GG_M_A (Vertex)&) addVertexBefore (size_t _next, TNonVoidVD&& _data)
	{
		ensureValidVertexIndex (_next);
		TVertex& vertex{ *new TVertex {*this, _next, std::move (_data)} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

#pragma endregion

}

#undef MF_GG_M_C

#endif