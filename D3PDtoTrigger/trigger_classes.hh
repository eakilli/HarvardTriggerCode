#include <iostream> // add
#include <TROOT.h>
#include <TChain.h>
#include <TLegend.h>
#include <TObject.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TSystem.h>
#include <vector>
#include <cmath>
/******************
* Event Container Class
*******************/
using namespace std;

class EventInfoContainer{

	public:

	// Tree variables
	Float_t jet_pt[20];
	Float_t jet_eta[20];
	Float_t jet_phi[20];
	
	bool bjetbranch;
	Float_t bjet_pt[20];
	Float_t bjet_eta[20];
	Float_t bjet_phi[20];

	Float_t JET_pt[20];
	Float_t JET_eta[20];
	Float_t JET_phi[20];
	Float_t jet_MV1[20];
	
	Float_t xe_pt;
	Float_t xe_phi;
	Float_t XE_pt;
	Float_t XE_phi;

	Float_t	shatR;
	Float_t	cosptR;
	Float_t	mdeltaR;
	Float_t gaminvR;
	Float_t dphivbR;

	// New variables
	Float_t Prod;
	Float_t	HT;
	Float_t HTC;
	Float_t	ht;
	
	EventInfoContainer(){
		bjetbranch = false;
		resetNewVars();
	}
	~EventInfoContainer(){}
	
	void resetNewVars(){
		Prod = 0;
		HT = 0;
		HTC = 0;
		ht = 0;
	}
	
	void setBranchVars(TTree * tree){
		tree->SetBranchAddress("jet_pt",&jet_pt);
		tree->SetBranchAddress("jet_eta",&jet_eta);
		tree->SetBranchAddress("jet_phi",&jet_phi);
		tree->SetBranchAddress("JET_pt",&JET_pt);
		tree->SetBranchAddress("JET_eta",&JET_eta);
		tree->SetBranchAddress("JET_phi",&JET_phi);
		
		if (tree->GetBranch("jet_MV1") == NULL){
			tree->SetBranchAddress("bjet_pt",&bjet_pt);
			tree->SetBranchAddress("bjet_eta",&bjet_eta);
			tree->SetBranchAddress("bjet_phi",&bjet_phi);
			bjetbranch = true;
		}
		else tree->SetBranchAddress("jet_MV1",&jet_MV1);
		
		tree->SetBranchAddress("xe_pt",&xe_pt);
		tree->SetBranchAddress("xe_phi",&xe_phi);
		tree->SetBranchAddress("XE_pt",&XE_pt);
		tree->SetBranchAddress("XE_phi",&XE_phi);
		
		tree->SetBranchAddress("shatR",&shatR);
		tree->SetBranchAddress("cosptR",&cosptR);
		tree->SetBranchAddress("mdeltaR",&mdeltaR);
		tree->SetBranchAddress("gaminvR",&gaminvR);
		tree->SetBranchAddress("dphivbR",&dphivbR);
	}
	void setNewVars(TTree * newtree){
		newtree->Branch("HT",	  &HT);
		newtree->Branch("HTC",  &HTC);
		newtree->Branch("ht",	  &ht);
		newtree->Branch("Prod",	&Prod);
	}
	
	//ClassDef(EventInfoContainer, 1);
};

/******************
* Jet Classes
*******************/


class Trigger: public TObject{
	public:
		Trigger(){}
		~Trigger(){}
		virtual bool passTrigger(EventInfoContainer * evt)=0;
		virtual TString getName() =0;//{return "super class name";};
		virtual TString getTreeTag() =0;//{return "super class name";};
	//ClassDef(Trigger, 1);
};

