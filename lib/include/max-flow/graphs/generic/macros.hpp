#ifndef INCLUDED_MAX_FLOW_GRAPH_GENERIC_MACROS
#define INCLUDED_MAX_FLOW_GRAPH_GENERIC_MACROS

#define MF_GG_D_TT template<typename TVD, typename TED>
#define MF_GG_D_TT_F template<typename TVertexData, typename TEdgeData>
#define MF_GG_D_TT_FD template<typename TVertexData = void, typename TEdgeData = TVertexData>

#define MF_GG_D_TG MaxFlow::Graphs::Generic:: MF_GG_D_T <TVD,TED>
#define MF_GG_D_TG_F MaxFlow::Graphs::Generic:: MF_GG_D_T <TVertexData,TEdgeData>

#define MF_GG_D_A(Alias) MF_GG_D_TG:: Alias
#define MF_GG_D_A_F(Alias) MF_GG_D_TG_F :: Alias

#define MF_GG_D_TFS(ReturnType) inline ReturnType 
#define MF_GG_D_FS(ReturnType) MF_GG_D_TT MF_GG_D_TFS(ReturnType)

#define MF_GG_D_TMS(ReturnType) MF_GG_D_TFS(ReturnType) MF_GG_D_TG ::
#define MF_GG_D_MS(ReturnType) MF_GG_D_TT MF_GG_D_TMS(ReturnType)

#define MF_GG_D_TCS inline MF_GG_D_TG :: MF_GG_D_T
#define MF_GG_D_CS MF_GG_D_TT MF_GG_D_TCS

#define MF_GG_D_PB(ReturnType, Method, ...) { return static_cast< ReturnType >( MaxFlow::Graphs::Base:: MF_GG_D_T :: Method ( __VA_ARGS__ )); }

#define MF_GG_D_P0(ReturnType, Method, Modifier) MF_GG_D_MS(ReturnType) Method () Modifier MF_GG_D_PB(ReturnType, Method)
#define MF_GG_D_P1(ReturnType, Method, Type1, Modifier) MF_GG_D_MS(ReturnType) Method (Type1 _a1) Modifier MF_GG_D_PB(ReturnType, Method, _a1)
#define MF_GG_D_P2(ReturnType, Method, Type1, Type2, Modifier) MF_GG_D_MS(ReturnType) Method (Type1 _a1, Type2 _a2) Modifier MF_GG_D_PB(ReturnType, Method, _a1, _a2)
#define MF_GG_D_P3(ReturnType, Method, Type1, Type2, Type3, Modifier) MF_GG_D_MS(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3) Modifier MF_GG_D_PB(ReturnType, Method, _a1, _a2, _a3)
#define MF_GG_D_P4(ReturnType, Method, Type1, Type2, Type3, Type4, Modifier) MF_GG_D_MS(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3, Type4 _a4) Modifier MF_GG_D_PB(ReturnType, Method, _a1, _a2, _a3, _a4)
#define MF_GG_D_P5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Modifier) MF_GG_D_MS(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3, Type4 _a4, Type4 _a5) Modifier MF_GG_D_PB(ReturnType, Method, _a1, _a2, _a3, _a4, _a5)
#define MF_GG_D_P6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6, Modifier) MF_GG_D_MS(ReturnType) Method (Type1 _a1, Type2 _a2, Type3 _a3, Type4 _a4, Type4 _a5, Type4 _a6) Modifier MF_GG_D_PB(ReturnType, Method, _a1, _a2, _a3, _a4, _a5, _a6)

#define MF_GG_D_PM0(ReturnType, Method) MF_GG_D_P0(ReturnType, Method, )
#define MF_GG_D_PM1(ReturnType, Method, Type1) MF_GG_D_P1(ReturnType, Method, Type1, )
#define MF_GG_D_PM2(ReturnType, Method, Type1, Type2) MF_GG_D_P2(ReturnType, Method, Type1, Type2, )
#define MF_GG_D_PM3(ReturnType, Method, Type1, Type2, Type3) MF_GG_D_P3(ReturnType, Method, Type1, Type2, Type3, )
#define MF_GG_D_PM4(ReturnType, Method, Type1, Type2, Type3, Type4) MF_GG_D_P4(ReturnType, Method, Type1, Type2, Type3, Type4, )
#define MF_GG_D_PM5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5) MF_GG_D_P5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, )
#define MF_GG_D_PM6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6) MF_GG_D_P6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6, ) 

