#include <max-flow-app/random.hpp>

#include <queue>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <utility>
#include <cmath>
#include <tuple>

#include <iostream>

using MaxFlow::Graphs::flow_t;

namespace MaxFlow::App
{

	void RandomParameters::validate() const
	{
		if (maxCapacity <= 0)
		{
			throw std::logic_error{ "maxCapacity <= 0" };
		}
		if (expand < 0)
		{
			throw std::logic_error{ "expand < 0" };
		}
		if (verticesCount < 2)
		{
			throw std::logic_error{ "verticesCount < 2" };
		}
		if (edgesCount < 0 || edgesCount >= verticesCount * (verticesCount - 1))
		{
			throw std::logic_error{ "edgesCount not in [0,verticesCount*(verticesCount-1))" };
		}
		if (backwardsEdgeDensityFactor < 0 || backwardsEdgeDensityFactor > 1)
		{
			throw std::logic_error{ "backwardsEdgeDensityFactor not in [0,1]" };
		}
		if (capacityDeviance < -1 || capacityDeviance > 1)
		{
			throw std::logic_error{ "capacityDeviance not in [-1,1]" };
		}
	}

	std::vector<size_t> randomCounts(size_t _verticesCount, size_t _edgesCount, double _backwardsEdgeDensityFactor, unsigned int _seed)
	{
		std::mt19937 generator{ _seed };
		std::vector<size_t> counts(_verticesCount);
		size_t countSum{};
		{
			std::vector<double> factors(_verticesCount);
			double sum{};
			{
				const std::uniform_real_distribution<double> distribution{ 0, 1 };
				for (size_t i{}; i < _verticesCount; i++)
				{
					const double backwardFactor{ static_cast<double>(i) / (_verticesCount - 1) };
					const double weight{ _backwardsEdgeDensityFactor * backwardFactor + (1 - backwardFactor) };
					sum += factors[i] = distribution(generator) * weight;
				}
				if (!sum)
				{
					std::uniform_int_distribution<size_t> altDistribution{ 0, _verticesCount - 1 };
					sum = factors[altDistribution(generator)] = 1;
				}
			}
			for (size_t i{}; i < _verticesCount; i++)
			{
				countSum += counts[i] = std::min(_verticesCount - 1, static_cast<size_t>(std::round(factors[i] / sum * _edgesCount)));
			}
		}
		if (countSum != _edgesCount)
		{
			const bool less{ countSum < _edgesCount };
			auto isTarget{ [less, max = _verticesCount - 1] (size_t _count) {return less ? _count < max : _count > 0; } };
			std::vector<size_t> targets;
			targets.reserve(_verticesCount);
			for (size_t i{}; i < _verticesCount; i++)
			{
				if (isTarget(counts[i]))
				{
					targets.push_back(i);
				}
			}
			while (countSum != _edgesCount)
			{
				std::uniform_int_distribution<size_t> distribution{ 0, targets.size() - 1 };
				size_t targetIndex{ distribution(generator) };
				size_t& target{ counts[targets[targetIndex]] };
				if (less)
				{
					target++;
					countSum++;
				}
				else
				{
					target--;
					countSum--;
				}
				if (!isTarget(target))
				{
					std::swap(targets[targetIndex], targets[targets.size() - 1]);
					targets.pop_back();
				}
			}
		}
		size_t c{};
		for (size_t x : counts) c += x;
		if (c != _edgesCount)
		{
			throw std::runtime_error{ "" };
		}
		return counts;
	}

	class Picker final
	{

	private:

		std::vector<size_t> m_data{};
		std::vector<size_t> m_undo{};
		size_t m_size{};

	public:

		Picker(size_t _count)
		{
			m_data.resize(_count);
			m_undo.reserve(_count);
			for (size_t i{ }; i < _count; i++)
			{
				m_data[i] = i;
			}
		}

		void reset(size_t _count)
		{
			for (const size_t undo : m_undo)
			{
				m_data[undo] = undo;
			}
			const size_t oldCount{ m_size + m_undo.size() };
			while (m_size < oldCount)
			{
				m_data[m_size] = m_size;
				m_size++;
			}
			m_undo.clear();
			m_size = _count;
		}

		size_t pop(std::mt19937& _engine)
		{
			if (empty())
			{
				throw std::logic_error{ "empty" };
			}
			std::uniform_int_distribution<size_t> distribution{ 0, m_size - 1 };
			std::swap(m_data[distribution(_engine)], m_data[m_size - 1]);
			m_size--;
			const size_t result{ m_data[m_size] };
			m_undo.push_back(result);
			return result;
		}

		bool empty() const
		{
			return !m_size;
		}

	};

	RandomProblem::RandomProblem(const RandomParameters& _parameters, unsigned int _seed) : m_graph{}
	{
		_parameters.validate();
		m_graph.addVertices(_parameters.verticesCount);
		std::vector<double> capacities(_parameters.edgesCount);
		{
			// Capacities
			std::mt19937 generator{ _seed };
			{
				constexpr double maxExponent{ 3 };
				const double exponentAbs{ 1 + std::abs(_parameters.capacityDeviance) * (maxExponent - 1) };
				const double exponent{ _parameters.capacityDeviance < 0 ? exponentAbs : 1 / exponentAbs };
				const std::uniform_real_distribution<double> distribution{ 0,1 };
				double maxAlpha{};
				for (size_t i{}; i < _parameters.edgesCount; i++)
				{
					capacities[i] = distribution(generator);
					if (capacities[i] > maxAlpha)
					{
						maxAlpha = capacities[i];
					}
				}
				if (!maxAlpha && _parameters.edgesCount)
				{
					maxAlpha = capacities[0] = 1;
				}
				for (size_t i{}; i < _parameters.edgesCount; i++)
				{
					capacities[i] = std::pow(capacities[i] / maxAlpha, exponent) * (_parameters.maxCapacity - 1) + 1;
				}
			}
		}
		{
			// Topology
			std::mt19937 generator{ _seed };
			const std::vector<size_t> counts{ randomCounts(_parameters.verticesCount, _parameters.edgesCount, _parameters.backwardsEdgeDensityFactor, _seed) };
			Picker forwardPicker{ _parameters.verticesCount - 1 }, backwardPicker{ _parameters.verticesCount - 1 };
			std::bernoulli_distribution backwardDistribution{ _parameters.backwardsEdgeDensityFactor };
			for (FlowVertex& vertex : m_graph)
			{
				forwardPicker.reset(_parameters.verticesCount - vertex.index() - 1);
				backwardPicker.reset(vertex.index());
				for (size_t c{}; c < counts[vertex.index()]; c++)
				{
					size_t to;
					if (!backwardPicker.empty() && (forwardPicker.empty() || backwardDistribution(generator)))
					{
						to = backwardPicker.pop(generator);
					}
					else
					{
						to = forwardPicker.pop(generator) + vertex.index() + 1;
					}
					const Graphs::flow_t capacity{ std::clamp<Graphs::flow_t>(static_cast<Graphs::flow_t>(std::round(capacities[m_graph.edgesCount()])), 1, _parameters.maxCapacity) };
					vertex.addOutEdge(to, { capacity });
				}
			}
		}
		expand(_parameters.expand, _parameters.maxCapacity);
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

	void RandomProblem::expand(size_t _count, size_t _capacity)
	{
		FlowVertex* pLast{ &sink() };
		m_graph.addVertices(_count);
		while (pLast != &sink())
		{
			FlowVertex& next{ m_graph[pLast->index() + 1] };
			pLast->addOutEdge(next, { _capacity });
			pLast = &next;
		}
	}

}