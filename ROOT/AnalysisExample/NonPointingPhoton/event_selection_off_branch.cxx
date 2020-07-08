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

	const char* tree_name = "output";
	TChain* chain = new TChain(tree_name, tree_name);
	chain->Add(file_name);
	chain->SetBranchStatus("*", 0);
	float el_eta1, el_pt1;
	float el_eta2, el_pt2;
	float mu_pt1, mu_eta1;
	float mu_pt2, mu_eta2;
	float ph_eta1, ph_pt1;
	float ph_eta2, ph_pt2;
	float ph_t1, ph_t2;
	float el_t1, el_t2;
	float m_met, wt;
	chain->SetBranchAddress("el_eta1", &el_eta1);
	chain->SetBranchAddress("el_pt1", &el_pt1);
	chain->SetBranchAddress("el_eta2", &el_eta2);
	chain->SetBranchAddress("el_pt2", &el_pt2);
	chain->SetBranchAddress("mu_pt1", &mu_pt1);
	chain->SetBranchAddress("mu_eta1", &mu_eta1);
	chain->SetBranchAddress("mu_eta2", &mu_eta2);
	chain->SetBranchAddress("mu_pt2", &mu_pt2);
	chain->SetBranchAddress("ph_eta1", &ph_eta1);
	chain->SetBranchAddress("ph_pt1", &ph_pt1);
	chain->SetBranchAddress("ph_eta2", &ph_eta2);
	chain->SetBranchAddress("ph_pt2", &ph_pt2);
	chain->SetBranchAddress("ph_t1", &ph_t1);
	chain->SetBranchAddress("ph_t2", &ph_t2);
	chain->SetBranchAddress("el_t1", &el_t1);
	chain->SetBranchAddress("el_t2", &el_t2);
	chain->SetBranchAddress("m_met", &m_met);
	chain->SetBranchAddress("wt", &wt);

	chain->SetBranchStatus("el_eta1", 1);
	chain->SetBranchStatus("el_pt1", 1);
	chain->SetBranchStatus("el_eta2", 1);
	chain->SetBranchStatus("el_pt2", 1);
	chain->SetBranchStatus("mu_pt1", 1);
	chain->SetBranchStatus("mu_eta1", 1);
	chain->SetBranchStatus("mu_eta2", 1);
	chain->SetBranchStatus("mu_pt2", 1);
	chain->SetBranchStatus("ph_eta1", 1);
	chain->SetBranchStatus("ph_pt1", 1);
	chain->SetBranchStatus("ph_eta2", 1);
	chain->SetBranchStatus("ph_pt2", 1);
	chain->SetBranchStatus("ph_t1", 1);
	chain->SetBranchStatus("ph_t2", 1);
	chain->SetBranchStatus("el_t1", 1);
	chain->SetBranchStatus("el_t2", 1);
	chain->SetBranchStatus("m_met", 1);
	chain->SetBranchStatus("wt", 1);

	long int nentries = chain->GetEntries();
	cout << "Processing: " << nentries << endl;


	for(long int ientry=0; ientry < nentries; ientry ++) {
		chain->GetEntry(ientry);
		bool opt1 = el_pt1 > 27 && pass_eta(el_eta1);
		bool opt2 = mu_pt1 > 27 && abs(mu_eta1) < 2.7;
		bool opt3 = el_pt2 > 27 && pass_eta(el_eta2);
		bool opt4 = mu_pt2 > 27 && abs(mu_eta2) < 2.7;
		bool opt5 = ph_pt1 > 10 && pass_eta(ph_eta1);
		bool opt6 = ph_pt2 > 10 && pass_eta(ph_eta2);
		bool opt7 = m_met < 20;
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
