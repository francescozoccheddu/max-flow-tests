#include <max-flow/graph/generic/graph.hpp>
#include <iostream>

int main ()
{
	MaxFlow::Graph::Generic::Graph<int,int> g;
	g.reserve (10);
	g.setMatrix (true);
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();
	g.addVertex ();

	g[0].addOutEdge (g[0], 1);
	g[1].addOutEdge (g[0], 2);
	g[1].addOutEdge (g[1], 3);
	g[1].addOutEdge (g[2], 4);
	g[1].addOutEdge (g[3], 5);
	g.shrinkToFit ();

	for (MaxFlow::Graph::Generic::Edge<int, int>& v : g[1])
	{
		std::cout << *v;
	}

	return 0;
}