#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>
#include <max-flow/solve.hpp>
#include <max-flow/graphs/algorithms/graphviz.hpp>
#include <max-flow-app/random.hpp>
#include <max-flow-app/test.hpp>
#include <max-flow-app/verify.hpp>
#include <max-flow-app/performance.hpp>

#ifdef WIN32
#include <Windows.h>
#endif

#include <iostream>

using std::cout;
using std::endl;
using namespace MaxFlow::Graphs;
using namespace MaxFlow;

namespace Tests
{

	namespace Internal
	{

		void test(const std::vector<App::SolverParameters>& _solvers, const std::vector<App::RandomParameters>& _problems, const std::string& _name, unsigned int _repetitions = 10, unsigned int _seedRepetitions = 1)
		{
			cout << "-------- " << _name << " --------" << endl;
			App::Test{ _problems, _solvers, _repetitions, 0, _seedRepetitions, true }.toCsvFile("c:/users/franc/desktop/tests/" + _name + ".csv");
		}

	}

	void zeroEdgeRemoval() // Zero edge removal does not affect performances
	{
		Internal::test(
			{
				{ESolver::FordFulkerson},
				{ESolver::FifoPreflowPush},
				{ESolver::FordFulkerson, ESolverFlags::RemoveZeroEdgeLabels},
				{ESolver::FifoPreflowPush, ESolverFlags::RemoveZeroEdgeLabels},
			},
			{
				{.maxCapacity{10000}, .verticesCount{10000}, .edgesCount{20000}},
			},
			"zeroEdgeRemoval", 10, 5
			);
	}

	void shortestPathMinCutDetection()
	{
		Internal::test(
			{
				{ESolver::ShortestPath},
				{ESolver::ShortestPath, ESolverFlags::ShortestPathDetectMinCut},
			},
			{
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{5000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{4000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{8000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{16000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{80000}},
			},
			"shortestPathMinCutDetection", 5, 5
			);
	}

	void capacityScalingSubSolver()
	{
		Internal::test(
			{
				{ESolver::CapacityScalingFordFulkerson},
				{ESolver::CapacityScalingShortestPath, ESolverFlags::ShortestPathDetectMinCut},
			},
			{
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{5000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{4000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{8000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{16000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{80000}},
			},
			"capacityScalingSubSolver", 5, 5
			);
	}

	void deltaEdgesRemoval() // Delta edge removal worsen performances
	{
		Internal::test(
			{
				{ESolver::CapacityScalingFordFulkerson},
				{ESolver::CapacityScalingFordFulkerson, ESolverFlags::CapacityScalingRemoveDeltaEdges},
			},
			{
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{5000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{4000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{8000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{16000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{80000}},
			},
			"deltaEdgesRemoval", 5, 5
			);
	}

	void deltaEdgesRemovalAndCapacityScalingSubSolver() // Delta edge removal worsen performances
	{
		Internal::test(
			{
				{ESolver::CapacityScalingFordFulkerson},
				{ESolver::CapacityScalingFordFulkerson, ESolverFlags::CapacityScalingRemoveDeltaEdges},
				{ESolver::CapacityScalingShortestPath, ESolverFlags::ShortestPathDetectMinCut},
				{ESolver::CapacityScalingShortestPath, ESolverFlags::CapacityScalingRemoveDeltaEdges + ESolverFlags::ShortestPathDetectMinCut},
			},
			{
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{5000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{4000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{10000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{8000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{20000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{16000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{40000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{80000}},
			},
			"deltaEdgesRemovalAndCapacityScalingSubSolver", 5, 5
			);
	}

	void capacityVariance() // Capacity variance does not affect performances
	{
		Internal::test(
			{
				{ESolver::FordFulkerson},
			},
			{
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},
				{.maxCapacity{100000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{100000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{100000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{100000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{100000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},
				{.maxCapacity{10000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{10000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{10000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{10000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{10000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},
				{.maxCapacity{1000000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{1000000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{1000000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{1000000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{1000000000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},
			},
			"capacityVariance", 10, 10
			);
	}

	void allFast()
	{
		Internal::test(
			{
				{ESolver::FordFulkerson},
				{ESolver::ShortestPath, ESolverFlags::ShortestPathDetectMinCut},
				{ESolver::CapacityScalingShortestPath, ESolverFlags::ShortestPathDetectMinCut},
				{ESolver::FifoPreflowPush},
				{ESolver::HighestLabelPreflowPush},
				{ESolver::ExcessScalingPreflowPush},
			},
			{
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},
				{.maxCapacity{100000}, .verticesCount{1000}, .edgesCount{2000}},

				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{4000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{20000}},
				{.maxCapacity{200000}, .verticesCount{2000}, .edgesCount{4000}},

				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{8000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{40000}},
				{.maxCapacity{400000}, .verticesCount{4000}, .edgesCount{8000}},

				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{16000}},
				{.maxCapacity{8000}, .verticesCount{8000}, .edgesCount{80000}},
				{.maxCapacity{800000}, .verticesCount{8000}, .edgesCount{16000}},

			},
			"allFast", 5, 5
			);
	}

}

#include <fstream>
#include <random>

int main()
{
#ifdef WIN32
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif	

	Algorithms::GraphVizSource::from(App::RandomProblem{ {.maxCapacity{1000}, .verticesCount{10}, .edgesCount{20}, .capacityDeviance{1}}, 3 }.graph()).exportToFile("c:/users/franc/desktop/r.pdf");
	return 0;

	Tests::capacityVariance();

#ifdef WIN32
	Beep(400, 300);
	Beep(600, 300);
	Beep(800, 300);
#endif

	return 0;
}