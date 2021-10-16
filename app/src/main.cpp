#include <max-flow/graph.hpp>
#include <iostream>

int main ()
{
	MaxFlow::Graph<int, int> g;
	g.addVertex (0);
	g.addVertex (1);
	g.addVertex (2);
	g.addVertex (3);
	g.addVertex (4);

	for (MaxFlow::Graph<int, int>::Vertex& v : g)
	{
		std::cout << *v;
	}

	std::cout << std::endl;

	for (auto it = g.crbegin (); it != g.crend (); ++it)
	{
		std::cout << **it;
	}
	std::cout << std::endl;

	return 0;
}