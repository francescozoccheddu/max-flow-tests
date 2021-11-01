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
			"zeroEdgeRemoval", 30
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

}

int main()
{
#ifdef WIN32
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif	

	Tests::shortestPathMinCutDetection();
	Tests::capacityScalingSubSolver();

#ifdef WIN32
	Beep(400, 300);
	Beep(600, 300);
	Beep(800, 300);
#endif

	return 0;
}