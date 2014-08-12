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
#include "TriggerAnalysis.hh"

TriggerAnalysis::TriggerAnalysis(char *sfile) : TriggerBase(sfile) {
}

TriggerAnalysis::~TriggerAnalysis(){
}

void TriggerAnalysis::AddVarLoop() {
  if(fChain == 0){
    cout << "NULL pointer to tree - not doing anything" << endl;
    return;
  }
  
  Long64_t nbytes = 0;
  Long64_t nb = 0;
  Long64_t nentries = fChain->GetEntries();
  cout << "Number of entries = " << nentries << endl;

  //Add the branches to the fChain tree
  InitHemBranch(); //Now, we set up the Hem branches for reading
  AddVarBranch();  //and add the variable branches to the tree

  //Event loop
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%10000 == 0)
      cout << ">>> Processing event # " << jentry << " of " << nentries << endl;

    //Fill the new branches
    FillVarBranch();
    
  }

}

void TriggerAnalysis::AddHemLoop() {
  if(fChain == 0){
    cout << "NULL pointer to tree - not doing anything" << endl;
    return;
  }
  
  Long64_t nbytes = 0;
  Long64_t nb = 0;
  Long64_t nentries = fChain->GetEntries();
  cout << "Number of entries = " << nentries << endl;

  //Add the branches to the fChain tree
  AddHemBranch();
  AddHempfwdBranch();
  AddHempmuBranch();
  AddHempfwdpmuBranch();

  //Event loop
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    if (jentry%10000 == 0)
      cout << ">>> Processing event # " << jentry << " of " << nentries << endl;

    //Fill the new branches
    FillHemBranch();
    FillHempfwdBranch();
    FillHempmuBranch();
    FillHempfwdpmuBranch();
    
  }

}

void TriggerAnalysis::WriteUpdatedTree(){
	
  f->cd();
  fChain->Write("",TObject::kOverwrite);
  
}
void TriggerAnalysis::InitHemBranch(){
  fChain->SetBranchAddress("HEM_pt", HEM_pt, &b_HEM_pt);
  fChain->SetBranchAddress("HEM_eta", HEM_eta, &b_HEM_eta);
  fChain->SetBranchAddress("HEM_phi", HEM_phi, &b_HEM_phi);
}

void TriggerAnalysis::AddVarBranch(){
  //add Variable branches to trigger tree
  b_shatR = (TBranch*) fChain->Branch("shatR", &shatR);
  b_cosptR = (TBranch*) fChain->Branch("cosptR", &cosptR);
  b_mdeltaR = (TBranch*) fChain->Branch("mdeltaR", &mdeltaR);
  b_gaminvR = (TBranch*) fChain->Branch("gaminvR", &gaminvR);
}
void TriggerAnalysis::FillVarBranch(){
  //check to see if hemisphere's are defined
  if(HEM_pt[0] <= 0.0){
    shatR = 0;
    cosptR = 0;
    mdeltaR = 0;
    gaminvR = 0;
  } else {
    //define hemispheres and MET
      TLorentzVector H1, H2;
      H1.SetPtEtaPhiM(HEM_pt[0],HEM_eta[0],HEM_phi[0],0.0);
      H2.SetPtEtaPhiM(HEM_pt[1],HEM_eta[1],HEM_phi[1],0.0);
      TVector3 MET;
      MET.SetPtEtaPhi(xe_pt,0.0,xe_phi);

      TVector3 vBETA_z = (1./(H1.E()+H2.E()))*(H1+H2).Vect(); 
      vBETA_z.SetX(0.0);         
      vBETA_z.SetY(0.0);

      //transformation from lab frame to approximate rest frame along beam-axis
      H1.Boost(-vBETA_z);
      H2.Boost(-vBETA_z);

      TVector3 pT_CM = (H1+H2).Vect() + MET;
      pT_CM.SetZ(0.0);     

      double Minv2 = (H1+H2).M2();
      double Einv = sqrt(MET.Mag2()+Minv2);
      //define shatR
      shatR = sqrt( ((H1+H2).E()+Einv)*((H1+H2).E()+Einv) - pT_CM.Mag2() );

      TVector3 vBETA_R = (1./sqrt(pT_CM.Mag2() + shatR*shatR))*pT_CM;

      double gamma_R = 1./sqrt(1.-vBETA_R.Mag2());

      //transformation from lab frame to R frame
      H1.Boost(-vBETA_R);
      H2.Boost(-vBETA_R); 

      /////////////
      //
      // R-frame
      //
      /////////////
      TVector3 vBETA_Rp1 = (1./(H1.E()+H2.E()))*(H1.Vect() - H2.Vect());
      //define gaminvR
      gaminvR = 1000.*sqrt(1.-vBETA_Rp1.Mag2());
     
      //transformation from R frame to R+1 frames
      H1.Boost(-vBETA_Rp1);
      H2.Boost(vBETA_Rp1);

      //////////////
      //
      // R+1-frames
      //
      //////////////
      //define mdeltaR
      mdeltaR = H1.E()+H2.E();      
      //define cosptR
      cosptR = 1000.*pT_CM.Mag()/sqrt(pT_CM.Mag2() + mdeltaR*mdeltaR);
      
  }
  b_shatR->Fill();
  b_cosptR->Fill();
  b_mdeltaR->Fill();
  b_gaminvR->Fill();
}


