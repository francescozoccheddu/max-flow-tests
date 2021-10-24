#include <max-flow/solvers/labeling/labeling.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::Algorithms::Pathfinder;
using MaxFlow::Graphs::Algorithms::EdgeSelector;

namespace MaxFlow::Solvers::Labeling
{

	const Pathfinder& LabelingSolver::pathfinder () const
	{
		return m_pathfinder;
	}

	Pathfinder& LabelingSolver::pathfinder ()
	{
		return m_pathfinder;
	}

	void LabelingSolver::calculatePaths ()
	{
		m_pathfinder.calculate (edgeSelector ());
	}

	void LabelingSolver::augmentMax ()
	{
		Graphs::Algorithms::augmentMax (m_pathfinder.begin (), m_pathfinder.end (), areZeroEdgesRemoved ());
		callback ().onAugmentMax (*this);
	}

	const EdgeSelector& LabelingSolver::edgeSelector () const
	{
		return *m_pEdgeSelector;
	}

	EdgeSelector& LabelingSolver::edgeSelector ()
	{
		return *m_pEdgeSelector;
	}

	void LabelingSolver::setEdgeSelector (EdgeSelector& _edgeSelector)
	{
		m_pEdgeSelector = &_edgeSelector;
	}

	const LabelingSolver::Callback& LabelingSolver::callback () const
	{
		return *m_pCallback;
	}

	LabelingSolver::Callback& LabelingSolver::callback ()
	{
		return *m_pCallback;
	}

	void LabelingSolver::setCallback (Callback& _callback)
	{
		m_pCallback = &_callback;
	}

	void LabelingSolver::Callback::onAugmentMax (const LabelingSolver& _solver)
	{}

	LabelingSolver::Callback LabelingSolver::Callback::none{};

}