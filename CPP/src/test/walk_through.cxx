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
#include <string>
#include <utility>
#include <chrono>

// Define the graph using Boost's adjacency_list
typedef boost::property<boost::vertex_name_t, int64_t > vertex_p;
typedef boost::property<boost::edge_weight_t, double> edge_p;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS,
    boost::bidirectionalS,
    vertex_p,
    edge_p,
    boost::no_property> Graph;

typedef boost::adjacency_list<
    boost::vecS, boost::vecS,
    boost::undirectedS,
    vertex_p,
    boost::no_property,
    boost::no_property> UndirectedGraph;

typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
using vertex_t = int32_t;

void print_track(const std::vector<int>& track) {
    for (int hit_id : track) {
        std::cout << hit_id << " ";
    }
    std::cout << std::endl;
}

std::vector<std::vector<int>> get_simple_path(const UndirectedGraph& G)
{
    std::vector<std::vector<int>> final_tracks;
    // Get weakly connected components
    std::vector<vertex_t> component(num_vertices(G));
    size_t num_components = boost::connected_components(G, &component[0]);
    std::cout << "Found " << num_components << " components." << std::endl;

    std::vector<std::vector<Vertex> > component_groups(num_components);
    for(size_t i = 0; i < component.size(); ++i) {
        component_groups[component[i]].push_back(i);
    }

    // loop over the sorted groups.
    for(const auto& sub_graph : component_groups) {
        if (sub_graph.size() < 3) {
            continue;
        }
        bool is_signal_path = true;
        // Check if all nodes in the sub_graph are signal paths
        for (int node : sub_graph) {
            /****
            int hit_id = boost::get(boost::vertex_name, G, node);
            if (hit_id == 2 || hit_id == 3934) {
                std::cout << "hit id: " << hit_id << " " << out_degree(node, G) \
                  << " " \
                  << in_degree(node, G) << " " \
                  << std::endl;
                for(auto idx: sub_graph) {
                    std::cout << " " << boost::get(boost::vertex_name, G, idx);
                }
                std::cout << std::endl;
            }
            ****/
            if (degree(node, G) > 2) {
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
            }
            final_tracks.push_back(track);
        }
    }
    std::cout << "Found " << final_tracks.size() << " simple paths." << std::endl;
    return final_tracks;
}

