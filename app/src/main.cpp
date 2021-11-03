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
#include <algorithm>

using std::cout;
using std::endl;
using namespace MaxFlow::Graphs;
using namespace MaxFlow;

constexpr bool fast = true;

namespace Tests
{

	namespace Internal
	{

		void test(const std::vector<App::SolverParameters>& _solvers, const std::vector<App::RandomParameters>& _problems, const std::string& _name, unsigned int _repetitions = 5, unsigned int _seedRepetitions = 5)
		{
			cout << "-------- " << _name << " --------" << endl;
			std::vector<App::RandomParameters> reversedProblems{ _problems };
			std::reverse(reversedProblems.begin(), reversedProblems.end());
			App::Test{ reversedProblems, _solvers, fast ? 1 : _repetitions, 0, fast ? 1 : _seedRepetitions, true }.toCsvFile("c:/users/franc/desktop/tests/" + _name + ".csv");
		}

	}

	void zeroEdgeRemoval()
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
			"zeroEdgeRemoval"
			);
	}

	void capacityVariance()
	{
		Internal::test(
			{
				{ESolver::FordFulkerson, ESolverFlags::FordFulkersonDepthFirst},
			},
			{
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{1000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},

				{.maxCapacity{2000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{2000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{2000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{2000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{2000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},

				{.maxCapacity{4000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{4000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{4000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{4000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{4000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},

				{.maxCapacity{8000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-1}},
				{.maxCapacity{8000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{-0.5}},
				{.maxCapacity{8000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0}},
				{.maxCapacity{8000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{0.5}},
				{.maxCapacity{8000}, .verticesCount{5000}, .edgesCount{10000}, .capacityDeviance{1}},
			},
			"capacityVariance"
			);
	}

	void fordFulkersonDepthFirst()
	{
		Internal::test(
			{
				{ESolver::FordFulkerson},
				{ESolver::FordFulkerson, ESolverFlags::FordFulkersonDepthFirst},
			},
			{
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{10000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},

				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{4000}},
				{.maxCapacity{20000}, .verticesCount{2000}, .edgesCount{40000}},
				{.maxCapacity{2000}, .verticesCount{2000}, .edgesCount{20000}},

				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{8000}},
				{.maxCapacity{40000}, .verticesCount{4000}, .edgesCount{8000}},
				{.maxCapacity{4000}, .verticesCount{4000}, .edgesCount{40000}},
			},
			"fordFulkersonDepthFirst"
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
			"shortestPathMinCutDetection"
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
			"capacityScalingSubSolver"
			);
	}

	void deltaEdgesRemoval()
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
			"deltaEdgesRemoval"
			);
	}

	void labeling()
	{
		Internal::test(
			{
				{ESolver::FordFulkerson},
				{ESolver::ShortestPath, ESolverFlags::ShortestPathDetectMinCut},
				{ESolver::CapacityScalingShortestPath, ESolverFlags::ShortestPathDetectMinCut},
			},
			{

				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},
				{.maxCapacity{1000000}, .verticesCount{1000}, .edgesCount{2000}},

				{.maxCapacity{5000}, .verticesCount{5000}, .edgesCount{10000}},
				{.maxCapacity{5000}, .verticesCount{5000}, .edgesCount{50000}},
				{.maxCapacity{5000000}, .verticesCount{5000}, .edgesCount{10000}},

				{.maxCapacity{10000}, .verticesCount{10000}, .edgesCount{20000}},
				{.maxCapacity{10000}, .verticesCount{10000}, .edgesCount{100000}},
				{.maxCapacity{10000000}, .verticesCount{10000}, .edgesCount{20000}},

			},
			"labeling"
			);
	}

	void preflowPush()
	{
		Internal::test(
			{
				{ESolver::FifoPreflowPush},
				{ESolver::HighestLabelPreflowPush},
				{ESolver::ExcessScalingPreflowPush},
			},
			{

				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{2000}},
				{.maxCapacity{1000}, .verticesCount{1000}, .edgesCount{10000}},
				{.maxCapacity{1000000}, .verticesCount{1000}, .edgesCount{2000}},

				{.maxCapacity{5000}, .verticesCount{5000}, .edgesCount{10000}},
				{.maxCapacity{5000}, .verticesCount{5000}, .edgesCount{50000}},
				{.maxCapacity{5000000}, .verticesCount{5000}, .edgesCount{10000}},

				{.maxCapacity{10000}, .verticesCount{10000}, .edgesCount{20000}},
				{.maxCapacity{10000}, .verticesCount{10000}, .edgesCount{100000}},
				{.maxCapacity{10000000}, .verticesCount{10000}, .edgesCount{20000}},

			},
			"preflowPush"
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

	if constexpr (fast)
	{
		cout << "Fast mode" << endl;
	}

	Tests::fordFulkersonDepthFirst();
	Tests::shortestPathMinCutDetection();
	Tests::capacityScalingSubSolver();

#ifdef WIN32
	Beep(400, 300);
	Beep(600, 300);
	Beep(800, 300);
#endif

	return 0;
}