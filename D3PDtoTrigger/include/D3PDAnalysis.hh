//-------------------------------------------------------
// Description:
//    Class for D3PDAnalysis analyses
// Authors:
//
//-------------------------------------------------------

/// The D3PDAnalysis class can be used to analyze D3PDs


#ifndef D3PDAnalysis_h
#define D3PDAnalysis_h

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
#include "NTUP_SUSYBase.hh"

using namespace std;
using namespace RooFit;

class D3PDAnalysis : public NTUP_SUSYBase{
public:

	D3PDAnalysis(TTree *tree=0); /// Class Constructor
	virtual ~D3PDAnalysis();     /// Class Destructor
	/// The function to run on each events
  
	// Build the PDFs for the analyses listed in ANA;
	void CreateTriggerTreeFile(char *output_file);
	
private:
	
	char *s_f_out;

	int TriggerTreeLoop();

  void InitTriggerTree(char *sname);
  void WriteTriggerTree();
  void FillTriggerTree();
  void ClearEventTriggerTree();

  //things for TriggerTree
  TTree *T_trigger;
  
  //jet_AntiKt4LCTopo pt > 10, |eta| < 3.2
  void Fill_jet();
  static const int Njet_max = 20;
  int jet_n;
  float jet_pt[Njet_max];
  float jet_eta[Njet_max];
  float jet_phi[Njet_max];
  float jet_MV1[Njet_max];

  //jet_AntiKt4LCTopo pt > 10, 3.2 <= |eta| < 4.4
  void Fill_fwdjet();
  static const int Nfwdjet_max = 20;
  int fwdjet_n;
  float fwdjet_pt[Nfwdjet_max];
  float fwdjet_eta[Nfwdjet_max];
  float fwdjet_phi[Nfwdjet_max];

  //trig_L1_jet pt > 10, |eta| < 3.
  void Fill_JET();
  static const int NJET_max = 20;
  int JET_n;
  float JET_pt[NJET_max];
  float JET_eta[NJET_max];
  float JET_phi[NJET_max];

  //el loosePP pt > 5, |eta| < 2.47
  void Fill_eloose();
  static const int Neloose_max = 5;
  int eloose_n;
  float eloose_pt[Neloose_max];
  float eloose_eta[Neloose_max];
  float eloose_phi[Neloose_max];

  //el loosePP pt > 5, |eta| < 2.47, isolation
  void Fill_eiloose();
  static const int Neiloose_max = 5;
  int eiloose_n;
  float eiloose_pt[Neiloose_max];
  float eiloose_eta[Neiloose_max];
  float eiloose_phi[Neiloose_max];

  //el mediumPP pt > 5, |eta| < 2.47
  void Fill_emedium();
  static const int Nemedium_max = 5;
  int emedium_n;
  float emedium_pt[Nemedium_max];
  float emedium_eta[Nemedium_max];
  float emedium_phi[Nemedium_max];

  //el mediumPP pt > 5, |eta| < 2.47, isolation
  void Fill_eimedium();
  static const int Neimedium_max = 5;
  int eimedium_n;
  float eimedium_pt[Neimedium_max];
  float eimedium_eta[Neimedium_max];
  float eimedium_phi[Neimedium_max];

//el tightPP pt > 5, |eta| < 2.47
  void Fill_etight();
  static const int Netight_max = 5;
  int etight_n;
  float etight_pt[Netight_max];
  float etight_eta[Netight_max];
  float etight_phi[Netight_max];

  //el tightPP pt > 5, |eta| < 2.47, isolation
  void Fill_eitight();
  static const int Neitight_max = 5;
  int eitight_n;
  float eitight_pt[Neitight_max];
  float eitight_eta[Neitight_max];
  float eitight_phi[Neitight_max];

  //mu_staco loose pt > 5, |eta| < 2.4
  void Fill_mu();
  static const int Nmu_max = 5;
  int mu_n;
  float mu_pt[Nmu_max];
  float mu_eta[Nmu_max];
  float mu_phi[Nmu_max];

  //mu_staco loose pt > 5, |eta| < 2.4, isolation
  void Fill_mui();
  static const int Nmui_max = 5;
  int mui_n;
  float mui_pt[Nmui_max];
  float mui_eta[Nmui_max];
  float mui_phi[Nmui_max];
  
  //tau_ loose pt > 10, |eta| < 2.47
  void Fill_tauloose();
  static const int Ntauloose_max = 5;
  int tauloose_n;
  float tauloose_pt[Ntauloose_max];
  float tauloose_eta[Ntauloose_max];
  float tauloose_phi[Ntauloose_max];

  //tau_ medium pt > 10, |eta| < 2.47
  void Fill_taumedium();
  static const int Ntaumedium_max = 5;
  int taumedium_n;
  float taumedium_pt[Ntaumedium_max];
  float taumedium_eta[Ntaumedium_max];
  float taumedium_phi[Ntaumedium_max];

  //tau_ tight pt > 10, |eta| < 2.47
  void Fill_tautight();
  static const int Ntautight_max = 5;
  int tautight_n;
  float tautight_pt[Ntautight_max];
  float tautight_eta[Ntautight_max];
  float tautight_phi[Ntautight_max];

  //MET_RefFinal
  void Fill_xewmu();
  float xewmu_pt;
  float xewmu_eta;
  float xewmu_phi;

  //MET_LocalHadTopo
  void Fill_xe();
  float xe_pt;
  float xe_eta;
  float xe_phi;

  //L1 MET - hacked for format without branch
  void Fill_XE();
  float XE_pt;
  float XE_eta;
  float XE_phi;

  //Lead parton pT
  void Fill_LeadPT();
  float LeadPt;

};
#endif
