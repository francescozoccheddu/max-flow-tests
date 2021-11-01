#include <max-flow-app/test.hpp>

#include <max-flow-app/verify.hpp>
#include <max-flow-app/performance.hpp>
#include <max-flow/solve.hpp>
#include <stdexcept>
#include <format>
#include <sstream>
#include <fstream>
#include <array>
#include <iostream>

using MaxFlow::App::Performance;

namespace MaxFlow::App
{

	size_t Test::index(size_t _problem, size_t _solver, size_t _repetition) const
	{
		return _repetition * m_problems.size() * m_solvers.size() + _solver * m_problems.size() + _problem;
	}

	Test::Test(const std::vector<RandomParameters>& _problems, const std::vector<SolverParameters>& _solvers, unsigned int _repetitions, unsigned int _seed)
		: m_problems{ _problems }, m_solvers{ _solvers }, m_repetitions{ _repetitions }, m_seed{ _seed }, m_data(_problems.size()* _solvers.size()* _repetitions)
	{
		if (_problems.empty() || _solvers.empty() || !_repetitions)
		{
			throw std::out_of_range{ "zero tests" };
		}
		run();
	}

	double Test::run(const RandomProblem& _problem, const SolverParameters& _parameters, Graphs::flow_t _maxFlowReference)
	{
		FlowGraph workingGraph{ _problem.graph() };
		workingGraph.setMatrix(true);
		Graphs::ResidualGraph residualGraph{ Graphs::createResidualGraph(workingGraph) };
		residualGraph.setMatrix(true);
		const GenericCapacityMatrix capacityMatrix{ workingGraph };
		Performance::start();
		solve(residualGraph, residualGraph[_problem.source().index()], residualGraph[_problem.sink().index()], capacityMatrix, _parameters.solver, _parameters.flags);
		const double time{ Performance::end() };
		Graphs::updateFlowsFromResidualGraph(residualGraph, workingGraph);
		ensureMaxFlow(workingGraph, workingGraph[_problem.source().index()], workingGraph[_problem.sink().index()], _maxFlowReference);
		return time;
	}

	void Test::run()
	{
		size_t count{};
		bool zeroFlows{};
		for (size_t p{ 0 }; p < m_problems.size(); p++)
		{
			const RandomProblem problem{ m_problems[p], m_seed };
			const Graphs::flow_t maxFlowReference{ getMaxFlow(problem.graph(), problem.source(), problem.sink()) };
			if (!maxFlowReference)
			{
				std::cout << "Warning: problem " << p << " has zero flow" << std::endl;
				zeroFlows = true;
			}
			for (size_t s{ 0 }; s < m_solvers.size(); s++)
			{
				const SolverParameters& solverParameters{ m_solvers[s] };
				for (size_t r{ 0 }; r < m_repetitions; r++)
				{
					if (m_problems.size() > 1)
					{
						std::cout << "Problem " << p + 1 << '/' << m_problems.size() << ' ';
					}
					if (m_solvers.size() > 1)
					{
						std::cout << "Solver " << s + 1 << '/' << m_solvers.size() << ' ';
					}
					if (m_problems.size() > 1)
					{
						std::cout << "Repetition " << r + 1 << '/' << m_repetitions << ' ';
					}
					std::cout << '(' << ++count << '/' << m_data.size() << ')' << std::endl;
					set(p, s, r, run(problem, solverParameters, maxFlowReference));
				}
			}
		}
		if (zeroFlows)
		{
			std::cout << "Warning: zero flows detected" << std::endl;
		}
	}

	void Test::set(size_t _problem, size_t _solver, size_t _repetition, double _performance)
	{
		m_data[index(_problem, _solver, _repetition)] = _performance;
	}

	const std::vector<RandomParameters> Test::problems() const
	{
		return m_problems;
	}

	const std::vector<SolverParameters> Test::solvers() const
	{
		return m_solvers;
	}

	unsigned int Test::repetitions() const
	{
		return m_repetitions;
	}

	unsigned int Test::seed() const
	{
		return m_seed;
	}

	double Test::test(size_t _problem, size_t _solver, size_t _repetition) const
	{
		return m_data[index(_problem, _solver, _repetition)];
	}

