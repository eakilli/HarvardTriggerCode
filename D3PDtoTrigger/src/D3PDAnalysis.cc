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
#include "D3PDAnalysis.hh"

D3PDAnalysis::D3PDAnalysis(TTree *tree) : NTUP_SUSYBase(tree) {
  

}

D3PDAnalysis::~D3PDAnalysis(){
}

void D3PDAnalysis::CreateTriggerTreeFile(char *output_file){
	
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

int D3PDAnalysis::TriggerTreeLoop() {
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
    //if there are less than two jets in the event, continue
    //if(jet_n < 2) continue;
  
    //Fill array with fwdjets
    Fill_fwdjet();

    //Fill arrays for loose electrons
    Fill_eloose();

    //Fill arrays for medium electrons
    Fill_emedium();

    //Fill arrays for tight electrons
    Fill_etight();

    //Fill arrays for loose electrons, w/ isolation
    Fill_eiloose();

    //Fill arrays for medium electrons, w/ isolation
    Fill_eimedium();

    //Fill arrays for tight electrons, w/ isolation
    Fill_eitight();

    //Fill array for muons
    Fill_mu();

    //Fill array for muons, w/ isolation
    Fill_mui();

    //Fill array for tauloose
    Fill_tauloose();

    //Fill array for taumedium
    Fill_taumedium();

    //Fill array for tautight
    Fill_tautight();

    //Fill L1 jets JET
    Fill_JET();

    //Fill MET xe
    Fill_xe();

    //Fill MET xewmu
    Fill_xewmu();

    //Fill MET XE
    Fill_XE();

    // Fill lead parton pT
    Fill_LeadPT();

    FillTriggerTree();
    
  }

  return 1;
}

void D3PDAnalysis::Fill_XE(){
  XE_eta = 0.0;
  // there is L1 MET in the ntuple - use it!
  if(fabs(trig_L1_esum_ExMiss)+fabs(trig_L1_esum_EyMiss) > 0.0){
    TVector3 myMET;
    myMET.SetXYZ(trig_L1_esum_ExMiss,trig_L1_esum_EyMiss,0.0);
    XE_phi = myMET.Phi();
    XE_pt = myMET.Mag();
  } else { //there is no L1 MET... hack it!
    XE_phi = 0.0;
    XE_pt = 0.0;
    if(L1_XE20) XE_pt = 20.1;
    if(L1_XE25) XE_pt = 25.1;
    if(L1_XE30) XE_pt = 30.1;
    if(L1_XE40) XE_pt = 40.1;
    if(L1_XE50) XE_pt = 50.1;
    if(L1_XE60) XE_pt = 60.1;
    if(L1_XE70) XE_pt = 70.1;
  }
}

void D3PDAnalysis::Fill_xewmu(){
  TVector3 vMET;
  vMET.SetXYZ(MET_Egamma10NoTau_RefFinal_etx+MET_RefFinal_Egamma10NoTau_etx,MET_Egamma10NoTau_RefFinal_ety+MET_RefFinal_Egamma10NoTau_ety,0.);
  xewmu_pt = vMET.Mag();
  xewmu_eta = 0.0;
  xewmu_phi = vMET.Phi();
}

void D3PDAnalysis::Fill_xe(){
  TVector3 METtopo;
  METtopo.SetXYZ(MET_LocHadTopo_etx,MET_LocHadTopo_ety,0.0);

  xe_pt = METtopo.Mag();
  xe_eta = 0.0;
  xe_phi = METtopo.Phi();
}

