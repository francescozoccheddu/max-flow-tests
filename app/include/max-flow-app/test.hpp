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

		void set (size_t _problem, size_t _solver, size_t _repetition, const double _performance);

		size_t index (size_t _problem, size_t _solver, size_t _repetition) const;

	public:

		Test (const std::vector<RandomParameters>& _problems, const std::vector<SolverParameters>& _solvers, unsigned int _repetitions = 10, unsigned int _seed = 0);

		const std::vector<RandomParameters> problems () const;
		const std::vector<SolverParameters> solvers () const;
		unsigned int repetitions () const;
		unsigned int seed () const;

		void run ();

		double test (size_t _problem, size_t _solver, size_t _repetition) const;
		double testSum (size_t _problem, size_t _solver) const;
		double testAverage (size_t _problem, size_t _solver) const;
		double problemSum (size_t _problem) const;
		double problemAverage (size_t _problem) const;
		double solverSum (size_t _solver) const;
		double solverAverage (size_t _solver) const;

		std::string toCsv () const;
		void toCsvFile (const std::string& _file) const;

	};


}

#endif