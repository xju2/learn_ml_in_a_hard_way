#/usr/bin/env python
from __future__ import print_function

import uproot
import re

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description='peek into a root file')
    add_arg = parser.add_argument
    add_arg('file_name', help='file name')
    add_arg('-b', '--branch', default='.*', help='branch patterns')
    args = parser.parse_args()

    file_name = args.file_name
    pattern = args.branch

    file = uproot.open(file_name)
    all_trees = dict(file.allitems(filterclass=lambda cls: issubclass(cls, uproot.tree.TTreeMethods)))
    print("Open \x1b[33m{}\x1b[0m".format(file_name))
    print("\t compressed with {}".format(file.compression))
    print("\t contains {} trees".format(len(all_trees.keys())))


    br_pattern = re.compile(pattern)
    for tree_name,tree in all_trees.items():
        # data = tree.lazyarrays(entrysteps=500)
        print("Tree: \033[95m{}\033[0m has {} branches, {} events".format(
            tree_name.decode('utf-8'),
            len(tree.keys()),
            tree.numentries,
        ))
        for key in tree.keys():
            branch = tree[key]
            key = key.decode('utf-8')
            if not br_pattern.match(key):
                continue
            print("\t \x1b[33m{: <20}\x1b[0m has {} baskets, {} ".format(
                key, branch.numbaskets, branch.interpretation))

    print(green_text("HELLO WORLD"))
        # for start, stop, data in tree.iterate(entrysteps=1, reportentries=True, namedecode='utf-8', ):
        #     print(start, stop)
        #     # print(data.keys())
        #     # print(data.values())
        #     break

    # print("\t".join(all_trees.keys()))