void D3PDAnalysis::Fill_tauloose(){
  tauloose_n = 0;
  for(int it = 0; it < tau_n && tauloose_n < Ntauloose_max; it++){
    if( (*tau_pt)[it]/1000. > 10. && 
	fabs( (*tau_eta)[it] ) < 2.47 &&
	fabs( (*tau_leadTrack_eta)[it] ) < 2.47 &&
	((*tau_numTrack)[it] == 1 || (*tau_numTrack)[it] == 3) &&
	fabs((*tau_charge)[it]) == 1 &&
	(*tau_JetBDTSigLoose)[it] &&
	!((*tau_EleBDTLoose)[it] && ((*tau_numTrack)[it] == 1)) ){
      tauloose_pt[tauloose_n] = (*tau_pt)[it];
      tauloose_eta[tauloose_n] = (*tau_eta)[it];
      tauloose_phi[tauloose_n] = (*tau_phi)[it];
      tauloose_n++;
      
    }
  }
}

void D3PDAnalysis::Fill_taumedium(){
  taumedium_n = 0;
  for(int it = 0; it < tau_n && taumedium_n < Ntaumedium_max; it++){
    if( (*tau_pt)[it]/1000. > 10. && 
	fabs( (*tau_eta)[it] ) < 2.47 &&
	fabs( (*tau_leadTrack_eta)[it] ) < 2.47 &&
	((*tau_numTrack)[it] == 1 || (*tau_numTrack)[it] == 3) &&
	fabs((*tau_charge)[it]) == 1 &&
	(*tau_JetBDTSigMedium)[it] &&
	!((*tau_EleBDTMedium)[it] && ((*tau_numTrack)[it] == 1)) &&
	!((*tau_muonVeto)[it]) ){
      taumedium_pt[taumedium_n] = (*tau_pt)[it];
      taumedium_eta[taumedium_n] = (*tau_eta)[it];
      taumedium_phi[taumedium_n] = (*tau_phi)[it];
      taumedium_n++;
      
    }
  }
}

void D3PDAnalysis::Fill_tautight(){
  tautight_n = 0;
  for(int it = 0; it < tau_n && tautight_n < Ntautight_max; it++){
    if( (*tau_pt)[it]/1000. > 10. && 
	fabs( (*tau_eta)[it] ) < 2.47 &&
	fabs( (*tau_leadTrack_eta)[it] ) < 2.47 &&
	((*tau_numTrack)[it] == 1 || (*tau_numTrack)[it] == 3) &&
	fabs((*tau_charge)[it]) == 1 &&
	(*tau_JetBDTSigTight)[it] &&
	!((*tau_EleBDTTight)[it] && ((*tau_numTrack)[it] == 1)) &&
	!((*tau_muonVeto)[it]) ){
      tautight_pt[tautight_n] = (*tau_pt)[it];
      tautight_eta[tautight_n] = (*tau_eta)[it];
      tautight_phi[tautight_n] = (*tau_phi)[it];
      tautight_n++;
      
    }
  }
}

void D3PDAnalysis::Fill_mu(){
  mu_n = 0;
  for(int im = 0; im < mu_staco_n && mu_n < Nmu_max; im++){
    if( (*mu_staco_pt)[im]/1000. > 5. && 
	fabs( (*mu_staco_eta)[im] ) < 2.4 &&
	((*mu_staco_isCombinedMuon)[im] || (*mu_staco_isSegmentTaggedMuon)[im]) == 1 && 
	(*mu_staco_loose)[im] == 1 &&
	(*mu_staco_nPixHits)[im] + (*mu_staco_nPixelDeadSensors)[im] >= 1 &&
	(*mu_staco_nSCTHits)[im] + (*mu_staco_nSCTDeadSensors)[im] >= 5 &&
	(*mu_staco_nPixHoles)[im] + (*mu_staco_nSCTHoles)[im] < 3){
      float trackEta = -log(tan((*mu_staco_id_theta)[im]/2.));
      int nTRTOutliers = (*mu_staco_nTRTOutliers)[im];
      int nTRTTotal    = nTRTOutliers + (*mu_staco_nTRTHits)[im];
      if(!(fabs(trackEta) < 1.9 && fabs(trackEta) > 0.1 && (nTRTTotal <= 5 || nTRTOutliers >= 0.9 * ((float) nTRTTotal)))){
	mu_pt[mu_n] = (*mu_staco_pt)[im];
	mu_eta[mu_n] = (*mu_staco_eta)[im];
	mu_phi[mu_n] = (*mu_staco_phi)[im];
	mu_n++;
      }
    }
  }
}

