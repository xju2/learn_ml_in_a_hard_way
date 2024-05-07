import click
import torch
import torch.nn as nn
from torch import optim
from pytorch_lightning import LightningModule
from pytorch_lightning import Trainer
from pytorch_lightning.strategies.ddp import DDPStrategy
from cuml.neighbors import NearestNeighbors
import cupy
from torch.utils.data import Dataset
from torch.utils.data import DataLoader

@click.command()
@click.argument("gpus")
def main(gpus):
    trainer = Trainer(
        accelerator="gpu",
        devices=gpus,
        num_nodes=1,
        max_epochs=5,
        strategy=DDPStrategy(find_unused_parameters=False, static_graph=True),
    )
    module = test_model()
    trainer.fit(module)

class test_model(LightningModule):
    def __init__(self):
        super().__init__()
        self.trainset = TestDataset(40)
        self.net = nn.Sequential(
            nn.Linear(3, 18),
            nn.Linear(18, 18),
            nn.Linear(18, 3)
        )
        self.knn = NearestNeighbors(n_neighbors=10)

    def train_dataloader(self):
        return DataLoader(self.trainset, batch_size=1, num_workers=16)

    def forward(self, batch):
        data = batch[0]
        data = self.net(data)
        with cupy.cuda.Device(self.device.index):
            data_cu = cupy.from_dlpack(data.detach())
            self.knn.fit(data_cu)
            _, graph_idxs = self.knn.kneighbors(data_cu)
            graph_idxs = torch.from_dlpack(graph_idxs)
        return data

    def training_step(self, batch, batch_idx):
        data = self(batch)
        loss = data.sum()
        self.log("train_loss", loss)
        return loss

    def configure_optimizers(self):
        optimizer = optim.Adam(self.parameters(), lr=1e-3)
        return optimizer


class TestDataset(Dataset):
    def __init__(
        self, data_size=10,
    ):
        super().__init__()
        self.data_size = data_size

    def __len__(self):
        return self.data_size

    def __getitem__(self, idx):
        data = torch.rand(100, 3)
        return data

if __name__ == "__main__":
    main()
