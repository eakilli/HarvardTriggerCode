//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Jul 12 02:17:19 2014 by ROOT version 5.34/17
// from TTree susy/susy
// found on file: JetTauEtmiss00200982.ntuple.root
//////////////////////////////////////////////////////////

#ifndef PrescaleBase_h
#define PrescaleBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class PrescaleBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         EventNumber;
   Float_t         RunNumber;
   Float_t         nJet;
   Float_t         nJet25;
   Float_t         nJet30;
   Float_t         nJet40;
   Float_t         nJet50;
   Float_t         nBJet25;
   Float_t         nBJet30;
   Float_t         nBJet40;
   Float_t         nBJet50;
   Float_t         met;
   Float_t         metFix;
   Float_t         metST;
   Float_t         metMU;
   Float_t         metphi;
   Float_t         metFixphi;
   Float_t         metSTphi;
   Float_t         metMUphi;
   Float_t         HT;
   Float_t         jet1Pt;
   Float_t         jet2Pt;
   Float_t         jet3Pt;
   Float_t         jet4Pt;
   Float_t         jet5Pt;
   Float_t         jet6Pt;
   Float_t         jet7Pt;
   Float_t         jet1Eta;
   Float_t         jet2Eta;
   Float_t         jet3Eta;
   Float_t         jet4Eta;
   Float_t         jet5Eta;
   Float_t         jet6Eta;
   Float_t         jet7Eta;
   Float_t         shatR;
   Float_t         gaminvR;
   Float_t         mdeltaR;
   Float_t         cosptR;
   Float_t         PresBin;
   Float_t         eventWeight;
   Float_t         AnalysisTrig;

   // List of branches
   TBranch        *b_EventNumber;   //!
   TBranch        *b_RunNumber;   //!
   TBranch        *b_nJet;   //!
   TBranch        *b_nJet25;   //!
   TBranch        *b_nJet30;   //!
   TBranch        *b_nJet40;   //!
   TBranch        *b_nJet50;   //!
   TBranch        *b_nBJet25;   //!
   TBranch        *b_nBJet30;   //!
   TBranch        *b_nBJet40;   //!
   TBranch        *b_nBJet50;   //!
   TBranch        *b_met;   //!
   TBranch        *b_metFix;   //!
   TBranch        *b_metST;   //!
   TBranch        *b_metMU;   //!
   TBranch        *b_metphi;   //!
   TBranch        *b_metFixphi;   //!
   TBranch        *b_metSTphi;   //!
   TBranch        *b_metMUphi;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_jet1Pt;   //!
   TBranch        *b_jet2Pt;   //!
   TBranch        *b_jet3Pt;   //!
   TBranch        *b_jet4Pt;   //!
   TBranch        *b_jet5Pt;   //!
   TBranch        *b_jet6Pt;   //!
   TBranch        *b_jet7Pt;   //!
   TBranch        *b_jet1Eta;   //!
   TBranch        *b_jet2Eta;   //!
   TBranch        *b_jet3Eta;   //!
   TBranch        *b_jet4Eta;   //!
   TBranch        *b_jet5Eta;   //!
   TBranch        *b_jet6Eta;   //!
   TBranch        *b_jet7Eta;   //!
   TBranch        *b_shatR;   //!
   TBranch        *b_gaminvR;   //!
   TBranch        *b_mdeltaR;   //!
   TBranch        *b_cosptR;   //!
   TBranch        *b_PresBin;   //!
   TBranch        *b_eventWeight;   //!
   TBranch        *b_AnalysisTrig;   //!

   PrescaleBase(TTree *tree=0);
   virtual ~PrescaleBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef PrescaleBase_cxx
PrescaleBase::PrescaleBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("JetTauEtmiss00200982.ntuple.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("JetTauEtmiss00200982.ntuple.root");
      }
      f->GetObject("susy",tree);

   }
   Init(tree);
}

PrescaleBase::~PrescaleBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t PrescaleBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PrescaleBase::LoadTree(Long64_t entry)
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

void PrescaleBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("nJet", &nJet, &b_nJet);
   fChain->SetBranchAddress("nJet25", &nJet25, &b_nJet25);
   fChain->SetBranchAddress("nJet30", &nJet30, &b_nJet30);
   fChain->SetBranchAddress("nJet40", &nJet40, &b_nJet40);
   fChain->SetBranchAddress("nJet50", &nJet50, &b_nJet50);
   fChain->SetBranchAddress("nBJet25", &nBJet25, &b_nBJet25);
   fChain->SetBranchAddress("nBJet30", &nBJet30, &b_nBJet30);
   fChain->SetBranchAddress("nBJet40", &nBJet40, &b_nBJet40);
   fChain->SetBranchAddress("nBJet50", &nBJet50, &b_nBJet50);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("metFix", &metFix, &b_metFix);
   fChain->SetBranchAddress("metST", &metST, &b_metST);
   fChain->SetBranchAddress("metMU", &metMU, &b_metMU);
   fChain->SetBranchAddress("metphi", &metphi, &b_metphi);
   fChain->SetBranchAddress("metFixphi", &metFixphi, &b_metFixphi);
   fChain->SetBranchAddress("metSTphi", &metSTphi, &b_metSTphi);
   fChain->SetBranchAddress("metMUphi", &metMUphi, &b_metMUphi);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("jet1Pt", &jet1Pt, &b_jet1Pt);
   fChain->SetBranchAddress("jet2Pt", &jet2Pt, &b_jet2Pt);
   fChain->SetBranchAddress("jet3Pt", &jet3Pt, &b_jet3Pt);
   fChain->SetBranchAddress("jet4Pt", &jet4Pt, &b_jet4Pt);
   fChain->SetBranchAddress("jet5Pt", &jet5Pt, &b_jet5Pt);
   fChain->SetBranchAddress("jet6Pt", &jet6Pt, &b_jet6Pt);
   fChain->SetBranchAddress("jet7Pt", &jet7Pt, &b_jet7Pt);
   fChain->SetBranchAddress("jet1Eta", &jet1Eta, &b_jet1Eta);
   fChain->SetBranchAddress("jet2Eta", &jet2Eta, &b_jet2Eta);
   fChain->SetBranchAddress("jet3Eta", &jet3Eta, &b_jet3Eta);
   fChain->SetBranchAddress("jet4Eta", &jet4Eta, &b_jet4Eta);
   fChain->SetBranchAddress("jet5Eta", &jet5Eta, &b_jet5Eta);
   fChain->SetBranchAddress("jet6Eta", &jet6Eta, &b_jet6Eta);
   fChain->SetBranchAddress("jet7Eta", &jet7Eta, &b_jet7Eta);
   fChain->SetBranchAddress("shatR", &shatR, &b_shatR);
   fChain->SetBranchAddress("gaminvR", &gaminvR, &b_gaminvR);
   fChain->SetBranchAddress("mdeltaR", &mdeltaR, &b_mdeltaR);
   fChain->SetBranchAddress("cosptR", &cosptR, &b_cosptR);
   fChain->SetBranchAddress("PresBin", &PresBin, &b_PresBin);
   fChain->SetBranchAddress("eventWeight", &eventWeight, &b_eventWeight);
   fChain->SetBranchAddress("AnalysisTrig", &AnalysisTrig, &b_AnalysisTrig);
   Notify();
}

Bool_t PrescaleBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void PrescaleBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PrescaleBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef PrescaleBase_cxx