void D3PDAnalysis::Fill_mui(){
  mui_n = 0;
  for(int im = 0; im < mu_staco_n && mui_n < Nmui_max; im++){
    if( (*mu_staco_pt)[im]/1000. > 5. && 
	fabs( (*mu_staco_eta)[im] ) < 2.4 &&
	((*mu_staco_isCombinedMuon)[im] || (*mu_staco_isSegmentTaggedMuon)[im]) == 1 && 
	(*mu_staco_loose)[im] == 1 &&
	(*mu_staco_nPixHits)[im] + (*mu_staco_nPixelDeadSensors)[im] >= 1 &&
	(*mu_staco_nSCTHits)[im] + (*mu_staco_nSCTDeadSensors)[im] >= 5 &&
	(*mu_staco_nPixHoles)[im] + (*mu_staco_nSCTHoles)[im] < 3 &&
	(*mu_staco_ptcone20)[im]/(*mu_staco_pt)[im] < 0.12){
      float trackEta = -log(tan((*mu_staco_id_theta)[im]/2.));
      int nTRTOutliers = (*mu_staco_nTRTOutliers)[im];
      int nTRTTotal    = nTRTOutliers + (*mu_staco_nTRTHits)[im];
      if(!(fabs(trackEta) < 1.9 && fabs(trackEta) > 0.1 && (nTRTTotal <= 5 || nTRTOutliers >= 0.9 * ((float) nTRTTotal)))){
	mui_pt[mui_n] = (*mu_staco_pt)[im];
	mui_eta[mui_n] = (*mu_staco_eta)[im];
	mui_phi[mui_n] = (*mu_staco_phi)[im];
	mui_n++;
      }
    }
  }
}

void D3PDAnalysis::Fill_eloose(){
  eloose_n = 0;
  for(int ie = 0; ie < el_n && eloose_n < Neloose_max; ie++){
    //no cracks: !(fabs( (*el_cl_eta)[ie] ) < 1.52 && fabs( (*el_cl_eta)[ie] ) > 1.37) &&
    if( (*el_pt)[ie]/1000. > 5. && 
	fabs( (*el_cl_eta)[ie] ) < 2.47 && 
	((*el_author)[ie] == 1 || (*el_author)[ie] == 3) &&
	((*el_OQ)[ie]&1446) == 0 &&
	(*el_loosePP)[ie] == 1){
      eloose_pt[eloose_n] = (*el_pt)[ie];
      eloose_eta[eloose_n] = (*el_eta)[ie];
      eloose_phi[eloose_n] = (*el_phi)[ie];
      eloose_n++;
    }
  }
}

void D3PDAnalysis::Fill_eiloose(){
  eiloose_n = 0;
  for(int ie = 0; ie < el_n && eiloose_n < Neiloose_max; ie++){
    //no cracks: !(fabs( (*el_cl_eta)[ie] ) < 1.52 && fabs( (*el_cl_eta)[ie] ) > 1.37) &&
    if( (*el_pt)[ie]/1000. > 5. && 
	fabs( (*el_cl_eta)[ie] ) < 2.47 && 
	((*el_author)[ie] == 1 || (*el_author)[ie] == 3) &&
	((*el_OQ)[ie]&1446) == 0 &&
	(*el_loosePP)[ie] == 1 &&
	(*el_ptcone20)[ie]/(*el_pt)[ie] < 0.1){
      eiloose_pt[eiloose_n] = (*el_pt)[ie];
      eiloose_eta[eiloose_n] = (*el_eta)[ie];
      eiloose_phi[eiloose_n] = (*el_phi)[ie];
      eiloose_n++;
    }
  }
}

