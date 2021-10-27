#include <max-flow-app/verify.hpp>

#include <vector>
#include <stdexcept>
#include <max-flow/solve.hpp>

using MaxFlow::Graphs::flow_t;

namespace MaxFlow::App
{

	bool isFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		std::vector<flow_t> outFlows (_graph.verticesCount (), 0);
		std::vector<flow_t> inFlows (_graph.verticesCount (), 0);
		for (const FlowVertex& vertex : _graph)
		{
			for (const FlowEdge& edge : vertex)
			{
				outFlows[edge.from ().index ()] += edge->flow ();
				inFlows[edge.to ().index ()] += edge->flow ();
			}
		}
		for (size_t i{ 0 }; i < _graph.verticesCount (); i++)
		{
			if (outFlows[i] != inFlows[i] && i != _source.index () && i != _sink.index ())
			{
				return false;
			}
		}
		if ((outFlows[_source.index ()] < inFlows[_source.index ()]) ||
			(outFlows[_sink.index ()] > inFlows[_sink.index ()]) ||
			(outFlows[_source.index ()] - inFlows[_source.index ()] != inFlows[_sink.index ()] - outFlows[_sink.index ()]))
		{
			return false;
		}
		return true;
	}

	bool isMaxFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		if (!isFlow (_graph, _source, _sink))
		{
			return false;
		}
		const FlowGraph maxFlowGraph{ solve (_graph, _source, _sink, ESolver::FordFulkerson) };
		flow_t maxFlow{}, flow{};
		for (const FlowEdge& edge : maxFlowGraph[_source.index ()])
		{
			maxFlow += edge->flow ();
		}
		for (const FlowEdge& edge : _source)
		{
			flow += edge->flow ();
		}
		return flow == maxFlow;
	}

	void ensureFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		if (!isFlow (_graph, _source, _sink))
		{
			throw std::logic_error{ "not a flow" };
		}
	}

	void ensureMaxFlow (const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		if (!isMaxFlow (_graph, _source, _sink))
		{
			throw std::logic_error{ "not a max flow" };
		}
	}

}