#!/usr/bin/env python

import numpy as np
import networkx as nx
import h5py

if __name__ == "__main__":
    with h5py.File("test_nx_h5py.hdf5", 'r') as f:
        for name in f:
            print(name)

        # nodes
        node_idx = f['node_index']
        node_info = f['nodes']
        node_features_data = [node_info[x] for x in node_info.keys()]
        node_features = [
            dict([(name, node_features_data[iname][ii]) for iname,name in enumerate(node_info.keys())]) for ii in range(len(node_idx))
        ]

        print(node_features)
        graph = nx.DiGraph()
        graph.add_nodes_from(zip(node_idx, node_features))
        print(graph.nodes(data=True))


        # edges
        senders = f['senders']
        receivers = f['receivers']
        edge_info = f['edges']
        edge_features_data = [edge_info[x] for x in edge_info.keys()]
        edge_features = [
            dict([(name, edge_features_data[iname][ii]) for iname,name in enumerate(edge_info.keys())]) for ii in range(len(senders))
        ]
        graph.add_edges_from(zip(senders, receivers, edge_features))
        print(graph.edges(data=True))
