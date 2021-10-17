#ifndef INCLUDED_MAX_FLOW_GRAPH
#define INCLUDED_MAX_FLOW_GRAPH

#include <vector>
#include <iterator>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <memory>
#include <max-flow/utils/user_data.hpp>
#include <max-flow/utils/reference_equatable.hpp>


namespace MaxFlow::Utils
{

#pragma region Declaration

	using std::size_t;

	template<typename TVertexData = void, typename TEdgeData = TVertexData>
	class Graph final : public ReferenceComparable
	{

		// Nested classes

	private:

		template<typename TData>
		class ManagedUserData : public UserData<TData>, public ReferenceComparable
		{

			// Deleted value operations

			ManagedUserData (const ManagedUserData&) = delete;
			ManagedUserData (ManagedUserData&&) = delete;
			ManagedUserData& operator=  (const ManagedUserData&) = delete;
			ManagedUserData& operator= (ManagedUserData&&) = delete;

		protected:

			// Construction

			using UserData<TData>::UserData;
			using ReferenceComparable::operator==;

		};

	public:

		class Edge;
		class Vertex;

		template<bool constant, bool reversed>
		class VertexIterator final
		{

		public:

			// Tags

			using iterator_category = std::random_access_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::conditional_t<constant, const Vertex, Vertex>;
			using pointer = value_type*;
			using reference = value_type&;

		private:

			// Friends

			friend class Graph;

			// Attributes

			pointer* m_p;

			// Construction

			VertexIterator (pointer* _pCurrent);


		public:

			// Getters

			const Edge& operator*() const;
			reference operator*();

			const Edge* operator->() const;
			pointer operator->();

			const Edge& operator[](difference_type _diff) const;
			reference operator[](difference_type _diff);

			// Iteration

			VertexIterator& operator++();
			VertexIterator operator++(int);

			VertexIterator& operator--();
			VertexIterator operator--(int);

			VertexIterator operator+(difference_type _diff) const;
			VertexIterator operator-(difference_type _diff) const;

			VertexIterator& operator+=(difference_type _diff);
			VertexIterator& operator-=(difference_type _diff);

			// Comparison

			auto operator<=>(const VertexIterator&) const = default;

		};

		template<bool constant, bool reversed>
		class EdgeIterator final
		{

		public:

			// Tags

			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = std::conditional_t<constant, const Edge, Edge>;
			using pointer = value_type*;
			using reference = value_type&;

		private:

			// Friends

			friend class Vertex;

			// Attributes

			pointer m_p;

			// Construction

			EdgeIterator (pointer _pCurrent);

		public:

			// Getters

			const Edge& operator*() const;
			reference operator*();

			const Edge* operator->() const;
			pointer operator->();

			// Iteration

			EdgeIterator& operator++();
			EdgeIterator operator++(int);

			EdgeIterator& operator--();
			EdgeIterator operator--(int);

			// Comparison

			bool operator== (const EdgeIterator& _other) const;

		};

		class Vertex final : public ManagedUserData<TVertexData>
		{

		private:

			// Friends

			friend class Graph;
			friend class Edge;

			// Attributes

			Edge* m_pFirstOutEdge{}, * m_pLastOutEdge{};
			std::vector<Edge*>* m_pOutVertexEdges{};
			size_t m_outEdgesCount{};
			size_t m_index;
			Graph* m_pGraph;

			// Utils

			static void ensureSameVertex (const Vertex& _a, const Vertex& _b);
			void ensureValidOrLastIndex (size_t _index);

			// Construction

			Vertex (Graph& _graph, size_t _index);
			MAX_FLOW_UD_VALID_T_DECL (VertexData) Vertex (const TValidVertexData& _data, Graph& _graph, size_t _index);
			MAX_FLOW_UD_VALID_T_DECL (VertexData) Vertex (TValidVertexData&& _data, Graph& _graph, size_t _index);

			// Edge insertion

			void addNewValidatedOutEdge (Edge& _edge);

			void ensureValidNewOutEdge (const Vertex& _to) const;
			void ensureValidNewOutEdgeBefore (const Vertex& _to, const Edge& _next) const;

			// Graph interface

