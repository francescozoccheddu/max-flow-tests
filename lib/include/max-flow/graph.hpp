#ifndef INCLUDED_MAX_FLOW_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH

#include <vector>
#include <iterator>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>

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

			const TData& operator*() const;
			TData& operator*();

			const TData* operator->() const;
			TData* operator->();

			bool operator==(const Node& _other) const;

		};

	public:

		class Edge;
		class Vertex;

		template<bool constant, bool reversed>
		class VertexIterator final
		{

		public:

			using iterator_category = std::random_access_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::conditional_t<constant, const Vertex, Vertex>;
			using pointer = value_type*;
			using reference = value_type&;

		private:

			pointer* m_p;

			VertexIterator (pointer* _pCurrent);

			friend class Graph;

		public:

			const Edge& operator*() const;
			reference operator*();

			const Edge* operator->() const;
			pointer operator->();

			const Edge& operator[](difference_type _diff) const;
			reference operator[](difference_type _diff);

			VertexIterator& operator++();
			VertexIterator operator++(int);

			VertexIterator& operator--();
			VertexIterator operator--(int);

			VertexIterator operator+(difference_type _diff) const;
			VertexIterator operator-(difference_type _diff) const;

			VertexIterator& operator+=(difference_type _diff);
			VertexIterator& operator-=(difference_type _diff);

			auto operator<=>(const VertexIterator&) const = default;

		};

		template<bool constant, bool reversed>
		class EdgeIterator final
		{

		public:

			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::conditional_t<constant, const Edge, Edge>;
			using pointer = value_type*;
			using reference = value_type&;

		private:

			pointer m_p;

			EdgeIterator (pointer _pCurrent);

			friend class Vertex;

		public:

			const Edge& operator*() const;
			reference operator*();

			const Edge* operator->() const;
			pointer operator->();

			EdgeIterator& operator++();
			EdgeIterator operator++(int);

			EdgeIterator& operator--();
			EdgeIterator operator--(int);

			bool operator== (const EdgeIterator& _other) const;

		};

		class Vertex final : public Node<TVertexData, Vertex>
		{

		private:

			friend class Graph;
			friend class Edge;

			Edge* m_pFirstOutEdge{}, * m_pLastOutEdge{};
			std::vector<Edge*> m_outVertexEdges;
			std::size_t m_outEdgesCount{};
			std::size_t m_index;
			Graph* m_pGraph;

			Vertex (const TVertexData& _data, Graph& _graph, std::size_t _index);
			Vertex (TVertexData&& _data, Graph& _graph, std::size_t _index);

			void addNewValidatedOutEdge (Edge& _edge);

			void insertVertex (Vertex& _vertex);
			void eraseVertex (Vertex& _vertex);

			void ensureValidNewOutEdge (const Vertex& _to) const;
			void ensureValidNewOutEdgeBefore (const Vertex& _to, const Edge& _next) const;

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

			void destroyAllOutEdges ();

			EdgeIterator<true, false> begin () const;
			EdgeIterator<false, false> begin ();
			EdgeIterator<true, true> rbegin () const;
			EdgeIterator<false, true> rbegin ();
			EdgeIterator<true, false> cbegin () const;
			EdgeIterator<true, true> crbegin () const;
			EdgeIterator<true, false> end () const;
			EdgeIterator<false, false> end ();
			EdgeIterator<true, true> rend () const;
			EdgeIterator<false, true> rend ();
			EdgeIterator<true, false> cend () const;
			EdgeIterator<true, true> crend () const;

			const Edge& operator[] (const Vertex& _to) const;
			Edge& operator[] (Vertex& _to);

			void destroy ();

		};

		class Edge final : public Node<TEdgeData, Edge>
		{

		private:

			friend class Vertex;
			friend class Graph;
			template<bool constant, bool reversed>
			friend class EdgeIterator;

			Vertex* m_pFrom{}, * m_pTo{};
			Edge* m_pPrevious{}, * m_pNext{};

			Edge (TEdgeData&& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);
			Edge (const TEdgeData& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);

		public:

			const Vertex& from () const;
			Vertex& from ();
			const Vertex& to () const;
			Vertex& to ();

			bool hasAntiParallel () const;
			const Edge& antiParallel () const;
			Edge& antiParallel ();

			const Vertex& operator[] (std::size_t _index) const;
			Vertex& operator[] (std::size_t _index);

			void destroy ();

		};

	private:

		std::vector<Vertex*> m_vertices{};
		std::size_t m_edgesCount{};

		void addNewVertex (Vertex& _vertex);

		const Vertex** firstVertex () const;
		const Vertex** lastVertex () const;
		Vertex** firstVertex ();
		Vertex** lastVertex ();

		static const Vertex** increasePtr (const Vertex** _p, std::size_t _diff);
		static Vertex** increasePtr (Vertex** _p, std::size_t _diff);

	public:

		Graph ();
		Graph (const Graph& _clone);
		Graph (Graph&& _moved);

		~Graph ();

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

		VertexIterator<true, false> begin () const;
		VertexIterator<false, false> begin ();
		VertexIterator<true, true> rbegin () const;
		VertexIterator<false, true> rbegin ();
		VertexIterator<true, false> cbegin () const;
		VertexIterator<true, true> crbegin () const;
		VertexIterator<true, false> end () const;
		VertexIterator<false, false> end ();
		VertexIterator<true, true> rend () const;
		VertexIterator<false, true> rend ();
		VertexIterator<true, false> cend () const;
		VertexIterator<true, true> crend () const;

		const Vertex& operator[] (std::size_t _index) const;
		Vertex& operator[] (std::size_t _index);

		std::size_t capacity () const;
		void reserve (std::size_t _capacity);
		void shrinkToFit ();

		bool operator==(const Graph& _other) const;

	};

