#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <fstream>
#include <regex>
#include <string>
#include <utility>

// Define the graph using Boost's adjacency_list
typedef boost::adjacency_list<
    boost::vecS, boost::vecS,
    boost::bidirectionalS,
    boost::no_property,
    boost::property<boost::edge_weight_t, double>
    > Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// Helper function to add an edge to the graph
void add_edge(Graph &G, int u, int v, double score) {
    boost::add_edge(u, v, score, G);  // score as the edge weight
}

using vertex_t = int32_t;
std::vector<std::vector<int>> get_simple_path(Graph& G)
{
    std::vector<std::vector<int>> final_tracks;
    // Get weakly connected components
    std::vector<vertex_t> component(num_vertices(G));
    size_t num_components = boost::connected_components(G, &component[0]);

    for (size_t i = 0; i < num_components; ++i) {
        bool is_signal_path = true;
        std::vector<int> sub_graph;

        // Collect nodes in the current component
        for (size_t v = 0; v < component.size(); ++v) {
            if (component[v] == i) {
                sub_graph.push_back(v);
            }
        }
        // Check if all nodes in the sub_graph are signal paths
        for (int node : sub_graph) {
            int in_deg = in_degree(node, G);
            int out_deg = out_degree(node, G);
            if (!(out_deg <= 1 && in_deg <= 1)) {
                is_signal_path = false;
                break;
            }
        }

        // If it's a signal path, collect the hit_ids
        if (is_signal_path) {
            std::vector<int> track;
            for (int node : sub_graph) {
                track.push_back(node);
            }
            if (track.size() > 2) {
                final_tracks.push_back(track);
            }
            // Remove nodes from the graph
            for (int node : sub_graph) {
                remove_vertex(node, G);
            }
        }
    }
    return final_tracks;
}

// Function to find the next hits
std::vector<int> find_next_hits(
    const Graph &G,
    int current_hit,
    const std::set<int> &used_hits,
    double th_min,
    double th_add
) {
    std::vector<int> next_hits;
    auto [begin, end] = boost::adjacent_vertices(current_hit, G);

    std::vector<std::pair<int, double>> neighbors_scores;
    for (auto it = begin; it != end; ++it) {
        int neighbor = *it;
        if (neighbor == current_hit) continue;
        // print out the neighbor
        // std::cout << "neighbor of " << current_hit << " -> " << neighbor << " ";
        if (used_hits.find(neighbor) == used_hits.end()) {
            double score = boost::get(boost::edge_weight, G, boost::edge(current_hit, neighbor, G).first);
            neighbors_scores.push_back({neighbor, score});
        }
    }

    if (neighbors_scores.empty()) return {};

    // Find the best neighbor
    auto best_neighbor = *std::max_element(neighbors_scores.begin(), neighbors_scores.end(),
                                      [](const std::pair<int, double> &a, const std::pair<int, double> &b) {
                                          return a.second < b.second;
                                      });

    if (best_neighbor.second <= th_min) return {};

    // Add neighbors with score > th_add
    for (const auto &neighbor : neighbors_scores) {
        if (neighbor.second > th_add) {
            next_hits.push_back(neighbor.first);
        }
    }

    // If no neighbors were added, add the best neighbor
    if (next_hits.empty()) {
        next_hits.push_back(best_neighbor.first);
    }
    // // print out the next hits
    // for (int nh : next_hits) {
    //     std::cout << nh << " ";
    // }

    return next_hits;
}

