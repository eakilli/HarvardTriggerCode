#include "../D3PD/trigger_classes.hh"
#include <vector>
#include "TStopwatch.h"
TObjArray * triggerMenu;
TObjArray * triggerList;
TString menu_string;
//bsub -q pleiades -Is /bin/bash
using namespace std;

enum FileType{ DATA8, JETS14, W8, Z8, TT8, W14, Z14, TT14};

void compute_rates();
TObjArray * computePassingTriggers_helper(TString filename, TObjArray* rate_info, float weight, int i_jobs = 0, int n_jobs = 1);
TObjArray * computePassingTriggers(TString filename, FileType f, float weight, int i_jobs = 0, int n_jobs = 1);
void makeTriggerList();
void makeTriggerMenu();
void printProgress(int n, int i);

class RateObject: public TObject{
	public:
	float rate;
	float err;
	float unique_rate;
	float unique_err;
	FileType f;
	TString trigger;
	RateObject(FileType inf, TString intrig = ""){
		rate = 0;
		err = 0;
		unique_rate = 0;
		unique_err = 0;
		f = inf;
		trigger = intrig;
	}
	//ClassDef(RateObject, 1);
};

int main(int argc, char* argv[]) {
	compute_rates();
}

void compute_rates(){
	TStopwatch watch; watch.Start();
	makeTriggerMenu();
	makeTriggerList();
	
	menu_string = "1";
	for (int j = 0; j < triggerMenu->GetEntries(); j++){
		menu_string += "&&" + ((MultiTrigger*)triggerMenu->At(j))->getTreeTag();
	}
	
	TString prefix = "SAMPLES/triggerInfo_";
	float eb_weight = 0.05*0.96*425810./(500.*992.) * 7.285e4*10;
	TObjArray * eb_rates = computePassingTriggers(prefix+"eb_re1.root",DATA8,eb_weight);
	
	float jets_weight = 0.80*2.7006e3*10*7.7499E-02*(206615+206627+206620)/(5000.*5.*30.)/3.;
	TObjArray * jets_rates = computePassingTriggers(prefix+"jf17_14TeV*_bcid_re1.root", JETS14, jets_weight);
	
	TString mc8TeV = "_8TeV_bcid_re1.root";
	TObjArray * w8pe_rates = computePassingTriggers(prefix+"Wplusenu"  + mc8TeV, W8, 6.8912E-3*10);
	TObjArray * w8me_rates = computePassingTriggers(prefix+"Wminusenu" + mc8TeV, W8, 4.7904E-3*10);
	TObjArray * w8mm_rates = computePassingTriggers(prefix+"Wminusmunu"+ mc8TeV, W8, 4.7904E-3*10);
	TObjArray * w8me_rates = computePassingTriggers(prefix+"Wplusmunu" + mc8TeV, W8, 6.8912E-3*10);
	TObjArray * z8e_rates  = computePassingTriggers(prefix+"Zee"       + mc8TeV, Z8, 1.1099E-3*10);
	TObjArray * z8m_rates  = computePassingTriggers(prefix+"Zmumu"     + mc8TeV, Z8, 1.1098E-3*10);	
	TObjArray * tt8_rates  = computePassingTriggers(prefix+"ttbar"     + mc8TeV, TT8,2.0813E-4*10 * 5.4263E-01);
	
	TString mc14TeV = "_bcid_re1.root";
	TObjArray * w14pe_rates = computePassingTriggers(prefix+"Wplusenu"  + mc14TeV, W14, 1.2160E-2*10);
	TObjArray * w14me_rates = computePassingTriggers(prefix+"Wminusenu" + mc14TeV, W14, 8.9770E-3*10);
	TObjArray * w14mm_rates = computePassingTriggers(prefix+"Wminusmunu"+ mc14TeV, W14, 1.2160E-2*10);
	TObjArray * w14pm_rates = computePassingTriggers(prefix+"Wplusmunu" + mc14TeV, W14, 8.9770E-3*10);
	TObjArray * z14e_rates  = computePassingTriggers(prefix+"Zee"       + mc14TeV, Z14, 2.0570e-3*10);
	TObjArray * z14m_rates  = computePassingTriggers(prefix+"Zmumu"     + mc14TeV, Z14, 2.0580E-3*10);
	TObjArray * tt14_rates  = computePassingTriggers(prefix+"ttbar"     + mc14TeV, TT14,8.1226E-4*10 * 5.4274E-01);

	cout << eb_rates->GetEntries() << endl;
	for (int i = 0; i < eb_rates->GetEntries(); i++){
		RateObject*rd = (RateObject*)eb_rates->At(i);
		RateObject*rj = (RateObject*)jets_rates->At(i);
		RateObject*w8 = (RateObject*)w8_rates->At(i);
		RateObject*z8 = (RateObject*)z8_rates->At(i);
		RateObject*tt8 = (RateObject*)tt8_rates->At(i);
		RateObject*w14 = (RateObject*)w14_rates->At(i);
		RateObject*z14 = (RateObject*)z14_rates->At(i);
		RateObject*tt14 = (RateObject*)tt14_rates->At(i);
		cout << " d:    " << rd->rate << endl;
		cout << " j:    " << rj->rate << endl;
		cout << " w8:   " << w8->rate << endl;
		cout << " z8:   " << z8->rate << endl;
		cout << " tt8:  " << tt8->rate << endl;
		cout << " w14:  " << w14->rate << endl;
		cout << " z14:  " << z14->rate << endl;
		cout << " tt14: " << tt14->rate << endl;
		cout << rd->trigger << " " << rj->rate*(rd->rate - w8->rate - z8->rate - tt8->rate) + w14->rate + z14->rate + tt14->rate << endl;
	}
	watch.Stop();
	watch.Print();
}

