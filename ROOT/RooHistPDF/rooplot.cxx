void rooplot(){
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
	h1->Scale(1/h1->Integral("width"));

	auto obs_x = new RooRealVar("obs_x", "observable", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
	auto data_hist = new RooDataHist("datahist", "datahist", *obs_x, h1);
	auto hist_pdf = new RooHistPdf("histpdf", "histpdf", *obs_x, *data_hist);
	hist_pdf->setUnitNorm(true);

	auto can2 = TCanvas("canvas2", "canvas", 600, 600);
    RooPlot* rp = obs_x->frame();
	cout << "DataHist: " << data_hist->sumEntries() << endl;
    data_hist->plotOn(rp);

	RooCmdArg plotCmd;
	RooCmdArg normCmd;
	//normCmd = RooFit::Normalization(h1->Integral(),RooAbsReal::NumEvent);
    // hist_pdf->plotOn(rp,plotCmd,normCmd);
    hist_pdf->plotOn(rp);
	rp->Draw();
	can2.SaveAs("rooplot.pdf");
}

