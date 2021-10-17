#include <max-flow/graph/base/graph.hpp>
#include <iostream>

int main ()
{
	MaxFlow::Graph::Base::Graph g;
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

	return 0;
}