#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_EDGE
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_EDGE

#include <cstddef>
#include <max-flow/utils/reference_type.hpp>
#include <max-flow/graph/generic/vertex.hpp>
#include <max-flow/graph/base/edge.hpp>

namespace MaxFlow::Graph::Generic 
{

#pragma region Declaration

	using std::size_t;

	template<typename, typename>
	class Vertex;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	class Edge final : public Base::Edge
	{

		// Aliases

		using TVertex = Vertex<TVertexData, TEdgeData>;

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

	};

#pragma endregion

#pragma region Implementation

#pragma endregion

}

#endif