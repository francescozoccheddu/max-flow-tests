#ifndef INCLUDED_MAX_FLOW_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH

#include <vector>
#include <utility>
#include <stdexcept>

namespace MaxFlow
{

#pragma region Declaration

	template<typename TVertexData, typename TEdgeData>
	class Graph final
	{

	private:

		template <typename TData, typename TNode>
		class Node
		{

		private:

			TData m_data;

			virtual ~Node () = default;

			Node (const Node& _node) = delete;
			Node (Node&& _node) = delete;
			Node& operator=  (const Node& _node) = delete;
			Node& operator= (Node&& _node) = delete;

		protected:

			Node (TData&& _data);
			Node (const TData& _data);

		public:

			const TData& data () const;
			TData& data ();

			// TODO: * const
			// TODO: *
			// TODO: -> const
			// TODO: ->

		};

	public:

		class Edge;

		class Vertex final : public Node<TVertexData, Vertex>
		{

		private:

			friend class Graph;

			Edge* m_pFirstOutEdge{}, * m_pLastOutEdge{};
			std::vector<Edge*> m_outEdges{};
			int m_outEdgesCount{};
			int m_index{ -1 };
			Graph* m_pGraph{};

			Vertex (Graph& _graph, int _index);

			void addNewOutEdge (Edge& _edge);


		public:

			const Graph& graph () const;
			Graph& graph ();

			int index () const;

			int outEdgesCount () const;
			bool hasOutEdge (const Vertex& _to) const;
			const Edge& outEdge (const Vertex& _to) const;
			Edge& outEdge (Vertex& _to);

			Edge& addOutEdge (const TVertexData& _data, Vertex& _to);
			Edge& addOutEdge (TVertexData&& _data, Vertex& _to);
			Edge& addOutEdgeBefore (const TVertexData& _data, Vertex& _to, Edge& _next);
			Edge& addOutEdgeBefore (TVertexData&& _data, Vertex& _to, Edge& _next);
			void swapOutEdges (Edge& _a, Edge& _b);
			void destroyAllOutEdges ();

			// TODO: iterator const
			// TODO: iterator

			void destroy ();

			bool operator==(const Vertex& _other) const;

		};

		class Edge final : public Node<TEdgeData, Edge>
		{

		private:

			friend class Vertex;

			Vertex* m_pFrom{}, * m_pTo{};
			Edge* m_pPrevious{}, * m_pNext{};

			Edge (TEdgeData&& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);
			Edge (const TEdgeData& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);


		public:

			const Vertex& from () const;
			Vertex& from ();
			const Vertex& to () const;
			Vertex& to ();

			void setFrom (Vertex& _from);
			void setTo (Vertex& _to);

			// TODO: hasReverse const
			// TODO: reverse const
			// TODO: reverse

			void destroy ();

			bool operator==(const Edge& _other) const;

		};

	private:

		std::vector<Vertex*> m_vertices{};
		int m_edgesCount{};

		void addNewVertex (Vertex& _vertex);

	public:

		Graph ();
		Graph (const Graph& _clone);
		Graph (Graph&& _moved);

		Graph& operator=(const Graph& _clone);
		Graph& operator=(Graph&& _moved);

		int edgesCount () const;

		int verticesCount () const;
		const Vertex& vertex (int _index) const;
		Vertex& vertex (int _index);

		Vertex& addVertex (const TVertexData& _data);
		Vertex& addVertex (TVertexData&& _data);
		Vertex& addVertexBefore (const TVertexData& _data, Vertex& _next);
		Vertex& addVertexBefore (TVertexData&& _data, Vertex& _next);
		void swapVertices (Vertex& _a, Vertex& _b);
		void destroyAllVertices ();

		// TODO: iterator const
		// TODO: iterator

		bool operator==(const Graph& _other) const;

	};

#pragma endregion

#pragma region Implementation

	template<typename TVertexData, typename TEdgeData>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData>::Node<TData, TNode>::Node (TData&& _data) : m_pGraph{ _graph }, m_data{ _data }
	{}

	template<typename TVertexData, typename TEdgeData>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData>::Node<TData, TNode>::Node (const TData& _data) : m_pGraph{ _graph }, m_data{ _data }
	{}

	template<typename TVertexData, typename TEdgeData>
	template <typename TData, typename TNode>
	inline const TData& Graph<TVertexData, TEdgeData>::Node<TData, TNode>::data () const
	{
		return m_data;
	}

