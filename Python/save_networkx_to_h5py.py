#!/usr/bin/env python

import numpy as np
import networkx as nx
import h5py

if __name__ == "__main__":
    graph = nx.DiGraph()
    graph.add_node(1, pos=[1, 2, 3], solution=[0.])
    graph.add_node(2, pos=[2, 3, 4], solution=[1.])
    graph.add_node(3, pos=[3, 3, 4], solution=[1.])

    graph.add_edge(1, 2, solution=[0], features=[0.1, 3, 4.4])
    graph.add_edge(2, 3, solution=[1], features=[0.2, 5, 4.4])

    number_of_nodes = graph.number_of_nodes()
    node_idxs, node_attr = zip(*graph.nodes(data=True))
    node_features = next(iter(graph.nodes(data=True)))[1].keys()
    node_datas = []

    senders, receivers, edge_attr_dicts = zip(*graph.edges(data=True))
    edge_features = next(iter(graph.edges(data=True)))[2].keys()

    with h5py.File("test_nx_h5py.hdf5", 'w') as f:
        dset = f.create_dataset('node_index', data=np.array(node_idxs))

        node_group = f.create_group('nodes')
        for feature in node_features:
            data = [x[feature] for x in node_attr if x[feature] is not None]
            if len(data) != number_of_nodes:
                raise ValueError("Either all the nodes should have features, or none of them")
            dset = node_group.create_dataset(feature, data=np.array(data))

        dset = f.create_dataset('senders', data=np.array(senders))
        dset = f.create_dataset('receivers', data=np.array(receivers))

        edge_group = f.create_group('edges')
        for feature in edge_features:
            data = [x[feature] for x in edge_attr_dicts if x[feature] is not None]
            dset = edge_group.create_dataset(feature, data=np.array(data))
