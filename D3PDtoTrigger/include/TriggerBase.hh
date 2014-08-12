//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Apr 26 15:07:59 2014 by ROOT version 5.34/17
// from TTree physics/physics
// found on file: GG_direct_1125_225.root
//////////////////////////////////////////////////////////

#ifndef TriggerBase_h
#define TriggerBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class TriggerBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          lbn;
   Float_t         xe_pt;
   Float_t         xe_eta;
   Float_t         xe_phi;
   Float_t         xewmu_pt;
   Float_t         xewmu_eta;
   Float_t         xewmu_phi;
   Int_t           tauloose_n;
   Float_t         tauloose_pt[5];
   Float_t         tauloose_eta[5];
   Float_t         tauloose_phi[5];
   Int_t           taumedium_n;
   Float_t         taumedium_pt[5];
   Float_t         taumedium_eta[5];
   Float_t         taumedium_phi[5];
   Int_t           tautight_n;
   Float_t         tautight_pt[5];
   Float_t         tautight_eta[5];
   Float_t         tautight_phi[5];
   Int_t           mu_n;
   Float_t         mu_pt[5];
   Float_t         mu_eta[5];
   Float_t         mu_phi[5];
   Int_t           mui_n;
   Float_t         mui_pt[5];
   Float_t         mui_eta[5];
   Float_t         mui_phi[5];
   Int_t           eloose_n;
   Float_t         eloose_pt[5];
   Float_t         eloose_eta[5];
   Float_t         eloose_phi[5];
   Int_t           emedium_n;
   Float_t         emedium_pt[5];
   Float_t         emedium_eta[5];
   Float_t         emedium_phi[5];
   Int_t           etight_n;
   Float_t         etight_pt[5];
   Float_t         etight_eta[5];
   Float_t         etight_phi[5];
   Int_t           eiloose_n;
   Float_t         eiloose_pt[5];
   Float_t         eiloose_eta[5];
   Float_t         eiloose_phi[5];
   Int_t           eimedium_n;
   Float_t         eimedium_pt[5];
   Float_t         eimedium_eta[5];
   Float_t         eimedium_phi[5];
   Int_t           eitight_n;
   Float_t         eitight_pt[5];
   Float_t         eitight_eta[5];
   Float_t         eitight_phi[5];
   Int_t           jet_n;
   Float_t         jet_pt[20];
   Float_t         jet_eta[20];
   Float_t         jet_phi[20];
   Float_t         jet_MV1[20];
   Int_t           fwdjet_n;
   Float_t         fwdjet_pt[20];
   Float_t         fwdjet_eta[20];
   Float_t         fwdjet_phi[20];
   Int_t           JET_n;
   Float_t         JET_pt[20];
   Float_t         JET_eta[20];
   Float_t         JET_phi[20];

   // List of branches
   TBranch        *b_lbn;   //!
   TBranch        *b_xe_pt;   //!
   TBranch        *b_xe_eta;   //!
   TBranch        *b_xe_phi;   //!
   TBranch        *b_xewmu_pt;   //!
   TBranch        *b_xewmu_eta;   //!
   TBranch        *b_xewmu_phi;   //!
   TBranch        *b_tauloose_n;   //!
   TBranch        *b_tauloose_pt;   //!
   TBranch        *b_tauloose_eta;   //!
   TBranch        *b_tauloose_phi;   //!
   TBranch        *b_taumedium_n;   //!
   TBranch        *b_taumedium_pt;   //!
   TBranch        *b_taumedium_eta;   //!
   TBranch        *b_taumedium_phi;   //!
   TBranch        *b_tautight_n;   //!
   TBranch        *b_tautight_pt;   //!
   TBranch        *b_tautight_eta;   //!
   TBranch        *b_tautight_phi;   //!
   TBranch        *b_mu_n;   //!
   TBranch        *b_mu_pt;   //!
   TBranch        *b_mu_eta;   //!
   TBranch        *b_mu_phi;   //!
   TBranch        *b_mui_n;   //!
   TBranch        *b_mui_pt;   //!
   TBranch        *b_mui_eta;   //!
   TBranch        *b_mui_phi;   //!
   TBranch        *b_eloose_n;   //!
   TBranch        *b_eloose_pt;   //!
   TBranch        *b_eloose_eta;   //!
   TBranch        *b_eloose_phi;   //!
   TBranch        *b_emedium_n;   //!
   TBranch        *b_emedium_pt;   //!
   TBranch        *b_emedium_eta;   //!
   TBranch        *b_emedium_phi;   //!
   TBranch        *b_etight_n;   //!
   TBranch        *b_etight_pt;   //!
   TBranch        *b_etight_eta;   //!
   TBranch        *b_etight_phi;   //!
   TBranch        *b_eiloose_n;   //!
   TBranch        *b_eiloose_pt;   //!
   TBranch        *b_eiloose_eta;   //!
   TBranch        *b_eiloose_phi;   //!
   TBranch        *b_eimedium_n;   //!
   TBranch        *b_eimedium_pt;   //!
   TBranch        *b_eimedium_eta;   //!
   TBranch        *b_eimedium_phi;   //!
   TBranch        *b_eitight_n;   //!
   TBranch        *b_eitight_pt;   //!
   TBranch        *b_eitight_eta;   //!
   TBranch        *b_eitight_phi;   //!
   TBranch        *b_jet_n;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_MV1;   //!
   TBranch        *b_fwdjet_n;   //!
   TBranch        *b_fwdjet_pt;   //!
   TBranch        *b_fwdjet_eta;   //!
   TBranch        *b_fwdjet_phi;   //!
   TBranch        *b_JET_n;   //!
   TBranch        *b_JET_pt;   //!
   TBranch        *b_JET_eta;   //!
   TBranch        *b_JET_phi;   //!

   TriggerBase(char *sfile);
   virtual ~TriggerBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init();
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

  TFile *f;
};