TObjArray * computePassingTriggers_helper(TString filename, TObjArray* rate_info, float weight, int i_jobs, int n_jobs){
	TChain * t = new TChain("physics");
	t->Add(filename);
	weight = 1.0;

	for (int j = 0; j < triggerList->GetEntries(); j++){
		MultiTrigger*trigger =(MultiTrigger*)triggerList->At(j); 
		RateObject* r = (RateObject*)rate_info->At(j);
		r->rate 			 += weight*t->GetEntries(trigger->getTreeTag());
		r->unique_rate += weight*t->GetEntries(trigger->getTreeTag()+"&& all_norazor");
	}

	
	/*EventInfoContainer * evt = new EventInfoContainer();
	evt->setBranchVars(t);
	int nEntries = t->GetEntries();
	int start = ceil(i_jobs*1.0/n_jobs*nEntries);
	int end = ceil((i_jobs+1)*1.0/n_jobs*nEntries)-1;
	cout << filename << endl;
	for (int i = start; i <= end; i++){
		t->GetEntry(i);
		//printProgress(end-start,i-start);
		bool pass_menu = true;
		//for (int j = 0; j < triggerMenu->GetEntries(); j++){
		//	pass_menu &= ((MultiTrigger*)triggerMenu->At(j))->checkPass(evt);
		//}
		for (int j = 0; j < triggerList->GetEntries(); j++){
			bool pass = ((MultiTrigger*)triggerList->At(j))->checkPass(evt);
			if (pass){
				RateObject* r = (RateObject*)rate_info->At(j);
				r->rate += weight;
				if (pass_menu) r->unique_rate += weight;
			}
		}
	}
	cout << effrate << " " << ((RateObject*)rate_info->At(0))->rate << endl;*/
	return rate_info;
}

TObjArray * computePassingTriggers(TString filename, FileType f, float weight, int i_jobs, int n_jobs){
	TObjArray * rate_info = new TObjArray();
	if(i_jobs >= n_jobs){
		cout << "Error setting job range";
		return rate_info;
	}
	for (int i = 0; i < triggerList->GetEntries(); i++)
		rate_info->Add(new RateObject(f,((MultiTrigger*)triggerList->At(i))->getName()));
	return computePassingTriggers_helper(filename, rate_info,weight,i_jobs,n_jobs);
}

/*             
        dataScaled[trigger][0] =    
        if (datarate8TeV[trigger][0]-ratesW8TeV[trigger][0]-ratesZ8TeV[trigger][0]-ratesttbar8TeV[trigger][0])>0.
        	ratesScaledJets[trigger][0]*((datarate8TeV[trigger][0]-ratesW8TeV[trigger][0]-ratesZ8TeV[trigger][0]-ratesttbar8TeV[trigger][0]) 
        else 0.)     
        +ratesW14TeV[trigger][0]+ratesZ14TeV[trigger][0]+ratesttbar14TeV[trigger][0]
*/

void makeTriggerList(){
	triggerList = new TObjArray();
	/*for (int i = 100; i < 300; i+=10){
		TString p = "ProdR";
		p += i;
		triggerList->Add(new MultiTrigger(p, "2J15","XE55"));
	}*/
	triggerList->Add(new MultiTrigger("4J20"));
	//triggerList->Add(new MultiTrigger("ProdR200.300.-50", "2J15","XE55"));
}

void makeTriggerMenu(){
	triggerMenu = new TObjArray();
	//Multijet Triggers
	triggerMenu->Add(new MultiTrigger("j400", 			"J100"));
	triggerMenu->Add(new MultiTrigger("3j175", 			"J100"));
	triggerMenu->Add(new MultiTrigger("4j100", 			"3J50"));
	triggerMenu->Add(new MultiTrigger("5j85", 			"4J20"));
	triggerMenu->Add(new MultiTrigger("6j50_eta25", "4J20"));
	triggerMenu->Add(new MultiTrigger("6j70", 			"4J20"));
	triggerMenu->Add(new MultiTrigger("6j50_eta25", "5J15_eta24"));
	triggerMenu->Add(new MultiTrigger("7J45", 			"6J15"));
	triggerMenu->Add(new MultiTrigger("8J40", 			"6J15"));
	
	//jets + bjets triggers
	triggerMenu->Add(new MultiTrigger("b175", "b60" , "J100"));
	triggerMenu->Add(new MultiTrigger("2b55", "2j55", "4J20"));
	triggerMenu->Add(new MultiTrigger("b75", 	"3j75", "4J20"));
	triggerMenu->Add(new MultiTrigger("2b45", "3j45", "5J15_eta24"));
	
	//Met trigger
	triggerMenu->Add(new MultiTrigger("xe100", "XE70"));
	
	//Ht trigger
	triggerMenu->Add(new MultiTrigger("ht1000", "HT200"));
	
	//Combined jet+met triggers
	triggerMenu->Add(new MultiTrigger("xe80", "j200", "J100"));
	triggerMenu->Add(new MultiTrigger("j150", "xe90", "J75", "XE40"));
	triggerMenu->Add(new MultiTrigger("b120", "xe70", "J75", "XE40"));
	triggerMenu->Add(new MultiTrigger("j175", "b70", "xe70", "J100"));
	//triggerMenu->Add(new MultiTrigger("j100_xe80_dphi10", "J40_XE50_DPHI10"));
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


