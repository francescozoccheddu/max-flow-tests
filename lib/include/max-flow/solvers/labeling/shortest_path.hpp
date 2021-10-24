#ifndef INCLUDED_MAX_FLOW_SOLVERS_LABELING_SHORTEST_PATH
#define INCLUDED_MAX_FLOW_SOLVERS_LABELING_SHORTEST_PATH

#include <max-flow/solvers/labeling/labeling.hpp>
#include <max-flow/graphs/algorithms/distance_labeler.hpp>
#include <vector>

namespace MaxFlow::Solvers::Labeling
{

	class ShortestPathSolver final : public LabelingSolver
	{

	private:

		Graphs::Algorithms::DistanceLabeler m_distanceLabeler{ graph (), source (), sink () };
		std::vector<size_t> m_distanceCounts{};
		bool m_detectMinCut{ true };

		void solveImpl () override;

	public:

		bool isMinCutDetectionEnabled () const;
		void setMinCutDetection (bool _enabled);

		using LabelingSolver::LabelingSolver;

	};

}

#endif