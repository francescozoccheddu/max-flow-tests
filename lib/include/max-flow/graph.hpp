#ifndef INCLUDED_MAX_FLOW_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH

#include <vector>
#include <utility>

namespace MaxFlow
{

#pragma region Declaration

	template<typename TVertexData, typename TEdgeData, bool directed = true>
	class Graph final
	{

	public:

		template <typename TData, typename TNode>
		class Node
		{

		private:

			TData m_data;
			Graph* m_pGraph{};

			virtual ~Node () = default;

			Node (const Node& _node) = delete;
			Node (Node&& _node) = delete;
			Node& operator=  (const Node& _node) = delete;
			Node& operator= (Node&& _node) = delete;

			friend class Graph;

		protected:

			Node (Graph& _graph, TData&& _data);
			Node (Graph& _graph, const TData& _data);

		public:

			const Graph& graph () const;
			Graph& graph ();

			const TData& data () const;
			TData& data ();

		};

		class Edge;

		class Vertex final : public Node<TVertexData, Vertex>
		{

		private:

			Edge* m_pfirstEdge{}, * m_pLastEdge{};
			std::vector<Edge*> m_edges{};
			int m_index{ -1 };

		public:

			int index () const;

			int outEdgesCount () const;
			const Edge& outEdge (const Vertex& _to) const;
			Edge& outEdge (Vertex& _to);

			Edge& addOutEdge (const TVertexData& _data, Vertex& _to);
			Edge& addOutEdge (TVertexData&& _data, Vertex& _to);
			Edge& addOutEdgeBefore (const TVertexData& _data, Vertex& _to, Edge& _previous);
			Edge& addOutEdgeBefore (TVertexData&& _data, Vertex& _to, Edge& _previous);
			void swapEdges (Edge& _a, Edge& _b);
			void destroyAllOutEdges ();

			// outEdges()

			void destroy ();

		};

		class Edge final : public Node<TEdgeData, Edge>
		{

		private:

			Vertex* m_from{}, * m_to{};
			Edge* m_pPrevious{}, * m_pNext{};

		public:

			const Vertex& from () const;
			Vertex& from ();
			const Vertex& to () const;
			Vertex& to ();

			void setFrom (Vertex& _from);
			void setTo (Vertex& _to);

			void destroy ();

		};

	private:

		std::vector<Vertex*> m_vertices{};
		int m_edgesCount{};

	public:

		Graph ();
		Graph (const Graph& _clone);
		Graph (Graph&& _moved);

		Graph& operator=(const Graph& _clone);
		Graph& operator=(Graph&& _moved);

		bool isDirected () const;

		int edgesCount () const;

		int verticesCount () const;
		const Vertex& vertex (int _index) const;
		Vertex& vertex (int _index);

		Vertex& addVertex (const TVertexData& _data);
		Vertex& addVertex (TVertexData&& _data);
		Vertex& addVertexBefore (const TVertexData& _data, Vertex& _vertex);
		Vertex& addVertexBefore (TVertexData&& _data, Vertex& _vertex);
		void swapVertices (Vertex& _a, Vertex& _b);
		void destroyAllVertices ();

		// vertices()

	};

#pragma endregion

#pragma region Implementation

	template<typename TVertexData, typename TEdgeData, bool directed>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData, directed>::Node<TData, TNode>::Node (Graph& _graph, TData&& _data)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData, directed>::Node<TData, TNode>::Node (Graph& _graph, const TData& _data)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	template <typename TData, typename TNode>
	inline const Graph<TVertexData, TEdgeData, directed>& Graph<TVertexData, TEdgeData, directed>::Node<TData, TNode>::graph () const
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	template <typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData, directed>& Graph<TVertexData, TEdgeData, directed>::Node<TData, TNode>::graph ()
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	template <typename TData, typename TNode>
	inline const TData& Graph<TVertexData, TEdgeData, directed>::Node<TData, TNode>::data () const
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	template <typename TData, typename TNode>
	inline TData& Graph<TVertexData, TEdgeData, directed>::Node<TData, TNode>::data ()
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline int Graph<TVertexData, TEdgeData, directed>::Vertex::index () const
	{
		return 0;
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline int Graph<TVertexData, TEdgeData, directed>::Vertex::outEdgesCount () const
	{
		return 0;
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline const Graph<TVertexData, TEdgeData, directed>::Edge& Graph<TVertexData, TEdgeData, directed>::Vertex::outEdge (const Vertex& _to) const
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Edge& Graph<TVertexData, TEdgeData, directed>::Vertex::outEdge (Vertex& _to)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Edge& Graph<TVertexData, TEdgeData, directed>::Vertex::addOutEdge (const TVertexData& _data, Vertex& _to)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Edge& Graph<TVertexData, TEdgeData, directed>::Vertex::addOutEdge (TVertexData&& _data, Vertex& _to)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Edge& Graph<TVertexData, TEdgeData, directed>::Vertex::addOutEdgeBefore (const TVertexData& _data, Vertex& _to, Edge& _previous)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Edge& Graph<TVertexData, TEdgeData, directed>::Vertex::addOutEdgeBefore (TVertexData&& _data, Vertex& _to, Edge& _previous)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::Vertex::swapEdges (Edge& _a, Edge& _b)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::Vertex::destroyAllOutEdges ()
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::Vertex::destroy ()
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline const Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::Edge::from () const
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::Edge::from ()
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline const Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::Edge::to () const
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::Edge::to ()
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::Edge::setFrom (Vertex& _from)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::Edge::setTo (Vertex& _to)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::Edge::destroy ()
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Graph ()
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Graph (const Graph& _clone)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Graph (Graph&& _moved)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>& Graph<TVertexData, TEdgeData, directed>::operator=(const Graph& _clone)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>& Graph<TVertexData, TEdgeData, directed>::operator=(Graph&& _moved)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline bool Graph<TVertexData, TEdgeData, directed>::isDirected () const
	{
		return false;
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline int Graph<TVertexData, TEdgeData, directed>::edgesCount () const
	{
		return 0;
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline int Graph<TVertexData, TEdgeData, directed>::verticesCount () const
	{
		return 0;
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline const Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::vertex (int _index) const
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::vertex (int _index)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::addVertex (const TVertexData& _data)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::addVertex (TVertexData&& _data)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::addVertexBefore (const TVertexData& _data, Vertex& _vertex)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline Graph<TVertexData, TEdgeData, directed>::Vertex& Graph<TVertexData, TEdgeData, directed>::addVertexBefore (TVertexData&& _data, Vertex& _vertex)
	{
		// TODO: insert return statement here
	}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::swapVertices (Vertex& _a, Vertex& _b)
	{}

	template<typename TVertexData, typename TEdgeData, bool directed>
	inline void Graph<TVertexData, TEdgeData, directed>::destroyAllVertices ()
	{}

#pragma endregion

}

#endif