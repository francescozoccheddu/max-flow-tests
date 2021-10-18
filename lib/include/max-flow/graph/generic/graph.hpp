#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH

#include <vector>
#include <cstddef>
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

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	class Graph final : public Base::Graph
	{

		// Aliases

		using TVertex = Vertex<TVertexData, TEdgeData>;

		template<bool constant, bool reversed>
		using TVertexIterator = Utils::Iteration::VecIterator<TVertex, constant, reversed>;

		using TVertexIteratorFM = TVertexIterator<false, false>;
		using TVertexIteratorFC = TVertexIterator<true, false>;
		using TVertexIteratorRM = TVertexIterator<false, true>;
		using TVertexIteratorRC = TVertexIterator<true, true>;

	public:

		// Construction

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

		virtual TVertex& addVertex () override;
		MF_U_NV_SA_D (VertexData) TVertex& addVertex (const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) TVertex& addVertex (TNonVoidVertexData&& _data);
		TVertex& addVertexBefore (Base::Vertex& _next);
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (Base::Vertex& _next, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (Base::Vertex& _next, TNonVoidVertexData&& _data);
		virtual TVertex& addVertexBefore (size_t _next) override;
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (size_t _next, const TNonVoidVertexData& _data);
		MF_U_NV_SA_D (VertexData) TVertex& addVertexBefore (size_t _next, TNonVoidVertexData&& _data);

		// Iteration

		TVertexIterator<true, false> begin () const;
		TVertexIterator<false, false> begin ();
		TVertexIterator<true, true> rbegin () const;
		TVertexIterator<false, true> rbegin ();
		TVertexIterator<true, false> cbegin () const;
		TVertexIterator<true, true> crbegin () const;
		TVertexIterator<true, false> end () const;
		TVertexIterator<false, false> end ();
		TVertexIterator<true, true> rend () const;
		TVertexIterator<false, true> rend ();
		TVertexIterator<true, false> cend () const;
		TVertexIterator<true, true> crend () const;

	};

#pragma endregion

#pragma region Implementation

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

#pragma endregion

}

#undef MF_GG_M_C

#endif