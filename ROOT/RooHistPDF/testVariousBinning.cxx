void testVariousBinning(){
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

	// h1->Scale(1./h1->Integral(), "width");
	auto obs_x = new RooRealVar("obs_x", "observable", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
	const char* binning_name = "bin";
	// auto binning = RooBinning(nbins, bins, binning_name);
	// obs_x->setBinning(binning, binning_name);

	auto data_hist = new RooDataHist("datahist", "datahist", *obs_x, h1);
	auto hist_pdf = new RooHistPdf("histpdf", "histpdf", *obs_x, *data_hist);
	hist_pdf->setUnitNorm(true);
	cout << "Interpolation Order: " << hist_pdf->getInterpolationOrder() << endl;

	auto h_asimov = hist_pdf->createHistogram("htemp", *obs_x, RooFit::IntrinsicBinning(false));
	// auto h_asimov = hist_pdf->createHistogram("htemp", *obs_x, RooFit::Binning(binning));
	h_asimov->Scale(h1->Integral()/h_asimov->Integral());

	auto h_data_hist = data_hist->createHistogram("htemp2", *obs_x);
	// h_data_hist->Scale(h1->Integral("width")/h_data_hist->Integral("width"));
	h_data_hist->Scale(h1->Integral()/h_data_hist->Integral());
	h_data_hist->SetMarkerSize(1);
	h_data_hist->SetMarkerStyle(4);


	auto weight = RooRealVar("weight", "weight", 0, 1000);
	RooDataSet* unbinnedData = new RooDataSet("asimov", "asimov", RooArgSet(*obs_x, weight), RooFit::WeightVar("weight"));
	for(int ix=1; ix<=h_asimov->GetNbinsX(); ++ix) {
		obs_x->setVal(h_asimov->GetXaxis()->GetBinCenter(ix));
		unbinnedData->add(*obs_x, 10*h_asimov->GetBinContent(ix)*h_asimov->GetBinWidth(ix));
		// cout << h_asimov->GetXaxis()->GetBinCenter(ix) << " " <<  h_asimov->GetBinContent(ix) << endl;
	}
	// now plot the unbinned data on top of canvas
	// auto h_data = unbinnedData->createHistogram("hdata", *obs_x, RooFit::Binning(binning));
	RooRealVar norm("norm", "norm", 0, 100);
	RooExtendPdf pdf = RooExtendPdf("pdf", "pdf", *hist_pdf, norm);
	pdf.fitTo(*unbinnedData);

	auto canvas = TCanvas("canvas", "canvas", 600, 600);

	auto h_data = unbinnedData->createHistogram("hdata", *obs_x);
	h_data->SetMarkerSize(1);
	h_data->SetMarkerStyle(4);
	h_data->Sumw2();
	h_data->Scale(1., "width");
	h1->GetYaxis()->SetRangeUser(0, 0.25);
	h1->SetLineColor(2);
	h1->SetLineStyle(2);
	h1->Draw("HIST");

	h_data->SetLineColor(1);
	h_data->SetLineWidth(2);
	// h_data->Draw("same");

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
	// legend.AddEntry(h_data, "from UnbinnedDataSet");
	legend.Draw("same");
	cout << h1->Integral("width") << " " << h_data->Integral("width") << " " << h_asimov->Integral("width") << endl;
	canvas.SaveAs("variable_binning_test.pdf");

	auto can2 = TCanvas("canvas2", "canvas", 600, 600);
    RooPlot* rp = obs_x->frame();
	rp->addTH1(h1, "HIST");
    data_hist->plotOn(rp);

	RooCmdArg plotCmd;
	RooCmdArg normCmd;
	normCmd = RooFit::Normalization(h1->Integral("width"), RooAbsReal::NumEvent);
    hist_pdf->plotOn(rp,plotCmd,normCmd);

	cout << "DataHist: " << data_hist->sumEntries() <<  " Histogram: " << " " << h1->Integral() << " " << h1->Integral("width") << " PDF: "  << hist_pdf->getNorm(*obs_x) << endl;
	cout << "DataHist::sum " << data_hist->sum(true, false) << endl;

	rp->Draw();
	can2.SaveAs("test.pdf");
}