	template<typename TVertexData, typename TEdgeData>
	template <typename TData, typename TNode>
	inline TData& Graph<TVertexData, TEdgeData>::Node<TData, TNode>::data ()
	{
		return m_data;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (Graph& _graph, int _index)
		: m_pGraph{ &_graph }, m_index{ _index }
	{
		if (_index < 0 || _index > _graph.m_vertices.size ())
		{
			throw std::out_of_range ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::addNewOutEdge (Edge& _edge)
	{
		if (_edge.m_pFrom != this)
		{
			throw std::invalid_argument ();
		}
		if (hasOutEdge (*_edge.m_pTo))
		{
			throw std::logic_error ();
		}
		if (!_edge.m_pNext)
		{
			m_pLastOutEdge = &_edge;
		}
		else
		{
			if (_edge.m_pNext->m_pFrom != this)
			{
				throw std::invalid_argument ();
			}
			_edge.m_pNext->m_pPrevious = &_edge;
		}
		if (!_edge.m_pPrevious)
		{
			m_pFirstOutEdge = &_edge;
		}
		else
		{
			if (_edge.m_pPrevious->m_pFrom != this)
			{
				throw std::invalid_argument ();
			}
			_edge.m_pPrevious->m_pNext = &_edge;
		}
		m_outEdges[_edge.m_pTo->m_index] = &_edge;
		m_pGraph->m_edgesCount++;
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>& Graph<TVertexData, TEdgeData>::Vertex::graph () const
	{
		return *m_pGraph;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>& Graph<TVertexData, TEdgeData>::Vertex::graph ()
	{
		return *m_pGraph;
	}

	template<typename TVertexData, typename TEdgeData>
	inline int Graph<TVertexData, TEdgeData>::Vertex::index () const
	{
		return m_index;
	}

	template<typename TVertexData, typename TEdgeData>
	inline int Graph<TVertexData, TEdgeData>::Vertex::outEdgesCount () const
	{
		return m_outEdgesCount;
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::Vertex::hasOutEdge (const Vertex& _to) const
	{
		if (_to.m_pGraph != m_pGraph)
		{
			throw std::invalid_argument ();
		}
		return m_outEdges[_to.m_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::outEdge (const Vertex& _to) const
	{
		return const_cast<Vertex*>(this)->edge (const_cast<Vertex&>(_to));
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::outEdge (Vertex& _to)
	{
		if (_to.m_pGraph != m_pGraph)
		{
			throw std::invalid_argument ();
		}
		return *m_outEdges[_to.m_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdge (const TVertexData& _data, Vertex& _to)
	{
		Edge& edge{ *new Edge{graph (), _data, *this, _to, m_pLastOutEdge, nullptr} };
		addNewOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdge (TVertexData&& _data, Vertex& _to)
	{
		Edge& edge{ *new Edge{graph (), _data, *this, _to, m_pLastOutEdge, nullptr} };
		addNewOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdgeBefore (const TVertexData& _data, Vertex& _to, Edge& _next)
	{
		Edge& edge{ *new Edge{graph (), _data, *this, _to, _next.m_pPrevious, &_next} };
		addNewOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdgeBefore (TVertexData&& _data, Vertex& _to, Edge& _next)
	{
		Edge& edge{ *new Edge{graph (), _data, *this, _to, _next.m_pPrevious, &_next} };
		addNewOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::swapOutEdges (Edge& _a, Edge& _b)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::destroyAllOutEdges ()
	{
		while (m_pFirstOutEdge)
		{
			m_pFirstOutEdge->destroy ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::destroy ()
	{
		destroyAllOutEdges ();
		std::vector<Vertex*>& vertices{ m_pGraph->m_vertices };
		vertices.erase (vertices.begin () + m_index);
		for (int i = m_index; i < vertices.size (); i++)
		{
			vertices[i]->m_index--;
		}
		for (Vertex* pVert : vertices)
		{
			std::vector<Edge*>& outEdges{ pVert->m_outEdges };
			if (outEdges[m_index])
			{
				m_pGraph->m_edgesCount--;
			}
			outEdges.erase (outEdges.begin () + m_index);
		}
		delete this;
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::Vertex::operator== (const Vertex& _other) const
	{
		return this == &_other;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge::Edge (const TEdgeData& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: Node{ _from.graph (), _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		if (_from.graph () != _to.graph ())
		{
			throw std::invalid_argument ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge::Edge (TEdgeData&& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: Node{ _from.graph (), _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		if (_from.graph () != _to.graph ())
		{
			throw std::invalid_argument ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::from () const
	{
		return *m_pFrom;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::from ()
	{
		return *m_pFrom;
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::to () const
	{
		return *m_pTo;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::to ()
	{
		return *m_pTo;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Edge::setFrom (Vertex& _from)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Edge::setTo (Vertex& _to)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Edge::destroy ()
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::Edge::operator== (const Edge& _other) const
	{
		return this == &_other;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph ()
	{}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph (const Graph& _clone)
	{}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph (Graph&& _moved)
	{}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>& Graph<TVertexData, TEdgeData>::operator=(const Graph& _clone)
	{
		// TODO
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>& Graph<TVertexData, TEdgeData>::operator=(Graph&& _moved)
	{
		// TODO
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::addNewVertex (Vertex& _vertex)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline int Graph<TVertexData, TEdgeData>::edgesCount () const
	{
		return m_edgesCount;
	}

	template<typename TVertexData, typename TEdgeData>
	inline int Graph<TVertexData, TEdgeData>::verticesCount () const
	{
		return m_vertices.size ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::vertex (int _index) const
	{
		if (_index < 0 || _index >= _graph.m_vertices.size ())
		{
			throw std::out_of_range ();
		}
		return m_vertices[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::vertex (int _index)
	{
		if (_index < 0 || _index >= _graph.m_vertices.size ())
		{
			throw std::out_of_range ();
		}
		return m_vertices[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (const TVertexData& _data)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (TVertexData&& _data)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (const TVertexData& _data, Vertex& _vertex)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (TVertexData&& _data, Vertex& _vertex)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::swapVertices (Vertex& _a, Vertex& _b)
	{
		std::swap (_a.m_index, _b.m_index);
		std::swap (m_vertices[_a.m_index], m_vertices[_b.m_index]);
		for (Vertex* pVert : m_vertices)
		{
			std::swap (pVert->m_outEdges[_a.m_index], pVert->m_outEdges[_b.m_index]);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::destroyAllVertices ()
	{
		while (m_vertices.size () > 0)
		{
			m_vertices[0]->destroy ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::operator== (const Graph& _other) const
	{
		return this == &_other;
	}

#pragma endregion

}

#endif