//-------------------------------------------------------
// Description:
//    Class for D3PDAnalysis analyses
// Authors:
//
//-------------------------------------------------------

/// The PrescaleAnalysis class can be used to analyze D3PDs


#ifndef PrescaleAnalysis_h
#define PrescaleAnalysis_h

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
#include "PrescaleBase.hh"

using namespace std;
using namespace RooFit;

class PrescaleAnalysis : public PrescaleBase{
public:

  PrescaleAnalysis(TTree *tree); /// Class Constructor
  virtual ~PrescaleAnalysis();     /// Class Destructor
  /// The function to run on each events
  
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

  //MET_LocalHadTopo
  void Fill_xe();
  float xe_pt;
  float xe_eta;
  float xe_phi;

  //info for adding Variable information to trigger tree
  void Fill_Var();

  float weight;
  TBranch *b_weight;

};
#endif
