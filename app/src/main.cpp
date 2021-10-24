#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>
#include <max-flow/solve.hpp>
#include <max-flow/graphs/algorithms/graphviz.hpp>
#include <max-flow/utils/performance.hpp>
#include <max-flow-app/random.hpp>
#include <max-flow-app/verify.hpp>

#include <iostream>

using std::cout;
using std::endl;
using namespace MaxFlow::Graphs;

int main ()
{

	MaxFlow::Utils::Performance::start ();

	FlowGraph<> o;
	o.addVertices (4);
	o[0].addOutEdge (1, EdgeFlow{ 4,0 });
	o[0].addOutEdge (2, EdgeFlow{ 2,0 });
	o[1].addOutEdge (3, EdgeFlow{ 5,0 });
	o[2].addOutEdge (1, EdgeFlow{ 3,0 });
	o[2].addOutEdge (3, EdgeFlow{ 1,0 });

	FlowGraph<> s{ o };

	MaxFlow::App::RandomFlowGraph r{ };
	r.clean ();

	MaxFlow::solve (s, s[0], s[3], MaxFlow::ESolver::HighestLabelPreflowPush);


	Algorithms::GraphVizSource::from (o).exportToFile ("c:/users/franc/desktop/original.pdf", Algorithms::GraphVizSource::EFormat::PDF);
	Algorithms::GraphVizSource::from (s).exportToFile ("c:/users/franc/desktop/solution.pdf", Algorithms::GraphVizSource::EFormat::PDF);
	Algorithms::GraphVizSource::from (r.graph()).exportToFile ("c:/users/franc/desktop/random_original.pdf", Algorithms::GraphVizSource::EFormat::PDF);
	MaxFlow::solve (r.graph(), r.source(), r.sink(), MaxFlow::ESolver::HighestLabelPreflowPush);
	Algorithms::GraphVizSource::from (r.graph()).exportToFile ("c:/users/franc/desktop/random_solution.pdf", Algorithms::GraphVizSource::EFormat::PDF);

	std::cout << MaxFlow::Utils::Performance::end ().ticks ();

	return 0;
}