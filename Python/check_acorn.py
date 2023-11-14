#!/usr/bin/env python
"""Check if the python environment is set up correctly for the acorn project.
Requirement following key python packages:
pytorch
pytorch-lightning
pyg
frnn
cugraph
torch_scatter
"""

def check():
    # python interpreter
    import sys
    print("python interpreter: ", sys.executable)

    try:
        import torch
        print("torch: ", torch.__version__)
        print("torch cuda: ", torch.cuda.is_available())
        print("torch cuda device count: ", torch.cuda.device_count())
        print("torch cuda device name: ", torch.cuda.get_device_name())
        print("torch cuda device capability: ", torch.cuda.get_device_capability())
    except ImportError:
        print("torch not found")

    try:
        import pytorch_lightning
        print("pytorch_lightning: ", pytorch_lightning.__version__)
    except ImportError:
        print("pytorch_lightning not found")

    try:
        import torch_geometric
        print("pyg: ", torch_geometric.__version__)
    except (ImportError, OSError) as error:
        print(error)
        print("pyg not found")

    try:
        import frnn
        print("frnn 1.0")
    except ImportError:
        print("frnn not found")

    try:
        import cugraph
        print("cugraph: ", cugraph.__version__)
    except ImportError:
        print("cugraph not found")

    try:
        import torch_scatter
        print("torch_scatter: ", torch_scatter.__version__)
    except ImportError:
        print("torch_scatter not found")


if __name__ == '__main__':
    check()
