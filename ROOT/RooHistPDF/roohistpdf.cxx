void roohistpdf(){
	bool equal_bins = false;
	double* bins = nullptr;
	int nbins;
	if(equal_bins) {
		//double bins_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		double bins_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
		bins = bins_array;
		nbins = 15;
	} else {
		double bins_array[] = {0, 1, 2, 3, 4, 5, 6, 9, 12, 15};
		bins = bins_array;
		nbins = 9;
	}

	auto h1 = new TH1F("h1", "h1", nbins, bins);
	int nevts = 1000;
	gRandom->SetSeed(10);
	for(int i=0; i < nevts; i++) {
		h1->Fill(gRandom->Gaus(6, 2));
	}

	auto obs_x = new RooRealVar("obs_x", "observable", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
	auto data_hist = new RooDataHist("datahist", "datahist", *obs_x, h1);
	auto hist_pdf = new RooHistPdf("histpdf", "histpdf", *obs_x, *data_hist);
	hist_pdf->setUnitNorm(true);

	auto h_asimov = hist_pdf->createHistogram("htemp", *obs_x, RooFit::IntrinsicBinning(false));
	// h_asimov->Scale(h1->Integral()/h_asimov->Integral("width"));
	h_asimov->Scale(h1->Integral()/h_asimov->Integral());

	// now plot the unbinned data on top of canvas
	// auto h_data = unbinnedData->createHistogram("hdata", *obs_x, RooFit::Binning(binning));
	auto canvas = TCanvas("canvas", "canvas", 600, 600);

	h1->GetYaxis()->SetRangeUser(0, 500);
	h1->SetLineColor(2);
	h1->SetLineStyle(2);
	h1->Draw("HIST");

	h_asimov->SetLineStyle(2);
	h_asimov->SetLineWidth(2);
	h_asimov->SetLineColor(4);
	h_asimov->SetMarkerSize(1);
	h_asimov->SetMarkerStyle(4);
	h_asimov->Draw("same");

	auto legend = TLegend(0.1, 0.8, 0.38, 0.95);
	legend.AddEntry(h1, "Origin Input");
	legend.AddEntry(h_asimov, "from RooHistPdf");
	// legend.AddEntry(h_data, "from UnbinnedDataSet");
	legend.Draw("same");
	canvas.SaveAs("roohistpdf.pdf");
}

