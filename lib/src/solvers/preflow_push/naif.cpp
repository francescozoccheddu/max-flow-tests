#include <max-flow/solvers/preflow_push/naif.hpp>

#include <max-flow/graphs/algorithms/residual.hpp>
#include <algorithm>

using MaxFlow::Graphs::ResidualGraph;
using MaxFlow::Graphs::ResidualVertex;
using MaxFlow::Graphs::ResidualEdge;
using MaxFlow::Graphs::flow_t;

namespace MaxFlow::Solvers::PreflowPush
{

	void NaifPreflowPushSolver::initialize ()
	{
		m_excesses.clear ();
		m_excesses.resize (graph ().verticesCount (), 0);
	}

	void NaifPreflowPushSolver::augment (ResidualEdge& _edge)
	{
		const flow_t amount{ _edge.from () == source () ? *_edge : std::min (*_edge, m_excesses[_edge.from ().index ()]) };
		if (_edge.from () != source())
		{
			m_excesses[_edge.from ().index ()] -= amount;
		}
		m_excesses[_edge.to ().index ()] = amount;
		Graphs::Algorithms::augment (_edge, amount, areZeroEdgesRemoved ());
	}

	ResidualVertex* NaifPreflowPushSolver::popActiveVertexIfExists ()
	{
		for (size_t i{ 0 }; i < graph ().verticesCount (); i++)
		{
			if (i != source ().index () && i != sink ().index () && m_excesses[i])
			{
				return &graph ()[i];
			}
		}
		return nullptr;
	}

}