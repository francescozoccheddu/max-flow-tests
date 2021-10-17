#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_GRAPH

#include <vector>
#include <cstddef>
#include <max-flow/utils/non_void.hpp>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/generic/vertex.hpp>
#include <max-flow/utils/vec_iterator.hpp>
#include <max-flow/graph/base/graph.hpp>

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

		template<bool constant, bool reversed>
		using TVertexIterator = Utils::VecIterator<TVertex, constant, reversed>;
		using TVertex = Vertex<TVertexData, TEdgeData>;

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
		MAX_FLOW_NON_VOID_T_SA_DECL (VertexData) TVertex& addVertex (const TNonVoidVertexData& _data);
		MAX_FLOW_NON_VOID_T_SA_DECL (VertexData) TVertex& addVertex (TNonVoidVertexData&& _data);
		TVertex& addVertexBefore (Base::Vertex& _next);
		MAX_FLOW_NON_VOID_T_SA_DECL (VertexData) TVertex& addVertexBefore (const TNonVoidVertexData& _data, Base::Vertex& _next);
		MAX_FLOW_NON_VOID_T_SA_DECL (VertexData) TVertex& addVertexBefore (TNonVoidVertexData&& _data, Base::Vertex& _next);
		virtual TVertex& addVertexBefore (size_t _next) override;
		MAX_FLOW_NON_VOID_T_SA_DECL (VertexData) TVertex& addVertexBefore (const TNonVoidVertexData& _data, size_t _next);
		MAX_FLOW_NON_VOID_T_SA_DECL (VertexData) TVertex& addVertexBefore (TNonVoidVertexData&& _data, size_t _next);

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

#pragma endregion

}

#endif