class MetTrigger: public Trigger{
	public:
	TString type;
	TString name;
	TString tag;
	int threshold;
	MetTrigger(TString inname){
		name = inname;
		if (name.First("X") >= 0){
			type = "XE";
			TString t = name(2,10);
			threshold = t.Atoi();
		}
		else if (name.First("x") >= 0){
			type = "xe";
			TString t = name(2,10);
			threshold = t.Atoi();
		}

		else cout << "Cannot parse Trigger Name" << endl;
		
		setTreeTag();
	}
	~MetTrigger(){}
	TString getName(){return name;}
	void setTreeTag(){
		tag = "";
		if (type == "XE") tag += "XE_pt/1000. > ";
		else if (type == "xe") tag += "XE_pt/1000. > ";
		tag += threshold;
	}
	TString getTreeTag(){
		return tag;
	}
	bool passTrigger(EventInfoContainer * evt){
		if (type == "XE") return evt->XE_pt/1000. > threshold;
		else if (type == "xe") return evt->xe_pt/1000. > threshold;
		else return false;
	}
};

class MetJetTrigger: public Trigger{
	public:
	TString type;
	TString name;
	TString tag;
	
	int met_thres;
	int jet_thres;
	int dphi_thres;
	float dphi_sep;
	
	MetJetTrigger(TString inname){
		name = inname;
		if (name.First("X") >= 0){
			type = "XE";
			met_thres  = getTagVal("XE");
			jet_thres  = getTagVal("J");
			dphi_thres = getTagVal("DPHI");
		}
		else if (name.First("x") >= 0){
			type = "xe";
			met_thres  = getTagVal("xe");
			jet_thres  = getTagVal("j");
			dphi_thres = getTagVal("dphi");
		}
		else cout << "Cannot parse Trigger Name" << endl;
		dphi_sep = dphi_thres/10.;
		
		setTreeTag();
	}
	int getTagVal(TString tag){
		int first = name.First(tag);
		if (first < 0 ) return 0;
		first += tag.Length();
		int end = name.Index("_",first);
		if (end < 0) end = 10;
		TString t = name(first,end-first);
		return t.Atoi();
	}
	float dphi(float phi1, float phi2){
		float d = fabs(phi1-phi2);
		if (d > 3.14159) return 2*3.14159 - d;
		else return d;
	}
	
	TString getName(){return name;}
	void setTreeTag(){
			tag = "";
			if (type == "XE"){
			tag = "XE_pt/1000 >";
			tag += met_thres;
			tag += "JET_pt[0]/1000 >";
			tag += jet_thres;
		}
		else if (type == "xe"){
			tag = "xe_pt/1000 >";
			tag += met_thres;
			tag += "jets_pt[0]/1000 >";
			tag += jet_thres;
		}
	}
	TString getTreeTag(){
		return tag;
	}
	
	bool passTrigger(EventInfoContainer * evt){
		if (type == "XE"){
			if ( evt->XE_pt/1000. < met_thres) return false;
			for(int i = 0; i < 20; i++){
				if(evt->JET_pt[i]/1000. > jet_thres
					&& dphi(evt->JET_phi[i],evt->XE_phi) > dphi_sep)
					return true;
			}
		}
		else if (type == "xe"){
			if ( evt->xe_pt/1000. < met_thres) return false;
			for(int i = 0; i < 20; i++){
				if(evt->jet_pt[i]/1000. > jet_thres
					&& dphi(evt->jet_phi[i],evt->xe_phi) > dphi_sep)
					return true;
			}
		}
		return false;
	}
};

class HtTrigger: public Trigger{
	public:
	TString type;
	TString name;
	TString tag;
	int threshold;
	HtTrigger(TString inname){
		name = inname;
		if (name.First("H") >= 0){
			type = "HT";
			TString t = name(2,10);
			threshold = t.Atoi();
		}
		else if (name.First("h") >= 0){
			type = "ht";
			TString t = name(2,10);
			threshold = t.Atoi();
		}
		else cout << "Cannot parse Trigger Name" << endl;
		setTreeTag();
	}
	TString getName(){return name;}
	void setTreeTag(){
		tag = "";
		if (type == "HT") tag = "HT >";
		else if (type == "ht") tag = "ht >" ;
		tag += threshold;
	}
	TString getTreeTag(){
		return tag;	
	}
	bool passTrigger(EventInfoContainer * evt){
		if (type == "HT") return evt->HT > threshold;
		else if (type == "ht") return evt->ht > threshold;
		else return false;
	}
};

