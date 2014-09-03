
Int_t       jet_n;
Float_t     jet_pt[20];
Float_t     jet_eta[20];
Float_t     jet_phi[20];
Float_t     JET_pt[20];
Float_t     JET_eta[20];
Float_t     JET_phi[20];
Float_t     jet_MV1[20];
Float_t     xe_pt;
Float_t XE_pt;

Float_t	shatR;
Float_t	cosptR;
Float_t	mdeltaR;
Float_t gaminvR;

Float_t Prod;
Bool_t TRIG_4J20;
Bool_t TRIG_6jet70_4J20;
Bool_t TRIG_5jet85_4J20;
Bool_t TRIG_J75_XE40_j150_xe90;
Bool_t TRIG_J100_j200_xe80;
Bool_t TRIG_XE70_xe100;
Bool_t All;

void add_new_vars(){
	TString dirList []= {"GG_direct", 
											"GG_onestepCC",
											"MONOJET_B",  
											"SS_direct",  
											"SS_onestepCC",  
											"T1",  "D5",
											"WimpPair"}
											
	//for (int i = 0; i < 8; i++)
	//parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var/"+dirList[i]+"/");
	parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var/WimpPair/");
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
			if (fname.EndsWith(".root") &&fname(0,12) != "triggerInfo_"){
				cout << "Attempting to tamper with file " << fname << endl;
				TFile * f = new TFile(dirname+fname, "READ");
				if (f->IsOpen()){
					TTree * t = (TTree*)f->Get("physics");
					if (t && t ->GetEntries() > 0) ParseTree(t,fname, dirname);
				}
				delete f;
			}
		}
	} else 		cout << "No files in " << dirname<<endl;
}

void ParseTree(TTree * tree, TString fname, TString dirname){
	passTrigger = new vector<int>();
	triggerName = new vector<TString>();

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
	tree->SetBranchAddress("XE_pt",&XE_pt);
	
	tree->SetBranchAddress("shatR",&shatR);
	tree->SetBranchAddress("cosptR",&cosptR);
	tree->SetBranchAddress("mdeltaR",&mdeltaR);
	tree->SetBranchAddress("gaminvR",&gaminvR);
	
	TFile *newfile = new TFile(dirname+"triggerInfo_"+fname,"recreate");
	TTree *newtree = tree->CloneTree(0);
	
	//set new branches
	Float_t		HT = 0;
	Float_t		ht = 0;

	TBranch * b_ht = newtree->Branch("HT",							&HT);
	TBranch * b_ht = newtree->Branch("ht",							&ht);
	TBranch * b_pr = newtree->Branch("Prod",		&Prod);
	TBranch * b_t0 = newtree->Branch("TRIG_4J20",&TRIG_4J20);
	TBranch * b_t1 = newtree->Branch("TRIG_6jet70_4J20",&TRIG_6jet70_4J20);
	TBranch * b_t2 = newtree->Branch("TRIG_5jet85_4J20",&TRIG_5jet85_4J20);
	TBranch * b_t3 = newtree->Branch("TRIG_J75_XE40_j150_xe90",&TRIG_J75_XE40_j150_xe90);
	TBranch * b_t4 = newtree->Branch("TRIG_J100_j200_xe80",&TRIG_J100_j200_xe80);
	TBranch * b_t5 = newtree->Branch("TRIG_XE70_xe100",&TRIG_XE70_xe100);
	TBranch * b_all = newtree->Branch("All",		&All);


	int nEntries = tree->GetEntries();
	cout << "Running over " << nEntries << " events." << endl;
	for (int i = 0; i < nEntries; i++){
		printProgress(nEntries,i);
		
		tree->GetEntry(i);
		
				
		HT = 0;
		for(int j = 0; j < 20; j++){ HT += JET_pt[j]/1000.;}
		for(int j = 0; j < 20; j++){ ht += jet_pt[j]/1000.;}
		
		Prod			= (shatR/1000. + 85)*(gaminvR + 0.048)/1000;
		TRIG_4J20 = TRIG_JET(4,20);
		TRIG_6jet70_4J20 	= TRIG_jet(6,70) && TRIG_JET(4,20);
		TRIG_5jet85_4J20 	= TRIG_jet(5,85) && TRIG_JET(4,20);
		TRIG_J75_XE40_j150_xe90 = xe_pt/1000. > 90 && XE_pt/1000. > 40 && TRIG_jet(1,150) && TRIG_JET(1,75);
		TRIG_J100_j200_xe80 = xe_pt/1000. > 80 && TRIG_jet(1,200) && TRIG_JET(1,100);
		TRIG_XE70_xe100 = xe_pt/1000. > 100 && XE_pt/1000. > 70;
		All = TRIG_J75_XE40_j150_xe90 || TRIG_J100_j200_xe80 || TRIG_XE70_xe100 ||
					TRIG_6jet70_4J20 || TRIG_5jet85_4J20 || (HT > 200 && ht > 1000);
		
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

bool TRIG_JET(int x, float y){
	int Njet = 0;
	for(int i = 0; i < 20; i++){
		if(JET_pt[i] <= 0.0) continue;
		if(JET_pt[i]/1000. > y) Njet++;
	}
	if(Njet >= x) return true;
	return false;
}

bool TRIG_jet(int x, float y){
	int Njet = 0;
	for(int i = 0; i < 20; i++){
		if(jet_pt[i] <= 0.0) continue;
		if(jet_pt[i]/1000. > y) Njet++;
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


/*
6jet70_4J20 5jet85_4J20 J75_XE40_j150_xe90
J100_j200_xe80
XE70_xe100
jet200_xe80_J100
2bjet60_jet175_J100
bjet70_jet175_xe7
2bjet55_4jet75_4J20
bjet75_4jet75_4J20
jet150_xe90_J75_XE40 
bjet120_xe70_
J75_XE40
fatjet460_J100
fwdjet200_FJ100
xe100_XE70 xe80_XE70 xe80_j100_XE50_J40 2bjet45_5j45_5J15*/

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
