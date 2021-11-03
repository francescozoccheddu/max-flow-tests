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

	size_t Test::index(size_t _problem, size_t _solver, unsigned int _repetition, unsigned int _seedRepetition) const
	{
		return _seedRepetition * m_problems.size() * m_solvers.size() * m_repetitions + _repetition * m_problems.size() * m_solvers.size() + _solver * m_problems.size() + _problem;
	}

	Test::Test(const std::vector<RandomParameters>& _problems, const std::vector<SolverParameters>& _solvers, unsigned int _repetitions, unsigned int _seed, unsigned int _seedRepetitions, bool _skipZeroFlows)
		: m_problems{ _problems }, m_solvers{ _solvers }, m_repetitions{ _repetitions }, m_seed{ _seed }, m_seedRepetitions{ _seedRepetitions }, m_skipZeroFlows{ _skipZeroFlows }, m_data(_problems.size()* _solvers.size()* _repetitions* _seedRepetitions)
	{
		if (_problems.empty() || _solvers.empty() || !_repetitions || !_seedRepetitions)
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

	void log(const std::string& _label, const std::string& _content)
	{
		std::cout << ' ' << _label << '[' << _content << ']';
	}

	std::string progress(size_t _current, size_t _total)
	{
		return std::to_string(_current + 1) + '/' + std::to_string(_total);
	}

	void log(const std::string& _label, size_t _current, size_t _total)
	{
		if (_total > 1)
		{
			log(_label, progress(_current, _total));
		}
	}

	void Test::logProgress(size_t _current) const
	{
		std::cout << '(' << progress(_current, m_data.size()) << ')';
	}

	void Test::logProblem(size_t _current) const
	{
		log("Prob", _current, m_problems.size());
	}

	void Test::logRepetition(size_t _current) const
	{
		log("Rept", _current, m_repetitions);
	}

	void Test::logSeedRepetition(size_t _current) const
	{
		log("Seed", _current, m_seedRepetitions);
	}

	void Test::logSolver(size_t _current) const
	{
		log("Solv", _current, m_solvers.size());
	}

	void Test::run()
	{
		size_t count{};
		bool anyZeroFlows{};
		for (size_t p{ 0 }; p < m_problems.size(); p++)
		{
			unsigned int zeroFlowSkips{ 0 };
			for (unsigned int sr{ 0 }; sr < m_seedRepetitions; sr++)
			{
				logProgress(count);
				logProblem(p);
				logSeedRepetition(sr);
				bool done{ false };
				bool zeroFlowSkippedThisRound{ false };
				while (!done)
				{
					const RandomProblem problem{ m_problems[p], m_seed + sr + zeroFlowSkips };
					const Graphs::flow_t maxFlowReference{ getMaxFlow(problem.graph(), problem.source(), problem.sink()) };
					if (!maxFlowReference)
					{
						if (m_skipZeroFlows)
						{
							if (!zeroFlowSkippedThisRound)
							{
								std::cout << " [";
								zeroFlowSkippedThisRound = true;
							}
							std::cout << '0';
							zeroFlowSkips++;
							continue;
						}
						anyZeroFlows |= !maxFlowReference;
					}
					done = true;
					if (zeroFlowSkippedThisRound)
					{
						std::cout << "]";
					}
					log("ActualSeed", std::to_string(m_seed + sr + zeroFlowSkips));
					std::cout << std::endl;
					for (size_t s{ 0 }; s < m_solvers.size(); s++)
					{
						const SolverParameters& solverParameters{ m_solvers[s] };
						for (unsigned int r{ 0 }; r < m_repetitions; r++)
						{
							logProgress(count++);
							logProblem(p);
							logSeedRepetition(sr);
							logSolver(s);
							logRepetition(r);
							const double time{ run(problem, solverParameters, maxFlowReference) };
							m_data[index(p, s, r, sr)] = time;
							log("Time", std::to_string(time) + "s");
							std::cout << std::endl;
						}
					}
				}
			}
		}
		if (anyZeroFlows)
		{
			std::cout << "Warning: zero flows detected" << std::endl;
		}
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

	double Test::test(size_t _problem, size_t _solver, unsigned int _repetition, unsigned int _seedRepetition) const
	{
		return m_data[index(_problem, _solver, _repetition, _seedRepetition)];
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
		bool nonempty{ false };
		if (_flags & ESolverFlags::RemoveZeroEdgeLabels)
		{
			ss << "RZE";
			nonempty = true;
		}
		if (_flags & ESolverFlags::CapacityScalingRemoveDeltaEdges)
		{
			if (nonempty)
			{
				ss << '+';
			}
			ss << "RDE";
			nonempty = true;
		}
		if (_flags & ESolverFlags::ShortestPathDetectMinCut)
		{
			if (nonempty)
			{
				ss << '+';
			}
			ss << "DMC";
		}
		if (_flags & ESolverFlags::FordFulkersonDepthFirst)
		{
			if (nonempty)
			{
				ss << '+';
			}
			ss << "DF";
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
		ss << "seedRepetition,";
		ss << "time";
		ss << std::endl;
		for (size_t p{ 0 }; p < m_problems.size(); p++)
		{
			const RandomParameters& problem{ m_problems[p] };
			for (unsigned int sr{ 0 }; sr < m_seedRepetitions; sr++)
			{
				for (size_t s{ 0 }; s < m_solvers.size(); s++)
				{
					const SolverParameters& solver{ m_solvers[s] };
					for (unsigned int r{ 0 }; r < m_repetitions; r++)
					{
						ss << problem.maxCapacity << ',';
						ss << problem.verticesCount << ',';
						ss << problem.edgesCount << ',';
						ss << problem.backwardsEdgeDensityFactor << ',';
						ss << problem.capacityDeviance << ',';
						ss << solverName(solver.solver) << ',';
						ss << solverFlagsName(solver.flags) << ',';
						ss << r + 1 << ',';
						ss << sr + 1 << ',';
						ss << test(p, s, r, sr);
						ss << std::endl;
					}
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