void D3PDAnalysis::Fill_emedium(){
  emedium_n = 0;
  for(int ie = 0; ie < el_n && emedium_n < Nemedium_max; ie++){
    //no cracks: !(fabs( (*el_cl_eta)[ie] ) < 1.52 && fabs( (*el_cl_eta)[ie] ) > 1.37) &&
    if( (*el_pt)[ie]/1000. > 5. && 
	fabs( (*el_cl_eta)[ie] ) < 2.47 && 
	((*el_author)[ie] == 1 || (*el_author)[ie] == 3) &&
	((*el_OQ)[ie]&1446) == 0 &&
	(*el_mediumPP)[ie] == 1){
      emedium_pt[emedium_n] = (*el_pt)[ie];
      emedium_eta[emedium_n] = (*el_eta)[ie];
      emedium_phi[emedium_n] = (*el_phi)[ie];
      emedium_n++;
    }
  }
}

void D3PDAnalysis::Fill_eimedium(){
  eimedium_n = 0;
  for(int ie = 0; ie < el_n && eimedium_n < Neimedium_max; ie++){
    //no cracks: !(fabs( (*el_cl_eta)[ie] ) < 1.52 && fabs( (*el_cl_eta)[ie] ) > 1.37) &&
    if( (*el_pt)[ie]/1000. > 5. && 
	fabs( (*el_cl_eta)[ie] ) < 2.47 && 
	((*el_author)[ie] == 1 || (*el_author)[ie] == 3) &&
	((*el_OQ)[ie]&1446) == 0 &&
	(*el_mediumPP)[ie] == 1 &&
	(*el_ptcone20)[ie]/(*el_pt)[ie] < 0.1){
      eimedium_pt[eimedium_n] = (*el_pt)[ie];
      eimedium_eta[eimedium_n] = (*el_eta)[ie];
      eimedium_phi[eimedium_n] = (*el_phi)[ie];
      eimedium_n++;
    }
  }
}

void D3PDAnalysis::Fill_etight(){
  etight_n = 0;
  for(int ie = 0; ie < el_n && etight_n < Netight_max; ie++){
    //no cracks: !(fabs( (*el_cl_eta)[ie] ) < 1.52 && fabs( (*el_cl_eta)[ie] ) > 1.37) &&
    if( (*el_pt)[ie]/1000. > 5. && 
	fabs( (*el_cl_eta)[ie] ) < 2.47 && 
	((*el_author)[ie] == 1 || (*el_author)[ie] == 3) &&
	((*el_OQ)[ie]&1446) == 0 &&
	(*el_tightPP)[ie] == 1){
      etight_pt[etight_n] = (*el_pt)[ie];
      etight_eta[etight_n] = (*el_eta)[ie];
      etight_phi[etight_n] = (*el_phi)[ie];
      etight_n++;
    }
  }
}

void D3PDAnalysis::Fill_eitight(){
  eitight_n = 0;
  for(int ie = 0; ie < el_n && eitight_n < Neitight_max; ie++){
    //no cracks: !(fabs( (*el_cl_eta)[ie] ) < 1.52 && fabs( (*el_cl_eta)[ie] ) > 1.37) &&
    if( (*el_pt)[ie]/1000. > 5. && 
	fabs( (*el_cl_eta)[ie] ) < 2.47 && 
	((*el_author)[ie] == 1 || (*el_author)[ie] == 3) &&
	((*el_OQ)[ie]&1446) == 0 &&
	(*el_tightPP)[ie] == 1 &&
	(*el_ptcone20)[ie]/(*el_pt)[ie] < 0.1){
      eitight_pt[eitight_n] = (*el_pt)[ie];
      eitight_eta[eitight_n] = (*el_eta)[ie];
      eitight_phi[eitight_n] = (*el_phi)[ie];
      eitight_n++;
    }
  }
}

