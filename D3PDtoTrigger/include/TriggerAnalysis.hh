//-------------------------------------------------------
// Description:
//    Class for D3PDAnalysis analyses
// Authors:
//
//-------------------------------------------------------

/// The TriggerAnalysis class can be used to analyze D3PDs


#ifndef TriggerAnalysis_h
#define TriggerAnalysis_h

#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include <RooFit.h>
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooKeysPdf.h"
#include "RooNDKeysPdf.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "mt2_bisect.hh"
#include "TriggerBase.hh"

using namespace std;
using namespace RooFit;

class TriggerAnalysis : public TriggerBase{
public:

  TriggerAnalysis(char *sfile); /// Class Constructor
  virtual ~TriggerAnalysis();     /// Class Destructor
  /// The function to run on each events
  
  //Loop to add HEM info
  void AddHemLoop();
  //Loop to add Variable info
  void AddVarLoop();
  //Write the updated tree to new output file
  void WriteUpdatedTree();
	
private:
	
  char *s_f_out;
  //utility functions for doing stuff
  vector<TLorentzVector> AddVectors(vector<TLorentzVector> V1, vector<TLorentzVector> V2);
  vector<TLorentzVector> GetHemi(vector<TLorentzVector> myjets);
  vector<TLorentzVector> GetJets(float pt_cut, float eta_cut);
  vector<TLorentzVector> GetFwdJets(float pt_cut, float eta_cut);
  vector<TLorentzVector> GetMuons(float pt_cut, float eta_cut);
  TVector3 GetMET();

  //info for adding Variable information to trigger tree
  void AddVarBranch();
  void InitHemBranch();
  void FillVarBranch();
  float shatR;
  float cosptR;
  float mdeltaR;
  float gaminvR;
  float dphivbR;
  float ptcm;
  TBranch *b_shatR;
  TBranch *b_cosptR;
  TBranch *b_mdeltaR;
  TBranch *b_gaminvR;
  TBranch *b_dphivbR;
  TBranch *b_ptcm;


  //info for adding Hem branches to trigger tree
  void AddHemBranch();
  void FillHemBranch();
  float HEM_pt[2];
  float HEM_eta[2];
  float HEM_phi[2];
  float HEM_M[2];
  TBranch *b_HEM_pt;
  TBranch *b_HEM_eta;
  TBranch *b_HEM_phi;
  TBranch *b_HEM_M;
  void AddHempfwdBranch();
  void FillHempfwdBranch();
  float HEMpfwd_pt[2];
  float HEMpfwd_eta[2];
  float HEMpfwd_phi[2];
  float HEMpfwd_M[2];
  TBranch *b_HEMpfwd_pt;
  TBranch *b_HEMpfwd_eta;
  TBranch *b_HEMpfwd_phi;
  TBranch *b_HEMpfwd_M;
  void AddHempmuBranch();
  void FillHempmuBranch();
  float HEMpmu_pt[2];
  float HEMpmu_eta[2];
  float HEMpmu_phi[2];
  float HEMpmu_M[2];
  TBranch *b_HEMpmu_pt;
  TBranch *b_HEMpmu_eta;
  TBranch *b_HEMpmu_phi;
  TBranch *b_HEMpmu_M;
  void AddHempfwdpmuBranch();
  void FillHempfwdpmuBranch();
  float HEMpfwdpmu_pt[2];
  float HEMpfwdpmu_eta[2];
  float HEMpfwdpmu_phi[2];
  float HEMpfwdpmu_M[2];
  TBranch *b_HEMpfwdpmu_pt;
  TBranch *b_HEMpfwdpmu_eta;
  TBranch *b_HEMpfwdpmu_phi;
  TBranch *b_HEMpfwdpmu_M;
 

};
#endif
