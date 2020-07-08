#!/usr/bin/env python
from __future__ import print_function

import ROOT
ROOT.gROOT.SetBatch()
from array import array
import random

if __name__ == "__main__":
    nbins = 4
    bins = array('f', [1, 3, 5, 9, 13])
    h1 = ROOT.TH1F("h1", "h1", 4, bins)
    for i in range(1000):
        val = random.gauss(7, 2)
        h1.Fill(val)
    for i in range(1, h1.GetNbinsX()+1):
        print(h1.GetBinLowEdge(i), h1.GetBinWidth(i), h1.GetBinContent(i))

    print(h1.Integral())
    canvas = ROOT.TCanvas("canvas", "canvas", 600, 600)
    h1.Draw("HIST TEXT")
    canvas.SaveAs("test.pdf")
    h1.Scale(1, 'width')
    h1.Draw("HIST TEXT")
    print(h1.Integral())
    print(h1.Integral('width'))
    canvas.SaveAs("test2.pdf")
