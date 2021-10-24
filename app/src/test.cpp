#include <max-flow-app/test.hpp>

#include <max-flow-app/verify.hpp>
#include <max-flow/solve.hpp>
#include <stdexcept>
#include <format>
#include <sstream>
#include <fstream>
#include <array>

using MaxFlow::Utils::Performance;

namespace MaxFlow::App
{

	size_t Test::index (size_t _problem, size_t _solver, size_t _repetition) const
	{
		return _problem * m_problems.size () * m_solvers.size () + _solver * m_problems.size () + _repetition;
	}

	Test::Test (const std::vector<RandomParameters>& _problems, const std::vector<SolverParameters>& _solvers, unsigned int _repetitions, unsigned int _seed)
		: m_problems{ _problems }, m_solvers{ _solvers }, m_repetitions{ _repetitions }, m_seed{ _seed }, m_data (_problems.size ()* _solvers.size ()* _repetitions)
	{
		if (_problems.empty () || _solvers.empty () || !_repetitions)
		{
			throw std::out_of_range{ "zero tests" };
		}
		for (size_t p{ 0 }; p < _problems.size (); p++)
		{
			RandomProblem problem{ _problems[p], _seed };
			for (size_t s{ 0 }; s < _solvers.size (); s++)
			{
				const SolverParameters& solverParameters{ _solvers[s] };
				for (size_t r{ 0 }; r < _repetitions; r++)
				{
					Performance::start ();
					FlowGraph solution{ solve (const_cast<const FlowGraph&>(problem.graph ()), problem.source (), problem.sink (), solverParameters.solver, solverParameters.flags) };
					m_data[index (p, s, r)] = Performance::end ();
					ensureMaxFlow (solution, solution[problem.source ().index ()], solution[problem.sink ().index ()]);
				}
			}
		}
	}

	void Test::set (size_t _problem, size_t _solver, size_t _repetition, const Performance& _performance)
	{
		m_data[index (_problem, _solver, _repetition)] = _performance;
	}

	const std::vector<RandomParameters> Test::problems () const
	{
		return m_problems;
	}

	const std::vector<SolverParameters> Test::solvers () const
	{
		return m_solvers;
	}

	unsigned int Test::repetitions () const
	{
		return m_repetitions;
	}

	unsigned int Test::seed () const
	{
		return m_seed;
	}

	const Performance& Test::test (size_t _problem, size_t _solver, size_t _repetition) const
	{
		return m_data[index (_problem, _solver, _repetition)];
	}

	Performance Test::testSum (size_t _problem, size_t _solver) const
	{
		Performance sum{};
		for (size_t r{ 0 }; r < m_repetitions; r++)
		{
			sum += test (_problem, _solver, r);
		}
		return sum;
	}

	Performance Test::testAverage (size_t _problem, size_t _solver) const
	{
		return testSum (_problem, _solver) / m_repetitions;
	}

	Performance Test::problemSum (size_t _problem) const
	{
		Performance sum{};
		for (size_t s{ 0 }; s < m_solvers.size (); s++)
		{
			sum += testSum (_problem, s);
		}
		return sum;
	}

	Performance Test::problemAverage (size_t _problem) const
	{
		return problemSum (_problem) / m_repetitions / m_solvers.size ();
	}

	Performance Test::solverSum (size_t _solver) const
	{
		Performance sum{};
		for (size_t p{ 0 }; p < m_solvers.size (); p++)
		{
			sum += testSum (p, _solver);
		}
		return sum;
	}

	Performance Test::solverAverage (size_t _solver) const
	{
		return solverSum (_solver) / m_repetitions / m_problems.size ();
	}

	constexpr std::string solverName (ESolver _solver)
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

	std::string solverFlagsName (ESolverFlags _flags)
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
		return ss.str ();
	}

	std::string Test::toCsv (bool _aggregate) const
	{
		std::stringstream ss{};
		ss << std::fixed;
		ss << "maxCapacity,";
		ss << "verticesCount,";
		ss << "edgeDensity,";
		ss << "backwardsEdgeDensityFactor,";
		ss << "capacityDeviance,";
		ss << "solver,";
		ss << "solverFlags,";
		if (m_repetitions > 1 && !_aggregate)
		{
			ss << "repetition,";
		}
		ss << "ticks,";
		ss << "time";
		ss << std::endl;
		for (size_t p{ 0 }; p < m_problems.size (); p++)
		{
			const RandomParameters& problem{ m_problems[p] };
			for (size_t s{ 0 }; s < m_solvers.size (); s++)
			{
				const SolverParameters& solver{ m_solvers[s] };
				if (_aggregate)
				{
					const Performance& performance{ testAverage (p,s) };
					ss << problem.maxCapacity << ',';
					ss << problem.verticesCount << ',';
					ss << problem.edgeDensity << ',';
					ss << problem.backwardsEdgeDensityFactor << ',';
					ss << problem.capacityDeviance << ',';
					ss << solverName (solver.solver) << ',';
					ss << solverFlagsName (solver.flags) << ',';
					ss << performance.ticks () << ',';
					ss << performance.time ();
					ss << std::endl;
				}
				else
				{
					for (size_t r{ 0 }; r < m_repetitions; r++)
					{
						const Performance& performance{ test (p,s,r) };
						ss << problem.maxCapacity << ',';
						ss << problem.verticesCount << ',';
						ss << problem.edgeDensity << ',';
						ss << problem.backwardsEdgeDensityFactor << ',';
						ss << problem.capacityDeviance << ',';
						ss << solverName (solver.solver) << ',';
						ss << solverFlagsName (solver.flags) << ',';
						if (m_repetitions > 1 && !_aggregate)
						{
							ss << r + 1 << ',';
						}
						ss << performance.ticks () << ',';
						ss << performance.time ();
						ss << std::endl;
					}
				}
			}
		}
		return ss.str ();
	}

	void Test::toCsvFile (const std::string& _file, bool _aggregate) const
	{
		std::ofstream file{};
		file.open (_file);
		file << toCsv (_aggregate);
		file.close ();
	}

}