void TriggerAnalysis::AddHemBranch(){
  //add HEM branches to trigger tree
  b_HEM_pt = (TBranch*) fChain->Branch("HEM_pt", HEM_pt, "HEM_pt[2]/F");
  b_HEM_eta = (TBranch*) fChain->Branch("HEM_eta", HEM_eta, "HEM_eta[2]/F");
  b_HEM_phi = (TBranch*) fChain->Branch("HEM_phi", HEM_phi, "HEM_phi[2]/F");
  b_HEM_M = (TBranch*) fChain->Branch("HEM_M", HEM_M, "HEM_M[2]/F");
}
void TriggerAnalysis::AddHempfwdBranch(){
  b_HEMpfwd_pt = (TBranch*) fChain->Branch("HEMpfwd_pt", HEMpfwd_pt, "HEMpfwd_pt[2]/F");
  b_HEMpfwd_eta = (TBranch*) fChain->Branch("HEMpfwd_eta", HEMpfwd_eta, "HEMpfwd_eta[2]/F");
  b_HEMpfwd_phi = (TBranch*) fChain->Branch("HEMpfwd_phi", HEMpfwd_phi, "HEMpfwd_phi[2]/F");
  b_HEMpfwd_M = (TBranch*) fChain->Branch("HEMpfwd_M", HEMpfwd_M, "HEMpfwd_M[2]/F");
}
void TriggerAnalysis::AddHempmuBranch(){
  b_HEMpmu_pt = (TBranch*) fChain->Branch("HEMpmu_pt", HEMpmu_pt, "HEMpmu_pt[2]/F");
  b_HEMpmu_eta = (TBranch*) fChain->Branch("HEMpmu_eta", HEMpmu_eta, "HEMpmu_eta[2]/F");
  b_HEMpmu_phi = (TBranch*) fChain->Branch("HEMpmu_phi", HEMpmu_phi, "HEMpmu_phi[2]/F");
  b_HEMpmu_M = (TBranch*) fChain->Branch("HEMpmu_M", HEMpmu_M, "HEMpmu_M[2]/F");
}
void TriggerAnalysis::AddHempfwdpmuBranch(){
  b_HEMpfwdpmu_pt = (TBranch*) fChain->Branch("HEMpfwdpmu_pt", HEMpfwdpmu_pt, "HEMpfwdpmu_pt[2]/F");
  b_HEMpfwdpmu_eta = (TBranch*) fChain->Branch("HEMpfwdpmu_eta", HEMpfwdpmu_eta, "HEMpfwdpmu_eta[2]/F");
  b_HEMpfwdpmu_phi = (TBranch*) fChain->Branch("HEMpfwdpmu_phi", HEMpfwdpmu_phi, "HEMpfwdpmu_phi[2]/F");
  b_HEMpfwdpmu_M = (TBranch*) fChain->Branch("HEMpfwdpmu_M", HEMpfwdpmu_M, "HEMpfwdpmu_M[2]/F");
}

