/******************************
Last modified by Emma Tolley (emma.tolley@cern.ch) 8 Sept '14

This code runs over razor trigger var ntuples and adds trigger variables

Produce razor trigger var ntuples from D3PDs by running:
	1. DP3DtoTrigger -d3pdfile.root -triggerntuple.root
	2. AddHemtoTrigger -triggerntuple.root -hem_triggerntuple.root
	3. AddVartoTrigger -hem_triggerntuple.root -var_triggerntuple.root
This code runs over the final var_triggerntuple.root file.

To add new triggers, simply add a new entry in the Initialize Triggers section
For example, this line adds the trigger j150_xe90_J75_XE40:
	triggerList->Add(new MultiTrigger("j150", "xe90", "J75", "XE40"));
	
MultiTrigger takes an AND of all the tags in the constructor
MultiTrigger and all the Trigger classes are defined in trigger_classes.hh

Supported tags are:

	J#, #J#, #J#_eta#
	j#, #j#, #j#_eta#
	b#, #b#, #b#_eta# eta number parsed like 255 => 2.55, 14 => 1.4, etc
	xe#, XE#
	ht#, HT#
	J#_XE#_DPHI#
	j#_xe#_dphi# phi number parsed like 10 => 1.0, 5 => 0.5, etc

To add a new trigger, make a new Trigger subclass and add the appropriate
tag parsing / trigger identification to the MultiTrigger constructor
*******************************/

#include "trigger_classes.hh"

// Lists of Trigger Objects
TObjArray * triggerList;
TObjArray * seedList;

void add_new_vars(){

	/************************
	*  Initialize Triggers
	*************************/
	triggerList = new TObjArray();
	cout << "Triggers Implemented: " << endl;
	//Multijet Triggers
	triggerList->Add(new MultiTrigger("j400", 			"J100"));
	triggerList->Add(new MultiTrigger("3j175", 			"J100"));
	triggerList->Add(new MultiTrigger("4j100", 			"3J50"));
	triggerList->Add(new MultiTrigger("5j85", 			"4J20"));
	triggerList->Add(new MultiTrigger("6j50_eta25", "4J20"));
	triggerList->Add(new MultiTrigger("6j70", 			"4J20"));
	triggerList->Add(new MultiTrigger("6j50_eta25", "5J15_eta24"));
	triggerList->Add(new MultiTrigger("7J45", 			"6J15"));
	triggerList->Add(new MultiTrigger("8J40", 			"6J15"));
	
	//jets + bjets triggers
	triggerList->Add(new MultiTrigger("b175", "b60" , "J100"));
	triggerList->Add(new MultiTrigger("2b55", "2j55", "4J20"));
	triggerList->Add(new MultiTrigger("b75", 	"3j75", "4J20"));
	triggerList->Add(new MultiTrigger("2b45", "3j45", "5J15_eta24"));
	
	//Met trigger
	triggerList->Add(new MultiTrigger("xe100", "XE70"));
	
	//Ht trigger
	triggerList->Add(new MultiTrigger("ht1000", "HT200"));
	
	//Combined jet+met triggers
	triggerList->Add(new MultiTrigger("xe80", "j200", "J100"));
	triggerList->Add(new MultiTrigger("j150", "xe90", "J75", "XE40"));
	triggerList->Add(new MultiTrigger("b120", "xe70", "J75", "XE40"));
	triggerList->Add(new MultiTrigger("j175", "b70", "xe70", "J100"));
	triggerList->Add(new MultiTrigger("j100_xe80_dphi10", "J40_XE50_DPHI10"));
	
	cout << "Triggers Implemented: " << endl;
	for (int i = 0; i < triggerList->GetEntries(); i++){
		cout << ((MultiTrigger*)triggerList->At(i))->getName() << endl;
	}
	cout << endl;
	
	//L1 Seeds for the Razor variables
	seedList = new TObjArray();
	seedList->Add(new MultiTrigger("4J20"));
	seedList->Add(new MultiTrigger("XE70"));
	seedList->Add(new MultiTrigger("J75","XE40"));
	seedList->Add(new MultiTrigger("J150"));

	seedList->Add(new MultiTrigger("2J30"));
	seedList->Add(new MultiTrigger("2j30"));
	seedList->Add(new MultiTrigger("2J10","XE60"));
	seedList->Add(new MultiTrigger("2J15","XE55"));
	seedList->Add(new MultiTrigger("2J20","XE50"));
	seedList->Add(new MultiTrigger("2J25","XE45"));
	seedList->Add(new MultiTrigger("2J30","XE40"));
	seedList->Add(new MultiTrigger("2J40","XE35"));
	seedList->Add(new MultiTrigger("2J45","XE30"));

	seedList->Add(new MultiTrigger("HT200"));
	cout << "L1 seeds Implemented: " << endl;
	for (int i = 0; i < seedList->GetEntries(); i++){
		cout << ((MultiTrigger*)seedList->At(i))->getName() << endl;
	}
	cout << endl;
	/************************
	*  Triggers Initialized
	*************************/
	run_over_dirs();
	//parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var/T1/");
	//parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var/MONOJET_B/");
	
}
void run_over_dirs(){
	TString dirList []= {"GG_direct", 
											//"GG_onestepCC",
											//"MONOJET_B",  
											"SS_direct",  
											//"SS_onestepCC",  
											"T1",
											"D5",
											"WimpPair","DATA","background_MC"};
											
	//for (int i = 0; i < 7; i++)vi
	//	parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/"+dirList[i]+"/");
	//parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/GG_direct/");
	parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/MONOJET_B/");
	/*parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/SS_direct/");
	parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/WimpPair/");
	parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/background_MC/");
	parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/DATA/");
	parseDir("/n/atlasdata1/etolley/Trigger/SAMPLES_var2/T1/");*/
}