// Function to find the next hits
std::vector<int> find_next_node(
    const Graph &G,
    int current_hit,
    double th_min,
    double th_add,
    const std::vector<int>& all_hit_ids,
    bool debug = false
) {
    std::vector<int> next_hits;
    auto [begin, end] = boost::out_edges(current_hit, G);

    std::vector<std::pair<int, double>> neighbors_scores;
    if (debug) {
        std::cout << "in find_next_node: " << all_hit_ids[current_hit] << std::endl;
    }
    for (auto it = begin; it != end; ++it) {
        int neighbor = target(*it, G);
        double score = boost::get(boost::edge_weight, G, *it);
        if (debug) {
            std::cout << "\tneighbor of " << all_hit_ids[current_hit] \
              << " -> " << all_hit_ids[neighbor] \
              << ", score: " << score \
              << " out d.: " << out_degree(neighbor, G) \
              << " in d.: " << in_degree(neighbor, G) \
              << std::endl;
        }
        // if (neighbor == current_hit) continue;
        if (neighbor == current_hit || score <= th_min) continue;
        neighbors_scores.push_back({neighbor, score});
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
    std::function<std::vector<int>(const Graph&, int, bool)> next_node_fn,
    std::map<int64_t, bool>& used_hits, const std::vector<int>& all_hit_ids,
    bool debug = false
) {
    std::vector<std::vector<int>> path = {{starting_node}};
    if (debug) {
        std::cout << "build roads from " << all_hit_ids[starting_node] << std::endl;
    }

    int iterations = 0;
    while (true) {
        std::vector<std::vector<int>> new_path;
        bool is_all_done = true;

        if (debug) {
            std::cout << "\titeration: " << iterations++ << ", with " << path.size() << " paths" << std::endl;
        }
        // loop over each path and extend it.
        for (const auto &pp : path) {
            int start = pp.back();

            if (start == -1) {
                new_path.push_back(pp);
                continue;
            }

            auto next_hits = next_node_fn(G, start, debug);
            if (debug) {
                for(int nh : next_hits) {
                    int hit_id = boost::get(boost::vertex_name, G, nh);
                    std::cout << "\t\tnext hit: " << hit_id << "(" << nh << ") " << used_hits[hit_id] << std::endl;
                }
            }
            // remove used hits.
            next_hits.erase(std::remove_if(next_hits.begin(), next_hits.end(),
                [&](int node_id) {
                    int hit_id = boost::get(boost::vertex_name, G, node_id);
                    return used_hits[hit_id];
                }), next_hits.end());

            if (next_hits.empty()) {
                new_path.push_back(pp);
            } else {
                is_all_done = false;
                for (int nh : next_hits) {
                    std::vector<int> pp_extended = pp;
                    pp_extended.push_back(nh);
                    new_path.push_back(pp_extended);
                }
            }
        }

        path = new_path;
        // print out the current paths.
        if (debug) {
            for (const auto &pp : path) {
                std::cout << "\t\t";
                for (int node : pp) {
                    std::cout << all_hit_ids[node] << " ";
                }
                std::cout << std::endl;
            }
        }
        if (is_all_done) break;
    }
    return path;
}

void test_graph(const Graph& G,
            const std::map<int, Vertex>& hit_id_to_vertex,
            const std::vector<int>& all_hit_ids,
            int hit_id = 14437
            ) {
    std::cout <<"Testing Graph: " << boost::num_vertices(G) << " vertices, " << boost::num_edges(G) << " edges." << std::endl;
    auto node_id = hit_id_to_vertex.at(hit_id);
    std::cout << "hit id: " << hit_id << "(" << node_id << ") " \
      << in_degree(node_id, G) << " " << out_degree(node_id, G) << " " << std::endl;
    auto [begin, end] = boost::out_edges(node_id, G);

    for (auto it = begin; it != end; ++it) {
        int neighbor = target(*it, G);
        double score = boost::get(boost::edge_weight, G, *it);
        std::cout << "neighbor of " << hit_id << " -> " << all_hit_ids[neighbor] << "(" << neighbor << "), score: " << score << std::endl;
    }
    auto [begin_o, end_o] = boost::in_edges(node_id, G);
    for (auto it = begin_o; it != end_o; ++it) {
        int neighbor = source(*it, G);
        double score = boost::get(boost::edge_weight, G, *it);
        std::cout << "neighbor of " << hit_id << " <- " << all_hit_ids[neighbor] << "(" << neighbor << "), score: " << score << std::endl;
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

Graph cleanup_graph(const Graph& G, double cc_cut) {
    // remove fake edges and isolated vertices.
    Graph newG;

    // add vertices of G to newG, including the vertex name.
    std::map<int, int> old_vertex_to_new;
    int old_vertex_id = 0;
    int new_vertex_id = 0;
    for (auto v : boost::make_iterator_range(vertices(G))) {
        auto name = boost::get(boost::vertex_name, G, v);
        if (in_degree(v, G) == 0 && out_degree(v, G) == 0) {
            old_vertex_id ++;
            continue; // remove isolated vertices.
        }
        add_vertex(name, newG);
        old_vertex_to_new[old_vertex_id] = new_vertex_id;
        new_vertex_id ++;
        old_vertex_id ++;
    }
    // add edges of G to newG.
    auto [edge_b, edge_e] = boost::edges(G);
    for (auto it = edge_b; it != edge_e; ++it) {
        int source = boost::source(*it, G);
        int target = boost::target(*it, G);
        source = old_vertex_to_new[source];
        target = old_vertex_to_new[target];
        double weight = boost::get(boost::edge_weight, G, *it);
        if (weight <= cc_cut) continue;
        add_edge(source, target, weight, newG);
    }
    return newG;
}

// Get tracks using Boost's topological_sort
std::vector<std::vector<int>> get_tracks(const Graph &G, double cc_cut, double th_min, double th_add)
{
    Graph newG = cleanup_graph(G, cc_cut);
    std::map<int64_t, bool> used_hits;
    std::map<int, Vertex> hit_id_to_vertex;
    std::vector<int> all_hit_ids;
    for (auto v : boost::make_iterator_range(vertices(newG))) {
        auto name = boost::get(boost::vertex_name, newG, v);
        used_hits[name] = false;
        hit_id_to_vertex[name] = v;
        all_hit_ids.push_back(name);
    }
    test_graph(newG, hit_id_to_vertex, all_hit_ids, 165);

    UndirectedGraph ugraph;
    // add vertices from newG to ugraph.
    for (auto v : boost::make_iterator_range(vertices(newG))) {
        auto name = boost::get(boost::vertex_name, newG, v);
        add_vertex(name, ugraph);
    }
    // add edges from newG to ugraph.
    auto [edge_b, edge_e] = boost::edges(newG);
    for (auto it = edge_b; it != edge_e; ++it) {
        int source = boost::source(*it, newG);
        int target = boost::target(*it, newG);
        add_edge(source, target, ugraph);
    }

    std::vector<std::vector<int>> sub_graphs = get_simple_path(ugraph);
    // mark the used hits.
    for (const auto& track : sub_graphs) {
        for (int hit_id : track) {
            used_hits[hit_id] = true;
            if (hit_id == 3934) {
                print_track(track);
            }
        }
    }
    auto count_used_hits = [](const std::map<int64_t, bool>& used_hits) {
        return std::count_if(used_hits.begin(), used_hits.end(),
            [](const std::pair<int64_t, bool>& p) {
                return p.second;
            });
    };
    int num_used_hits = count_used_hits(used_hits);
    std::cout << "Used hits: " << num_used_hits << " after simple path. " << std::endl;
    int num_simple_paths = sub_graphs.size();
    std::cout << "From CC&&Walk: Number of tracks found by CC: " << num_simple_paths << std::endl;

    // Perform topological sort using Boost's topological_sort function
    // then find non-isolated vertices.
    std::vector<Vertex> topo_order;
    boost::topological_sort(newG, std::back_inserter(topo_order));

    // Define the next_hit function
    auto next_node_fn = [&](const Graph &G, int current_hit, bool debug) {
        return find_next_node(G, current_hit, th_min, th_add, all_hit_ids, debug);
    };

    bool debug = false;
    // Traverse the nodes in topological order
    for(auto it = topo_order.rbegin(); it != topo_order.rend(); ++it) {
        auto node_id = *it;
        int hit_id = boost::get(boost::vertex_name, newG, node_id);
        if (used_hits[hit_id]) continue;
        // if(hit_id == 4532 || hit_id == 83363) debug = true;
        // else debug = false;
        if (debug) {
            std::cout << "node: " << hit_id << "(" << node_id << ") " << used_hits[hit_id] << std::endl;
        }
        // Build roads (tracks) starting from the current node
        auto roads = build_roads(newG, node_id, next_node_fn, used_hits, all_hit_ids, debug);
        if (debug) {
            std::cout << "hit: " << hit_id << " " << roads.size();
        }
        used_hits[hit_id] = true;
        if (roads.empty()) {
            continue;
        }

        // Find the longest road and remove the last element (-1)
        std::vector<int>& longest_road = *std::max_element(roads.begin(), roads.end(),
            [](const std::vector<int> &a, const std::vector<int> &b) {
                return a.size() < b.size();
            });
        if (debug) {
            std::cout << " with the longest road: " << longest_road.size() << std::endl;
        }

        if (longest_road.size() >= 3) {
            std::vector<int> track;
            track.reserve(longest_road.size());
            for (int node : longest_road) {
                int hit_id = boost::get(boost::vertex_name, newG, node);
                used_hits[hit_id] = true;
                track.push_back(hit_id);
            }
            sub_graphs.push_back(track);
        }
    }
    std::cout << "From CC&&Walk: Number of tracks found by Walkthrough: " << sub_graphs.size() - num_simple_paths << std::endl;
    return sub_graphs;
}

void write_tracks(const std::vector<std::vector<int>>& tracks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to open file." << std::endl;
        return;
    }

    for (const auto &track : tracks) {
        for (int hit_id : track) {
            file << hit_id << " ";
        }
        file << "-1 ";
    }
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

    // Define thresholds
    double cc_cut = 0.01, th_min = 0.1, th_add = 0.6;

    Graph G;
    boost::dynamic_properties dp;
    dp.property("hit_id", boost::get(boost::vertex_name, G));
    dp.property("edge_scores", boost::get(boost::edge_weight, G));

    if (!boost::read_graphviz(dot_file, G, dp, "hit_id")) {
        std::cerr << "Error: Unable to parse graph from DOT file." << std::endl;
        return 1;
    }

    bool debug = false;
    if (debug){
        Graph newG = cleanup_graph(G, cc_cut);

        // print out how many edges.
        std::map<int64_t, bool> used_hits_map;
        std::vector<int> all_hit_ids(boost::num_vertices(newG));
        std::map<int, Vertex> hit_id_to_vertex;
        for (auto v : boost::make_iterator_range(vertices(newG))) {
            auto hit_id = boost::get(boost::vertex_name, newG, v);
            all_hit_ids[v] = hit_id;
            used_hits_map[hit_id] = false;
            hit_id_to_vertex[hit_id] = v;
        }

        test_graph(newG, hit_id_to_vertex, all_hit_ids, 83363);

        auto next_node_fn = [&](const Graph &G, int current_hit, bool debug) {
            return find_next_node(G, current_hit, th_min, th_add, all_hit_ids, debug);
        };
        auto road = build_roads(newG, hit_id_to_vertex.at(4532),
            next_node_fn, used_hits_map, all_hit_ids, true);
        return 0;
    }

    // Get tracks (subgraphs) and measure the time.
    auto start_time = std::chrono::high_resolution_clock::now();
    auto final_tracks = get_tracks(G, cc_cut, th_min, th_add);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed_time = end_time - start_time;

    // Print the results
    std::cout << "From CC&&Walk:: Total " << final_tracks.size() <<  " tracks" << std::endl;
    std::cout << "Time taken: " << elapsed_time.count() << " ms" << std::endl;
    // // print 5 tracks.
    // int idx = 0;
    // for (const auto &track : final_tracks) {
    //     std::cout << "Track " << idx++ << ": ";
    //     for (int hit_id : track) {
    //         std::cout << hit_id << " ";
    //     }
    //     std::cout << std::endl;
    //     if (idx > 5) break;
    // }
    std::cout << "From ACORN: " << "Number of tracks found by CC: 2949\nNumber of tracks found by Walkthrough: 1299" << std::endl;
    std::cout << "From ACORN: Total  4248 tracks." << std::endl;
    // Write the tracks to a file
    write_tracks(final_tracks, "tracks.txt");

    return 0;
}
