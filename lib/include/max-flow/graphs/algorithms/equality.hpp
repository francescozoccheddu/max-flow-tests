#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_EQUALITY
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_EQUALITY

#include <max-flow/graphs/base/graph.hpp>
#include <max-flow/graphs/generic/graph.hpp>
#include <type_traits>

namespace MaxFlow::Graphs::Algorithms
{

#pragma region Declaration

	class EdgeComparer
	{

		friend bool equals (const Base::Edge&, const Base::Edge&, const EdgeComparer&);

	protected:

		virtual bool operator()(const Base::Edge& _a, const Base::Edge& _b) const;

	};

	class VertexComparer
	{

		friend bool equals (const Base::Vertex&, const Base::Vertex&, const VertexComparer&, const EdgeComparer&);

	protected:

		virtual bool operator()(const Base::Vertex& _a, const Base::Vertex& _b) const;

	};

	template<typename TLambda>
	class LambdaEdgeComparer final : public EdgeComparer
	{

	private:

		TLambda m_lambda;

		bool operator()(const Base::Edge& _a, const Base::Edge& _b) const override;

	public:

		LambdaEdgeComparer (TLambda _lambda);

	};

	template<typename TLambda>
	class LambdaVertexComparer final : public VertexComparer
	{

	private:

		TLambda m_lambda;

		bool operator()(const Base::Vertex& _a, const Base::Vertex& _b) const override;

	public:

		LambdaVertexComparer (TLambda _lambda);

	};

	template<typename TVertexData, typename TEdgeData, typename TLambda>
	class LambdaGenericEdgeComparer final : public EdgeComparer
	{

	private:

		TLambda m_lambda;

		bool operator()(const Base::Edge& _a, const Base::Edge& _b) const override;

	public:

		LambdaGenericEdgeComparer (TLambda _lambda);

	};

	template<typename TVertexData, typename TEdgeData, typename TLambda>
	class LambdaGenericVertexComparer final : public VertexComparer
	{

	private:

		TLambda m_lambda;

		bool operator()(const Base::Vertex& _a, const Base::Vertex& _b) const override;

	public:

		LambdaGenericVertexComparer (TLambda _lambda);

	};

	template<typename TVertexData, typename TEdgeData>
	class DefaultGenericEdgeComparer final : public EdgeComparer
	{

	private:

		bool operator()(const Base::Edge& _a, const Base::Edge& _b) const override;

	};

	template<typename TVertexData, typename TEdgeData>
	class DefaultGenericVertexComparer final : public VertexComparer
	{

	private:

		bool operator()(const Base::Vertex& _a, const Base::Vertex& _b) const override;

	};

	bool equals (const Base::Edge& _a, const Base::Edge& _b, const EdgeComparer& _edgeComparer = {});
	bool equals (const Base::Vertex& _a, const Base::Vertex& _b, const VertexComparer& _vertexComparer = {}, const EdgeComparer& _edgeComparer = {});
	bool equals (const Base::Graph& _a, const Base::Graph& _b, const VertexComparer& _vertexComparer = {}, const EdgeComparer& _edgeComparer = {});

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Generic::Edge<TVertexData, TEdgeData>& _a, const Generic::Edge<TVertexData, TEdgeData>& _b);

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Generic::Vertex<TVertexData, TEdgeData>& _a, const Generic::Vertex<TVertexData, TEdgeData>& _b, const VertexComparer& _vertexComparer);

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Generic::Vertex<TVertexData, TEdgeData>& _a, const Generic::Vertex<TVertexData, TEdgeData>& _b, const EdgeComparer& _edgeComparer);

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Generic::Vertex<TVertexData, TEdgeData>& _a, const Generic::Vertex<TVertexData, TEdgeData>& _b);

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Generic::Graph<TVertexData, TEdgeData>& _a, const Generic::Graph<TVertexData, TEdgeData>& _b, const VertexComparer& _vertexComparer);

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Generic::Graph<TVertexData, TEdgeData>& _a, const Generic::Graph<TVertexData, TEdgeData>& _b, const EdgeComparer& _edgeComparer);

	template<typename TVertexData, typename TEdgeData>
	bool equals (const Generic::Graph<TVertexData, TEdgeData>& _a, const Generic::Graph<TVertexData, TEdgeData>& _b);

#pragma endregion

