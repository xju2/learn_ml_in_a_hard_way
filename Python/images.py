#!/usr/bin/env python

import numpy as np
import wandb
import matplotlib.pyplot as plt

def test_wandb_image():
    wandb.init()
    examples = []
    for i in range(3):
        pixels = np.random.randint(low=0, high=256, size=(100, 100, 3))
        image = wandb.Image(pixels, caption=f"random field {i}")
        examples.append(image)
    wandb.log({"examples": examples})

def test_plt_image():
    wandb.init()

    array = np.random.randint(low=0, high=256, size=1000)
    fig, ax = plt.subplots()
    ax.hist(array)
    # fig.tight_layout(pad=0)
    fig.canvas.draw()
    pixels = np.frombuffer(fig.canvas.tostring_rgb(), dtype=np.uint8)
    pixels = pixels.reshape(fig.canvas.get_width_height()[::-1] + (3,))
    image = wandb.Image(pixels, caption=f"random field")

    wandb.log({"examples": [image]})

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description='test image functions')
    add_arg = parser.add_argument
    
    args = parser.parse_args()
    test_plt_image()
    
