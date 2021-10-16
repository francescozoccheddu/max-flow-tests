#include <max-flow/utils/graph.hpp>
#include <max-flow/solve.hpp>
#include <iostream>

int main ()
{

	MaxFlow::FlowGraphEdge<>* p;

	MaxFlow::Utils::Graph<void, int> g;
	g.reserve (10);
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();

	g[1].addOutEdge (g[0]);
	g[1].addOutEdge (g[1]);
	g[1].addOutEdge (g[2]);
	g[1].addOutEdge (g[3]);
	g.shrinkToFit ();


	for (MaxFlow::Utils::Graph<void, int>::Edge& e : g[1])
	{
		std::cout << *e;
	}

	std::cout << g.capacity ();

	return 0;
}