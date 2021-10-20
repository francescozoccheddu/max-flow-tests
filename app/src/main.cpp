#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>
#include <max-flow/solve.hpp>
#include <max-flow/graphs/algorithms/graphviz.hpp>

#include <iostream>

using std::cout;
using std::endl;
using namespace MaxFlow::Graphs;

int main ()
{
	FlowGraph<> g;
	g.addVertices (4);
	g[0].addOutEdge (1, EdgeFlow{ 4,0 });
	g[0].addOutEdge (2, EdgeFlow{ 2,0 });
	g[1].addOutEdge (2, EdgeFlow{ 10,2 });
	g[1].addOutEdge (3, EdgeFlow{ 5,0 });
	g[2].addOutEdge (1, EdgeFlow{ 5,3 });
	g[2].addOutEdge (3, EdgeFlow{ 1,0 });

	Algorithms::GraphVizSource::from (g).exportToFile ("c:/users/franc/desktop/original.pdf", Algorithms::GraphVizSource::EFormat::PDF);
	Algorithms::GraphVizSource::from (ResidualGraph::create(g)).exportToFile ("c:/users/franc/desktop/residual.pdf", Algorithms::GraphVizSource::EFormat::PDF);

	return 0;
}