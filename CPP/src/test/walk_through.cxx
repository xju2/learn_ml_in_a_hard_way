#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/iteration_macros.hpp>
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
typedef boost::property<boost::vertex_name_t, std::string, boost::property < boost::vertex_color_t, float > > vertex_p;
typedef boost::property<boost::edge_weight_t, double> edge_p;
typedef boost::adjacency_list<
    boost::listS, boost::vecS,
    boost::bidirectionalS,
    vertex_p,
    edge_p,
    boost::no_property> Graph;
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
    std::cout << "Found " << num_components << " components." << std::endl;

    std::map<int, std::vector<Vertex>> component_groups;
    for(size_t i = 0; i < component.size(); ++i) {
        component_groups[component[i]].push_back(i);
    }
    for(const auto& [k, sub_graph] : component_groups) {
        if (sub_graph.size() < 3) {
            continue;
        }
        bool is_signal_path = true;
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
            final_tracks.push_back(track);
        }
    }
    std::cout << "Found " << final_tracks.size() << " simple paths." << std::endl;
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

typedef boost::graph_traits<Graph>::edge_descriptor EdgeDescriptor;
struct WeightThresholdPredicate {
    WeightThresholdPredicate(Graph &G, double th_min) : G(G), th_min(th_min) {}
    bool operator()(const EdgeDescriptor &e) const {
        return boost::get(boost::edge_weight, G, e) <= th_min;
    }
    Graph &G;
    double th_min;
};

// Get tracks using Boost's topological_sort
std::vector<std::vector<int>> get_tracks(
    Graph &G,
    double th_min,
    double th_add
) {
    // remove edges with weight < th_min, using the remove_edge_if function.
    WeightThresholdPredicate predicate(G, th_min);
    remove_edge_if(predicate, G);
    std::cout << "after moving edges with weight < " << th_min << ", number of edges: " << boost::num_edges(G) \
      << ", and " << boost::num_vertices(G) << " nodes." << std::endl;

    std::vector<std::vector<int>> sub_graphs = get_simple_path(G);

    // Perform topological sort using Boost's topological_sort function
    // then find non-isolated vertices.
    std::vector<Vertex> topo_order(num_vertices(G));
    boost::topological_sort(G, std::back_inserter(topo_order));
    std::vector<Vertex> non_isolated_vertices;
    for (auto it = topo_order.begin(); it != topo_order.end(); ++it) {
        if (in_degree(*it, G) == 0 && out_degree(*it, G) == 0) {
            continue;
        }
        non_isolated_vertices.push_back(*it);
    }
    std::cout << "Found " << non_isolated_vertices.size() << " non-isolated vertices." << std::endl;
    // // Remove isolated vertices (in reverse order for safety)
    // for (auto it = isolated_vertices.rbegin(); it != isolated_vertices.rend(); ++it) {
    //     remove_vertex(*it, G);
    // }
    // std::cout << "after removing isolated vertices, number of edges: " << boost::num_edges(G) \
    //   << ", and " << boost::num_vertices(G) << " nodes." << std::endl;

    std::set<int> used_nodes;
    // Define the next_hit function
    auto next_hit_fn = [&](const Graph &G, int current_hit, const std::set<int> &used_hits) {
        return find_next_hits(G, current_hit, used_hits, th_min, th_add);
    };

    // Traverse the nodes in topological order
    for (auto node_id : non_isolated_vertices) {
        if (used_nodes.count(node_id) > 0) {
            continue;
        }

        // Build roads (tracks) starting from the current node
        auto roads = build_roads(G, node_id, next_hit_fn, used_nodes);
        if (roads.empty() || roads[0].size() < 3) {
            used_nodes.insert(node_id);
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

        if (longest_road.size() >= 3) {
            sub_graphs.push_back(longest_road);
            used_nodes.insert(longest_road.begin(), longest_road.end());
        } else {
            used_nodes.insert(node_id);
        }
    }

    return sub_graphs;
}

int main() {
    // Check boost graph:
    // https://www.boost.org/doc/libs/1_86_0/libs/graph/doc/table_of_contents.html
    // Create a directed graph (DiGraph) using Boost's adjacency_list
    Graph G;
    boost::dynamic_properties dp;
    boost::property_map<Graph, boost::vertex_name_t>::type name = boost::get(boost::vertex_name, G);
    dp.property("hit_id",name);
    boost::property_map<Graph, boost::edge_weight_t>::type weight = boost::get(boost::edge_weight, G);
    dp.property("edge_scores",weight);
    // copied from /global/cfs/cdirs/m3443/usr/xju/ITk/For2023Paper/metric_learning_testing/debug/debug_graph.dot
    std::ifstream dot("debug_graph.dot");
    read_graphviz(dot, G, dp, "hit_id");

    // print out how many edges.
    std::cout << "Number of edges: " << boost::num_edges(G) << std::endl;

    // Define thresholds
    double th_min = 0.1;
    double th_add = 0.6;

    // Get tracks (subgraphs)
    auto final_tracks = get_tracks(G, th_min, th_add);

    // Print the results
    std::cout << "Tracks: " << final_tracks.size() << std::endl;
    std::cout << "From ACORN: " << "Number of tracks found by CC: 2950\nNumber of tracks found by Walkthrough: 1294" << std::endl;
    std::cout << "From ACORN: Total  4244 tracks." << std::endl;

    return 0;
}
