#include <ROOT/RNTupleOptions.hxx>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleDS.hxx>

#include <TFile.h>
#include <TChain.h>
#include <TH1F.h>
#include <iostream>

using namespace std;
bool pass_eta(float eta) {
	return abs(eta) < 1.37 || (abs(eta) > 1.52 && abs(eta) < 2.47);
}

void event_selection(const char* file_name, const char* tree_name)  {

	TFile* output = TFile::Open("output.root", "recreate");
	ROOT::RDF::TH1FModel h_met({"h_met", "h_met", 5, 0, 100 });

	// using RDataFrame
	using RNTupleReader = ROOT::Experimental::RNTupleReader;
	using RNTupleModel = ROOT::Experimental::RNTupleModel;
	using RNTupleReadOptions = ROOT::Experimental::RNTupleReadOptions;

	auto event = std::make_shared<ROOT:RDataFrame>(tree_name, file_name);
	const string evt_selections = "((el_pt1>27.0 && (fabs(el_eta1)<1.37 || (fabs(el_eta1)>1.52 && fabs(el_eta1)<2.47))) || (mu_pt1>27.0 && fabs(mu_eta1)<2.7) || (el_pt2>27.0 && (fabs(el_eta2)<1.37 || (fabs(el_eta2)>1.52 && fabs(el_eta2)<2.47))) || (mu_pt2>27.0 && fabs(mu_eta2)<2.7)) && ((ph_pt1>10.0 && fabs(ph_eta1)<1.37) || (ph_pt2>10.0 && fabs(ph_eta2)<1.37)) && m_met<20.0";
	auto df_signal = std::make_shared<ROOT::RDF::RNode>( (*event).Filter(evt_selections) );
	ROOT::RDF::RResultPtr<TH1D> hist_ptr = df_signal.Histo1D(h_met, h_met.fName.Data(), "w");
	output->cd();
	hist_ptr->Write();
	output.Close();
}

int main(int argc, char** argv)  {
	if(argc < 2) {
		cout << argv[0] << " file_name" << endl;
	}
	event_selection(argv[1], "output");
	return 0;
}