class RazorTrigger: public Trigger{
	public:
	TString type;
	TString name;
	TString tag;
	int threshold;
	int par1;
	int par2;
	RazorTrigger(TString inname){
		name = inname;
		if (name(0,5) == "ProdR"){
			type = "ProdR";

			if (name.First('.') > 0 && name.Last('.') != name.First('.')){
				threshold = TString(name(5,name.First('.')-5)).Atoi();
				par1 = TString(name(name.First('.')+1,name.Last('.')-name.First('.')-1)).Atoi();
				par2 = TString(name(name.Last('.')+1,10)).Atoi();
				
			}else{
				threshold = TString(name(5,10)).Atoi();
				par1 = 0;
				par2 = 0;
			}
		}
		else cout << "Cannot parse Trigger Name" << endl;
		setTreeTag();
	}
	TString getName(){return name;}
	void setTreeTag(){
		tag = "(shatR/1000. +(";
		tag +=   par1;
		tag += "))*(gaminvR/1000. + (";
		tag += par2;
		tag += ")/1000.) > ";
		tag += threshold;
	}
	TString getTreeTag(){
		return tag;
	}
	bool passTrigger(EventInfoContainer * evt){
		return (evt->shatR/1000. + par1)*(evt->gaminvR/1000. + par2/1000.) > threshold;
	}
};

class JetTrigger: public Trigger{
	public:
	int n;
	int pt;
	float eta;
	int word_eta;
	TString type;
	TString name;
	TString tag;

	JetTrigger(TString inname){
		name = inname;
		readName();
		setTreeTag();
		//cout << n << "j" << pt << " eta" << eta << endl; 
	}
	TString getName(){
	return name;
	}
	
	void setTreeTag(){
		tag = "1";
		if (type == "J"){
			for (int i = n-1; i >= 0; i--){
				TString temp= "JET_pt[";
				temp += i;
				temp += "]/1000 > ";
				temp += pt;
				tag += "&&" + temp;
			}
		}
		else if (type == "b"){
			for (int i = n-1; i >= 0; i--){
							TString temp= "bjet_pt[";
				temp += i;
				temp += "]/1000 > ";
				temp += pt;
				tag += "&&" + temp;
			}
		}
		else if (type == "j"){
			for (int i = n-1; i >= 0; i--){
				TString temp= "jet_pt[";
				temp += i;
				temp += "]/1000 > ";
				temp += pt;
				tag += "&&" + temp;
			}
		}
	}
	
	TString getTreeTag(){

		return tag;
	}
	
	void readName(){
		if (name.First("J") >= 0){
			type = "J"; parseJetTag();
		}
		else if (name.First("j") >= 0){
			type = "j"; parseJetTag();
		}
		else if (name.First("b") >= 0){
			type = "b"; parseJetTag();
		}
		else cout << "Cannot parse Trigger Name" << endl;
	}
	
	void parseJetTag(){
		int start = name.First(type) + 1;
		if (start > 1){ TString s = name(0); n = s.Atoi();}
		else n = 1;
		int end = name.First("_");
		if (end < 0) end = name.Length();
		TString pts = name(start,end-start);
		pt = pts.Atoi();
		
		if (name.First("eta") > 0){
			TString s = name(name.First("eta") +3,name.Length());
			word_eta = s.Atoi();
		}else word_eta = 0;
		seteta();
	}
	
	void seteta(){
		eta = 10.;
		if (word_eta > 0){
			eta = word_eta;
			while (eta > 10) eta /= 10.;
		}
	}
	

