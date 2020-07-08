#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <ROOT/RDataFrame.hxx>

using namespace std;

void copytree(const char* origin_file, const char* bdt_file, const char* output_file, const char* tree_name)
{
   TFile oldfile(origin_file, "READ");
   TTree *oldtree;
   oldfile.GetObject(tree_name, oldtree);

   // disable not used branches
   oldtree->SetBranchStatus("ClassOut_XGB_fJVT_Higgs", 0);
   oldtree->SetBranchStatus("ClassOut_XGB_fJVT_VBF", 0);
   oldtree->SetBranchStatus("Event_XGB_fJVT_Category", 0);


   // Create a new file + a clone of old tree in new file
   TFile newfile(output_file, "recreate");
   auto newtree = oldtree->CloneTree(0);

   Float_t ClassOut_XGB_fJVT_Higgs;
   Float_t ClassOut_XGB_fJVT_VBF;
   Short_t Event_XGB_fJVT_Category;
   newtree->Branch("ClassOut_XGB_fJVT_Higgs", &ClassOut_XGB_fJVT_Higgs);
   newtree->Branch("ClassOut_XGB_fJVT_VBF", &ClassOut_XGB_fJVT_VBF);
   newtree->Branch("Event_XGB_fJVT_Category", &Event_XGB_fJVT_Category);

   TFile bdtFile(bdt_file, "READ");
   TTree* bdt_tree =  (TTree*) bdtFile.Get(tree_name);
   Float_t ClassOut_XGB_fJVT_Higgs_;
   Float_t ClassOut_XGB_fJVT_VBF_;
   Float_t Event_XGB_fJVT_Category_;
   bdt_tree->SetBranchAddress("ClassOut_XGB_fJVT_Higgs", &ClassOut_XGB_fJVT_Higgs_);
   bdt_tree->SetBranchAddress("ClassOut_XGB_fJVT_VBF", &ClassOut_XGB_fJVT_VBF_);
   bdt_tree->SetBranchAddress("Event_XGB_fJVT_Category", &Event_XGB_fJVT_Category_);


   cout << "Nevents Origin: " << oldtree->GetEntries() << endl;
   cout << "Nevents BDT: " << bdt_tree->GetEntries() << endl;

   Long64_t nentries = oldtree->GetEntries();
   for(Long64_t i=0; i < nentries; i++) {
		oldtree->GetEntry(i);
		bdt_tree->GetEntry(i);
		ClassOut_XGB_fJVT_Higgs = ClassOut_XGB_fJVT_Higgs_;
		ClassOut_XGB_fJVT_VBF = ClassOut_XGB_fJVT_VBF_;
		Event_XGB_fJVT_Category = Event_XGB_fJVT_Category_;
		newtree->Fill();
   }
   newtree->AutoSave();
}

void copytree2(const char* origin_file, 
		const char* bdt_file, const char* output_file, 
		const char* tree_name, int nThreads)
{
	ROOT::EnableImplicitMT(nThreads); // Tell ROOT you want to go parallel

	TFile oldfile(origin_file, "READ");
	TTree *oldtree;
	oldfile.GetObject(tree_name, oldtree);
	
	TFile bdtFile(bdt_file, "READ");
	TTree *bdttree;
	bdtFile.GetObject(tree_name, bdttree);

	oldtree->AddFriend(bdttree, "BDT");
	ROOT::RDataFrame data(*oldtree);
	data.Snapshot(tree_name, output_file);
}

int main(int argc, char** argv){
	bool help = false;
	int method = 0;
	string tree_name = "DiMuonNtuple";

	int opt;
	while ((opt = getopt(argc, argv, "ht:m:")) != -1) {
		switch (opt) {
		case 't':
			tree_name = optarg;
			break;
		case 'm':
			method = atoi(optarg);
			break;
		case 'h':
			help = true;
		default: /* '?' */
			fprintf(stderr, "Usage: %s [-h] [-t TREENAME] [-m METHOD] file0 file1 outfile_name\n",
					argv[0]);
			if (help) {
				printf("		-t T : tree name. Default is \"DiMuonNtuple\"\n");
				printf("		-m M : method. Default is 0 \n");
				printf("			   0: TTree\n");
				printf("			   1: RDataFrame\n");
				printf("			   N: RDataFrame N Threads\n");
			}
			exit(EXIT_FAILURE);
		}
	}
	if(argc - optind !=3){
		fprintf(stderr, "Usage: %s [-h] [-t TREENAME] file0 file1 outfile_name\n",
				argv[0]);
		exit(EXIT_FAILURE);
	}

	string file0(argv[optind]);
	string file1(argv[optind+1]);
	string file2(argv[optind+2]);

	auto start = std::chrono::system_clock::now();
	if(method == 0) {
		copytree(file0.c_str(), file1.c_str(), file2.c_str(), tree_name.c_str());
	} else {
		copytree2(file0.c_str(), file1.c_str(), file2.c_str(), tree_name.c_str(), method);
	}

	auto end = std::chrono::system_clock::now();
	std::chrono::duration<double> diff = end-start;
	printf("Time to copy %.2f seconds\n", diff.count());

	return 0;
}
