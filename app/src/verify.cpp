#include <max-flow-app/verify.hpp>

#include <vector>
#include <stdexcept>
#include <max-flow/solve.hpp>

using MaxFlow::Graphs::flow_t;

namespace MaxFlow::App
{

	bool isFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		FlowGraph::ensureSameGraph(_graph, _source.graph(), _sink.graph());
		std::vector<flow_t> outFlows(_graph.verticesCount(), 0);
		std::vector<flow_t> inFlows(_graph.verticesCount(), 0);
		for (const FlowVertex& vertex : _graph)
		{
			for (const FlowEdge& edge : vertex)
			{
				outFlows[edge.from().index()] += edge->flow();
				inFlows[edge.to().index()] += edge->flow();
			}
		}
		for (size_t i{ 0 }; i < _graph.verticesCount(); i++)
		{
			if (outFlows[i] != inFlows[i] && i != _source.index() && i != _sink.index())
			{
				return false;
			}
		}
		if ((outFlows[_source.index()] < inFlows[_source.index()]) ||
			(outFlows[_sink.index()] > inFlows[_sink.index()]) ||
			(outFlows[_source.index()] - inFlows[_source.index()] != inFlows[_sink.index()] - outFlows[_sink.index()]))
		{
			return false;
		}
		return true;
	}

	bool isMaxFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink, flow_t _maxFlow)
	{
		if (!isFlow(_graph, _source, _sink))
		{
			return false;
		}
		return getFlow(_graph, _source) == _maxFlow;
	}

	bool isMaxFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink) {
		return isMaxFlow(_graph, _source, _sink, getMaxFlow(_graph, _source, _sink));
	}

	void ensureFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		if (!isFlow(_graph, _source, _sink))
		{
			throw std::logic_error{ "not a flow" };
		}
	}

	void ensureMaxFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink, flow_t _maxFlow)
	{
		if (!isMaxFlow(_graph, _source, _sink, _maxFlow))
		{
			throw std::logic_error{ "not a max flow" };
		}
	}

	void ensureMaxFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		ensureMaxFlow(_graph, _source, _sink, getMaxFlow(_graph, _source, _sink));
	}

	Graphs::flow_t getMaxFlow(const FlowGraph& _graph, const FlowVertex& _source, const FlowVertex& _sink)
	{
		const FlowGraph maxFlowGraph{ solve(_graph, _source, _sink, ESolver::FordFulkerson) };
		return getFlow(maxFlowGraph, maxFlowGraph[_source.index()]);
	}

	Graphs::flow_t getFlow(const FlowGraph& _graph, const FlowVertex& _source)
	{
		FlowGraph::ensureSameGraph(_graph, _source.graph());
		flow_t outFlow{}, inFlow{};
		for (const FlowEdge& edge : _source)
		{
			outFlow += edge->flow();
		}
		for (const FlowVertex& vertex : _graph) {
			for (const FlowEdge& edge : vertex)
			{
				if (edge.to() == _source)
				{
					inFlow += edge->flow();
				}
			}
		}
		if (outFlow < inFlow)
		{
			throw std::logic_error{ "outFlow < inFlow" };
		}
		return outFlow - inFlow;
	}

}