void TriggerAnalysis::FillHemBranch(){
  vector<TLorentzVector> JETs = GetJets(30.,-1.);
  if(JETs.size() < 2){
    for(int i = 0; i < 2; i++){
      HEM_pt[i] = 0;
      HEM_eta[i] = 0;
      HEM_phi[i] = 0;
      HEM_M[i] = 0;
    }
  } else {
    vector<TLorentzVector> HEMs = GetHemi(JETs);
    for(int i = 0; i < 2; i++){
      HEM_pt[i] = HEMs[i].Pt();
      HEM_eta[i] = HEMs[i].Eta();
      HEM_phi[i] = HEMs[i].Phi();
      HEM_M[i] = HEMs[i].M();
    }
    HEMs.clear();
  }
  b_HEM_pt->Fill();
  b_HEM_eta->Fill();
  b_HEM_phi->Fill();
  b_HEM_M->Fill();
  JETs.clear();
}

void TriggerAnalysis::FillHempfwdBranch(){
  vector<TLorentzVector> JETs = GetJets(30.,-1.);
  vector<TLorentzVector> FwdJETs = GetFwdJets(30.,-1.);
  vector<TLorentzVector> LIST = AddVectors(JETs,FwdJETs);
  if(LIST.size() < 2){
    for(int i = 0; i < 2; i++){
      HEMpfwd_pt[i] = 0;
      HEMpfwd_eta[i] = 0;
      HEMpfwd_phi[i] = 0;
      HEMpfwd_M[i] = 0;
    }
  } else {
    vector<TLorentzVector> HEMs = GetHemi(LIST);
    for(int i = 0; i < 2; i++){
      HEMpfwd_pt[i] = HEMs[i].Pt();
      HEMpfwd_eta[i] = HEMs[i].Eta();
      HEMpfwd_phi[i] = HEMs[i].Phi();
      HEMpfwd_M[i] = HEMs[i].M();
    }
    HEMs.clear();
  }
  b_HEMpfwd_pt->Fill();
  b_HEMpfwd_eta->Fill();
  b_HEMpfwd_phi->Fill();
  b_HEMpfwd_M->Fill();
  JETs.clear();
  FwdJETs.clear();
  LIST.clear();
}

void TriggerAnalysis::FillHempmuBranch(){
  vector<TLorentzVector> JETs = GetJets(30.,-1.);
  vector<TLorentzVector> MUs = GetMuons(10.,-1.);
  vector<TLorentzVector> LIST = AddVectors(JETs,MUs);
  if(LIST.size() < 2){
    for(int i = 0; i < 2; i++){
      HEMpmu_pt[i] = 0;
      HEMpmu_eta[i] = 0;
      HEMpmu_phi[i] = 0;
      HEMpmu_M[i] = 0;
    }
  } else {
    vector<TLorentzVector> HEMs = GetHemi(LIST);
    for(int i = 0; i < 2; i++){
      HEMpmu_pt[i] = HEMs[i].Pt();
      HEMpmu_eta[i] = HEMs[i].Eta();
      HEMpmu_phi[i] = HEMs[i].Phi();
      HEMpmu_M[i] = HEMs[i].M();
    }
    HEMs.clear();
  }
  b_HEMpmu_pt->Fill();
  b_HEMpmu_eta->Fill();
  b_HEMpmu_phi->Fill();
  b_HEMpmu_M->Fill();
  JETs.clear();
  MUs.clear();
  LIST.clear();
}

void TriggerAnalysis::FillHempfwdpmuBranch(){
  vector<TLorentzVector> JETs = GetJets(30.,-1.);
  vector<TLorentzVector> FwdJETs = GetFwdJets(30.,-1.);
  vector<TLorentzVector> MUs = GetMuons(10.,-1.);
  vector<TLorentzVector> LIST = AddVectors(FwdJETs,AddVectors(JETs,MUs));
  if(LIST.size() < 2){
    for(int i = 0; i < 2; i++){
      HEMpfwdpmu_pt[i] = 0;
      HEMpfwdpmu_eta[i] = 0;
      HEMpfwdpmu_phi[i] = 0;
      HEMpfwdpmu_M[i] = 0;
    }
  } else {
    vector<TLorentzVector> HEMs = GetHemi(LIST);
    for(int i = 0; i < 2; i++){
      HEMpfwdpmu_pt[i] = HEMs[i].Pt();
      HEMpfwdpmu_eta[i] = HEMs[i].Eta();
      HEMpfwdpmu_phi[i] = HEMs[i].Phi();
      HEMpfwdpmu_M[i] = HEMs[i].M();
    }
    HEMs.clear();
  }
  b_HEMpfwdpmu_pt->Fill();
  b_HEMpfwdpmu_eta->Fill();
  b_HEMpfwdpmu_phi->Fill();
  b_HEMpfwdpmu_M->Fill();
  JETs.clear();
  FwdJETs.clear();
  MUs.clear();
  LIST.clear();
}



