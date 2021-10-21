#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_GRAPHVIZ
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_GRAPHVIZ

#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>
#include <string>
#include <sstream>
#include <format>

namespace MaxFlow::Graphs::Algorithms
{

	using std::size_t;

#pragma region Declaration


	class GraphVizSource final
	{

	public:

		enum class EFormat
		{
			SVG, PNG, PDF
		};

	private:

		class Builder final
		{

		private:

			std::stringstream m_dot{};
			size_t m_level{};

			void addSpace ();
			void addNewLine ();
			void addStatementEnd ();
			void addIndent ();
			void addLabel (const std::string& _label);
			void addNode (size_t _node, const std::string& _label);
			void addEdge (size_t _from, size_t _to, const std::string& _label);

			static std::string formatCommandArgument (EFormat _format);
			static std::string command (const std::string& _inputFile, const std::string& _outputFile, EFormat _format);

		public:

			Builder ();

			void addComment (const std::string& _comment);
			void addNode (size_t _node);
			void addNode (size_t _node, size_t _label);
			void addEdge (size_t _from, size_t _to, flow_t _residualCapacity);
			void addEdge (size_t _from, size_t _to, flow_t _flow, flow_t _capacity);
			void addPush ();
			void addPop ();

			std::string toDot () const;

		};

		std::string m_dot{};

		GraphVizSource (const std::string& _dot);

		static std::string formatCommandArgument (EFormat _format);
		static std::string command (const std::string& _inputFile, const std::string& _outputFile, EFormat _format);

	public:

		MF_GG_TT_F static GraphVizSource from (const FlowGraph<TVertexData, TEdgeData>& _graph);
		static GraphVizSource from (const ResidualGraph& _graph, bool _includeVertexLabel = true, bool _skipZeroEdges = true);

		void toDotFile (const std::string& _file) const;
		void exportToFile (const std::string& _file, EFormat _format = EFormat::PNG) const;
		std::string toDot () const;

	};


#pragma endregion

#pragma region Implementation

	MF_GG_TT GraphVizSource GraphVizSource::from (const FlowGraph<TVD, TED>& _graph)
	{
		using Graph = FlowGraph<TVD, TED>;
		Builder builder{};
		builder.addPush ();
		builder.addComment ("Residual Graph");
		{
			builder.addPush ();
			builder.addComment ("Vertices:");
			builder.addComment ("v [index]");
			for (const Graph::Vertex& v : _graph)
			{
				builder.addNode (v.index ());
			}
			builder.addPop ();
		}
		{
			builder.addPush ();
			builder.addComment ("Edges:");
			builder.addComment ("a -> b [flow/capacity]");
			for (const Graph::Vertex& v : _graph)
			{
				if (v.outEdgesCount ())
				{
					builder.addComment (std::format ("From {}", v.index ()));
					for (const Graph::Edge& e : v)
					{
						builder.addEdge (e.from ().index (), e.to ().index (), e->flow (), e->capacity ());
					}
				}
			}
			builder.addPop ();
		}
		builder.addPop ();
		return GraphVizSource{ builder.toDot () };
	}

#pragma endregion

}

#endif