import networkx as nx
import my_graph_module

# Create a graph in NetworkX
G = nx.Graph()
G.add_edges_from([(0, 1), (1, 2), (3, 4)])

# Call the function
tracks = my_graph_module.get_tracks(G, cc_cut=0.5, th_min=1.0, th_add=0.1)
print(tracks)
