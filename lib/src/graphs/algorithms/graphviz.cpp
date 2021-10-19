#include <max-flow/graphs/algorithms/graphviz.hpp>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <format>
#include <stdexcept>

namespace MaxFlow::Graphs::Algorithms
{

	GraphVizBuilder::GraphVizBuilder ()
	{
		m_dot << "digraph" << '\n';
		m_dot << '{' << '\n';
	}

	void GraphVizBuilder::addLabel (const std::string& _label)
	{
		m_dot << '[' << "label" << '=' << '"' << _label << '"' << ']';
	}

	void GraphVizBuilder::addComment (const std::string& _comment)
	{
		for (char c : _comment)
		{
			if (!std::isprint (c))
			{
				throw std::invalid_argument{ "non print character" };
			}
		}
		m_dot << '\t';
		m_dot << "//" << ' ' << _comment;
		m_dot << '\n';
	}

	void GraphVizBuilder::addNode (size_t _node)
	{
		m_dot << '\t';
		m_dot << _node;
		addLabel (std::format ("{}", _node));
		m_dot << ';' << '\n';
	}

	void GraphVizBuilder::addNode (size_t _node, size_t _label)
	{
		m_dot << '\t';
		m_dot << _node;
		addLabel (std::format ("{},{}", _node, _label));
		m_dot << ';' << '\n';
	}

	void GraphVizBuilder::addEdge (size_t _from, size_t _to, flow_t _flow)
	{
		m_dot << '\t';
		m_dot << _from << "->" << _to;
		addLabel (std::format ("{}", _flow));
		m_dot << ';' << '\n';
	}

	void GraphVizBuilder::addEdge (size_t _from, size_t _to, flow_t _flow, size_t _capacity)
	{
		m_dot << '\t';
		m_dot << _from << "->" << _to;
		addLabel (std::format ("{},{}", _flow, _capacity));
		m_dot << ';' << '\n';
	}

	void GraphVizBuilder::toDotFile (const std::string& _file) const
	{
		std::ofstream f{};
		f.open (_file);
		f << toDot ();
		f.close ();
	}

	std::string GraphVizBuilder::formatCommandArgument (EFormat _format)
	{
		switch (_format)
		{
			case GraphVizBuilder::EFormat::SVG:
				return "svg";
			case GraphVizBuilder::EFormat::PNG:
				return "png";
			case GraphVizBuilder::EFormat::PDF:
				return "pdf";
			default:
				throw std::invalid_argument{ "unknown format" };
		}
	}

	std::string GraphVizBuilder::command (const std::string& _input, const std::string& _output, EFormat _format)
	{
		std::stringstream cmd{};
		cmd << "dot" << ' ';
		cmd << "-T" << ' ' << formatCommandArgument (_format) << ' ';
		cmd << "-o" << ' ' << _output << ' ';
		cmd << "-Nshape" << '=' << "circle" << ' ';
		cmd << "-Goverlap" << '=' << "false" << ' ';
		cmd << "-Eminlen" << '=' << "2" << ' ';
		cmd << "-Elen" << '=' << "2" << ' ';
		cmd << "-Kneato" << ' ';
		cmd << _input;
		return cmd.str ();
	}

	void GraphVizBuilder::exportToFile (const std::string& _file, EFormat _format) const
	{
		const char* pTempFileName{ std::tmpnam (nullptr) };
		if (!pTempFileName)
		{
			throw std::runtime_error{ "no temp file" };
		}
		const std::string tempFile{ pTempFileName };
		toDotFile (tempFile);
		const std::string cmd{ command (tempFile, _file, _format) };
		if (std::system (cmd.c_str ()))
		{
			throw std::runtime_error{ "command failed" };
		}
	}

	std::string GraphVizBuilder::toDot () const
	{
		return m_dot.str () + '}';
	}

	GraphVizBuilder toGraphvizDot (const ResidualGraph& _graph)
	{
		using Graph = ResidualGraph::UnderlyingGraph;
		GraphVizBuilder builder{};
		builder.addComment ("Residual Graph");
		builder.addComment ("Vertices (index,label):");
		for (const Graph::Vertex& v : _graph.graph ())
		{
			builder.addNode (v.index (), *v);
		}
		builder.addComment ("Edges (residual_capacity):");
		for (const Graph::Vertex& v : _graph.graph ())
		{
			for (const Graph::Edge& e : v)
			{
				builder.addEdge (e.from ().index (), e.to ().index (), *e);
			}
		}
		return builder;
	}

}