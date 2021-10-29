#ifndef INCLUDED_MAX_FLOW_APP_VERIFY
#define INCLUDED_MAX_FLOW_APP_VERIFY

#include <max-flow/graphs/flow.hpp>

namespace MaxFlow::App
{

	using FlowGraph = MaxFlow::Graphs::FlowGraph<>;
	using FlowVertex = FlowGraph::Vertex;
	using FlowEdge = FlowGraph::Edge;

	bool isFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink);
	bool isMaxFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink);
	bool isMaxFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink, Graphs::flow_t _maxFlow);
	void ensureFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink);
	void ensureMaxFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink);
	void ensureMaxFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink, Graphs::flow_t _maxFlow);
	Graphs::flow_t getMaxFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink);
	Graphs::flow_t getFlow(const FlowGraph& _graph, const FlowVertex& _source);

}

#endif