	bool passJET(EventInfoContainer * evt){
		int count = 0;
		for(int i = 0; i < 20; i++){
			if(evt->JET_pt[i]/1000. > pt
				&& fabs(evt->JET_eta[i]) < eta)
				count++;
				if(count >= n) return true;
		}
		return false;
	}
	bool passjet(EventInfoContainer * evt){
		int count = 0;
		for(int i = 0; i < 20; i++){
			if(evt->jet_pt[i]/1000. > pt
				&& fabs(evt->jet_eta[i]) < eta)
				count++;
				if(count >= n) return true;
		}
		return false;
	}
		bool passbjet(EventInfoContainer * evt){
		int count = 0;
		if (evt->bjetbranch){
			for(int i = 0; i < 20; i++){
				if(evt->bjet_pt[i]/1000. > pt
					&& fabs(evt->bjet_eta[i]) < eta)
					count++;
					if(count >= n) return true;
			}
		}else{
			for(int i = 0; i < 20; i++){
				if(evt->jet_pt[i]/1000. > pt
					&& fabs(evt->jet_eta[i]) < eta
					&& evt->jet_MV1[i] > 0.7892)
					count++;
					if(count >= n) return true;
			}
		}
		return false;
	}
	bool passTrigger(EventInfoContainer * evt){
		if (type == "J") return passJET(evt);
		else if (type == "b") return passbjet(evt);
		else if (type == "j") return passjet(evt);
		cout << "Jet Type not implemented" << endl;
		return false;
	}
	//ClassDef(JetTrigger, 1);
};

class MultiTrigger: public TObject{
	
	public:
	bool pass;
	TObjArray * triggers;
	
	MultiTrigger(TString tag1, TString tag2="",TString tag3="",TString tag4=""){
		vector<TString> triggerTags;
		triggerTags.push_back(tag1);
		if (tag2.Length() > 0) triggerTags.push_back(tag2);
		if (tag2.Length() > 0)triggerTags.push_back(tag3);
		if (tag2.Length() > 0)triggerTags.push_back(tag4);
		BuildMultiTrigger(triggerTags);
	}
	void BuildMultiTrigger(vector<TString> triggerTags){
		triggers = new TObjArray();
		pass = false;
		for (int i = 0; i < triggerTags.size(); i++){
			TString tag = triggerTags[i];
			//Jet + met trigger
			if ( 		 (tag.Contains("dphi") && tag.Contains("j") && tag.Contains("xe"))
						|| (tag.Contains("DPHI") && tag.Contains("J") && tag.Contains("XE")))
						triggers->Add( new MetJetTrigger(tag));
			// Jet or bjet trigger
			else if (	 tag.Contains("b")
					|| tag.Contains("j")
					|| tag.Contains("J")) triggers->Add(new JetTrigger(tag));
			//met rigger
			else if (	 tag.Contains("XE")
					|| tag.Contains("xe")) triggers->Add(new MetTrigger(tag));
			// ht trigger
			else if (tag.Contains("ht")|| tag.Contains("HT"))
				triggers->Add(new HtTrigger(tag));
			//razor trigger
			else if (tag.Contains("Prod")) triggers->Add(new RazorTrigger(tag));
		}
		//cerr << ((Trigger*)triggers->At(0))->getName() << endl;
	}
	TString getName(){
		TString name = "";
		for (int i = 0; i < triggers->GetEntries(); i++){
			name += "_" + ((Trigger*)triggers->At(i))->getName();
		}
		return name(1,name.Length());
	}
		
	TString getTreeTag(){
		TString tag = "";
		for (int i = 0; i < triggers->GetEntries(); i++){
			tag += "&&" + ((Trigger*)triggers->At(i))->getTreeTag();
		}
		return tag(2,tag.Length());
	}
	bool checkPass(EventInfoContainer * evt){
		pass = true;
		for (int i = 0; i < triggers->GetEntries(); i++){
			pass &= ((Trigger*)triggers->At(i))->passTrigger(evt);
		}
		return pass;
	}
	//ClassDef(MultiTrigger, 1);
};