			void vertexAdded (Vertex& _vertex);
			void vertexDestroyed (Vertex& _vertex);
			void vertexSwapped (Vertex& _a, Vertex& _b);

			void reserve (size_t _capacity);
			void shrinkToFit ();

			bool hasMatrix () const;
			void setMatrix (bool _enabled);

			void setIndex (size_t _index);

			void setGraph (Graph& _graph);

		public:

			//Getters

			const Graph& graph () const;
			Graph& graph ();

			size_t index () const;

			size_t outEdgesCount () const;

			bool hasOutEdge (const Vertex& _to) const;

			const Edge& outEdge (const Vertex& _to) const;
			Edge& outEdge (Vertex& _to);

			const Edge* outEdgeIfExists (const Vertex& _to) const;
			Edge* outEdgeIfExists (Vertex& _to);

			const Edge& operator[] (const Vertex& _to) const;
			Edge& operator[] (Vertex& _to);

			// Edge insertion

			Edge& addOutEdge (Vertex& _to);
			MAX_FLOW_UD_VALID_T_DECL (EdgeData) Edge& addOutEdge (const TValidEdgeData& _data, Vertex& _to);
			MAX_FLOW_UD_VALID_T_DECL (EdgeData) Edge& addOutEdge (TValidEdgeData&& _data, Vertex& _to);
			Edge& addOutEdgeBefore (Vertex& _to, Edge& _next);
			MAX_FLOW_UD_VALID_T_DECL (EdgeData) Edge& addOutEdgeBefore (const TValidEdgeData& _data, Vertex& _to, Edge& _next);
			MAX_FLOW_UD_VALID_T_DECL (EdgeData) Edge& addOutEdgeBefore (TValidEdgeData&& _data, Vertex& _to, Edge& _next);

			// Destruction

			void destroyEdge (Edge& _edge);
			void destroyAllOutEdges ();
			void destroy ();

			// Iteration

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

		};

		class Edge final : public ManagedUserData<TEdgeData>
		{

		private:

			// Friends

			friend class Vertex;
			friend class Graph;

			template<bool constant, bool reversed>
			friend class EdgeIterator;

			// Attributes

			Vertex* const m_pFrom{}, * const m_pTo{};
			Edge* m_pPrevious{}, * m_pNext{};

			// Construction

			Edge (Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);
			MAX_FLOW_UD_VALID_T_DECL (EdgeData) Edge (TValidEdgeData&& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);
			MAX_FLOW_UD_VALID_T_DECL (EdgeData) Edge (const TValidEdgeData& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext);

			// Vertex interface

			void detachFromList ();
			void attachToList ();

			const Edge* next () const;
			Edge* next ();
			const Edge* previous () const;
			Edge* previous ();

		public:

			// Getters

			const Vertex& from () const;
			Vertex& from ();
			const Vertex& to () const;
			Vertex& to ();

			bool hasAntiParallel () const;
			const Edge& antiParallel () const;
			Edge& antiParallel ();
			const Edge* antiParallelIfExists () const;
			Edge* antiParallelIfExists ();

			const Vertex& operator[] (size_t _index) const;
			Vertex& operator[] (size_t _index);

			// Destruction

			void destroy ();

		};

	private:

		// Attributes

		std::vector<Vertex*> m_vertices{};
		size_t m_edgesCount{};
		bool m_hasMatrix{};

		// Utils

		static void ensureSameGraph (const Graph& _a, const Graph& _b);
		void updateIndices (size_t _first);
		void ensureValidIndex (size_t _index) const;

		// Vertex insertion

		void addNewValidatedVertex (Vertex& _vertex);

		// Iteration

		static const Vertex** increasePtr (const Vertex** _p, size_t _diff);
		static Vertex** increasePtr (Vertex** _p, size_t _diff);

		const Vertex** firstVertex () const;
		Vertex** firstVertex ();

		const Vertex** lastVertex () const;
		Vertex** lastVertex ();

		// Vertex interface

		void edgeAdded ();
		void edgeRemoved ();

	public:

		// Construction

		Graph ();
		Graph (const Graph& _clone);
		Graph (Graph&& _moved);

		// Assignments