// Build roads (tracks) using Boost Graph
std::vector<std::vector<int>> build_roads(
    const Graph &G,
    int starting_node,
    std::function<std::vector<int>(const Graph&, int, const std::set<int>&)> next_hit_fn,
    std::set<int> &used_hits
) {
    std::vector<std::vector<int>> path = {{starting_node}};
    // std::cout << "build roads from " << starting_node << std::endl;
    while (true) {
        std::vector<std::vector<int>> new_path;
        bool is_all_none = true;

        for (const auto &pp : path) {
            int start = pp.back();

            if (start == -1) {
                new_path.push_back(pp);
                continue;
            }

            std::set<int> current_used_hits = used_hits;
            current_used_hits.insert(pp.begin(), pp.end());

            auto next_hits = next_hit_fn(G, start, current_used_hits);
            if (next_hits.empty()) {
                std::vector<int> pp_extended = pp;
                pp_extended.push_back(-1);
                new_path.push_back(pp_extended);
            } else {
                is_all_none = false;
                for (int nh : next_hits) {
                    std::vector<int> pp_extended = pp;
                    pp_extended.push_back(nh);
                    new_path.push_back(pp_extended);
                }
            }
        }

        path = new_path;
        if (is_all_none) break;
    }

    // // print out the path
    // for (const auto &pp : path) {
    //     for (int node : pp) {
    //         std::cout << node << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return path;
}

// Get tracks using Boost's topological_sort
std::vector<std::vector<int>> get_tracks(
    const Graph &G,
    double th_min,
    double th_add
) {
    std::set<int> used_nodes;
    std::vector<std::vector<int>> sub_graphs;

    // Perform topological sort using Boost's topological_sort function
    std::vector<Vertex> topo_order;
    boost::topological_sort(G, std::back_inserter(topo_order));

    // Define the next_hit function
    auto next_hit_fn = [&](const Graph &G, int current_hit, const std::set<int> &used_hits) {
        return find_next_hits(G, current_hit, used_hits, th_min, th_add);
    };

    // Traverse the nodes in topological order
    for (auto node_id : topo_order) {
        if (used_nodes.find(node_id) != used_nodes.end()) {
            continue;
        }

        // Build roads (tracks) starting from the current node
        auto roads = build_roads(G, node_id, next_hit_fn, used_nodes);
        if (roads.size() < 3) {
            continue;
        }

        // Find the longest road and remove the last element (-1)
        std::vector<int> longest_road = *std::max_element(roads.begin(), roads.end(),
            [](const std::vector<int> &a, const std::vector<int> &b) {
                return a.size() < b.size();
            });

        if (!longest_road.empty() && longest_road.back() == -1) {
            longest_road.pop_back();
        }

        if (longest_road.size() < 3) {
            used_nodes.insert(node_id);
            continue;
        }

        sub_graphs.push_back(longest_road);
        used_nodes.insert(longest_road.begin(), longest_road.end());
    }

    return sub_graphs;
}

// Function to parse the graph from a file with only edge info
Graph read_graph_from_file(const std::string& filename) {
    std::ifstream infile(filename, std::ios::in);
    std::string line;

    // Graph to store the result
    Graph G;

    // Read the edge information
    if (std::getline(infile, line)) {
        // Use regex to extract edges and their scores
        std::regex edge_regex(R"(\((\d+), (\d+), \{'edge_scores': ([\d\.]+)\}\))");
        std::smatch match;
        std::string::const_iterator search_start(line.cbegin());

        // Read edges with scores
        while (std::regex_search(search_start, line.cend(), match, edge_regex)) {
            int source = std::stoi(match[1]);
            int target = std::stoi(match[2]);
            double score = std::stod(match[3]);

            // Add edge to the graph with the score as the edge weight
            boost::add_edge(source, target, score, G);
            search_start = match.suffix().first;
        }
    }

    return G;
}

int main() {
    // Create a directed graph (DiGraph) using Boost's adjacency_list
    Graph G;
    dynamic_properties dp;
    read_graphviz("graph_data.dot", G, dp);

    Graph G = read_graph_from_file("graph_data.txt");

    // print out how many edges.
    std::cout << "Number of edges: " << boost::num_edges(G) << std::endl;

    // Define thresholds
    double th_min = 0.1;
    double th_add = 0.6;

    // Get tracks (subgraphs)
    auto final_tracks = get_simple_path(G);
    auto walk_tracks = get_tracks(G, th_min, th_add);

    final_tracks.insert(final_tracks.end(), walk_tracks.begin(), walk_tracks.end());

    // Print the results
    std::cout << "Tracks (subgraphs): " << tracks.size() << std::endl;
    for (const auto &track : tracks) {
        for (int node : track) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
