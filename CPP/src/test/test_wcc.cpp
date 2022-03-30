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
  , float                  // property associated with vertices
  > Graph; 

    int v=6;
    Graph g(v);

    boost::add_edge(0,1);
    boost::add_edge(1,2);
    boost::add_edge(2,3);
    boost::add_edge(3,1);
    boost::add_edge(4,5);

    std::vector<int> component (boost::num_vertices (g));
    size_t num_components = boost::connected_components (c, &component[0]);
    std::cout << "Number of components: " << num_components << std::endl;
    return 0;
}