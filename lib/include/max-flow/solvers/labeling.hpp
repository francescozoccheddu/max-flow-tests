#ifndef INCLUDED_MAX_FLOW_SOLVERS_LABELING
#define INCLUDED_MAX_FLOW_SOLVERS_LABELING

#include <max-flow/graphs/residual.hpp>

namespace MaxFlow::Solvers
{

	void labeling (Graphs::ResidualGraph& _residualGraph, Graphs::ResidualVertex& _source, Graphs::ResidualVertex& _sink);

}

#endif