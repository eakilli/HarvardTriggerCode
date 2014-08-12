Int_t       jet_n;
Float_t     jet_pt[20];
Float_t     jet_eta[20];
Float_t     jet_phi[20];
Float_t     JET_pt[20];
Float_t     JET_eta[20];
Float_t     JET_phi[20];
Float_t     jet_MV1[20];
Float_t     xe_pt;

enum trigger_type {HL, L1};

void add_new_vars(){
	parseDir("/afs/cern.ch/user/e/etolley/work/D3PD/TRIGGERvar_SAMPLES/DM/");
}

void parseDir(TString dirname, bool isData = false){
	cout << "Operating in " << dirname << endl;
	TSystemDirectory dir(dirname, dirname);
	TList *files = dir.GetListOfFiles();
	if (files){
		cout << "Found files." << endl;
		TSystemFile *file;
		TString fname;
		TIter next(files);
		while ((file=(TSystemFile*)next())) {
			fname = file->GetName();
			if (fname.EndsWith(".root") &&fname(0,5) != 'trigg'){
				cout << "Attempting to tamper with file " << fname << endl;
				TFile * f = new TFile(dirname+fname, "READ");
				if (f->IsOpen()){
					TTree * t = (TTree*)f->Get("physics");
					if (t) ParseTree(t,fname, dirname);
				}
				delete f;
			}
		}
	} else 		cout << "No files in " << dirname<<endl;
}

void ParseTree(TTree * tree, TString fname, TString dirname){
	
	//set old branches
	tree->SetBranchAddress("jet_n",&jet_n);
	tree->SetBranchAddress("jet_pt",&jet_pt);
	tree->SetBranchAddress("jet_eta",&jet_eta);
	tree->SetBranchAddress("jet_phi",&jet_phi);
	tree->SetBranchAddress("JET_pt",&JET_pt);
	tree->SetBranchAddress("JET_eta",&JET_eta);
	tree->SetBranchAddress("JET_phi",&JET_phi);
	tree->SetBranchAddress("jet_MV1",&jet_MV1);
	tree->SetBranchAddress("xe_pt",&xe_pt);
	

	TFile *newfile = new TFile(dirname+"triggerInfo_"+fname,"recreate");
	TTree *newtree = tree->CloneTree(0);
	
	//set new branches
	Float_t		HT = 0;
	Bool_t		TRIG_5J85 = false;
	Bool_t		TRIG_5J75_ETA25 = false;
	Bool_t		TRIG_B75_4J75 = false;
	Bool_t		TRIG_2B55_4J55 = false;
	TBranch * b_ht = newtree->Branch("HT",&HT);
	TBranch * b_t1 = newtree->Branch("TRIG_5J85",&TRIG_5J85);
	TBranch * b_t2 = newtree->Branch("TRIG_5J75_ETA25",&TRIG_5J75_ETA25);
	TBranch * b_t3 = newtree->Branch("TRIG_B75_4J75",&TRIG_B75_4J75);
	TBranch * b_t4 = newtree->Branch("TRIG_2B55_4J55",&TRIG_2B55_4J55);
	
	int nEntries = tree->GetEntries();
	for (int i = 0; i < nEntries; i++){
		if (nEntries > 10000 && i % (nEntries/10) == 0) cout <<"\t\t"<< ceil(i*100.0/nEntries) << "%" << endl;
		
		tree->GetEntry(i);
		
		if (TRIG_5J85())		TRIG_5J85 = true;		else TRIG_5J85 = false;
		if (TRIG_5J75_ETA25())	TRIG_5J75_ETA25 = true; else TRIG_5J75_ETA25 = false;
		if (TRIG_B75_4J75())	TRIG_B75_4J75 = true;	else TRIG_B75_4J75 = false;
		if (TRIG_2B55_4J55())	TRIG_2B55_4J55 = true;	else TRIG_2B55_4J55 = false;
		
		HT = 0;
		for(int j = 0; j < 20; j++) HT += jet_pt[j];
		
		newtree->Fill();
	}
	newtree->Write();
	delete newfile;
	
}

/***********************
6jet50_5J15.ETA25
5jet85_4J20
4jet100_5J50
6jet70_4J20
jet400_J100
3jet175_J100
jet200_xe80_J100
2bjet60_jet175_J100
bjet70_jet175_xe7
0_J100
2bjet55_4jet75_4J20
bjet75_4jet75_4J20
jet150_xe90_J75_XE40
bjet120_xe70_
J75_XE40
fatjet460_J100
fwdjet200_FJ100
xe100_XE70
xe80_XE70
xe80_j100_XE50_J40
2bjet45_5j45_5J15
************************/

