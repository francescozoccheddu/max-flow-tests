#ifndef INCLUDED_MAX_FLOW_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH

#include <vector>
#include <cstddef>
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

			Node (const Node& _node) = delete;
			Node (Node&& _node) = delete;
			Node& operator=  (const Node& _node) = delete;
			Node& operator= (Node&& _node) = delete;

		protected:

			Node (TData&& _data);
			Node (const TData& _data);

			virtual ~Node () = default;

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
			friend class Edge;

			Edge* m_pFirstOutEdge{}, * m_pLastOutEdge{};
			std::vector<Edge*> m_outEdges;
			std::size_t m_outEdgesCount{};
			std::size_t m_index;
			Graph* m_pGraph;

			Vertex (const TVertexData& _data, Graph& _graph, std::size_t _index);
			Vertex (TVertexData&& _data, Graph& _graph, std::size_t _index);

			void addNewOutEdge (Edge& _edge);


		public:

			const Graph& graph () const;
			Graph& graph ();

			std::size_t index () const;

			std::size_t outEdgesCount () const;
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
			// TODO: operator[] const
			// TODO: operator[]

			void destroy ();

			bool operator==(const Vertex& _other) const;

		};

		class Edge final : public Node<TEdgeData, Edge>
		{

		private:

			friend class Vertex;
			friend class Graph;

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
			// TODO: operator[] const
			// TODO: operator[]

			void destroy ();

			bool operator==(const Edge& _other) const;

		};

	private:

		std::vector<Vertex*> m_vertices{};
		std::size_t m_edgesCount{};
		std::size_t m_capacity;

		void addNewVertex (Vertex& _vertex);

	public:

		Graph (std::size_t _capacity = 10);
		Graph (const Graph& _clone);
		Graph (Graph&& _moved);

		Graph& operator=(const Graph& _clone);
		Graph& operator=(Graph&& _moved);

		std::size_t edgesCount () const;

		std::size_t verticesCount () const;
		const Vertex& vertex (std::size_t _index) const;
		Vertex& vertex (std::size_t _index);

		Vertex& addVertex (const TVertexData& _data);
		Vertex& addVertex (TVertexData&& _data);
		Vertex& addVertexBefore (const TVertexData& _data, Vertex& _next);
		Vertex& addVertexBefore (TVertexData&& _data, Vertex& _next);
		void swapVertices (Vertex& _a, Vertex& _b);
		void destroyAllVertices ();

		// TODO: iterator const
		// TODO: iterator
		// TODO: operator[] const
		// TODO: operator[]
		// TODO: capacity() const
		// TODO: setCapacity()

		bool operator==(const Graph& _other) const;

	};

#pragma endregion