#pragma endregion

#pragma region Implementation

#pragma region Node

#pragma region Constructors

	template<typename TVertexData, typename TEdgeData>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData>::Node<TData, TNode>::Node (TData&& _data) : m_data{ _data }
	{}

	template<typename TVertexData, typename TEdgeData>
	template<typename TData, typename TNode>
	inline Graph<TVertexData, TEdgeData>::Node<TData, TNode>::Node (const TData& _data) : m_data{ _data }
	{}

#pragma endregion

#pragma region Data getters

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
	template <typename TData, typename TNode>
	inline const TData& Graph<TVertexData, TEdgeData>::Node<TData, TNode>::operator* () const
	{
		return m_data;
	}

	template<typename TVertexData, typename TEdgeData>
	template <typename TData, typename TNode>
	inline TData& Graph<TVertexData, TEdgeData>::Node<TData, TNode>::operator*  ()
	{
		return m_data;
	}

	template<typename TVertexData, typename TEdgeData>
	template <typename TData, typename TNode>
	inline const TData* Graph<TVertexData, TEdgeData>::Node<TData, TNode>::operator-> () const
	{
		return std::addressof (m_data);
	}

	template<typename TVertexData, typename TEdgeData>
	template <typename TData, typename TNode>
	inline TData* Graph<TVertexData, TEdgeData>::Node<TData, TNode>::operator->  ()
	{
		return std::addressof (m_data);
	}

#pragma endregion

	template<typename TVertexData, typename TEdgeData>
	template <typename TData, typename TNode>
	inline bool Graph<TVertexData, TEdgeData>::Node<TData, TNode>::operator== (const Node& _other) const
	{
		return this == &_other;
	}

#pragma endregion

#pragma region VertexIterator

#pragma region Constructors

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::VertexIterator (pointer* _pCurrent) : m_p{ _pCurrent }
	{}

#pragma endregion