void D3PDAnalysis::Fill_jet(){
  jet_n = 0;
  for(int ijet = 0; ijet < jet_AntiKt4LCTopo_n && jet_n < Njet_max; ijet++){
    if( (*jet_AntiKt4LCTopo_pt)[ijet]/1000. > 10. && fabs( (*jet_AntiKt4LCTopo_eta)[ijet] ) < 3.2){
      jet_pt[jet_n] = (*jet_AntiKt4LCTopo_pt)[ijet];
      jet_eta[jet_n] = (*jet_AntiKt4LCTopo_eta)[ijet];
      jet_phi[jet_n] = (*jet_AntiKt4LCTopo_phi)[ijet];
      jet_MV1[jet_n] = (*jet_AntiKt4LCTopo_flavor_weight_MV1)[ijet];
      jet_n++;
    }
  }
}

void D3PDAnalysis::Fill_fwdjet(){
  fwdjet_n = 0;
  for(int ijet = 0; ijet < jet_AntiKt4LCTopo_n && fwdjet_n < Nfwdjet_max; ijet++){
    if( (*jet_AntiKt4LCTopo_pt)[ijet]/1000. > 10. && fabs( (*jet_AntiKt4LCTopo_eta)[ijet] ) < 4.4 && fabs( (*jet_AntiKt4LCTopo_eta)[ijet] ) >= 3.2){
      fwdjet_pt[fwdjet_n] = (*jet_AntiKt4LCTopo_pt)[ijet];
      fwdjet_eta[fwdjet_n] = (*jet_AntiKt4LCTopo_eta)[ijet];
      fwdjet_phi[fwdjet_n] = (*jet_AntiKt4LCTopo_phi)[ijet];
      fwdjet_n++;
    }
  }
}

void D3PDAnalysis::Fill_JET(){
  JET_n = 0;
  for(int ijet = 0; ijet < trig_L1_jet_n && JET_n < NJET_max; ijet++){
    if( (*trig_L1_jet_et8x8)[ijet]/1000. > 10. && fabs( (*trig_L1_jet_eta)[ijet] ) < 3.){
      JET_pt[JET_n] = (*trig_L1_jet_et8x8)[ijet];
      JET_eta[JET_n] = (*trig_L1_jet_eta)[ijet];
      JET_phi[JET_n] = (*trig_L1_jet_phi)[ijet];
      JET_n++;
    }
  }
}

