//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 15 11:30:55 2016 by ROOT version 6.04/14
// from TTree miniTree/miniTree
// found on file: data2015/main_data2015.root
//////////////////////////////////////////////////////////

#ifndef miniTree_h
#define miniTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"

using namespace std;

class miniTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           runNumber;
   Int_t           mcChannelNumber;
   Long64_t        eventNumber;
   Int_t           lumiBlock;
   Double_t        mu;
   Bool_t          isMC;
   Double_t        weight;
   Int_t           njet;
   Int_t           njet_soft;
   Int_t           njet_ak4;
   Int_t           nbjet_Fix70;
   Int_t           nbjet_Flt70;
   vector<double>  *jet_pt;
   vector<double>  *jet_eta;
   vector<double>  *jet_phi;
   vector<double>  *jet_m;
   vector<int>     *jet_bmatched_Fix70;
   vector<int>     *jet_bmatched_Flt70;
   Double_t        dEta;
   Double_t        HT_ak4;
   Double_t        MJ;

   // List of branches
   TBranch        *b_runNumber;   //!
   TBranch        *b_mcChannelNumber;   //!
   TBranch        *b_eventNumber;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_mu;   //!
   TBranch        *b_isMC;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_njet_soft;   //!
   TBranch        *b_njet_ak4;   //!
   TBranch        *b_nbjet_Fix70;   //!
   TBranch        *b_nbjet_Flt70;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_m;   //!
   TBranch        *b_jet_bmatched_Fix70;   //!
   TBranch        *b_jet_bmatched_Flt70;   //!
   TBranch        *b_dEta;   //!
   TBranch        *b_HT_ak4;   //!
   TBranch        *b_MJ;   //!

   miniTree(TTree *tree=0);
   virtual ~miniTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef miniTree_cxx
miniTree::miniTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("data2015/main_data2015.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("data2015/main_data2015.root");
      }
      f->GetObject("miniTree",tree);

   }
   Init(tree);
}

miniTree::~miniTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t miniTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t miniTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void miniTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_m = 0;
   jet_bmatched_Fix70 = 0;
   jet_bmatched_Flt70 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runNumber", &runNumber, &b_runNumber);
   fChain->SetBranchAddress("mcChannelNumber", &mcChannelNumber, &b_mcChannelNumber);
   fChain->SetBranchAddress("eventNumber", &eventNumber, &b_eventNumber);
   fChain->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock);
   fChain->SetBranchAddress("mu", &mu, &b_mu);
   fChain->SetBranchAddress("isMC", &isMC, &b_isMC);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("njet_soft", &njet_soft, &b_njet_soft);
   fChain->SetBranchAddress("njet_ak4", &njet_ak4, &b_njet_ak4);
   fChain->SetBranchAddress("nbjet_Fix70", &nbjet_Fix70, &b_nbjet_Fix70);
   fChain->SetBranchAddress("nbjet_Flt70", &nbjet_Flt70, &b_nbjet_Flt70);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_m", &jet_m, &b_jet_m);
   fChain->SetBranchAddress("jet_bmatched_Fix70", &jet_bmatched_Fix70, &b_jet_bmatched_Fix70);
   fChain->SetBranchAddress("jet_bmatched_Flt70", &jet_bmatched_Flt70, &b_jet_bmatched_Flt70);
   fChain->SetBranchAddress("dEta", &dEta, &b_dEta);
   fChain->SetBranchAddress("HT_ak4", &HT_ak4, &b_HT_ak4);
   fChain->SetBranchAddress("MJ", &MJ, &b_MJ);
   Notify();
}

Bool_t miniTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void miniTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t miniTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef miniTree_cxx
