// std includes
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// ROOT includes
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>

// local includes
#include "PrescaleAnalysis.hh"

PrescaleAnalysis::PrescaleAnalysis(TTree *tree) : PrescaleBase(tree) {
}

PrescaleAnalysis::~PrescaleAnalysis(){
}

void PrescaleAnalysis::CreateTriggerTreeFile(char *output_file){
	
  s_f_out = output_file;
	
  InitTriggerTree((char*)"physics");
  int loop_success = TriggerTreeLoop();
  if(loop_success < 0){
    cout << "problem with event loop - not doing anything" << endl;
    return;
  }
  WriteTriggerTree();
	
  /*
    int write_success = WRITE_OUTPUT();
    if(write_success < 0){
    cout << "problem PDF output - not doing anything" << endl;
    return;
    }
  */
}

int PrescaleAnalysis::TriggerTreeLoop() {
  if(fChain == 0){
    cout << "NULL pointer to tree - not doing anything" << endl;
    return -1;
  }
  
  Long64_t nbytes = 0;
  Long64_t nb = 0;
  Long64_t nentries = fChain->GetEntries();
  cout << "Number of entries = " << nentries << endl;
	
  //Event loop
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    ClearEventTriggerTree();
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%10000 == 0)
      cout << ">>> Processing event # " << jentry << " of " << nentries << endl;

    //Fill array with jets
    Fill_jet();
  
    //Fill MET xe
    Fill_xe();

    //Fill EF variables
    Fill_Var();

    FillTriggerTree();
    
  }

  return 1;
}

void PrescaleAnalysis::Fill_xe(){
  xe_pt = met*1000.;
  xe_eta = 0.0;
  xe_phi = metphi;
}

void PrescaleAnalysis::Fill_jet(){
  jet_n = 0;
  if(jet1Pt > 0.0){
    jet_pt[0] = jet1Pt*1000.;
    jet_eta[0] = jet1Eta*1000.;
    jet_n++;
  } else {
    return;
  }
  if(jet2Pt > 0.0){
    jet_pt[1] = jet2Pt*1000.;
    jet_eta[1] = jet2Eta*1000.;
    jet_n++;
  } else {
    return;
  }
  if(jet3Pt > 0.0){
    jet_pt[2] = jet3Pt*1000.;
    jet_eta[2] = jet3Eta*1000.;
    jet_n++;
  } else {
    return;
  }
  if(jet4Pt > 0.0){
    jet_pt[3] = jet4Pt*1000.;
    jet_eta[3] = jet4Eta*1000.;
    jet_n++;
  } else {
    return;
  }
  if(jet5Pt > 0.0){
    jet_pt[4] = jet5Pt*1000.;
    jet_eta[4] = jet5Eta*1000.;
    jet_n++;
  } else {
    return;
  }
  if(jet6Pt > 0.0){
    jet_pt[5] = jet6Pt*1000.;
    jet_eta[5] = jet6Eta*1000.;
    jet_n++;
  } else {
    return;
  }
  if(jet7Pt > 0.0){
    jet_pt[6] = jet7Pt*1000.;
    jet_eta[6] = jet7Eta*1000.;
    jet_n++;
  } else {
    return;
  }
}

void PrescaleAnalysis::Fill_Var(){
  shatR *= 1000.;
  cosptR *= 1000.;
  mdeltaR *= 1000.;
  gaminvR *= 1000.;
  weight = eventWeight;
}

void PrescaleAnalysis::ClearEventTriggerTree(){
  
  //reset all fields for Trigger Tree
  xe_pt = 0;
  xe_eta = 0;
  xe_phi = 0;

  jet_n = 0;
  for(int ijet = 0; ijet < Njet_max; ijet++){
    jet_pt[ijet] = 0.0;
    jet_eta[ijet] = 0.0;
    jet_phi[ijet] = 0.0;
    jet_MV1[ijet] = 0.0;
  }
  weight = 0.0;
}

void PrescaleAnalysis::FillTriggerTree(){
  T_trigger->Fill();
}

void PrescaleAnalysis::WriteTriggerTree(){
  TFile *fout = new TFile(s_f_out,"RECREATE");
  fout->cd();
  T_trigger->Write();
  fout->Close();

  T_trigger->Delete();
  fout->Close();
}

void PrescaleAnalysis::InitTriggerTree(char *sname){
  T_trigger = (TTree*) new TTree(sname,sname);

  char *sbranch = new char[200];

  //xe fields
  T_trigger->Branch("xe_pt",&xe_pt);
  T_trigger->Branch("xe_eta",&xe_eta);
  T_trigger->Branch("xe_phi",&xe_phi);

  //jet fields
  T_trigger->Branch("jet_n",&jet_n);
  sprintf(sbranch,"jet_pt[%d]/F",Njet_max);
  T_trigger->Branch("jet_pt",jet_pt,sbranch);
  sprintf(sbranch,"jet_eta[%d]/F",Njet_max);
  T_trigger->Branch("jet_eta",jet_eta,sbranch);
  sprintf(sbranch,"jet_phi[%d]/F",Njet_max);
  T_trigger->Branch("jet_phi",jet_phi,sbranch);
  sprintf(sbranch,"jet_MV1[%d]/F",Njet_max);
  T_trigger->Branch("jet_MV1",jet_MV1,sbranch);

  //add Variable branches to trigger tree
  T_trigger->Branch("shatR", &shatR);
  T_trigger->Branch("cosptR", &cosptR);
  T_trigger->Branch("mdeltaR", &mdeltaR);
  T_trigger->Branch("gaminvR", &gaminvR);
  T_trigger->Branch("weight", &weight);
}
  