// Boring directory parsing
// Loads trees in a given directory
void parseDir(TString dirname){
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
	} else cout << "No files in " << dirname<<endl;
}

/************************
	*  Main Ntuple reading code
	*************************/
void ParseTree(TTree * tree, TString fname, TString dirname){
	EventInfoContainer * evt = new EventInfoContainer();
	
	//set vars in evt to old tree branches
	evt->setBranchVars(tree);

	TFile *newfile = new TFile(dirname+"triggerInfo_"+fname,"recreate");
	TTree *newtree = tree->CloneTree(0);
	
	// set vars in evt to new tree branches
	evt->setNewVars(newtree);
	
	bool all_nr = false;
	newtree->Branch("all_norazor",&all_nr);
	
	for (int i = 0; i < triggerList->GetEntries(); i++){
		MultiTrigger * mt = (MultiTrigger*)(triggerList->At(i));
		newtree->Branch(mt->getName(),&(mt->pass));
	}

	for (int i = 0; i < seedList->GetEntries(); i++){
		MultiTrigger * mt = (MultiTrigger*)(seedList->At(i));
		newtree->Branch(mt->getName(),&(mt->pass));
	}

	int nEntries = tree->GetEntries();
	cout << "Running over " << nEntries << " events." << endl;
	for (int i = 0; i < nEntries; i++){
		printProgress(nEntries,i);
		tree->GetEntry(i);
					
		// set the new tree vars to default values
		evt->resetNewVars();
		int prod_req = 0;
		for(int j = 0; j < 20; j++){
			if (evt->JET_pt[j]/1000. >= 10 && fabs(evt->JET_eta[j]) < 3.2)
				evt->HT += evt->JET_pt[j]/1000.;
			if (evt->JET_pt[j]/1000. >= 15 && fabs(evt->JET_eta[j]) < 2.0)
				evt->HTC += evt->JET_pt[j]/1000.;
			if (evt->jet_pt[j]/1000. >= 10 && fabs(evt->jet_eta[j]) < 3.2)
		 		evt->ht += evt->jet_pt[j]/1000.;
		 	if (evt->jet_pt[j]/1000. >= 30) prod_req ++;
		 }
		
		if (prod_req >= 2)
			evt->Prod	= (evt->shatR/1000. + 200)*(evt->gaminvR/1000 - 0.000) ;
		
		all_nr = false;
		for (int j = 0; j < triggerList->GetEntries(); j++){
			bool res = ((MultiTrigger*)triggerList->At(j))->checkPass(evt);
			all_nr = all_nr || res;
		}
		for (int j = 0; j < seedList->GetEntries(); j++){
			bool res = ((MultiTrigger*)seedList->At(j))->checkPass(evt);
		}
		newtree->Fill();
	}

	newtree->Write();
	delete newfile;
}

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