#define MF_GG_D_PC0(ReturnType, Method) MF_GG_D_P0(ReturnType, Method, const)
#define MF_GG_D_PC1(ReturnType, Method, Type1) MF_GG_D_P1(ReturnType, Method, Type1, const)
#define MF_GG_D_PC2(ReturnType, Method, Type1, Type2) MF_GG_D_P2(ReturnType, Method, Type1, Type2, const)
#define MF_GG_D_PC3(ReturnType, Method, Type1, Type2, Type3) MF_GG_D_P3(ReturnType, Method, Type1, Type2, Type3, const)
#define MF_GG_D_PC4(ReturnType, Method, Type1, Type2, Type3, Type4) MF_GG_D_P4(ReturnType, Method, Type1, Type2, Type3, Type4, const)
#define MF_GG_D_PC5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5) MF_GG_D_P5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, const)
#define MF_GG_D_PC6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6) MF_GG_D_P6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6, const) 

#define MF_GG_D_PMC0(ReturnType, Method) MF_GG_D_PC0(const ReturnType, Method) MF_GG_D_PM0(ReturnType, Method)
#define MF_GG_D_PMC1(ReturnType, Method, Type1) MF_GG_D_PC1(const ReturnType, Method, const Type1) MF_GG_D_PM1(ReturnType, Method, Type1)
#define MF_GG_D_PMC2(ReturnType, Method, Type1, Type2) MF_GG_D_PC2(const ReturnType, Method, const Type1, const Type2) MF_GG_D_PM2(ReturnType, Method, Type1, Type2)
#define MF_GG_D_PMC3(ReturnType, Method, Type1, Type2, Type3) MF_GG_D_PC3(const ReturnType, Method, const Type1, const Type2, const Type3) MF_GG_D_PM3(ReturnType, Method, Type1, Type2, Type3)
#define MF_GG_D_PMC4(ReturnType, Method, Type1, Type2, Type3, Type4) MF_GG_D_PC4(const ReturnType, Method, const Type1, const Type2, const Type3, const Type4) MF_GG_D_PM4(ReturnType, Method, Type1, Type2, Type3, Type4)
#define MF_GG_D_PMC5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5) MF_GG_D_PC5(const ReturnType, Method, const Type1, const Type2, const Type3, const Type4, const Type5) MF_GG_D_PM5(ReturnType, Method, Type1, Type2, Type3, Type4, Type5)
#define MF_GG_D_PMC6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6) MF_GG_D_PC6(const ReturnType, Method, const Type1, const Type2, const Type3, const Type4, const Type5, const Type6) MF_GG_D_PM6(ReturnType, Method, Type1, Type2, Type3, Type4, Type5, Type6)

#define MF_GG_D_UG using Graph = ::MaxFlow::Graphs::Generic::Graph<TVertexData, TEdgeData>; 
#define MF_GG_D_UV using Vertex = ::MaxFlow::Graphs::Generic::Vertex<TVertexData, TEdgeData>; 
#define MF_GG_D_UE using Edge = ::MaxFlow::Graphs::Generic::Edge<TVertexData, TEdgeData>; 

#define MF_GG_D_UI \
template<bool constant, bool reversed> \
using VertexIterator = MaxFlow::Utils::Iteration::ContiguousIndirectIterator<TVertexData, constant, reversed>; \
\
template<bool constant, bool reversed> \
using EdgeIterator = MaxFlow::Utils::Iteration::DoublyLinkedIterator<Edge, constant, reversed>; \
\
using VertexIteratorFM = VertexIterator<false, false>; \
using VertexIteratorFC = VertexIterator<true, false>; \
using VertexIteratorRM = VertexIterator<false, true>; \
using VertexIteratorRC = VertexIterator<true, true>; \
\
using EdgeIteratorFM = EdgeIterator<false, false>; \
using EdgeIteratorFC = EdgeIterator<true, false>; \
using EdgeIteratorRM = EdgeIterator<false, true>; \
using EdgeIteratorRC = EdgeIterator<true, true>;

namespace MaxFlow::Utils::Iteration
{

	template<typename TData, bool constant, bool reversed>
	class ContiguousIndirectIterator;

	template<typename TNode, bool constant, bool reversed>
	class DoublyLinkedIterator;

}

namespace MaxFlow::Graphs::Base
{

	class Graph;
	class Vertex;
	class Edge;

}

namespace MaxFlow::Graphs::Generic
{

	using BGraph = Base::Graph;
	using BVertex = Base::Vertex;
	using BEdge = Base::Edge;

	MF_GG_D_TT_FD class Graph;
	MF_GG_D_TT_FD class Edge;
	MF_GG_D_TT_FD class Vertex;

}

#endif