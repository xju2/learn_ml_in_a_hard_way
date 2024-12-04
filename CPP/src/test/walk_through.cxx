#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/property_map/property_map.hpp>
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
using vertex_t = int32_t;

std::vector<std::vector<int>> get_simple_path(
    Graph& G, std::map<int, bool>& used_hits)
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
            int in_deg = 1, out_deg = 1;
            // int in_deg = in_degree(node, G);
            // int out_deg = out_degree(node, G);
            if (!(out_deg <= 1 && in_deg <= 1)) {
                is_signal_path = false;
                break;
            }
        }

        // If it's a signal path, collect the hit_ids
        if (is_signal_path) {
            std::vector<int> track;
            for (int node : sub_graph) {
                int hit_id = boost::get(boost::vertex_name, G, node);
                track.push_back(hit_id);
                used_hits[hit_id] = true;
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
    double th_add,
    const std::vector<int>& all_hit_ids
) {
    std::vector<int> next_hits;
    auto [begin, end] = boost::out_edges(current_hit, G);

    std::vector<std::pair<int, double>> neighbors_scores;
    for (auto it = begin; it != end; ++it) {
        int neighbor = target(*it, G);
        double score = boost::get(boost::edge_weight, G, *it);
        // std::cout << "neighbor of " << all_hit_ids[current_hit] << " -> " << all_hit_ids[neighbor] << ", score: " << score << std::endl;
        if (neighbor == current_hit) continue;

        if (used_hits.find(neighbor) == used_hits.end()) {

            neighbors_scores.push_back({neighbor, score});
        }
    }

    if (neighbors_scores.empty()) return {};

    // Find the best neighbor
    auto best_neighbor = *std::max_element(neighbors_scores.begin(), neighbors_scores.end(),
                                      [](const std::pair<int, double> &a, const std::pair<int, double> &b) {
                                          return a.second < b.second;
                                      });

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

    return next_hits;
}

// Build roads (tracks) using Boost Graph
std::vector<std::vector<int>> build_roads(
    const Graph &G,
    int starting_node,
    std::function<std::vector<int>(const Graph&, int, const std::set<int>&)> next_hit_fn,
    std::set<int> &used_hits, const std::vector<int>& all_hit_ids
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
    // std::cout << "path for: " << all_hit_ids[starting_node] << ": " << std::endl;
    // int idx = 0;
    // for (const auto &pp : path) {
    //     std::cout << "\t path " << idx++ << ": ";
    //     for (int node : pp) {
    //         std::cout << all_hit_ids[node] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    return path;
}

typedef boost::graph_traits<Graph>::edge_descriptor EdgeDescriptor;
struct WeightThresholdPredicate {
    WeightThresholdPredicate(Graph &G, double th_min) : G(G), th_min(th_min) {}
    bool operator()(const EdgeDescriptor &e) const {
        return boost::get(boost::edge_weight, G, e) < th_min;
    }
    Graph &G;
    double th_min;
};

void test_graph(const Graph& G,
            const std::map<int, Vertex>& hit_id_to_vertex,
            const std::vector<int>& all_hit_ids) {
    std::cout <<" Graph: " << boost::num_vertices(G) << " vertices, " << boost::num_edges(G) << " edges." << std::endl;
    int hit_id = 14424;
    auto node_id = hit_id_to_vertex.at(hit_id);
    std::cout << "hit id: " << hit_id << " " \
      << in_degree(node_id, G) << " " << out_degree(node_id, G) << " " << std::endl;
    auto [begin, end] = boost::out_edges(node_id, G);

    for (auto it = begin; it != end; ++it) {
        int neighbor = target(*it, G);
        double score = boost::get(boost::edge_weight, G, *it);
        std::cout << "neighbor of " << hit_id << " -> " << all_hit_ids[neighbor] << ", score: " << score << std::endl;
    }
/****
    // print the first 5 edges.
    auto [edge_b, edge_e] = boost::edges(G);
    int idx = 0;
    for (auto it = edge_b; it != edge_e; ++it) {
        int source = boost::source(*it, G);
        int target = boost::target(*it, G);
        double weight = boost::get(boost::edge_weight, G, *it);
        std::cout << "edge: " << all_hit_ids[source] << " -> " << all_hit_ids[target] << ", weight: " << weight << std::endl;
        idx ++;
        if (idx > 5) break;
    }
    // print the first 5 vertices.
    auto [vertex_b, vertex_e] = boost::vertices(G);
    idx = 0;
    for (auto it = vertex_b; it != vertex_e; ++it) {
        std::cout << "vertex: " << all_hit_ids[*it] << std::endl;
        idx ++;
        if (idx > 5) break;
    }
***/
}
// Get tracks using Boost's topological_sort
std::vector<std::vector<int>> get_tracks(
    const Graph &G,
    double th_min,
    double th_add,
    std::map<int, bool> &used_hits,
    const std::map<int, Vertex>& hit_id_to_vertex,
    const std::vector<int>& all_hit_ids
) {
    // remove edges with weight < th_min, using the remove_edge_if function.
    // WeightThresholdPredicate predicate(G, th_min);
    // remove_edge_if(predicate, G);

    Graph newG;
    // add vertices of G to newG, including the vertex name.
    for (auto v : boost::make_iterator_range(vertices(G))) {
        auto name = boost::get(boost::vertex_name, G, v);
        add_vertex(name, newG);
    }
    // add edges of G to newG.
    auto [edge_b, edge_e] = boost::edges(G);
    for (auto it = edge_b; it != edge_e; ++it) {
        int source = boost::source(*it, G);
        int target = boost::target(*it, G);
        double weight = boost::get(boost::edge_weight, G, *it);
        if (weight < th_min) continue;
        add_edge(source, target, weight, newG);
    }

    std::vector<std::vector<int>> sub_graphs = get_simple_path(newG, used_hits);

    // Perform topological sort using Boost's topological_sort function
    // then find non-isolated vertices.
    std::vector<Vertex> topo_order;
    boost::topological_sort(newG, std::back_inserter(topo_order));
    std::vector<Vertex> non_isolated_vertices;
    for (auto it = topo_order.begin(); it != topo_order.end(); ++it) {
        auto hit_id = boost::get(boost::vertex_name, newG, *it);
        if (used_hits[hit_id]) {
            continue;
        }
        if (in_degree(*it, newG) == 0 && out_degree(*it, newG) == 0) {
            used_hits[hit_id] = true;
            continue;
        }
        non_isolated_vertices.push_back(*it);
    }
    std::cout << "Found " << non_isolated_vertices.size() << " non-isolated vertices." << std::endl;

    std::set<int> used_nodes;
    // Define the next_hit function
    auto next_hit_fn = [&](const Graph &G, int current_hit, const std::set<int> &used_hits) {
        return find_next_hits(G, current_hit, used_hits, th_min, th_add, all_hit_ids);
    };

    // Traverse the nodes in topological order
    for (auto node_id : non_isolated_vertices) {
        // node_id = hit_id_to_vertex.at(14424);
        // Build roads (tracks) starting from the current node
        auto roads = build_roads(newG, node_id, next_hit_fn, used_nodes, all_hit_ids);
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
            std::vector<int> track;
            for (int node : longest_road) {
                int hit_id = boost::get(boost::vertex_name, G, node);
                used_hits[hit_id] = true;
                track.push_back(hit_id);
            }
            sub_graphs.push_back(track);
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
    // copied from /global/cfs/cdirs/m3443/usr/xju/ITk/For2023Paper/metric_learning_testing/debug/debug_graph.dot
    std::ifstream dot_file("debug_graph.dot");
    if (!dot_file) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return 1;
    }

    Graph G;
    boost::dynamic_properties dp;
    dp.property("hit_id", boost::get(boost::vertex_name, G));
    dp.property("edge_scores", boost::get(boost::edge_weight, G));

    if (!boost::read_graphviz(dot_file, G, dp, "hit_id")) {
        std::cerr << "Error: Unable to parse graph from DOT file." << std::endl;
        return 1;
    }
    // print out how many edges.
    std::map<int, bool> used_hits_map;
    std::vector<int> all_hit_ids(boost::num_vertices(G));
    std::map<int, Vertex> hit_id_to_vertex;
    for (auto v : boost::make_iterator_range(vertices(G))) {
        auto hit_id = boost::get(boost::vertex_name, G, v);
        all_hit_ids[v] = hit_id;
        used_hits_map[hit_id] = false;
        hit_id_to_vertex[hit_id] = v;
    }

    test_graph(G, hit_id_to_vertex, all_hit_ids);
    // Define thresholds
    double th_min = 0.1;
    double th_add = 0.6;

    // Get tracks (subgraphs)
    auto final_tracks = get_tracks(G, th_min, th_add, used_hits_map, hit_id_to_vertex, all_hit_ids);

    // Print the results
    std::cout << "Tracks: " << final_tracks.size() << std::endl;
    // print 5 tracks.
    int idx = 0;
    for (const auto &track : final_tracks) {
        std::cout << "Track " << idx++ << ": ";
        for (int hit_id : track) {
            std::cout << hit_id << " ";
        }
        std::cout << std::endl;
        if (idx > 5) break;
    }
    std::cout << "From ACORN: " << "Number of tracks found by CC: 2950\nNumber of tracks found by Walkthrough: 1294" << std::endl;
    std::cout << "From ACORN: Total  4244 tracks." << std::endl;

    return 0;
}
