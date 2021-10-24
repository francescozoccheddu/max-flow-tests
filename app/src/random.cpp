#include <max-flow-app/random.hpp>

#include <queue>
#include <vector>
#include <stdexcept>
#include <random>
#include <cmath>

using MaxFlow::Graphs::flow_t;

namespace MaxFlow::App
{

	void RandomParameters::validate () const
	{
		if (maxCapacity < 0)
		{
			throw std::logic_error{ "maxCapacity < 0" };
		}
		if (verticesCount < 2)
		{
			throw std::logic_error{ "verticesCount < 2" };
		}
		if (edgeDensity < 0 || edgeDensity > 1)
		{
			throw std::logic_error{ "edgeDensity not in [0,1]" };
		}
		if (backwardsEdgeDensityFactor < 0 || backwardsEdgeDensityFactor > 1)
		{
			throw std::logic_error{ "backwardsEdgeDensityFactor not in [0,1]" };
		}
		if (capacityDeviance < 0 || capacityDeviance > 1)
		{
			throw std::logic_error{ "capacityDeviance not in [0,1]" };
		}
	}

	RandomProblem::RandomProblem (const RandomParameters& _parameters, unsigned int _seed) : m_graph{}
	{
		_parameters.validate ();
		m_graph.addVertices (_parameters.verticesCount);
		std::mt19937 generator{ _seed };
		const double exponent{ 1.0 + (1.0 - _parameters.capacityDeviance) * 2.0 };
		std::uniform_real_distribution<double> normalDistribution{ 0.0, 1.0 };
		std::bernoulli_distribution forwardEdgeDistribution{ _parameters.edgeDensity };
		std::bernoulli_distribution backwardEdgeDistribution{ _parameters.edgeDensity * _parameters.backwardsEdgeDensityFactor };
		for (FlowVertex& vertex : m_graph)
		{
			for (size_t i{ 0 }; i < m_graph.verticesCount (); i++)
			{
				if ((i < vertex.index () && backwardEdgeDistribution (generator)) ||
					(i > vertex.index () && forwardEdgeDistribution (generator)))
				{
					const flow_t capacity{ static_cast<flow_t>(std::round ((1.0 - std::pow (normalDistribution (generator), exponent)) * _parameters.maxCapacity)) };
					vertex.addOutEdge (i, { capacity });
				}
			}
		}
	}

	void RandomProblem::clean ()
	{
		std::vector<bool> reached (graph ().verticesCount (), false);
		reached[sink ().index ()] = true;
		std::queue<FlowVertex*> queue{};
		queue.push (&sink ());
		while (!queue.empty ())
		{
			FlowVertex& vertex{ *queue.front () };
			queue.pop ();
			for (FlowEdge& edge : vertex)
			{
				if (!reached[edge.to ().index ()])
				{
					reached[edge.to ().index ()] = true;
					queue.push (&edge.to ());
				}
			}
		}
		for (size_t i{ sink ().index () - 1 }; i > 0; i--)
		{
			if (!reached[i])
			{
				graph ().destroyVertex (i);
			}
		}
	}

	const FlowGraph& RandomProblem::graph () const
	{
		return m_graph;
	}

	FlowGraph& RandomProblem::graph ()
	{
		return m_graph;
	}

	const FlowVertex& RandomProblem::source () const
	{
		return m_graph[0];
	}

	FlowVertex& RandomProblem::source ()
	{
		return m_graph[0];
	}

	const FlowVertex& RandomProblem::sink () const
	{
		return m_graph[m_graph.verticesCount () - 1];
	}

	FlowVertex& RandomProblem::sink ()
	{
		return m_graph[m_graph.verticesCount () - 1];
	}

}