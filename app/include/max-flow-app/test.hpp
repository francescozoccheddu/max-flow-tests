#ifndef INCLUDED_MAX_FLOW_APP_TEST
#define INCLUDED_MAX_FLOW_APP_TEST

#include <max-flow/solve.hpp>
#include <max-flow-app/random.hpp>
#include <vector>
#include <string>
#include <sstream>

namespace MaxFlow::App
{

	struct SolverParameters final
	{
		ESolver solver{ defaultSolver };
		ESolverFlags flags{ ESolverFlags::None };
	};


	class Test final
	{

	private:

		std::vector<double> m_data;
		const std::vector<RandomParameters> m_problems;
		const std::vector<SolverParameters> m_solvers;
		const unsigned int m_repetitions;
		const unsigned int m_seed;
		const unsigned int m_seedRepetitions;
		const bool m_skipZeroFlows;

		size_t index(size_t _problem, size_t _solver, unsigned int _repetition, unsigned int _seedRepetition) const;

		static double run(const RandomProblem& _problem, const SolverParameters& _parameters, Graphs::flow_t _maxFlowReference);

	public:

		Test(const std::vector<RandomParameters>& _problems, const std::vector<SolverParameters>& _solvers, unsigned int _repetitions = 10, unsigned int _seed = 0, unsigned int _seedRepetitions = 1, bool _skipZeroFlows = false);

		const std::vector<RandomParameters> problems() const;
		const std::vector<SolverParameters> solvers() const;
		unsigned int repetitions() const;
		unsigned int seed() const;
		unsigned int seedRepetitions() const;
		bool skipZeroFlows() const;

		void run();

		double test(size_t _problem = 0, size_t _solver = 0, unsigned int _repetition = 0, unsigned int _seedRepetition = 0) const;

		std::string toCsv() const;
		void toCsvFile(const std::string& _file) const;

	};


}

#endif