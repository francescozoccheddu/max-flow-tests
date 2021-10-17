#include <max-flow/utils/graph.hpp>
#include <max-flow/utils/graph_equality.hpp>
#include <max-flow/solve.hpp>
#include <iostream>

#include <functional>

int main ()
{

	MaxFlow::Utils::Graph<int, int> g;
	g.reserve (10);
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

	std::cout << MaxFlow::Utils::GraphEquality::equals (b, g);

	return 0;
}