vector<TLorentzVector> TriggerAnalysis::AddVectors(vector<TLorentzVector> V1, vector<TLorentzVector> V2){
  int N_V1 = V1.size();
  int N_V2 = V2.size();
  vector<TLorentzVector> V1p2;
  
  for(int i = 0; i < N_V1; i++){
    V1p2.push_back(V1[i]);
  }
  for(int i = 0; i < N_V2; i++){
    V1p2.push_back(V2[i]);
  }

  return V1p2;
}

vector<TLorentzVector> TriggerAnalysis::GetHemi(vector<TLorentzVector> myjets){
  vector<TLorentzVector> mynewjets;
  TLorentzVector j1, j2;
  bool foundGood = false;
  int N_comb = 1;
  for(int i = 0; i < myjets.size(); i++){
    N_comb *= 2;
  }
  double M_min = -1.;
  int j_count;
  
  for(int i = 1; i < N_comb-1; i++){
    TLorentzVector j_temp1, j_temp2;
    int itemp = i;
    j_count = N_comb/2;
    int count = 0;
    while(j_count > 0){
      if(itemp/j_count == 1){
        j_temp1 += myjets[count];
      } else {
        j_temp2 += myjets[count];
      }
      itemp -= j_count*(itemp/j_count);
      j_count /= 2;
      count++;
    }
    double M_temp = j_temp1.M2()+j_temp2.M2();
    
    // smallest mass
    if(M_temp < M_min || M_min < 0){
      M_min = M_temp;
      j1 = j_temp1;
      j2 = j_temp2;
    }
  }  
  if(j2.Pt() > j1.Pt()){
    TLorentzVector temp = j1;
    j1 = j2;
    j2 = temp;
  }
  mynewjets.push_back(j1);
  mynewjets.push_back(j2);
  
  return mynewjets;
}

vector<TLorentzVector> TriggerAnalysis::GetJets(float pt_cut, float eta_cut){
  vector<TLorentzVector> JETs;

  for(int i = 0; i < 20; i++){
    if(jet_pt[i] <= 0.0) break;
    if(jet_pt[i] >= pt_cut*1000. && (fabs(jet_eta[i]) < eta_cut || eta_cut < 0)){
      TLorentzVector JET;
      JET.SetPtEtaPhiM(jet_pt[i],jet_eta[i],jet_phi[i],0.0);
      JETs.push_back(JET);
    }
  }

  return JETs;
}

vector<TLorentzVector> TriggerAnalysis::GetFwdJets(float pt_cut, float eta_cut){
  vector<TLorentzVector> JETs;

  for(int i = 0; i < 20; i++){
    if(fwdjet_pt[i] <= 0.0) break;
    if(fwdjet_pt[i] >= pt_cut*1000. && (fabs(fwdjet_eta[i]) < eta_cut || eta_cut < 0)){
      TLorentzVector JET;
      JET.SetPtEtaPhiM(fwdjet_pt[i],fwdjet_eta[i],fwdjet_phi[i],0.0);
      JETs.push_back(JET);
    }
  }

  return JETs;
}

vector<TLorentzVector> TriggerAnalysis::GetMuons(float pt_cut, float eta_cut){
  vector<TLorentzVector> MUs;

  for(int i = 0; i < 5; i++){
    if(mu_pt[i] <= 0.0) break;
    if(mu_pt[i] >= pt_cut*1000. && (fabs(mu_eta[i]) < eta_cut || eta_cut < 0)){
      TLorentzVector MU;
      MU.SetPtEtaPhiM(mu_pt[i],mu_eta[i],mu_phi[i],0.0);
      MUs.push_back(MU);
    }
  }

  return MUs;
}

TVector3 TriggerAnalysis::GetMET(){
  TVector3 MET; 
  MET.SetPtEtaPhi(xe_pt,0.0,xe_phi);

  return MET;
}
  