	double Test::testSum(size_t _problem, size_t _solver) const
	{
		double sum{};
		for (size_t r{ 0 }; r < m_repetitions; r++)
		{
			sum += test(_problem, _solver, r);
		}
		return sum;
	}

	double Test::testAverage(size_t _problem, size_t _solver) const
	{
		return testSum(_problem, _solver) / m_repetitions;
	}

	double Test::problemSum(size_t _problem) const
	{
		double sum{};
		for (size_t s{ 0 }; s < m_solvers.size(); s++)
		{
			sum += testSum(_problem, s);
		}
		return sum;
	}

	double Test::problemAverage(size_t _problem) const
	{
		return problemSum(_problem) / m_repetitions / m_solvers.size();
	}

	double Test::solverSum(size_t _solver) const
	{
		double sum{};
		for (size_t p{ 0 }; p < m_solvers.size(); p++)
		{
			sum += testSum(p, _solver);
		}
		return sum;
	}

	double Test::solverAverage(size_t _solver) const
	{
		return solverSum(_solver) / m_repetitions / m_problems.size();
	}

	constexpr std::string solverName(ESolver _solver)
	{
		switch (_solver)
		{
		case MaxFlow::ESolver::FordFulkerson:
			return "FordFulkerson";
		case MaxFlow::ESolver::CapacityScalingFordFulkerson:
			return "CapacityScalingFordFulkerson";
		case MaxFlow::ESolver::CapacityScalingShortestPath:
			return "CapacityScalingShortestPath";
		case MaxFlow::ESolver::ShortestPath:
			return "ShortestPath";
		case MaxFlow::ESolver::NaifPreflowPush:
			return "NaifPreflowPush";
		case MaxFlow::ESolver::FifoPreflowPush:
			return "FifoPreflowPush";
		case MaxFlow::ESolver::HighestLabelPreflowPush:
			return "HighestLabelPreflowPush";
		case MaxFlow::ESolver::ExcessScalingPreflowPush:
			return "ExcessScalingPreflowPush";
		default:
			throw std::invalid_argument{ "unknown solver" };
		}
	}

	std::string solverFlagsName(ESolverFlags _flags)
	{
		std::stringstream ss{};
		if (_flags & ESolverFlags::RemoveZeroEdgeLabels)
		{
			ss << "[RemoveZeroEdgeLabels]";
		}
		if (_flags & ESolverFlags::CapacityScalingRemoveDeltaEdges)
		{
			ss << "[CapacityScalingRemoveDeltaEdges]";
		}
		if (_flags & ESolverFlags::ShortestPathDetectMinCut)
		{
			ss << "[ShortestPathDetectMinCut]";
		}
		return ss.str();
	}

	std::string Test::toCsv() const
	{
		std::stringstream ss{};
		ss << std::fixed;
		ss << "maxCapacity,";
		ss << "verticesCount,";
		ss << "edgesCount,";
		ss << "backwardsEdgeDensityFactor,";
		ss << "capacityDeviance,";
		ss << "solver,";
		ss << "solverFlags,";
		ss << "repetition,";
		ss << "time";
		ss << std::endl;
		for (size_t p{ 0 }; p < m_problems.size(); p++)
		{
			const RandomParameters& problem{ m_problems[p] };
			for (size_t s{ 0 }; s < m_solvers.size(); s++)
			{
				const SolverParameters& solver{ m_solvers[s] };
				for (size_t r{ 0 }; r < m_repetitions; r++)
				{
					ss << problem.maxCapacity << ',';
					ss << problem.verticesCount << ',';
					ss << problem.edgesCount << ',';
					ss << problem.backwardsEdgeDensityFactor << ',';
					ss << problem.capacityDeviance << ',';
					ss << solverName(solver.solver) << ',';
					ss << solverFlagsName(solver.flags) << ',';
					ss << r + 1 << ',';
					ss << test(p, s, r);
					ss << std::endl;
				}
			}
		}
		return ss.str();
	}

	void Test::toCsvFile(const std::string& _file) const
	{
		std::ofstream file{};
		file.open(_file);
		file << toCsv();
		file.close();
	}

}