/*bool TRIG_j(int x, float y){
	int Njet = 0;
	for(int i = 0; i < 20; i++){
		if(jet_pt[i] <= 0.0) continue;
		if(jet_pt[i]/1000. > y) Njet++;
	}
	if(Njet >= x) return true;
	return false;
}*/

bool TRIG_J(int x, float y, trigger_type t){
	Float_t j_pt[20];
	if (t == HL) j_pt = jet_pt;
	else if (t == L1) j_pt = JET_pt;
	int Njet = 0;
	for(int i = 0; i < 20; i++){
		if(j_pt[i] <= 0.0) continue;
		if(j_pt[i]/1000. > y) Njet++;
	}
	if(Njet >= x) return true;
	return false;
}

bool TRIG_Jeta(int x, float y, float eta){
	int Njet = 0;
	for(int i = 0; i < 20; i++){
		if(JET_pt[i] <= 0.0) continue;
		if(JET_pt[i]/1000. > y && fabs(JET_eta[i]) < eta) Njet++;
	}
	if(Njet >= x) return true;
	return false;
}

bool TRIG_bJ(int x, float y){
	int Njet = 0;
	for(int i = 0; i < 20; i++){
		if(jet_pt[i] <= 0.0) continue;
		if(jet_pt[i]/1000. > y && jet_MV1[i] > 0.7892) Njet++;
	}
	if(Njet >= x) return true;
	return false;
}

bool TRIG_XE(float thres, trigger_type t){
	return xe_pt > thres;
}

bool TRIG_6jet50_5J15_ETA25(){
	return TRIG_J(6,50,HL) && TRIG_Jeta(5,15,25);
}

bool TRIG_5jet85_4J20(){
	return TRIG_J(5,85,HL) && TRIG_J(4,20,L1);
}
bool TRIG_4jet100_5J50(){
	return TRIG_J(4,100,HL) && TRIG_J(5,50,L1);
}
bool TRIG_6jet70_4J20(){
	return TRIG_J(6,70,HL) && TRIG_J(4,20,L1);
}
bool TRIG_jet400_J100(){
	return TRIG_J(1,400,HL) && TRIG_J(1,100,L1);
}
bool TRIG_3jet175_J100(){
	return TRIG_J(3,175,HL) && TRIG_J(1,100,L1);
}
/*bool TRIG_jet200_xe80_J100
bool TRIG_2bjet60_jet175_J100
bool TRIG_bjet70_jet175_xe7
bool TRIG_0_J100
bool TRIG_2bjet55_4jet75_4J20
bool TRIG_bjet75_4jet75_4J20
bool TRIG_jet150_xe90_J75_XE40
bool TRIG_bjet120_xe70_
bool TRIG_J75_XE40
bool TRIG_fatjet460_J100
bool TRIG_fwdjet200_FJ100
bool TRIG_xe100_XE70
bool TRIG_xe80_XE70
bool TRIG_xe80_j100_XE50_J40
bool TRIG_2bjet45_5j45_5J15*/

/*

bool TRIG_5j85(){
	return TRIG_J(5,85.,JET);
}

bool TRIG_5J75_ETA25(){
  int Njet = 0;
  for(int i = 0; i < 20; i++){
    if(jet_pt[i] <= 0.0) continue;
    if(jet_pt[i]/1000. > 75. && fabs(jet_eta[i]) < 2.5) Njet++;
  }
  if(Njet >= 5) return true;
  return false;
}
bool TRIG_B75_4J75(){
  int Njet = 0;
  int Nbjet = 0;
  for(int i = 0; i < 20; i++){
    if(jet_pt[i] <= 0.0) continue;
    if(jet_pt[i]/1000. > 75.) Njet++;
    if(jet_pt[i]/1000. > 75. && jet_MV1[i] > 0.7892) Nbjet++;
  }
  if(Njet >= 4 && Nbjet >= 1) return true;
  return false;
}
bool TRIG_2B55_4J55(){
  int Njet = 0;
  int Nbjet = 0;
  for(int i = 0; i < 20; i++){
    if(jet_pt[i] <= 0.0) continue;
    if(jet_pt[i]/1000. > 55.) Njet++;
    if(jet_pt[i]/1000. > 55. && jet_MV1[i] > 0.7892) Nbjet++;
  }
  if(Njet >= 4 && Nbjet >= 2) return true;
  return false;
}*/

void printProgress(int n, int i){
	if  (i % (n/100) == 0){
		cerr <<"\t"<< ceil(i*100.0/n) << "%";
		cerr << "\t|";
		for (int k = 0; k < 10; k++){
			if (k <= ceil(i*10.0/n)) cerr << "-";
			else cerr << " ";
		}
		cerr << "| \t"<<i <<"/"<<n;
		cerr <<"\r";
	}	
}