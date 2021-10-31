#include <max-flow-app/random.hpp>

#include <queue>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <utility>
#include <cmath>
#include <tuple>

using MaxFlow::Graphs::flow_t;

namespace MaxFlow::App
{

	void RandomParameters::validate() const
	{
		if (maxCapacity <= 0)
		{
			throw std::logic_error{ "maxCapacity <= 0" };
		}
		if (verticesCount < 2)
		{
			throw std::logic_error{ "verticesCount < 2" };
		}
		if (edgesCount < 0)
		{
			throw std::logic_error{ "edgesCount < 0" };
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

	RandomProblem::RandomProblem(const RandomParameters& _parameters, unsigned int _seed) : m_graph{}
	{
		_parameters.validate();
		m_graph.addVertices(_parameters.verticesCount);
		std::mt19937 generator{ _seed };
		std::vector<double> capacityFactors(_parameters.edgesCount);
		{
			const std::uniform_real_distribution<double> distribution{ 0,1 };
			std::generate(capacityFactors.begin(), capacityFactors.end(), [&distribution, &generator]() { return distribution(generator); });
		}
		std::vector<std::tuple<size_t, size_t>> forwardEdgePool;
		std::vector<std::tuple<size_t, size_t>> backwardEdgePool;
		forwardEdgePool.reserve(_parameters.verticesCount * (_parameters.verticesCount - 1) / 2);
		backwardEdgePool.reserve(_parameters.verticesCount * (_parameters.verticesCount - 1) / 2);
		for (size_t a{}; a < _parameters.verticesCount; a++) {
			for (size_t b{}; b < _parameters.verticesCount; b++) {
				if (a == b) 
				{
					continue;
				}
				std::vector<std::tuple<size_t, size_t>>& pool{ b > a ? forwardEdgePool : backwardEdgePool };
				pool.push_back({ a, b });
			}
		}
		std::bernoulli_distribution fbDistribution{ _parameters.backwardsEdgeDensityFactor };
		double maxCapacityFactor{ *std::max_element(capacityFactors.begin(), capacityFactors.end()) };
		if (!maxCapacityFactor) {
			capacityFactors[0] = maxCapacityFactor = 1;
		}
		while (m_graph.edgesCount() < _parameters.edgesCount) {
			std::vector<std::tuple<size_t, size_t>>& pool{ fbDistribution(generator) ? backwardEdgePool : forwardEdgePool };
			const std::uniform_int_distribution<size_t> poolDistribution{ 0, pool.size() - 1 };
			std::swap(pool[pool.size() - 1], pool[poolDistribution(generator)]);
			const std::tuple<size_t, size_t> edge{ pool[pool.size() - 1] };
			pool.pop_back();
			const double capacityFactor{ capacityFactors[m_graph.edgesCount()] / maxCapacityFactor };
			const double realCapacity{ capacityFactor * (_parameters.maxCapacity - 1) + 1 };
			Graphs::flow_t capacity{ std::clamp<Graphs::flow_t>(static_cast<Graphs::flow_t>(std::round(realCapacity)), 1, _parameters.maxCapacity) };
			m_graph[std::get<0>(edge)].addOutEdge(std::get<1>(edge), { capacity });
		}
	}

	void RandomProblem::clean()
	{
		std::vector<bool> reached(graph().verticesCount(), false);
		reached[sink().index()] = true;
		std::queue<FlowVertex*> queue{};
		queue.push(&sink());
		while (!queue.empty())
		{
			FlowVertex& vertex{ *queue.front() };
			queue.pop();
			for (FlowEdge& edge : vertex)
			{
				if (!reached[edge.to().index()])
				{
					reached[edge.to().index()] = true;
					queue.push(&edge.to());
				}
			}
		}
		for (size_t i{ sink().index() - 1 }; i > 0; i--)
		{
			if (!reached[i])
			{
				graph().destroyVertex(i);
			}
		}
	}

	const FlowGraph& RandomProblem::graph() const
	{
		return m_graph;
	}

	FlowGraph& RandomProblem::graph()
	{
		return m_graph;
	}

	const FlowVertex& RandomProblem::source() const
	{
		return m_graph[0];
	}

	FlowVertex& RandomProblem::source()
	{
		return m_graph[0];
	}

	const FlowVertex& RandomProblem::sink() const
	{
		return m_graph[m_graph.verticesCount() - 1];
	}

	FlowVertex& RandomProblem::sink()
	{
		return m_graph[m_graph.verticesCount() - 1];
	}

}