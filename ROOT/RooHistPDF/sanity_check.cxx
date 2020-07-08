/// This macro is used to validate the behaviour of PDFs, Asimovs and datasets
/// when generating distributions from histograms with a 
/// variable binning.
/// (c) Xianyang Yu, small modifications by Max Goblirsch 


#include "TCanvas.h"
#include "TH1F.h"
#include "TRandom.h"
#include "TLatex.h"
#include "TLegend.h"

#include "RooGaussian.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooHistPdf.h"
#include "RooExtendPdf.h"
// #include "HZZWorkspace/AtlasStyle.h"

using std::cout;
using std::endl;


int sanity_check(){
    // AtlasStyleHelper::SetAtlasStyle();

    /// Seed Parameters for our distributions. 
    /// These should be returned when fitting our models to the generated
    /// asimov. 

    const double in_mean = 6;
    const double in_sigma = 2;
    const int in_nEvt = 10;

    // this flag steers whether or not to generate equal bins (otherwise variable sizes)
    bool equal_bins = false;

    /// Generate input binning 
    std::vector<double> bins_array; 
    if(equal_bins) {
        bins_array = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    } else {
        bins_array = {0, 1, 2, 3, 4, 5, 6, 9, 12, 15};
    }


    /// turn into a TH1F and randomly populate it 
    auto h1 = new TH1F("h1", "h1", bins_array.size()-1, &(bins_array[0]));
    int nevts = 1000;
    gRandom->SetSeed(10);

    for(int i=0; i < nevts; i++) {
        h1->Fill(gRandom->Gaus(in_mean, in_sigma));
    }
	cout << "Integral: " << h1->Integral() << endl;
	cout << "Integral(width): " << h1->Integral("width") << endl;

    /// massage the TH1 to make it a valid input for a RooHistPdf
//    for (int ibin = 1; ibin < h1->GetNbinsX()+1; ++ibin){
//        h1->SetBinContent(ibin, h1->GetBinContent(ibin)/h1->GetBinWidth(ibin));
//    }
//    //h1->Scale(1/h1->Integral("width"));
    //h1->Scale(1/h1->Integral("width"));
    h1->Scale(1/h1->Integral(), "width");
    //h1->Scale(1, "width");
	//h1->Scale(1/h1->Integral("width"));

	cout << "After Scaling Integral: " << h1->Integral() << endl;
	cout << "After Scaling Integral(width): " << h1->Integral("width") << endl;

    // generate a PDF
    auto obs_x = new RooRealVar("obs_x", "observable", h1->GetXaxis()->GetXmin(), h1->GetXaxis()->GetXmax());
    auto data_hist = new RooDataHist("datahist", "datahist", *obs_x, h1);
    auto hist_pdf = new RooHistPdf("histpdf", "histpdf", *obs_x, *data_hist);
    hist_pdf->setUnitNorm(true);

    cout << "Interpolation Order: " << hist_pdf->getInterpolationOrder() << endl;


    // generate asimov histogram
    auto h_asimov = hist_pdf->createHistogram("htemp", *obs_x, RooFit::IntrinsicBinning(false));
    h_asimov->Scale(h1->Integral()/h_asimov->Integral());


    // generate histogram of the RooDataHist
    auto h_data_hist = data_hist->createHistogram("htemp2", *obs_x);
    h_data_hist->Scale(h1->Integral()/h_data_hist->Integral());
    h_data_hist->SetMarkerSize(1);
    h_data_hist->SetMarkerStyle(4);

    // generate an unbinned asimov dataset from our PDF
    int nEvts = in_nEvt;
    auto weight = RooRealVar("weight", "weight", 0, 1000);
    RooDataSet* unbinnedData = new RooDataSet("asimov", "asimov", RooArgSet(*obs_x, weight), RooFit::WeightVar("weight"));
    for(int ix=1; ix<=h_asimov->GetNbinsX(); ++ix) {
        obs_x->setVal(h_asimov->GetXaxis()->GetBinCenter(ix));
        unbinnedData->add(*obs_x, nEvts*h_asimov->GetBinContent(ix)*h_asimov->GetBinWidth(ix));
    }
    
    // then perform the fit of our histogram model - best fitted value of norm should equal to nEvts
    RooRealVar norm("norm", "norm", 0, 100);
    RooExtendPdf pdf = RooExtendPdf("pdf", "pdf", *hist_pdf, norm);
    pdf.fitTo(*unbinnedData);
  

    // also fit a gaussian - should reproduce mean, sigma, norm
      RooRealVar muGauss("muGauss","mean",0,100) ;
      muGauss.setVal(3);
    RooRealVar sigmaGauss("sigmaGauss","sigma",0.1,500) ;
    sigmaGauss.setVal(3);
    RooRealVar normGauss("normGauss","norm",0.1,1000) ;
    normGauss.setVal(5);

    RooAbsPdf* gaussPdf =new RooGaussian( "Gauss", "Gaussian", *obs_x, muGauss, sigmaGauss );
    RooExtendPdf gaussWithNorm("GaussWithNorm","Gauss",*gaussPdf, normGauss);


    gaussWithNorm.fitTo(*unbinnedData);


    // now plot the data on top of canvas
    // all histograms should be on top of each other
    auto canvas = TCanvas("canvas", "canvas", 600, 600);

    h1->GetYaxis()->SetRangeUser(0, 0.25);
    h1->SetLineColor(kRed);
    h1->SetMarkerColor(kRed);
    h1->SetFillColor(kRed);
    h1->SetFillStyle(3004);
    h1->Draw("HIST");

    auto h_data = unbinnedData->createHistogram("hdata", *obs_x);
    h_data->SetLineColor(kBlue);
    h_data->SetMarkerColor(kBlue);
    h_data->SetMarkerStyle(kOpenCircle);

    h_data->Scale(1./nEvts, "width");
    h_data->Draw("same");

    h_asimov->SetLineColor(kOrange-3);
    h_asimov->SetMarkerColor(kOrange-3);
    h_asimov->SetMarkerStyle(kOpenSquare);
    h_asimov->Draw("HIST same");
    
    h_data_hist->SetLineColor(kBlack);
    h_data_hist->SetMarkerColor(kBlack);
    h_data_hist->SetMarkerStyle(kFullDotLarge);
    h_data_hist->Draw("same");

    auto legend = TLegend(0.1, 0.8, 0.38, 0.95);
    legend.AddEntry(h1, "Origin Input","f");
    legend.AddEntry(h_data_hist, "from RooDataHist");
    legend.AddEntry(h_asimov, "from RooHistPdf");
    legend.AddEntry(h_data, "from UnbinnedDataSet");
    legend.Draw("same");

    // also print the outcome
    TLatex t1 (0.5,0.6,"Hi");
    t1.SetNDC();
    t1.SetTextSize(15);
    t1.SetTextFont(43);
    t1.SetTextColor(kRed);
    t1.DrawLatex(0.7,0.85,Form("Input norm: %i",in_nEvt));
    t1.DrawLatex(0.7,0.8,Form("Input mean: %.2f",in_mean));
    t1.DrawLatex(0.7,0.75,Form("Input sigma: %.2f",in_sigma));
    t1.SetTextColor(kBlack);
    t1.DrawLatex(0.7,0.68,Form("Histo norm: %.2f",norm.getVal()));
    t1.DrawLatex(0.7,0.6,Form("Gauss norm: %.2f",normGauss.getVal()));
    t1.DrawLatex(0.7,0.55,Form("Gauss mean: %.2f",muGauss.getVal()));
    t1.DrawLatex(0.7,0.5,Form("Gauss sigma: %.2f",sigmaGauss.getVal()));
    canvas.SaveAs("final_test.pdf");

    return 0;
}
