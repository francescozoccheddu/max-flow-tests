#include <max-flow/graph/generic/graph.hpp>
#include <max-flow/graph/algorithms/equality.hpp>
#include <iostream>

using namespace MaxFlow::Graph::Algorithms::Equality;
using BGraph = MaxFlow::Graph::Base::Graph;
using BVertex = MaxFlow::Graph::Base::Vertex;
using BEdge = MaxFlow::Graph::Base::Edge;
template<typename V, typename E>
using GGraph = MaxFlow::Graph::Generic::Graph<V,E>;
template<typename V, typename E>
using GVertex = MaxFlow::Graph::Generic::Vertex<V,E>;
template<typename V, typename E>
using GEdge = MaxFlow::Graph::Generic::Edge<V,E>;

int main ()
{
	GGraph<int,int> g;
	g.reserve (10);
	g.setMatrix (true);
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();

	g[0].addOutEdge (g[0]);
	g[1].addOutEdge (g[0]);
	g[1].addOutEdge (g[1]);
	g[1].addOutEdge (g[2]);
	g[1].addOutEdge (g[3]);
	g.shrinkToFit ();

	auto b = g;
	*b[0] = 7;

	std::cout << equals(b, g);

	return 0;
}