#pragma region Implementation

	template<typename TL>
	inline LambdaEdgeComparer<TL>::LambdaEdgeComparer (TL _lambda) : m_lambda{ _lambda }
	{}

	template<typename TL>
	inline bool LambdaEdgeComparer<TL>::operator()(const Base::Edge& _a, const Base::Edge& _b) const
	{
		return m_lambda (_a, _b);
	}

	template<typename TL>
	inline LambdaVertexComparer<TL>::LambdaVertexComparer (TL _lambda) : m_lambda{ _lambda }
	{}

	template<typename TL>
	inline bool LambdaVertexComparer<TL>::operator()(const Base::Vertex& _a, const Base::Vertex& _b) const
	{
		return m_lambda (_a, _b);
	}

	template<typename TVD, typename TED, typename TL>
	inline LambdaGenericEdgeComparer<TVD, TED, TL>::LambdaGenericEdgeComparer (TL _lambda) : m_lambda{ _lambda }
	{}

	template<typename TVD, typename TED, typename TL>
	inline bool LambdaGenericEdgeComparer<TVD, TED, TL>::operator()(const Base::Edge& _a, const Base::Edge& _b) const
	{
		using TEdge = const Generic::Edge<TVD, TED>&;
		return m_lambda (*static_cast<TEdge>(_a), *static_cast<TEdge>(_b));
	}

	template<typename TVD, typename TED, typename TL>
	inline LambdaGenericVertexComparer<TVD, TED, TL>::LambdaGenericVertexComparer (TL _lambda) : m_lambda{ _lambda }
	{}

	template<typename TVD, typename TED, typename TL>
	inline bool LambdaGenericVertexComparer<TVD, TED, TL>::operator()(const Base::Vertex& _a, const Base::Vertex& _b) const
	{
		using TVertex = const Generic::Vertex<TVD, TED>&;
		return m_lambda (*static_cast<TVertex>(_a), *static_cast<TVertex>(_b));
	}

	template<typename TVD, typename TED>
	inline bool DefaultGenericEdgeComparer<TVD, TED>::operator()(const Base::Edge& _a, const Base::Edge& _b) const
	{
		if constexpr (std::is_void_v<TED>)
		{
			return true;
		}
		else
		{
			using TEdge = const Generic::Edge<TVD, TED>&;
			return *static_cast<TEdge>(_a) == *static_cast<TEdge>(_b);
		}
	}

	template<typename TVD, typename TED>
	inline bool DefaultGenericVertexComparer<TVD, TED>::operator()(const Base::Vertex& _a, const Base::Vertex& _b) const
	{
		if constexpr (std::is_void_v<TVD>)
		{
			return true;
		}
		else
		{
			using TVertex = const Generic::Vertex<TVD, TED>&;
			return *static_cast<TVertex>(_a) == *static_cast<TVertex>(_b);
		}
	}

	template<typename TVD, typename TED>
	inline bool equals (const Generic::Edge<TVD, TED>& _a, const Generic::Edge<TVD, TED>& _b)
	{
		return equals (static_cast<const Base::Edge&>(_a), static_cast<const Base::Edge&>(_b), DefaultGenericVertexComparer<TVD, TED>{}, DefaultGenericEdgeComparer<TVD, TED>{});
	}

	template<typename TVD, typename TED>
	inline bool equals (const Generic::Vertex<TVD, TED>& _a, const Generic::Vertex<TVD, TED>& _b, const VertexComparer& _vertexComparer)
	{
		return equals (static_cast<const Base::Vertex&>(_a), static_cast<const Base::Vertex&>(_b), _vertexComparer, DefaultGenericEdgeComparer<TVD, TED>{});
	}

	template<typename TVD, typename TED>
	inline bool equals (const Generic::Vertex<TVD, TED>& _a, const Generic::Vertex<TVD, TED>& _b, const EdgeComparer& _edgeComparer)
	{
		return equals (static_cast<const Base::Vertex&>(_a), static_cast<const Base::Vertex&>(_b), DefaultGenericVertexComparer<TVD, TED>{}, _edgeComparer);
	}

	template<typename TVD, typename TED>
	inline bool equals (const Generic::Vertex<TVD, TED>& _a, const Generic::Vertex<TVD, TED>& _b)
	{
		return equals (static_cast<const Base::Vertex&>(_a), static_cast<const Base::Vertex&>(_b), DefaultGenericVertexComparer<TVD, TED>{}, DefaultGenericEdgeComparer<TVD, TED>{});
	}

	template<typename TVD, typename TED>
	inline bool equals (const Generic::Graph<TVD, TED>& _a, const Generic::Graph<TVD, TED>& _b, const VertexComparer& _vertexComparer)
	{
		return equals (static_cast<const Base::Graph&>(_a), static_cast<const Base::Graph&>(_b), _vertexComparer, DefaultGenericEdgeComparer<TVD, TED>{});
	}

	template<typename TVD, typename TED>
	inline bool equals (const Generic::Graph<TVD, TED>& _a, const Generic::Graph<TVD, TED>& _b, const EdgeComparer& _edgeComparer)
	{
		return equals (static_cast<const Base::Graph&>(_a), static_cast<const Base::Graph&>(_b), DefaultGenericVertexComparer<TVD, TED>{}, _edgeComparer);
	}

	template<typename TVD, typename TED>
	inline bool equals (const Generic::Graph<TVD, TED>& _a, const Generic::Graph<TVD, TED>& _b)
	{
		return equals (static_cast<const Base::Graph&>(_a), static_cast<const Base::Graph&>(_b), DefaultGenericVertexComparer<TVD, TED>{}, DefaultGenericEdgeComparer<TVD, TED>{});
	}

#pragma endregion

}

#endif