void D3PDAnalysis::ClearEventTriggerTree(){
  
  //reset all fields for Trigger Tree
  xe_pt = 0;
  xe_eta = 0;
  xe_phi = 0;
  xewmu_pt = 0;
  xewmu_eta = 0;
  xewmu_phi = 0;
  XE_pt = 0;
  XE_eta = 0;
  XE_phi = 0;

  jet_n = 0;
  for(int ijet = 0; ijet < Njet_max; ijet++){
    jet_pt[ijet] = 0.0;
    jet_eta[ijet] = 0.0;
    jet_phi[ijet] = 0.0;
    jet_MV1[ijet] = 0.0;
  }

  fwdjet_n = 0;
  for(int ijet = 0; ijet < Nfwdjet_max; ijet++){
    fwdjet_pt[ijet] = 0.0;
    fwdjet_eta[ijet] = 0.0;
    fwdjet_phi[ijet] = 0.0;
  }

  tauloose_n = 0;
  for(int it = 0; it < Ntauloose_max; it++){
    tauloose_pt[it] = 0.0;
    tauloose_eta[it] = 0.0;
    tauloose_phi[it] = 0.0;
  }

  taumedium_n = 0;
  for(int it = 0; it < Ntaumedium_max; it++){
    taumedium_pt[it] = 0.0;
    taumedium_eta[it] = 0.0;
    taumedium_phi[it] = 0.0;
  }

  tautight_n = 0;
  for(int it = 0; it < Ntautight_max; it++){
    tautight_pt[it] = 0.0;
    tautight_eta[it] = 0.0;
    tautight_phi[it] = 0.0;
  }

  mu_n = 0;
  for(int im = 0; im < Nmu_max; im++){
    mu_pt[im] = 0.0;
    mu_eta[im] = 0.0;
    mu_phi[im] = 0.0;
  }

  mui_n = 0;
  for(int im = 0; im < Nmui_max; im++){
    mui_pt[im] = 0.0;
    mui_eta[im] = 0.0;
    mui_phi[im] = 0.0;
  }

  eloose_n = 0;
  for(int ie = 0; ie < Neloose_max; ie++){
    eloose_pt[ie] = 0.0;
    eloose_eta[ie] = 0.0;
    eloose_phi[ie] = 0.0;
  }

  emedium_n = 0;
  for(int ie = 0; ie < Nemedium_max; ie++){
    emedium_pt[ie] = 0.0;
    emedium_eta[ie] = 0.0;
    emedium_phi[ie] = 0.0;
  }

  etight_n = 0;
  for(int ie = 0; ie < Netight_max; ie++){
    etight_pt[ie] = 0.0;
    etight_eta[ie] = 0.0;
    etight_phi[ie] = 0.0;
  }

  eiloose_n = 0;
  for(int ie = 0; ie < Neiloose_max; ie++){
    eiloose_pt[ie] = 0.0;
    eiloose_eta[ie] = 0.0;
    eiloose_phi[ie] = 0.0;
  }

  eimedium_n = 0;
  for(int ie = 0; ie < Neimedium_max; ie++){
    eimedium_pt[ie] = 0.0;
    eimedium_eta[ie] = 0.0;
    eimedium_phi[ie] = 0.0;
  }

  eitight_n = 0;
  for(int ie = 0; ie < Neitight_max; ie++){
    eitight_pt[ie] = 0.0;
    eitight_eta[ie] = 0.0;
    eitight_phi[ie] = 0.0;
  }

  JET_n = 0;
  for(int ijet = 0; ijet < NJET_max; ijet++){
    JET_pt[ijet] = 0.0;
    JET_eta[ijet] = 0.0;
    JET_phi[ijet] = 0.0;
  }

}

//Lead parton pT
void D3PDAnalysis::Fill_LeadPT(){
  LeadPt = 0.;
  if(mc_n <= 0) return;
  for (unsigned int p = 0; p < mc_n; p++) {
    if ((*mc_status)[p] != 3) break; //particles are ordered according to status code, 3 comes first
    if (abs((*mc_pdgId)[p]) == 21 || abs((*mc_pdgId)[p]) < 5) {
      LeadPt = (LeadPt > (*mc_pt)[p]) ? LeadPt : (*mc_pt)[p];
    }
  }
}

void D3PDAnalysis::FillTriggerTree(){
  T_trigger->Fill();
}

void D3PDAnalysis::WriteTriggerTree(){
  TFile *fout = new TFile(s_f_out,"RECREATE");
  fout->cd();
  T_trigger->Write();
  fout->Close();

  T_trigger->Delete();
  fout->Close();
}