		Graph& operator=(const Graph& _clone);
		Graph& operator=(Graph&& _moved);

		// Getters

		size_t edgesCount () const;

		size_t verticesCount () const;

		const Vertex& vertex (size_t _index) const;
		Vertex& vertex (size_t _index);

		const Vertex& operator[] (size_t _index) const;
		Vertex& operator[] (size_t _index);

		// Vertex insertion

		Vertex& addVertex ();
		MAX_FLOW_UD_VALID_T_DECL (VertexData) Vertex& addVertex (const TValidVertexData& _data);
		MAX_FLOW_UD_VALID_T_DECL (VertexData) Vertex& addVertex (TValidVertexData&& _data);
		Vertex& addVertexBefore (Vertex& _next);
		MAX_FLOW_UD_VALID_T_DECL (VertexData) Vertex& addVertexBefore (const TValidVertexData& _data, Vertex& _next);
		MAX_FLOW_UD_VALID_T_DECL (VertexData) Vertex& addVertexBefore (TValidVertexData&& _data, Vertex& _next);

		void swapVertices (Vertex& _a, Vertex& _b);

		// Storage

		size_t capacity () const;
		void reserve (size_t _capacity);
		void shrinkToFit ();
		void setMatrix (bool _enabled);
		bool hasMatrix () const;

		// Destruction

		void destroyVertex (Vertex& _vertex);
		void destroyAllVertices ();

		~Graph ();

