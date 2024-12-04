#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/property_map.hpp>

// Define the graph using Boost's adjacency_list
typedef boost::property<boost::vertex_name_t, int > vertex_p;
typedef boost::property<boost::edge_weight_t, double> edge_p;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS,
    boost::bidirectionalS,
    vertex_p,
    edge_p,
    boost::no_property> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

int main() {
    // Create a graph object
    Graph g;

    // Open the DOT file
    std::ifstream dot_file("graph.dot");
    if (!dot_file) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return 1;
    }

    // Setup dynamic properties with writable maps
    boost::dynamic_properties dp;
    dp.property("hit_id", boost::get(boost::vertex_name, g));
    dp.property("edge_scores", boost::get(boost::edge_weight, g));

    // Read the graph from the DOT file
    if (!boost::read_graphviz(dot_file, g, dp, "hit_id")) {
        std::cerr << "Error: Unable to parse graph from DOT file." << std::endl;
        return 1;
    }

    // Output the graph's vertices and properties
    std::cout << "Vertices:" << std::endl;
    for (auto v : boost::make_iterator_range(vertices(g))) {
        auto name = boost::get(boost::vertex_name, g, v);
        std::cout << "Vertex " << v
                  << " [hit_id=" << name << "]" << std::endl;
    }

    // Output the graph's edges and properties
    std::cout << "\nEdges:" << std::endl;
    for (auto e : boost::make_iterator_range(edges(g))) {
        double score = boost::get(boost::edge_weight, g, e);
        std::cout << "Edge " << source(e, g) << " -> " << target(e, g)
                  << " [edge_scores=" << score << "]" << std::endl;
    }

    return 0;
}
