#ifndef INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_GRAPHVIZ
#define INCLUDED_MAX_FLOW_GRAPH_ALGORITHMS_GRAPHVIZ

#include <max-flow/graphs/flow.hpp>
#include <max-flow/graphs/residual.hpp>
#include <string>
#include <sstream>

namespace MaxFlow::Graphs::Algorithms
{

	using std::size_t;

#pragma region Declaration

	class GraphVizBuilder final
	{

	public:

		enum class EFormat
		{
			SVG, PNG, PDF
		};

	private:

		std::stringstream m_dot{};

		void addLabel (const std::string& _label);

		static std::string formatCommandArgument (EFormat _format);
		static std::string command (const std::string& _inputFile, const std::string& _outputFile,EFormat _format);

	public:

		GraphVizBuilder ();

		void addComment (const std::string& _comment);
		void addNode (size_t _node);
		void addNode (size_t _node, size_t _label);
		void addEdge (size_t _from, size_t _to, flow_t _residualCapacity);
		void addEdge (size_t _from, size_t _to, flow_t _flow, flow_t _capacity);

		void toDotFile (const std::string& _file) const;
		void exportToFile (const std::string& _file, EFormat _format = EFormat::PNG) const;
		std::string toDot () const;

	};

	MF_GG_TT_F GraphVizBuilder toGraphvizDot (const FlowGraph<TVertexData, TEdgeData>& _graph);
	GraphVizBuilder toGraphvizDot (const ResidualGraph& _graph);

#pragma endregion

#pragma region Implementation

	MF_GG_TT GraphVizBuilder toGraphvizDot (const FlowGraph<TVD, TED>& _graph)
	{
		using Graph = FlowGraph<TVD, TED>;
		GraphVizBuilder builder{};
		builder.addComment ("Flow Graph");
		builder.addComment ("Vertices (index):");
		for (const Graph::Vertex& v : _graph)
		{
			builder.addNode (v.index ());
		}
		builder.addComment ("Edges (flow,capacity):");
		for (const Graph::Vertex& v : _graph)
		{
			for (const Graph::Edge& e : v)
			{
				builder.addEdge (e.from ().index (), e.to ().index (), e->flow (), e->capacity ());
			}
		}
		return builder;
	}

#pragma endregion

}

#endif