		// Iteration

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

	};

#pragma endregion

#pragma region Implementation

#pragma region VertexIterator

#pragma region Construction

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
		VertexIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed> Graph<TVertexData, TEdgeData>::VertexIterator<constant, reversed>::operator-- (int)
	{
		VertexIterator last{ *this };
		--* this;
		return last;
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

#pragma region Construction

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
			m_p = m_p->previous ();
		}
		else
		{
			m_p = m_p->next ();
		}
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>& Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator-- ()
	{
		if constexpr (reversed)
		{
			m_p = m_p->next ();
		}
		else
		{
			m_p = m_p->previous ();
		}
		return *this;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed> Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator++ (int)
	{
		EdgeIterator last{ *this };
		++* this;
		return last;
	}

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed> Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator-- (int)
	{
		EdgeIterator last{ *this };
		--* this;
		return last;
	}


#pragma endregion

#pragma region Comparison

	template<typename TVertexData, typename TEdgeData>
	template<bool constant, bool reversed>
	inline bool Graph<TVertexData, TEdgeData>::EdgeIterator<constant, reversed>::operator== (const EdgeIterator& _other) const
	{
		return _other.m_p == m_p;
	}

#pragma endregion

#pragma endregion

#pragma region Vertex

#pragma region Utils

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::ensureSameVertex (const Vertex& _a, const Vertex& _b)
	{
		if (&_a != &_b)
		{
			throw std::logic_error{ "not the same vertex" };
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::ensureValidOrLastIndex (size_t _index)
	{
		if (_index != graph ().verticesCount ())
		{
			graph ().ensureValidIndex (_index);
		}
	}

#pragma endregion

#pragma region Construction

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (Graph& _graph, size_t _index)
		: ManagedUserData<TVertexData>{ }, m_pGraph{ &_graph }, m_index{ _index }
	{
		ensureValidOrLastIndex (_index);
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (VertexData) inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (const TValidVertexData& _data, Graph& _graph, size_t _index)
		: ManagedUserData<TVertexData>{ _data }, m_pGraph{ &_graph }, m_index{ _index }
	{
		ensureValidOrLastIndex (_index);
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (VertexData) inline Graph<TVertexData, TEdgeData>::Vertex::Vertex (TValidVertexData&& _data, Graph& _graph, size_t _index)
		: ManagedUserData<TVertexData>{ std::move (_data) }, m_pGraph{ &_graph }, m_index{ _index }
	{
		ensureValidOrLastIndex (_index);
	}

#pragma endregion

#pragma region Edge insertion

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::addNewValidatedOutEdge (Edge& _edge)
	{
		_edge.attachToList ();
		if (!_edge.next ())
		{
			m_pLastOutEdge = &_edge;
		}
		if (!_edge.previous ())
		{
			m_pFirstOutEdge = &_edge;
		}
		if (hasMatrix ())
		{
			(*m_pOutVertexEdges)[_edge.to ().index ()] = &_edge;
		}
		m_outEdgesCount++;
		graph ().edgeAdded ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::ensureValidNewOutEdge (const Vertex& _to) const
	{
		ensureSameGraph (_to.graph (), graph ());
		if (hasOutEdge (_to))
		{
			throw std::logic_error{ "edge already exists" };
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::ensureValidNewOutEdgeBefore (const Vertex& _to, const Edge& _next) const
	{
		ensureValidNewOutEdge (_to);
		ensureSameVertex (_next.from (), *this);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdge (Vertex& _to)
	{
		ensureValidNewOutEdge (_to);
		Edge& edge{ *new Edge{*this, _to, m_pLastOutEdge, nullptr} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (EdgeData) inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdge (const TValidEdgeData& _data, Vertex& _to)
	{
		ensureValidNewOutEdge (_to);
		Edge& edge{ *new Edge{_data, *this, _to, m_pLastOutEdge, nullptr} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (EdgeData) inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdge (TValidEdgeData&& _data, Vertex& _to)
	{
		ensureValidNewOutEdge (_to);
		Edge& edge{ *new Edge{std::move (_data), *this, _to, m_pLastOutEdge, nullptr} };
		addNewValidatedOutEdge (edge);
		return edge;
	}


	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdgeBefore (Vertex& _to, Edge& _next)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		Edge& edge{ *new Edge{*this, _to, _next.previous (), &_next} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (EdgeData) inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdgeBefore (const TValidEdgeData& _data, Vertex& _to, Edge& _next)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		Edge& edge{ *new Edge{_data, *this, _to, _next.previous (), &_next} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (EdgeData) inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::addOutEdgeBefore (TValidEdgeData&& _data, Vertex& _to, Edge& _next)
	{
		ensureValidNewOutEdgeBefore (_to, _next);
		Edge& edge{ *new Edge{std::move (_data), *this, _to, _next.previous (), &_next} };
		addNewValidatedOutEdge (edge);
		return edge;
	}

#pragma endregion

#pragma region Graph interface

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::vertexAdded (Vertex& _vertex)
	{
		ensureSameGraph (_vertex.graph (), graph ());
		if (hasMatrix ())
		{
			m_pOutVertexEdges->insert (m_pOutVertexEdges->begin () + _vertex.index (), nullptr);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::vertexDestroyed (Vertex& _vertex)
	{
		ensureSameGraph (_vertex.graph (), graph ());
		if (hasOutEdge (_vertex))
		{
			outEdge (_vertex).destroy ();
		}
		if (hasMatrix ())
		{
			m_pOutVertexEdges->erase (m_pOutVertexEdges->begin () + _vertex.index ());
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::vertexSwapped (Vertex& _a, Vertex& _b)
	{
		ensureSameGraph (_a.graph (), graph ());
		ensureSameGraph (_b.graph (), graph ());
		if (hasMatrix ())
		{
			std::swap ((*m_pOutVertexEdges)[_a.index ()], (*m_pOutVertexEdges)[_a.index ()]);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::reserve (size_t _capacity)
	{
		if (hasMatrix ())
		{
			m_pOutVertexEdges->reserve (_capacity);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::shrinkToFit ()
	{
		if (hasMatrix ())
		{
			m_pOutVertexEdges->shrink_to_fit ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::Vertex::hasMatrix () const
	{
		return m_pOutVertexEdges;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::setMatrix (bool _enabled)
	{
		if (_enabled != hasMatrix ())
		{
			if (_enabled)
			{
				m_pOutVertexEdges = new std::vector<Edge*>{};
				reserve (graph ().capacity ());
				m_pOutVertexEdges->insert (m_pOutVertexEdges->begin (), graph ().verticesCount (), nullptr);
				for (Edge& edge : *this)
				{
					(*m_pOutVertexEdges)[edge.to ().index ()] = &edge;
				}
			}
			else
			{
				delete m_pOutVertexEdges;
				m_pOutVertexEdges = nullptr;
			}
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::setIndex (size_t _index)
	{
		graph ().ensureValidIndex (_index);
		m_index = _index;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::setGraph (Graph& _graph)
	{
		m_pGraph = _graph;
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
	inline size_t Graph<TVertexData, TEdgeData>::Vertex::index () const
	{
		return m_index;
	}

	template<typename TVertexData, typename TEdgeData>
	inline size_t Graph<TVertexData, TEdgeData>::Vertex::outEdgesCount () const
	{
		return m_outEdgesCount;
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::Vertex::hasOutEdge (const Vertex& _to) const
	{
		return outEdgeIfExists (_to);
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::outEdge (const Vertex& _to) const
	{
		return const_cast<Vertex&>(*this).outEdge (const_cast<Vertex&>(_to));
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge& Graph<TVertexData, TEdgeData>::Vertex::outEdge (Vertex& _to)
	{
		Edge* pEdge{ outEdgeIfExists (_to) };
		if (!pEdge)
		{
			throw std::logic_error{ "no edge" };
		}
		return *pEdge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Vertex::outEdgeIfExists (const Vertex& _to) const
	{
		return const_cast<Vertex&>(*this).outEdgeIfExists (const_cast<Vertex&>(_to));
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Vertex::outEdgeIfExists (Vertex& _to)
	{
		ensureSameGraph (_to.graph (), graph ());
		if (hasMatrix ())
		{
			return (*m_pOutVertexEdges)[_to.index ()];
		}
		else
		{
			for (Edge& edge : *this)
			{
				if (edge.to () == _to)
				{
					return &edge;
				}
			}
			return nullptr;
		}
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
	inline void Graph<TVertexData, TEdgeData>::Vertex::destroyEdge (Edge& _edge)
	{
		ensureSameVertex (_edge.from (), *this);
		if (_edge == *m_pFirstOutEdge)
		{
			m_pFirstOutEdge = _edge.next ();
		}
		if (_edge == *m_pLastOutEdge)
		{
			m_pLastOutEdge = _edge.previous ();
		}
		_edge.detachFromList ();
		graph ().edgeRemoved ();
		delete& _edge;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::destroyAllOutEdges ()
	{
		while (m_pFirstOutEdge)
		{
			destroyEdge (*m_pFirstOutEdge);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Vertex::destroy ()
	{
		graph ().destroyVertex (*this);
	}

#pragma endregion

#pragma endregion

#pragma region Edge

#pragma region Construction

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge::Edge (Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: ManagedUserData<TEdgeData>{ }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		ensureSameGraph (_from.graph (), _to.graph ());
		if (_pPrevious)
		{
			Vertex::ensureSameVertex (_from, _pPrevious->from ());
		}
		if (_pNext)
		{
			Vertex::ensureSameVertex (_from, _pNext->from ());
		}
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (EdgeData) inline Graph<TVertexData, TEdgeData>::Edge::Edge (const TValidEdgeData& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: ManagedUserData<TEdgeData>{ _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		ensureSameGraph (_from.graph (), _to.graph ());
		if (_pPrevious)
		{
			Vertex::ensureSameVertex (_from, _pPrevious->from ());
		}
		if (_pNext)
		{
			Vertex::ensureSameVertex (_from, _pNext->from ());
		}
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (EdgeData) inline Graph<TVertexData, TEdgeData>::Edge::Edge (TValidEdgeData&& _data, Vertex& _from, Vertex& _to, Edge* _pPrevious, Edge* _pNext)
		: ManagedUserData<TEdgeData>{ _data }, m_pFrom{ &_from }, m_pTo{ &_to }, m_pPrevious{ _pPrevious }, m_pNext{ _pNext }
	{
		ensureSameGraph (_from.graph (), _to.graph ());
		if (_pPrevious)
		{
			Vertex::ensureSameVertex (_from, _pPrevious->from ());
		}
		if (_pNext)
		{
			Vertex::ensureSameVertex (_from, _pNext->from ());
		}
	}

#pragma endregion

#pragma region Vertex interface

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Edge::detachFromList ()
	{
		if (previous ())
		{
			m_pPrevious->m_pNext = next ();
		}
		if (next ())
		{
			m_pNext->m_pPrevious = previous ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::Edge::attachToList ()
	{
		if (previous ())
		{
			m_pPrevious->m_pNext = this;
		}
		if (next ())
		{
			m_pNext->m_pPrevious = this;
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Edge::previous () const
	{
		return m_pPrevious;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Edge::previous ()
	{
		return m_pPrevious;
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Edge::next () const
	{
		return m_pNext;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Edge::next ()
	{
		return m_pNext;
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
	inline const Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Edge::antiParallelIfExists () const
	{
		return to ().outEdgeIfExists (from ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Edge* Graph<TVertexData, TEdgeData>::Edge::antiParallelIfExists ()
	{
		return to ().outEdgeIfExists (from ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::operator[] (size_t _index) const
	{
		return const_cast<Edge&>(*this)[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::Edge::operator[] (size_t _index)
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
		from ().destroyEdge (*this);
	}

#pragma endregion

#pragma endregion

#pragma region Graph

#pragma region Utils

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::ensureSameGraph (const Graph& _a, const Graph& _b)
	{
		if (&_a != &_b)
		{
			throw std::logic_error{ "not the same graph" };
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::updateIndices (size_t _first)
	{
		for (size_t i{ _first }; i < verticesCount (); i++)
		{
			(*this)[i].setIndex (i);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::ensureValidIndex (size_t _index) const
	{
		if (_index < 0)
		{
			throw std::out_of_range{ "index < 0" };
		}
		if (_index > verticesCount ())
		{
			throw std::out_of_range{ "index > verticesCount" };
		}
	}

#pragma endregion

#pragma region Construction

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
	inline void Graph<TVertexData, TEdgeData>::destroyVertex (Vertex& _vertex)
	{
		ensureSameGraph (_vertex.graph (), *this);
		_vertex.destroyAllOutEdges ();
		m_vertices.erase (m_vertices.begin () + _vertex.index ());
		updateIndices (_vertex.index ());
		delete& vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::destroyAllVertices ()
	{
		for (Vertex& vertex : *this)
		{
			vertex.destroyAllOutEdges ();
			delete& vertex;
		}
		m_vertices.clear ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::~Graph ()
	{
		destroyAllVertices ();
	}

#pragma endregion

#pragma region Vertex interface

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::edgeAdded ()
	{
		m_edgesCount++;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::edgeRemoved ()
	{
		m_edgesCount--;
	}

#pragma endregion

#pragma region Assignment

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>& Graph<TVertexData, TEdgeData>::operator=(const Graph& _clone)
	{
		destroyAllVertices ();
		shrinkToFit ();
		reserve (_clone.capacity ());
		setMatrix (_clone.hasMatrix ());
		for (const Vertex& vertex : _clone)
		{
			if constexpr (std::is_void_v<TVertexData>)
			{
				addVertex ();
			}
			else
			{
				addVertex (*vertex);
			}
		}
		for (const Vertex& vertex : _clone)
		{
			Vertex& thisVert{ (*this)[vertex.index ()] };
			for (const Edge& edge : vertex)
			{
				if constexpr (std::is_void_v<TEdgeData>)
				{
					thisVert.addOutEdge ((*this)[edge.to ().index ()]);
				}
				else
				{
					thisVert.addOutEdge (*edge, (*this)[edge.to ().index ()]);
				}
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
		std::swap (m_hasMatrix, _moved.m_hasMatrix);
		for (Vertex& vertex : *this)
		{
			vertex.setGraph (*this);
		}
		return *this;
	}

#pragma endregion

#pragma region Vertex insertion

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::addNewValidatedVertex (Vertex& _vertex)
	{
		for (Vertex& vertex : *this)
		{
			vertex.vertexAdded (_vertex);
		}
		m_vertices.insert (m_vertices.begin () + _vertex.index (), &_vertex);
		updateIndices (_vertex.index () + 1);
		_vertex.setMatrix (hasMatrix ());
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex ()
	{
		Vertex& vertex{ *new Vertex{*this, verticesCount ()} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (VertexData)
		inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (const TValidVertexData& _data)
	{
		Vertex& vertex{ *new Vertex{_data, *this, verticesCount ()} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (VertexData)
		inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertex (TValidVertexData&& _data)
	{
		Vertex& vertex{ *new Vertex{std::move (_data), *this, verticesCount ()} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (Vertex& _next)
	{
		ensureSameGraph (_next.graph (), *this);
		Vertex& vertex{ *new Vertex{*this, _next.index ()} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (VertexData)
		inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (const TValidVertexData& _data, Vertex& _next)
	{
		ensureSameGraph (_next.graph (), *this);
		Vertex& vertex{ *new Vertex{_data, *this, _next.index ()} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	MAX_FLOW_UD_VALID_T_IMPL (VertexData)
		inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::addVertexBefore (TValidVertexData&& _data, Vertex& _next)
	{
		ensureSameGraph (_next.graph (), *this);
		Vertex& vertex{ *new Vertex{std::move (_data), *this, _next.index ()} };
		addNewValidatedVertex (vertex);
		return vertex;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::swapVertices (Vertex& _a, Vertex& _b)
	{
		ensureSameGraph (_a.graph (), *this);
		ensureSameGraph (_b.graph (), *this);
		{
			const size_t aIndex{ _a.index () };
			_a.setIndex (_b.index ());
			_b.setIndex (aIndex);
		}
		std::swap (m_vertices[_a.index ()], m_vertices[_b.index ()]);
		for (Vertex& vertex : *this)
		{
			vertex.vertexSwapped (_a, _b);
		}
	}

#pragma endregion

#pragma region Getters

	template<typename TVertexData, typename TEdgeData>
	inline size_t Graph<TVertexData, TEdgeData>::edgesCount () const
	{
		return m_edgesCount;
	}

	template<typename TVertexData, typename TEdgeData>
	inline size_t Graph<TVertexData, TEdgeData>::verticesCount () const
	{
		return m_vertices.size ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::vertex (size_t _index) const
	{
		return const_cast<Graph&>(*this).vertex (_index);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::vertex (size_t _index)
	{
		ensureValidIndex (_index);
		return *m_vertices[_index];
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::operator[] (size_t _index) const
	{
		return vertex (_index);
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex& Graph<TVertexData, TEdgeData>::operator[] (size_t _index)
	{
		return vertex (_index);
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
		return verticesCount () ? &m_vertices[0] : nullptr;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::lastVertex ()
	{
		return verticesCount () ? &m_vertices[0] + verticesCount () - 1 : nullptr;
	}

	template<typename TVertexData, typename TEdgeData>
	inline const Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::increasePtr (const Vertex** _p, size_t _diff)
	{
		return _p ? _p + _diff : nullptr;
	}

	template<typename TVertexData, typename TEdgeData>
	inline Graph<TVertexData, TEdgeData>::Vertex** Graph<TVertexData, TEdgeData>::increasePtr (Vertex** _p, size_t _diff)
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

#pragma region Storage

	template<typename TVertexData, typename TEdgeData>
	inline size_t Graph<TVertexData, TEdgeData>::capacity () const
	{
		return m_vertices.capacity ();
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::reserve (size_t _capacity)
	{
		if (_capacity < 0)
		{
			throw std::out_of_range{ "capacity < 0" };
		}
		m_vertices.reserve (_capacity);
		for (Vertex& vertex : *this)
		{
			vertex.reserve (_capacity);
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::shrinkToFit ()
	{
		m_vertices.shrink_to_fit ();
		for (Vertex& vertex : *this)
		{
			vertex.shrinkToFit ();
		}
	}

	template<typename TVertexData, typename TEdgeData>
	inline bool Graph<TVertexData, TEdgeData>::hasMatrix () const
	{
		return m_hasMatrix;
	}

	template<typename TVertexData, typename TEdgeData>
	inline void Graph<TVertexData, TEdgeData>::setMatrix (bool _enabled)
	{
		if (_enabled != hasMatrix ())
		{
			m_hasMatrix = _enabled;
			for (Vertex& vertex : *this)
			{
				vertex.setMatrix (_enabled);
			}
		}
	}

#pragma endregion

#pragma endregion

#pragma endregion

}

#endif