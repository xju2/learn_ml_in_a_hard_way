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

void event_selection(const char* file_name)  {

	TFile* output = TFile::Open("output.root", "recreate");
	TH1F* h_met = new TH1F("h_met", "h_met", 5, 0, 100);

	// using RDataFrame
	using RNTupleReader = ROOT::Experimental::RNTupleReader;
	using RNTupleModel = ROOT::Experimental::RNTupleModel;
	using RNTupleReadOptions = ROOT::Experimental::RNTupleReadOptions;

	const char* tree_name = "output";

	auto model = RNTupleModel::Create();
	RNTupleReadOptions options;
	options.SetClusterCache(RNTupleReadOptions::kMemory);
	cout << "Using in-memory source" << endl;
	auto ntuple = RNTupleReader::Open(std::move(model), tree_name,
			file_name, options);
	auto viewEl_Eta1 = ntuple->GetView<float>("el_eta1");
	auto viewEl_PT1  = ntuple->GetView<float>("el_pt1");
	auto viewEl_Eta2 = ntuple->GetView<float>("el_eta2");
	auto viewEl_PT2  = ntuple->GetView<float>("el_pt2");
	auto viewMu_Eta1 = ntuple->GetView<float>("mu_eta1");
	auto viewMu_PT1  = ntuple->GetView<float>("mu_pt1");
	auto viewMu_Eta2 = ntuple->GetView<float>("mu_eta2");
	auto viewMu_PT2  = ntuple->GetView<float>("mu_pt2");
	auto viewPh_Eta1 = ntuple->GetView<float>("ph_eta1");
	auto viewPh_PT1  = ntuple->GetView<float>("ph_pt1");
	auto viewPh_Eta2 = ntuple->GetView<float>("ph_eta2");
	auto viewPh_PT2  = ntuple->GetView<float>("ph_pt2");
	auto viewPh_T1   = ntuple->GetView<float>("ph_t1");
	auto viewPh_T2   = ntuple->GetView<float>("ph_t2");
	auto viewEl_T1   = ntuple->GetView<float>("el_t1");
	auto viewEl_T2   = ntuple->GetView<float>("el_t2");
	auto viewMET	 = ntuple->GetView<float>("m_met");
	auto viewWt		 = ntuple->GetView<float>("wt");


	unsigned nevents = 0;
	for(auto i : ntuple->GetViewRange()) {
		nevents ++;
		bool opt1 = viewEl_PT1(i) > 27 && pass_eta(viewEl_Eta1(i));
		bool opt2 = viewMu_PT1(i) > 27 && abs(viewMu_Eta1(i)) < 2.7;
		bool opt3 = viewEl_PT2(i) > 27 && pass_eta(viewEl_Eta2(i));
		bool opt4 = viewMu_PT2(i) > 27 && abs(viewMu_Eta2(i)) < 2.7;
		bool opt5 = viewPh_PT1(i) > 10 && pass_eta(viewPh_Eta1(i));
		bool opt6 = viewPh_PT2(i) > 10 && pass_eta(viewPh_Eta2(i));
		bool opt7 = viewMET(i) < 20;
		if( (opt1 || opt2 || opt3 || opt4) && (opt5 || opt6) && opt7)  {
			h_met->Fill(m_met);
		}
	} 
	output->Close();
}

int main(int argc, char** argv)  {
	if(argc < 2) {
		cout << argv[0] << " file_name" << endl;
	}
	event_selection(argv[1]);
	return 0;
}