void D3PDAnalysis::InitTriggerTree(char *sname){
  T_trigger = (TTree*) new TTree(sname,sname);

  T_trigger->Branch("lbn",&lbn);

  char *sbranch = new char[200];

  // RunNumber
  T_trigger->Branch("RunNumber",&RunNumber);

  // lead parton pT
  T_trigger->Branch("LeadPt",&LeadPt);

  //xe fields
  T_trigger->Branch("xe_pt",&xe_pt);
  T_trigger->Branch("xe_eta",&xe_eta);
  T_trigger->Branch("xe_phi",&xe_phi);
  T_trigger->Branch("xewmu_pt",&xewmu_pt);
  T_trigger->Branch("xewmu_eta",&xewmu_eta);
  T_trigger->Branch("xewmu_phi",&xewmu_phi);
  T_trigger->Branch("XE_pt",&XE_pt);
  T_trigger->Branch("XE_eta",&XE_eta);
  T_trigger->Branch("XE_phi",&XE_phi);

  //tauloose fields
  T_trigger->Branch("tauloose_n",&tauloose_n);
  sprintf(sbranch,"tauloose_pt[%d]/F",Ntauloose_max);
  T_trigger->Branch("tauloose_pt",tauloose_pt,sbranch);
  sprintf(sbranch,"tauloose_eta[%d]/F",Ntauloose_max);
  T_trigger->Branch("tauloose_eta",tauloose_eta,sbranch);
  sprintf(sbranch,"tauloose_phi[%d]/F",Ntauloose_max);
  T_trigger->Branch("tauloose_phi",tauloose_phi,sbranch);

  //taumedium fields
  T_trigger->Branch("taumedium_n",&taumedium_n);
  sprintf(sbranch,"taumedium_pt[%d]/F",Ntaumedium_max);
  T_trigger->Branch("taumedium_pt",taumedium_pt,sbranch);
  sprintf(sbranch,"taumedium_eta[%d]/F",Ntaumedium_max);
  T_trigger->Branch("taumedium_eta",taumedium_eta,sbranch);
  sprintf(sbranch,"taumedium_phi[%d]/F",Ntaumedium_max);
  T_trigger->Branch("taumedium_phi",taumedium_phi,sbranch);

  //tautight fields
  T_trigger->Branch("tautight_n",&tautight_n);
  sprintf(sbranch,"tautight_pt[%d]/F",Ntautight_max);
  T_trigger->Branch("tautight_pt",tautight_pt,sbranch);
  sprintf(sbranch,"tautight_eta[%d]/F",Ntautight_max);
  T_trigger->Branch("tautight_eta",tautight_eta,sbranch);
  sprintf(sbranch,"tautight_phi[%d]/F",Ntautight_max);
  T_trigger->Branch("tautight_phi",tautight_phi,sbranch);

  //mu fields
  T_trigger->Branch("mu_n",&mu_n);
  sprintf(sbranch,"mu_pt[%d]/F",Nmu_max);
  T_trigger->Branch("mu_pt",mu_pt,sbranch);
  sprintf(sbranch,"mu_eta[%d]/F",Nmu_max);
  T_trigger->Branch("mu_eta",mu_eta,sbranch);
  sprintf(sbranch,"mu_phi[%d]/F",Nmu_max);
  T_trigger->Branch("mu_phi",mu_phi,sbranch);

  //mui fields
  T_trigger->Branch("mui_n",&mui_n);
  sprintf(sbranch,"mui_pt[%d]/F",Nmui_max);
  T_trigger->Branch("mui_pt",mui_pt,sbranch);
  sprintf(sbranch,"mui_eta[%d]/F",Nmui_max);
  T_trigger->Branch("mui_eta",mui_eta,sbranch);
  sprintf(sbranch,"mui_phi[%d]/F",Nmui_max);
  T_trigger->Branch("mui_phi",mui_phi,sbranch);

  //eloose fields
  T_trigger->Branch("eloose_n",&eloose_n);
  sprintf(sbranch,"eloose_pt[%d]/F",Neloose_max);
  T_trigger->Branch("eloose_pt",eloose_pt,sbranch);
  sprintf(sbranch,"eloose_eta[%d]/F",Neloose_max);
  T_trigger->Branch("eloose_eta",eloose_eta,sbranch);
  sprintf(sbranch,"eloose_phi[%d]/F",Neloose_max);
  T_trigger->Branch("eloose_phi",eloose_phi,sbranch);

  //emedium fields
  T_trigger->Branch("emedium_n",&emedium_n);
  sprintf(sbranch,"emedium_pt[%d]/F",Nemedium_max);
  T_trigger->Branch("emedium_pt",emedium_pt,sbranch);
  sprintf(sbranch,"emedium_eta[%d]/F",Nemedium_max);
  T_trigger->Branch("emedium_eta",emedium_eta,sbranch);
  sprintf(sbranch,"emedium_phi[%d]/F",Nemedium_max);
  T_trigger->Branch("emedium_phi",emedium_phi,sbranch);

  //etight fields
  T_trigger->Branch("etight_n",&etight_n);
  sprintf(sbranch,"etight_pt[%d]/F",Netight_max);
  T_trigger->Branch("etight_pt",etight_pt,sbranch);
  sprintf(sbranch,"etight_eta[%d]/F",Netight_max);
  T_trigger->Branch("etight_eta",etight_eta,sbranch);
  sprintf(sbranch,"etight_phi[%d]/F",Netight_max);
  T_trigger->Branch("etight_phi",etight_phi,sbranch);

  //eiloose fields
  T_trigger->Branch("eiloose_n",&eiloose_n);
  sprintf(sbranch,"eiloose_pt[%d]/F",Neiloose_max);
  T_trigger->Branch("eiloose_pt",eiloose_pt,sbranch);
  sprintf(sbranch,"eiloose_eta[%d]/F",Neiloose_max);
  T_trigger->Branch("eiloose_eta",eiloose_eta,sbranch);
  sprintf(sbranch,"eiloose_phi[%d]/F",Neiloose_max);
  T_trigger->Branch("eiloose_phi",eiloose_phi,sbranch);

  //eimedium fields
  T_trigger->Branch("eimedium_n",&eimedium_n);
  sprintf(sbranch,"eimedium_pt[%d]/F",Neimedium_max);
  T_trigger->Branch("eimedium_pt",eimedium_pt,sbranch);
  sprintf(sbranch,"eimedium_eta[%d]/F",Neimedium_max);
  T_trigger->Branch("eimedium_eta",eimedium_eta,sbranch);
  sprintf(sbranch,"eimedium_phi[%d]/F",Neimedium_max);
  T_trigger->Branch("eimedium_phi",eimedium_phi,sbranch);

  //eitight fields
  T_trigger->Branch("eitight_n",&eitight_n);
  sprintf(sbranch,"eitight_pt[%d]/F",Neitight_max);
  T_trigger->Branch("eitight_pt",eitight_pt,sbranch);
  sprintf(sbranch,"eitight_eta[%d]/F",Neitight_max);
  T_trigger->Branch("eitight_eta",eitight_eta,sbranch);
  sprintf(sbranch,"eitight_phi[%d]/F",Neitight_max);
  T_trigger->Branch("eitight_phi",eitight_phi,sbranch);

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

  //fwdjet fields
  T_trigger->Branch("fwdjet_n",&fwdjet_n);
  sprintf(sbranch,"fwdjet_pt[%d]/F",Nfwdjet_max);
  T_trigger->Branch("fwdjet_pt",fwdjet_pt,sbranch);
  sprintf(sbranch,"fwdjet_eta[%d]/F",Nfwdjet_max);
  T_trigger->Branch("fwdjet_eta",fwdjet_eta,sbranch);
  sprintf(sbranch,"fwdjet_phi[%d]/F",Nfwdjet_max);
  T_trigger->Branch("fwdjet_phi",fwdjet_phi,sbranch);

  //JET fields
  T_trigger->Branch("JET_n",&JET_n);
  sprintf(sbranch,"JET_pt[%d]/F",NJET_max);
  T_trigger->Branch("JET_pt",JET_pt,sbranch);
  sprintf(sbranch,"JET_eta[%d]/F",NJET_max);
  T_trigger->Branch("JET_eta",JET_eta,sbranch);
  sprintf(sbranch,"JET_phi[%d]/F",NJET_max);
  T_trigger->Branch("JET_phi",JET_phi,sbranch);
  
}