#pragma region Implementation

	template<typename TVertexData, typename TEdgeData>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData>::Node<TData, TNode>::Node (TData&& _data) : m_data{ _data }
	{}

	template<typename TVertexData, typename TEdgeData>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData>::Node<TData, TNode>::Node (const TData& _data) : m_data{ _data }
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
	inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (const TVertexData& _data, Graph& _graph, std::size_t _index)
		: Node<TVertexData, Vertex>{ _data }, m_pGraph{ &_graph }, m_index{ _index }, m_outEdges{ _graph.m_vertices.size () + 1 }
	{
		if (_index < 0 || _index > _graph.m_vertices.size ())
		{
			throw std::out_of_range{ "bad index" };
		}
		m_outEdges.reserve (_graph.m_capacity);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (TVertexData&& _data, Graph& _graph, std::size_t _index)
		: Node<TVertexData, Vertex>{ _data }, m_pGraph{ &_graph }, m_index{ _index }, m_outEdges{ _graph.m_vertices.size () + 1 }
	{
		if (_index < 0 || _index > _graph.m_vertices.size ())
		{
			throw std::out_of_range{ "bad index" };
		}
		m_outEdges.reserve (_graph.m_capacity);
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::addNewOutEdge (Edge& _edge)
	{
		if (_edge.m_pFrom != this)
		{
			delete& _edge;
			throw std::invalid_argument{ "not the same vertex" };
		}
		if (hasOutEdge (*_edge.m_pTo))
		{
			delete& _edge;
			throw std::logic_error{ "edge already exists" };
		}
		if (!_edge.m_pNext)
		{
			m_pLastOutEdge = &_edge;
		}
		else
		{
			if (_edge.m_pNext->m_pFrom != this)
			{
				delete& _edge;
				throw std::invalid_argument{ "not the same vertex" };
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
				delete& _edge;
				throw std::invalid_argument{ "not the same vertex" };
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
	inline std::size_t Graph<TVertexData, TEdgeData>::Vertex::index () const
	{
		return m_index;
	}

	template<typename TVertexData, typename TEdgeData>
	inline std::size_t Graph<TVertexData, TEdgeData>::Vertex::outEdgesCount () const
	{
		return m_outEdgesCount;
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::Vertex::hasOutEdge (const Vertex& _to) const
	{
		if (_to.m_pGraph != m_pGraph)
		{
			throw std::invalid_argument{ "not the same graph" };
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
			throw std::invalid_argument{ "not the same graph" };
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
		for (std::size_t i = m_index; i < vertices.size (); i++)
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
		: Node<TEdgeData, Edge>{ _from.graph (), _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		if (_from.m_pGraph != _to.m_pGraph)
		{
			throw std::invalid_argument{ "not the same graph" };
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge::Edge (TEdgeData&& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: Node<TEdgeData, Edge>{ _from.graph (), _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		if (_from.m_pGraph != _to.m_pGraph)
		{
			throw std::invalid_argument{ "not the same graph" };
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
		if (m_pPrevious)
		{
			m_pPrevious->m_pNext = m_pNext;
		}
		else
		{
			m_pFrom->m_pFirstOutEdge = m_pNext;
		}
		if (m_pNext)
		{
			m_pNext->m_pPrevious = m_pPrevious;
		}
		else
		{
			m_pFrom->m_pLastOutEdge = m_pPrevious;
		}
		m_pFrom->m_pGraph->m_edgesCount--;
		m_pFrom->m_outEdges[m_pTo->m_index] = nullptr;
		delete this;
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::Edge::operator== (const Edge& _other) const
	{
		return this == &_other;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph (std::size_t _capacity) : m_capacity{ _capacity }
	{
		if (_capacity < 0)
		{
			throw std::out_of_range{ "capacity < 0" };
		}
		m_vertices.reserve (_capacity);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph (const Graph& _clone)
	{
		// TODO
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph (Graph&& _moved)
	{
		// TODO
	}

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
		if (_vertex.m_index < 0 || _vertex.m_index > m_vertices.size ())
		{
			delete& _vertex;
			throw std::out_of_range{ "bad vertex index" };
		}
		for (Vertex* pVert : m_vertices)
		{
			pVert->m_outEdges.insert (pVert->m_outEdges.begin () + _vertex.m_index, nullptr);
		}
		m_vertices.insert (m_vertices.begin () + _vertex.m_index, &_vertex);
		if (m_vertices.size () <= m_capacity)
		{
			m_capacity = m_vertices.size ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline std::size_t Graph<TVertexData, TEdgeData>::edgesCount () const
	{
		return m_edgesCount;
	}

	template<typename TVertexData, typename TEdgeData>
	inline std::size_t Graph<TVertexData, TEdgeData>::verticesCount () const
	{
		return m_vertices.size ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::vertex (std::size_t _index) const
	{
		if (_index < 0 || _index >= m_vertices.size ())
		{
			throw std::out_of_range{ "bad vertex index" };
		}
		return m_vertices[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::vertex (std::size_t _index)
	{
		if (_index < 0 || _index >= m_vertices.size ())
		{
			throw std::out_of_range{ "bad vertex index" };
		}
		return m_vertices[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (const TVertexData& _data)
	{
		Vertex& vertex{ *new Vertex{_data, *this, m_vertices.size ()} };
		addNewVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (TVertexData&& _data)
	{
		Vertex& vertex{ *new Vertex{_data, *this, m_vertices.size ()} };
		addNewVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (const TVertexData& _data, Vertex& _next)
	{
		if (_next.m_pGraph != this)
		{
			throw std::invalid_argument{ "not the same graph" };
		}
		Vertex& vertex{ *new Vertex{_data, *this, _next.m_index} };
		addNewVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (TVertexData&& _data, Vertex& _next)
	{
		if (_next.m_pGraph != this)
		{
			throw std::invalid_argument{ "not the same graph" };
		}
		Vertex& vertex{ *new Vertex{_data, *this, _next.m_index} };
		addNewVertex (vertex);
		return vertex;
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