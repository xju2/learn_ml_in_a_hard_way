void final_test(){
	bool equal_bins = false;
	double* bins = nullptr;
	int nbins;
	if(equal_bins) {
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

	// h1->Scale(1./h1->Integral(), "width");
	auto obs_x = new RooRealVar("obs_x", "observable", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
	auto data_hist = new RooDataHist("datahist", "datahist", *obs_x, h1);
	auto hist_pdf = new RooHistPdf("histpdf", "histpdf", *obs_x, *data_hist);
	hist_pdf->setUnitNorm(true);

	cout << "Interpolation Order: " << hist_pdf->getInterpolationOrder() << endl;

	auto h_asimov = hist_pdf->createHistogram("htemp", *obs_x, RooFit::IntrinsicBinning(false));

	auto h_data_hist = data_hist->createHistogram("htemp2", *obs_x);
	h_data_hist->Scale(h1->Integral()/h_data_hist->Integral());
	h_data_hist->SetMarkerSize(1);
	h_data_hist->SetMarkerStyle(4);


	int nEvts = 10;
	h_asimov->Scale(nEvts/h_asimov->Integral("width"));
	auto weight = RooRealVar("weight", "weight", 0, 1000);
	RooDataSet* unbinnedData = new RooDataSet("asimov", "asimov", RooArgSet(*obs_x, weight), RooFit::WeightVar("weight"));
	for(int ix=1; ix<=h_asimov->GetNbinsX(); ++ix) {
		obs_x->setVal(h_asimov->GetXaxis()->GetBinCenter(ix));
		unbinnedData->add(*obs_x, nEvts*h_asimov->GetBinContent(ix)*h_asimov->GetBinWidth(ix));
	}

	// scale to input histogram for comparison
	h_asimov->Scale(h1->Integral()/h_asimov->Integral());
	
	// then perform the fit, best fitted value of norm should equal to nEvts
	RooRealVar norm("norm", "norm", 0, 100);
	RooExtendPdf pdf = RooExtendPdf("pdf", "pdf", *hist_pdf, norm);
	pdf.fitTo(*unbinnedData);

	// now plot the data on top of canvas
	// all histograms should be on top of each other
	auto canvas = TCanvas("canvas", "canvas", 600, 600);

	// h1->GetYaxis()->SetRangeUser(0, 0.25);
	h1->SetLineColor(2);
	h1->SetLineStyle(2);
	h1->Draw("HIST");

	auto h_data = unbinnedData->createHistogram("hdata", *obs_x);
	h_data->SetMarkerSize(1);
	h_data->SetMarkerStyle(5);
	h_data->SetLineColor(1);
	h_data->SetLineWidth(2);

	h_data->Scale(1./nEvts, "width");
	h_data->Draw("same");

	h_asimov->SetLineStyle(2);
	h_asimov->SetLineWidth(2);
	h_asimov->SetLineColor(4);
	h_asimov->Draw("HIST same");
	
	h_data_hist->SetLineColor(6);
	h_data_hist->Draw("same");

	auto legend = TLegend(0.1, 0.8, 0.38, 0.95);
	legend.AddEntry(h1, "Origin Input");
	legend.AddEntry(h_data_hist, "from RooDataHist");
	legend.AddEntry(h_asimov, "from RooHistPdf");
	legend.AddEntry(h_data, "from UnbinnedDataSet");
	legend.Draw("same");
	canvas.SaveAs("final_test.pdf");
}

