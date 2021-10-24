#ifndef INCLUDED_MAX_FLOW_APP_RANDOM
#define INCLUDED_MAX_FLOW_APP_RANDOM

#include <max-flow/graphs/flow.hpp>

namespace MaxFlow::App
{

	using FlowGraph = MaxFlow::Graphs::FlowGraph<>;
	using FlowVertex = FlowGraph::Vertex;
	using FlowEdge = FlowGraph::Edge;

	struct RandomParameters final
	{

		Graphs::flow_t maxCapacity{ 100 };
		size_t verticesCount{ 10 };
		double edgeDensity{ 0.3 };
		double backwardsEdgeDensityFactor{ 0.5 };
		double capacityDeviance{ 0.5 };

		void validate () const;

	};

	class RandomProblem final
	{

	private:

		FlowGraph m_graph;

	public:

		RandomProblem (const RandomParameters& _parameters = {}, unsigned int _seed = 0);

		const FlowGraph& graph () const;
		FlowGraph& graph ();

		const FlowVertex& source () const;
		FlowVertex& source ();

		const FlowVertex& sink () const;
		FlowVertex& sink ();

		void clean ();

	};

}

#endif