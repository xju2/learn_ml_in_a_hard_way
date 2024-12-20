#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include <iostream>
#include <vector>

int main()
{
    typedef
    boost::adjacency_list<
    boost::vecS            // edge list
  , boost::vecS            // vertex list
  , boost::undirectedS     // directedness
  , boost::no_property     // property associated with vertices
  , float                  // property associated with edges
  > Graph;

    size_t v=10;
    Graph g(v);

    boost::add_edge(0, 1, 1.2, g);
    boost::add_edge(1, 2, 0.2, g);
    boost::add_edge(2, 3, 0.3, g);
    boost::add_edge(7, 3, 0.4, g);
    boost::add_edge(4, 5, 1.1, g);

    std::vector<int> component (boost::num_vertices (g));
    size_t num_components = boost::connected_components (g, &component[0]);
    std::cout << "Number of components: " << num_components << std::endl;
    for (size_t i = 0; i < v; ++i){
        std::cout << "Node[" << i << "] = " << component[i] << std::endl;
    }
    return 0;
}