#ifndef INCLUDED_MAX_FLOW_APP_TEST
#define INCLUDED_MAX_FLOW_APP_TEST

#include <max-flow/solve.hpp>
#include <max-flow-app/random.hpp>
#include <max-flow/utils/performance.hpp>
#include <vector>
#include <string>
#include <sstream>

namespace MaxFlow::App
{

	struct SolverParameters final
	{
		ESolver solver{ defaultSolver };
		ESolverFlags flags{ ESolverFlags::Default };
	};


	class Test final
	{

	private:

		std::vector<Utils::Performance> m_data;
		const std::vector<RandomParameters> m_problems;
		const std::vector<SolverParameters> m_solvers;
		const unsigned int m_repetitions;
		const unsigned int m_seed;

		void set (size_t _problem, size_t _solver, size_t _repetition, const Utils::Performance& _performance);

		size_t index (size_t _problem, size_t _solver, size_t _repetition) const;

	public:

		Test (const std::vector<RandomParameters>& _problems, const std::vector<SolverParameters>& _solvers, unsigned int _repetitions = 10, unsigned int _seed = 0);

		const std::vector<RandomParameters> problems () const;
		const std::vector<SolverParameters> solvers () const;
		unsigned int repetitions () const;
		unsigned int seed () const;

		void run ();

		const Utils::Performance& test (size_t _problem, size_t _solver, size_t _repetition) const;
		Utils::Performance testSum (size_t _problem, size_t _solver) const;
		Utils::Performance testAverage (size_t _problem, size_t _solver) const;
		Utils::Performance problemSum (size_t _problem) const;
		Utils::Performance problemAverage (size_t _problem) const;
		Utils::Performance solverSum (size_t _solver) const;
		Utils::Performance solverAverage (size_t _solver) const;

		std::string toCsv () const;
		void toCsvFile (const std::string& _file) const;

	};


}

#endif