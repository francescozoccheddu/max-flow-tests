#include <max-flow/graph.hpp>
#include <iostream>

int main ()
{
	MaxFlow::Graph<int, int> g;
	g.addVertex (0);
	g.addVertex (1);
	g.addVertex (2);
	g.addVertex (3);
	g.addVertex (4);

	g[1].addOutEdge (0, g[0]);
	g[1].addOutEdge (1, g[1]);
	g[1].addOutEdge (2, g[2]);
	g[1].addOutEdge (3, g[3]);

	auto b = std::move(g);

	for (MaxFlow::Graph<int, int>::Edge& e : g[1])
	{
		std::cout << *e;
	}

	return 0;
}