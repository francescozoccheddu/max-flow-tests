#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>
#include <max-flow/solve.hpp>
#include <max-flow/graphs/algorithms/graphviz.hpp>
#include <max-flow-app/random.hpp>
#include <max-flow-app/test.hpp>
#include <max-flow-app/verify.hpp>

#include <iostream>

using std::cout;
using std::endl;
using namespace MaxFlow::Graphs;
using namespace MaxFlow;


void ES()
{
	FlowGraph<> o;
	o.addVertices(4);
	o[0].addOutEdge(1, EdgeFlow{ 4,0 });
	o[0].addOutEdge(2, EdgeFlow{ 2,0 });
	o[1].addOutEdge(3, EdgeFlow{ 5,0 });
	o[2].addOutEdge(1, EdgeFlow{ 3,0 });
	o[2].addOutEdge(3, EdgeFlow{ 1,0 });

	FlowGraph<> s{ o };
	MaxFlow::solve(s, s[0], s[3], MaxFlow::ESolver::NaifPreflowPush);

	Algorithms::GraphVizSource::from(o).exportToFile("c:/users/franc/desktop/original.pdf");
	Algorithms::GraphVizSource::from(s).exportToFile("c:/users/franc/desktop/solution.pdf");
}

int main()
{

	std::vector<App::SolverParameters> solvers{
		{ESolver::FordFulkerson},
		{ESolver::ShortestPath, ESolverFlags::ShortestPathDetectMinCut},
		{ESolver::CapacityScalingFordFulkerson, ESolverFlags::CapacityScalingRemoveDeltaEdges},
		{ESolver::CapacityScalingShortestPath},
		{ESolver::NaifPreflowPush},
		{ESolver::FifoPreflowPush},
		{ESolver::HighestLabelPreflowPush},
		{ESolver::ExcessScalingPreflowPush}
	};
	std::vector<App::RandomParameters> problems{
		{.verticesCount{100}},
		{.verticesCount{500}},
		{.verticesCount{1000}},
		{.verticesCount{2000}},
	};
	App::Test{ problems, solvers }.toCsvFile("c:/users/franc/desktop/tests.csv");
	return 0;
}