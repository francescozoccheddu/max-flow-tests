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
	FlowGraph<> g{ 4 };
	g[0].addOutEdge (1, EdgeFlow{ 4,0 });
	g[0].addOutEdge (2, EdgeFlow{ 2,0 });
	g[1].addOutEdge (2, EdgeFlow{ 3,0 });
	g[1].addOutEdge (3, EdgeFlow{ 5,0 });
	g[2].addOutEdge (3, EdgeFlow{ 1,0 });

	Algorithms::GraphVizSource::from (g).toDotFile ("c:/users/franc/desktop/ciao.dot");
	Algorithms::GraphVizSource::from (g).exportToFile ("c:/users/franc/desktop/ciao.pdf", Algorithms::GraphVizSource::EFormat::PDF);

	return 0;
}