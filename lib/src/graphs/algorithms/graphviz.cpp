#include <max-flow/graphs/algorithms/graphviz.hpp>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <format>
#include <stdexcept>
#include <algorithm>

namespace MaxFlow::Graphs::Algorithms
{

#pragma region Builder

	GraphVizSource::Builder::Builder ()
	{
		m_dot << "digraph";
		addNewLine ();
	}

	void GraphVizSource::Builder::addSpace ()
	{
		m_dot << ' ';
	}

	void GraphVizSource::Builder::addNewLine ()
	{
		m_dot << '\n';
	}

	void GraphVizSource::Builder::addStatementEnd ()
	{
		m_dot << ';';
		addNewLine ();
	}

	void GraphVizSource::Builder::addIndent ()
	{
		for (size_t i{}; i < m_level; i++)
		{
			m_dot << '\t';
		}
	}

	void GraphVizSource::Builder::addLabel (const std::string& _label)
	{
		m_dot << '[' << "label" << '=' << '"' << _label << '"' << ']';
	}

	void GraphVizSource::Builder::addNode (size_t _node, const std::string& _label)
	{
		addIndent ();
		m_dot << _node;
		addSpace ();
		addLabel (_label);
		addStatementEnd ();
	}

	void GraphVizSource::Builder::addEdge (size_t _from, size_t _to, const std::string& _label)
	{
		addIndent ();
		m_dot << _from;
		addSpace ();
		m_dot << "->";
		addSpace ();
		m_dot << _to;
		addSpace ();
		addLabel (_label);
		addStatementEnd ();
	}

	void GraphVizSource::Builder::addComment (const std::string& _comment)
	{
		for (char c : _comment)
		{
			if (!std::isprint (c))
			{
				throw std::invalid_argument{ "non print character" };
			}
		}
		addIndent ();
		m_dot << "//";
		addSpace ();
		m_dot << _comment;
		addNewLine ();
	}

	void GraphVizSource::Builder::addNode (size_t _node)
	{
		addNode (_node, std::format ("{}", _node));
	}

	void GraphVizSource::Builder::addNode (size_t _node, size_t _label)
	{
		addNode (_node, std::format ("{} ({})", _node, _label));
	}

	void GraphVizSource::Builder::addEdge (size_t _from, size_t _to, flow_t _flow)
	{
		addEdge (_from, _to, std::format ("{}", _flow));
	}

	void GraphVizSource::Builder::addEdge (size_t _from, size_t _to, flow_t _flow, size_t _capacity)
	{
		addEdge (_from, _to, std::format ("{}/{}", _flow, _capacity));
	}

	void GraphVizSource::Builder::addPush ()
	{
		addIndent ();
		m_dot << '{';
		addNewLine ();
		m_level++;
	}

	void GraphVizSource::Builder::addPop ()
	{
		if (m_level == 0)
		{
			throw std::logic_error{ "level zero" };
		}
		m_level--;
		addIndent ();
		m_dot << '}';
		addNewLine ();
	}

	std::string GraphVizSource::Builder::toDot () const
	{
		return m_dot.str ();
	}

#pragma endregion

#pragma region Source

	GraphVizSource::GraphVizSource (const std::string& _dot) : m_dot{ _dot }
	{}

	std::string GraphVizSource::formatCommandArgument (EFormat _format)
	{
		switch (_format)
		{
			case EFormat::SVG:
				return "svg";
			case EFormat::PNG:
				return "png";
			case EFormat::PDF:
				return "pdf";
			default:
				throw std::invalid_argument{ "unknown format" };
		}
	}

	std::string GraphVizSource::command (const std::string& _input, const std::string& _output, EFormat _format)
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

	std::string GraphVizSource::toDot () const
	{
		return m_dot;
	}

	void GraphVizSource::toDotFile (const std::string& _file) const
	{
		std::ofstream f{};
		f.open (_file);
		f << toDot ();
		f.close ();
	}

	void GraphVizSource::exportToFile (const std::string& _file) const
	{
		const size_t sepIndex{ _file.find_last_of ('.') };
		if (sepIndex < 0)
		{
			throw std::logic_error{ "unknown extension" };
		}
		std::string extension{ _file.substr (sepIndex + 1) };
		std::transform (extension.begin (), extension.end (), extension.begin (), [] (unsigned char _c) { return  std::tolower (_c); });
		EFormat format;
		if (extension == "png")
		{
			format = EFormat::PNG;
		}
		else if (extension == "pdf")
		{
			format = EFormat::PDF;
		}
		else if (extension == "svg")
		{
			format = EFormat::SVG;
		}
		else
		{
			throw std::logic_error{ "unknown extension" };
		}
		exportToFile (_file, format);
	}

	void GraphVizSource::exportToFile (const std::string& _file, EFormat _format) const
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

	GraphVizSource GraphVizSource::from (const ResidualGraph& _graph, bool _skipZeroEdges, const size_t* _pLabels)
	{
		Builder builder{};
		builder.addPush ();
		builder.addComment ("Residual Graph");
		{
			builder.addPush ();
			builder.addComment ("Vertices:");
			builder.addComment (_pLabels ? "v [index (label)]" : "v [index]");
			for (const ResidualVertex& v : _graph)
			{
				if (_pLabels)
				{
					builder.addNode (v.index (), _pLabels[v.index ()]);
				}
				else
				{
					builder.addNode (v.index ());
				}
			}
			builder.addPop ();
		}
		{
			builder.addPush ();
			builder.addComment ("Edges:");
			builder.addComment ("a -> b [residual_capacity]");
			for (const ResidualVertex& v : _graph)
			{
				if (v.outEdgesCount ())
				{
					bool commented{};
					for (const ResidualEdge& e : v)
					{
						if (!_skipZeroEdges || *e)
						{
							if (!commented)
							{
								builder.addComment (std::format ("From {}", v.index ()));
								commented = true;
							}
							builder.addEdge (e.from ().index (), e.to ().index (), *e);
						}
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