#endif

#ifdef TriggerBase_cxx
TriggerBase::TriggerBase(char *sfile) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  f = (TFile*) new TFile(sfile,"UPDATE");
  fChain = (TTree*) f->Get("physics");
   Init();
}

TriggerBase::~TriggerBase()
{
   
  if (!fChain) return;
  delete fChain;
  f->Close();
}

Int_t TriggerBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TriggerBase::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
 
   return centry;
}

void TriggerBase::Init()
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!fChain) return;
   // fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("lbn", &lbn, &b_lbn);
   fChain->SetBranchAddress("xe_pt", &xe_pt, &b_xe_pt);
   fChain->SetBranchAddress("xe_eta", &xe_eta, &b_xe_eta);
   fChain->SetBranchAddress("xe_phi", &xe_phi, &b_xe_phi);
   fChain->SetBranchAddress("xewmu_pt", &xewmu_pt, &b_xewmu_pt);
   fChain->SetBranchAddress("xewmu_eta", &xewmu_eta, &b_xewmu_eta);
   fChain->SetBranchAddress("xewmu_phi", &xewmu_phi, &b_xewmu_phi);
   fChain->SetBranchAddress("tauloose_n", &tauloose_n, &b_tauloose_n);
   fChain->SetBranchAddress("tauloose_pt", tauloose_pt, &b_tauloose_pt);
   fChain->SetBranchAddress("tauloose_eta", tauloose_eta, &b_tauloose_eta);
   fChain->SetBranchAddress("tauloose_phi", tauloose_phi, &b_tauloose_phi);
   fChain->SetBranchAddress("taumedium_n", &taumedium_n, &b_taumedium_n);
   fChain->SetBranchAddress("taumedium_pt", taumedium_pt, &b_taumedium_pt);
   fChain->SetBranchAddress("taumedium_eta", taumedium_eta, &b_taumedium_eta);
   fChain->SetBranchAddress("taumedium_phi", taumedium_phi, &b_taumedium_phi);
   fChain->SetBranchAddress("tautight_n", &tautight_n, &b_tautight_n);
   fChain->SetBranchAddress("tautight_pt", tautight_pt, &b_tautight_pt);
   fChain->SetBranchAddress("tautight_eta", tautight_eta, &b_tautight_eta);
   fChain->SetBranchAddress("tautight_phi", tautight_phi, &b_tautight_phi);
   fChain->SetBranchAddress("mu_n", &mu_n, &b_mu_n);
   fChain->SetBranchAddress("mu_pt", mu_pt, &b_mu_pt);
   fChain->SetBranchAddress("mu_eta", mu_eta, &b_mu_eta);
   fChain->SetBranchAddress("mu_phi", mu_phi, &b_mu_phi);
   fChain->SetBranchAddress("mui_n", &mui_n, &b_mui_n);
   fChain->SetBranchAddress("mui_pt", mui_pt, &b_mui_pt);
   fChain->SetBranchAddress("mui_eta", mui_eta, &b_mui_eta);
   fChain->SetBranchAddress("mui_phi", mui_phi, &b_mui_phi);
   fChain->SetBranchAddress("eloose_n", &eloose_n, &b_eloose_n);
   fChain->SetBranchAddress("eloose_pt", eloose_pt, &b_eloose_pt);
   fChain->SetBranchAddress("eloose_eta", eloose_eta, &b_eloose_eta);
   fChain->SetBranchAddress("eloose_phi", eloose_phi, &b_eloose_phi);
   fChain->SetBranchAddress("emedium_n", &emedium_n, &b_emedium_n);
   fChain->SetBranchAddress("emedium_pt", emedium_pt, &b_emedium_pt);
   fChain->SetBranchAddress("emedium_eta", emedium_eta, &b_emedium_eta);
   fChain->SetBranchAddress("emedium_phi", emedium_phi, &b_emedium_phi);
   fChain->SetBranchAddress("etight_n", &etight_n, &b_etight_n);
   fChain->SetBranchAddress("etight_pt", etight_pt, &b_etight_pt);
   fChain->SetBranchAddress("etight_eta", etight_eta, &b_etight_eta);
   fChain->SetBranchAddress("etight_phi", etight_phi, &b_etight_phi);
   fChain->SetBranchAddress("eiloose_n", &eiloose_n, &b_eiloose_n);
   fChain->SetBranchAddress("eiloose_pt", eiloose_pt, &b_eiloose_pt);
   fChain->SetBranchAddress("eiloose_eta", eiloose_eta, &b_eiloose_eta);
   fChain->SetBranchAddress("eiloose_phi", eiloose_phi, &b_eiloose_phi);
   fChain->SetBranchAddress("eimedium_n", &eimedium_n, &b_eimedium_n);
   fChain->SetBranchAddress("eimedium_pt", eimedium_pt, &b_eimedium_pt);
   fChain->SetBranchAddress("eimedium_eta", eimedium_eta, &b_eimedium_eta);
   fChain->SetBranchAddress("eimedium_phi", eimedium_phi, &b_eimedium_phi);
   fChain->SetBranchAddress("eitight_n", &eitight_n, &b_eitight_n);
   fChain->SetBranchAddress("eitight_pt", eitight_pt, &b_eitight_pt);
   fChain->SetBranchAddress("eitight_eta", eitight_eta, &b_eitight_eta);
   fChain->SetBranchAddress("eitight_phi", eitight_phi, &b_eitight_phi);
   fChain->SetBranchAddress("jet_n", &jet_n, &b_jet_n);
   fChain->SetBranchAddress("jet_pt", jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_MV1", jet_MV1, &b_jet_MV1);
   fChain->SetBranchAddress("fwdjet_n", &fwdjet_n, &b_fwdjet_n);
   fChain->SetBranchAddress("fwdjet_pt", fwdjet_pt, &b_fwdjet_pt);
   fChain->SetBranchAddress("fwdjet_eta", fwdjet_eta, &b_fwdjet_eta);
   fChain->SetBranchAddress("fwdjet_phi", fwdjet_phi, &b_fwdjet_phi);
   fChain->SetBranchAddress("JET_n", &JET_n, &b_JET_n);
   fChain->SetBranchAddress("JET_pt", JET_pt, &b_JET_pt);
   fChain->SetBranchAddress("JET_eta", JET_eta, &b_JET_eta);
   fChain->SetBranchAddress("JET_phi", JET_phi, &b_JET_phi);
   Notify();

   for(int i = 0; i < 5; i++){
    tauloose_pt[i] = 0;
    tauloose_eta[i] = 0;
    tauloose_phi[i] = 0;
    taumedium_pt[i] = 0;
    taumedium_eta[i] = 0;
    taumedium_phi[i] = 0;
    tautight_pt[i] = 0;
    tautight_eta[i] = 0;
    tautight_phi[i] = 0;
    mu_pt[i] = 0;
    mu_eta[i] = 0;
    mu_phi[i] = 0;
    mui_pt[i] = 0;
    mui_eta[i] = 0;
    mui_phi[i] = 0;
    eloose_pt[i] = 0;
    eloose_eta[i] = 0;
    eloose_phi[i] = 0;
    emedium_pt[i] = 0;
    emedium_eta[i] = 0;
    emedium_phi[i] = 0;
    etight_eta[i] = 0;
    etight_phi[i] = 0;
    eiloose_pt[i] = 0;
    eiloose_eta[i] = 0;
    eiloose_phi[i] = 0;
    eimedium_pt[i] = 0;
    eimedium_eta[i] = 0;
    eimedium_phi[i] = 0;
    eitight_pt[i] = 0;
    eitight_eta[i] = 0;
    eitight_phi[i] = 0;
  }
  for(int i = 0; i < 20; i++){
    jet_pt[i] = 0;
    jet_eta[i] = 0;
    jet_phi[i] = 0;
    jet_MV1[i] = 0;
    fwdjet_pt[i] = 0;
    fwdjet_eta[i] = 0;
    fwdjet_phi[i] = 0;
    JET_pt[i] = 0;
    JET_eta[i] = 0;
    JET_phi[i] = 0;
  }
}

Bool_t TriggerBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TriggerBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TriggerBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TriggerBase_cxx