#pragma region Getters

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator* () const
	{
		return **m_p;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::reference Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator* ()
	{
		return **m_p;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline const Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator-> () const
	{
		return *m_p;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::pointer Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator-> ()
	{
		return *m_p;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator[] (difference_type _diff) const
	{
		return const_cast<VertexIterator&>(*this)[_diff];
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::reference Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator[] (difference_type _diff)
	{
		if constexpr (reversed)
		{
			return *m_p[-_diff];
		}
		else
		{
			return *m_p[_diff];
		}
	}

#pragma endregion

#pragma region Iteration

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>& Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator++ ()
	{
		return *this += 1;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>& Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator-- ()
	{
		return *this -= 1;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed> Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator++ (int)
	{
		return VertexIterator{ *this }++;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed> Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator-- (int)
	{
		return VertexIterator{ *this }--;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed> Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator+ (difference_type _diff) const
	{
		return VertexIterator{ *this } += _diff;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed> Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator- (difference_type _diff) const
	{
		return VertexIterator{ *this } -= _diff;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>& Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator+= (difference_type _diff)
	{
		if constexpr (reversed)
		{
			m_p -= _diff;
		}
		else
		{
			m_p += _diff;
		}
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>& Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator-= (difference_type _diff)
	{
		return *this += -_diff;
	}

#pragma endregion

#pragma endregion

#pragma region EdgeIterator

#pragma region Constructors

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::EdgeIterator (pointer _pCurrent) : m_p{ _pCurrent }
	{}

#pragma endregion

#pragma region Getters

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator* () const
	{
		return *m_p;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::reference Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator* ()
	{
		return *m_p;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline const Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator-> () const
	{
		return m_p;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::pointer Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator-> ()
	{
		return m_p;
	}

#pragma endregion

#pragma region Iteration

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>& Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator++ ()
	{
		if constexpr (reversed)
		{
			m_p = m_p->m_pPrevious;
		}
		else
		{
			m_p = m_p->m_pNext;
		}
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>& Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator-- ()
	{
		if constexpr (reversed)
		{
			m_p = m_p->m_pNext;
		}
		else
		{
			m_p = m_p->m_pPrevious;
		}
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed> Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator++ (int)
	{
		return EdgeIterator{ *this }++;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed> Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator-- (int)
	{
		return EdgeIterator{ *this }--;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline bool Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator== (const EdgeIterator& _other) const
	{
		return _other.m_p == m_p;
	}

#pragma endregion

#pragma endregion

#pragma region Vertex

#pragma region Constructors

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (const TVertexData& _data, Graph& _graph, std::size_t _index)
		: Node<TVertexData, Vertex>{ _data }, m_pGraph{ &_graph }, m_index{ _index }, m_outVertexEdges{ _graph.m_vertices.size () + 1 }
	{
		if (_index < 0 || _index > _graph.verticesCount ())
		{
			throw std::out_of_range{ "bad index" };
		}
		m_outVertexEdges.reserve (_graph.capacity ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (TVertexData&& _data, Graph& _graph, std::size_t _index)
		: Node<TVertexData, Vertex>{ _data }, m_pGraph{ &_graph }, m_index{ _index }, m_outVertexEdges{ _graph.m_vertices.size () + 1 }
	{
		if (_index < 0 || _index > _graph.verticesCount ())
		{
			throw std::out_of_range{ "bad index" };
		}
		m_outVertexEdges.reserve (_graph.capacity ());
	}

#pragma endregion

#pragma region Edge insertion

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::addNewValidatedOutEdge (Edge& _edge)
	{
		if (!_edge.m_pNext)
		{
			m_pLastOutEdge = &_edge;
		}
		else
		{
			_edge.m_pNext->m_pPrevious = &_edge;
		}
		if (!_edge.m_pPrevious)
		{
			m_pFirstOutEdge = &_edge;
		}
		else
		{
			_edge.m_pPrevious->m_pNext = &_edge;
		}
		m_outVertexEdges[_edge.to ().index ()] = &_edge;
		m_outEdgesCount++;
		graph ().m_edgesCount++;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::ensureValidNewOutEdge (const Vertex& _to) const
	{
		if (_to.graph () != graph ())
		{
			throw std::invalid_argument{ "not the same graph" };
		}
		if (hasOutEdge (_to))
		{
			throw std::logic_error{ "edge already exists" };
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::ensureValidNewOutEdgeBefore (const Vertex& _to, const Edge& _next) const
	{
		ensureValidNewOutEdge (_to);
		if (_next.from () != *this)
		{
			throw std::invalid_argument{ "not the same vertex" };
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdge (const TVertexData& _data, Vertex& _to)
	{
		ensureValidNewOutEdge (_to);
		Edge& edge{ *new Edge{_data, *this, _to, m_pLastOutEdge, nullptr} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdge (TVertexData&& _data, Vertex& _to)
	{
		ensureValidNewOutEdge (_to);
		Edge& edge{ *new Edge{std::move (_data), *this, _to, m_pLastOutEdge, nullptr} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdgeBefore (const TVertexData& _data, Vertex& _to, Edge& _next)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		Edge& edge{ *new Edge{_data, *this, _to, _next.m_pPrevious, &_next} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdgeBefore (TVertexData&& _data, Vertex& _to, Edge& _next)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		Edge& edge{ *new Edge{std::move (_data), *this, _to, _next.m_pPrevious, &_next} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

#pragma endregion

#pragma region Vertex update

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::insertVertex (Vertex& _vertex)
	{
		m_outVertexEdges.insert (m_outVertexEdges.begin () + _vertex.index (), nullptr);
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::eraseVertex (Vertex& _vertex)
	{
		if (hasOutEdge (_vertex))
		{
			outEdge (_vertex).destroy ();
		}
		m_outVertexEdges.erase (m_outVertexEdges.begin () + _vertex.index ());
	}

#pragma endregion

#pragma region Getters

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
		if (_to.graph () != graph ())
		{
			throw std::invalid_argument{ "not the same graph" };
		}
		return m_outVertexEdges[_to.index ()];
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::outEdge (const Vertex& _to) const
	{
		return const_cast<Vertex&>(*this).edge (const_cast<Vertex&>(_to));
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::outEdge (Vertex& _to)
	{
		if (!hasOutEdge (_to))
		{
			throw std::invalid_argument{ "no edge" };
		}
		return *m_outVertexEdges[_to.index ()];
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::operator[] (const Vertex& _to) const
	{
		return outEdge (_to);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::operator[] (Vertex& _to)
	{
		return outEdge (_to);
	}

#pragma endregion

#pragma region Iteration

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, false> Graph<TVertexData, TEdgeData>::Vertex::begin () const
	{
		return EdgeIterator<true, false>{m_pFirstOutEdge};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<false, false> Graph<TVertexData, TEdgeData>::Vertex::begin ()
	{
		return EdgeIterator<false, false>{m_pFirstOutEdge};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, false> Graph<TVertexData, TEdgeData>::Vertex::cbegin () const
	{
		return EdgeIterator<true, false>{m_pFirstOutEdge};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, true> Graph<TVertexData, TEdgeData>::Vertex::rbegin () const
	{
		return EdgeIterator<true, true>{m_pLastOutEdge};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<false, true> Graph<TVertexData, TEdgeData>::Vertex::rbegin ()
	{
		return EdgeIterator<false, true>{m_pLastOutEdge};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, true> Graph<TVertexData, TEdgeData>::Vertex::crbegin () const
	{
		return EdgeIterator<true, true>{m_pLastOutEdge};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, false> Graph<TVertexData, TEdgeData>::Vertex::end () const
	{
		return EdgeIterator<true, false>{nullptr};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<false, false> Graph<TVertexData, TEdgeData>::Vertex::end ()
	{
		return EdgeIterator<false, false>{nullptr};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, false> Graph<TVertexData, TEdgeData>::Vertex::cend () const
	{
		return EdgeIterator<true, false>{nullptr};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, true> Graph<TVertexData, TEdgeData>::Vertex::rend () const
	{
		return EdgeIterator<true, true>{nullptr};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<false, true> Graph<TVertexData, TEdgeData>::Vertex::rend ()
	{
		return EdgeIterator<false, true>{nullptr};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<true, true> Graph<TVertexData, TEdgeData>::Vertex::crend () const
	{
		return EdgeIterator<true, true>{nullptr};
	}

#pragma endregion

#pragma region Destruction

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::destroyAllOutEdges ()
	{
		graph ().m_edgesCount -= outEdgesCount ();
		m_outEdgesCount = 0;
		while (m_pFirstOutEdge)
		{
			Edge* pCurrent = m_pFirstOutEdge;
			m_pFirstOutEdge = pCurrent->m_pNext;
			delete pCurrent;
		}
		m_pFirstOutEdge = m_pLastOutEdge = nullptr;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::destroy ()
	{
		destroyAllOutEdges ();
		for (Vertex& vert : graph ())
		{
			vert.eraseVertex (*this);
		}
		graph ().m_vertices.erase (graph ().m_vertices.begin () + index ());
		for (std::size_t i{ index () }; i < graph ().verticesCount (); i++)
		{
			graph ()[i].m_index--;
		}
		m_index = -1;
		m_pGraph = nullptr;
		delete this;
	}

#pragma endregion

#pragma endregion

#pragma region Edge

#pragma region Constructors

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge::Edge (const TEdgeData& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: Node<TEdgeData, Edge>{ _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		if (_from.graph () != _to.graph ())
		{
			throw std::invalid_argument{ "not the same graph" };
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge::Edge (TEdgeData&& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: Node<TEdgeData, Edge>{ _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		if (_from.graph () != _to.graph ())
		{
			throw std::invalid_argument{ "not the same graph" };
		}
	}

#pragma endregion

#pragma region Getters

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
	inline bool Graph<TVertexData, TEdgeData>::Edge::hasAntiParallel () const
	{
		return to ().hasOutEdge (from ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Edge::antiParallel () const
	{
		return to ().outEdge (from ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Edge::antiParallel ()
	{
		return to ().outEdge (from ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::operator[] (std::size_t _index) const
	{
		return const_cast<Edge&>(*this)[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::operator[] (std::size_t _index)
	{
		switch (_index)
		{
			case 0:
				return from ();
			case 1:
				return to ();
			default:
				throw std::out_of_range{ "bad index" };
		}
	}

#pragma endregion

#pragma region Destruction

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
		from ().graph ().m_edgesCount--;
		from ().m_outEdgesCount--;
		from ().m_outVertexEdges[to ().index ()] = nullptr;
		m_pPrevious = m_pNext = nullptr;
		m_pFrom = m_pTo = nullptr;
		delete this;
	}

#pragma endregion

#pragma endregion

#pragma region Graph

#pragma region Constructors

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph ()
	{}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph (const Graph& _clone)
	{
		*this = _clone;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Graph (Graph&& _moved)
	{
		*this = std::move (_moved);
	}

#pragma endregion

#pragma region Destruction

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::~Graph ()
	{
		destroyAllVertices ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::destroyAllVertices ()
	{
		for (Vertex& vert : *this)
		{
			vert.destroyAllOutEdges ();
			delete& vert;
		}
		m_vertices.clear ();
	}

#pragma endregion

#pragma region Assignment

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>& Graph<TVertexData, TEdgeData>::operator=(const Graph& _clone)
	{
		destroyAllVertices ();
		reserve (_clone.capacity ());
		for (Vertex& vert : _clone)
		{
			addVertex (*vert);
		}
		for (Vertex& vert : _clone)
		{
			Vertex& thisVert{ (*this)[vert.index ()] };
			Edge* pEdge{ vert.m_pFirstOutEdge };
			while (pEdge)
			{
				thisVert.addOutEdge (**pEdge, (*this)[pEdge->to ().index ()]);
				pEdge = pEdge->m_pNext;
			}
		}
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>& Graph<TVertexData, TEdgeData>::operator=(Graph&& _moved)
	{
		destroyAllVertices ();
		m_vertices.swap (_moved.m_vertices);
		std::swap (m_edgesCount, _moved.m_edgesCount);
		for (Vertex& vert : *this)
		{
			vert.m_pGraph = this;
		}
		return *this;
	}

#pragma endregion

#pragma region Vertex insertion

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::addNewVertex (Vertex& _vertex)
	{
		for (std::size_t i{ _vertex.index () }; i < verticesCount (); i++)
		{
			(*this)[i].m_index++;
		}
		for (Vertex& vert : *this)
		{
			vert.insertVertex (_vertex);
		}
		m_vertices.insert (m_vertices.begin () + _vertex.index (), &_vertex);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (const TVertexData& _data)
	{
		Vertex& vertex{ *new Vertex{_data, *this, verticesCount ()} };
		addNewVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (TVertexData&& _data)
	{
		Vertex& vertex{ *new Vertex{std::move (_data), *this, verticesCount ()} };
		addNewVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (const TVertexData& _data, Vertex& _next)
	{
		if (_next.graph () != *this)
		{
			throw std::invalid_argument{ "not the same graph" };
		}
		Vertex& vertex{ *new Vertex{_data, *this, _next.index ()} };
		addNewVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (TVertexData&& _data, Vertex& _next)
	{
		if (_next.graph () != *this)
		{
			throw std::invalid_argument{ "not the same graph" };
		}
		Vertex& vertex{ *new Vertex{std::move (_data), *this, _next.index ()} };
		addNewVertex (vertex);
		return vertex;
	}

#pragma endregion

#pragma region Getters

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
		return const_cast<Graph&>(*this).vertex (_index);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::vertex (std::size_t _index)
	{
		if (_index < 0 || _index >= verticesCount ())
		{
			throw std::out_of_range{ "bad vertex index" };
		}
		return *m_vertices[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::operator[] (std::size_t _index) const
	{
		return vertex (_index);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::operator[] (std::size_t _index)
	{
		return vertex (_index);
	}

#pragma endregion

#pragma region Swap vertices

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::swapVertices (Vertex& _a, Vertex& _b)
	{
		std::swap (_a.m_index, _b.m_index);
		std::swap (m_vertices[_a.index ()], m_vertices[_b.index ()]);
		for (Vertex& vert : *this)
		{
			std::swap (vert.m_outVertexEdges[_a.index ()], vert.m_outVertexEdges[_b.index ()]);
		}
	}

#pragma endregion

#pragma region Iteration

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::firstVertex () const
	{
		return const_cast<const Vertex**>(const_cast<Graph*>(this)->firstVertex ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::lastVertex () const
	{
		return const_cast<const Vertex**>(const_cast<Graph*>(this)->lastVertex ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::firstVertex ()
	{
		return m_vertices.empty () ? nullptr : &m_vertices[0];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::lastVertex ()
	{
		return m_vertices.empty () ? nullptr : &m_vertices[0] + m_vertices.size () - 1;
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::increasePtr (const Vertex** _p, std::size_t _diff)
	{
		return _p ? _p + _diff : nullptr;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::increasePtr (Vertex** _p, std::size_t _diff)
	{
		return _p ? _p + _diff : nullptr;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, false> Graph<TVertexData, TEdgeData>::begin () const
	{
		return VertexIterator<true, false>{firstVertex ()};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<false, false> Graph<TVertexData, TEdgeData>::begin ()
	{
		return VertexIterator<false, false>{firstVertex ()};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, false> Graph<TVertexData, TEdgeData>::cbegin () const
	{
		return VertexIterator<true, false>{firstVertex ()};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, true> Graph<TVertexData, TEdgeData>::rbegin () const
	{
		return VertexIterator<true, true>{lastVertex ()};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<false, true> Graph<TVertexData, TEdgeData>::rbegin ()
	{
		return VertexIterator<false, true>{lastVertex ()};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, true> Graph<TVertexData, TEdgeData>::crbegin () const
	{
		return VertexIterator<true, true>{lastVertex ()};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, false> Graph<TVertexData, TEdgeData>::end () const
	{
		return VertexIterator<true, false>{increasePtr (lastVertex (), 1)};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<false, false> Graph<TVertexData, TEdgeData>::end ()
	{
		return VertexIterator<false, false>{increasePtr (lastVertex (), 1)};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, false> Graph<TVertexData, TEdgeData>::cend () const
	{
		return VertexIterator<true, false>{increasePtr (lastVertex (), 1)};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, true> Graph<TVertexData, TEdgeData>::rend () const
	{
		return VertexIterator<true, true>{increasePtr (firstVertex (), -1)};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<false, true> Graph<TVertexData, TEdgeData>::rend ()
	{
		return VertexIterator<false, true>{increasePtr (firstVertex (), -1)};
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<true, true> Graph<TVertexData, TEdgeData>::crend () const
	{
		return VertexIterator<true, true>{increasePtr (firstVertex (), -1)};
	}

#pragma endregion

#pragma region Capacity

	template<typename TVertexData, typename TEdgeData>
	inline std::size_t Graph<TVertexData, TEdgeData>::capacity () const
	{
		return m_vertices.capacity ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::reserve (std::size_t _capacity)
	{
		if (_capacity < 0)
		{
			throw std::out_of_range{ "capacity < 0" };
		}
		m_vertices.reserve (_capacity);
		for (Vertex& vert : *this)
		{
			vert.m_outVertexEdges.reserve (_capacity);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::shrinkToFit ()
	{
		m_vertices.shrink_to_fit ();
		for (Vertex& vert : *this)
		{
			vert.m_outVertexEdges.shrink_to_fit ();
		}
	}

#pragma endregion

#pragma region Comparison

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::operator== (const Graph& _other) const
	{
		return this == &_other;
	}

#pragma endregion

#pragma endregion

#pragma endregion

}

#endif