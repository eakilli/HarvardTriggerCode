/******************************
Last modified by Emma Tolley (emma.tolley@cern.ch) 15 Sept '14
    
******************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include "common_plotting_functions.hh"

TCanvas* c1;
const int nSeeds = 6;
TString seeds[] = {"4J20", "2J30_XE40", "HT200", "XE70","2J10_XE60","2J15_XE55"};
int colors[] = {kRed, kCyan+1, kSpring-5, kOrange-3,kViolet+1,kBlack};
TNtuple * razor_rates;
TNtuple * met_rates;
vector<TGraph*> graphs;
TGraph * met;
TGraph * metpt;
map<TGraph*,TGraph*> points;
TTree * effTree;
TGraph * dummy ;
//TLegend legend(0.15,0.7,0.38,0.95);
TLegend legend(0.7,0.6,0.85,0.85);
bool unique = false;
		       							
void rate_eff_plot(){
	//setstyle();
	legend.Clear();
	legend.SetBorderSize(1);
  legend.SetFillColor(0);
	gStyle->SetOptStat(0);
	
	c1 = new TCanvas("c1", "First canvas", 1200, 800);
	
	//makeRatePlot("WimpPair","WimpPair_D5_DM400_QCUT80");
 	//makeRatePlot("WimpPair","WimpPair_D5_DM50_QCUT80");
	//makeRatePlot("SS_direct","SS_direct_1125_975");
	makeRatePlot("GG_direct","GG_direct_1125_975");
	//makeRatePlot("T1","Tt_T700_L500");
}

/*************************************************************
Rate vs Efficiency code
**************************************************************/
void makeRatePlot(TString dir,TString samplename){
	setEffTree(dir, samplename);
	
	TFile * fRate;
	if (unique) fRate = new TFile("../HLTRates_wRVARS/unique_rate_info.root","read");
	else fRate = new TFile("../HLTRates_wRVARS/rate_info.root","read");
	razor_rates = (TNtuple*)fRate->Get("razor_rates");
	met_rates = (TNtuple*)fRate->Get("met_rates");
	
	met = makeRateGraph(met_rates,"xe_pt/1000 >= ","XE70",kAzure+7,100,20);
	
	for (int i = 0; i < nSeeds; i++)
		 graphs.push_back(makeRateGraph(razor_rates,"Prod >= ",seeds[i],colors[i],200));

	//dummy plot to set up canvas
	float y_dummy [] = {0,130};
	float x_dummy [] = {0.009,1.0};
	dummy = new TGraph(2,x_dummy,y_dummy);
	dummy->SetTitle("");
	dummy->Draw("p a");
	dummy->SetMarkerColor(0);
	dummy->GetXaxis()->SetTitle("Efficiency");
	if (unique) dummy->GetYaxis()->SetTitle("Unique Rate (Hz)");
	else dummy->GetYaxis()->SetTitle("Rate (Hz)");
	
	//draw the plots & markers
	for (int i = 0; i < graphs.size(); i++){
		graphs.at(i)->Draw("l same")  ;
		points[graphs.at(i)]->Draw("p same");
		legend.AddEntry(graphs.at(i),"ProdR*_"+seeds[i]);
	}
	met->Draw("l same");
	points[met]->Draw("p same");
	legend.AddEntry(met,"xe*_XE70");
	
	//legend.AddEntry(points200.at(points200.size()-1),"ProdR > 200 GeV");
	//legend.AddEntry(points225.at(points225.size()-1),"ProdR > 225 GeV");
	//legend.AddEntry(points175.at(points175.size()-1),"ProdR > 175 GeV");
	legend.Draw();
	//filter string
 	TLatex l; 
  l.SetNDC();
  l.SetTextColor(kAzure+1);
  l.SetTextSize(0.03);
  l.DrawLatex(0.1, 0.02, "Filter: 2 Jets with p_{T} > 30 GeV");
  
  //title
  TLatex lt; 
  lt.SetNDC();
  lt.SetTextColor(kBlack);
  lt.SetTextSize(0.03);
  lt.DrawLatex(0.40, 0.92, "Efficiency vs Rate for " +getSampleName(samplename));
}

TGraph * makeRateGraph(TNtuple * rates, TString tag, TString seed, int color, int num, int mark = 22){
	 drawTag = "v:" + seed;
	 rates->Draw(drawTag,"","goff");
	 int n = rates->GetSelectedRows();
	 double * eff = getEff(tag, seed,rates->GetV1(),n);
	 TGraph *gr = new TGraph(n,eff,rates->GetV2());
	 gr->SetLineColor(color);
	 gr->SetFillColor(0);
	 gr->SetMarkerColor(0);
	 gr->SetLineWidth(3);
	 
		if (num >0 ){
	 		//add markers denoting ProdR threshold values
	 		TGraph* marker;
		 for (int j = 0; j < n; j++){
			 	float r = rates->GetV2()[j];
			 	float e = eff[j];
			 	if (rates->GetV1()[j] == num)
			 		marker = makeMarkerPoint( r, e, color, mark);
			 	if (rates->GetV1()[j]  == 200) cout << tag <<" " <<e << " " <<r << endl;
			}
			points[gr] = marker;
	 }
	 
	 return gr;

}

//plot helper function
TGraph* makeMarkerPoint( float rate, float eff, int c, int m){
		 		float r_d [] = {rate};
		 		float r_e [] = {eff};
		 		TGraph *pt = new TGraph(1,r_e,r_d);
		 		//formatMarker(pt, colors[i], 20);
		 		pt->SetMarkerColor(c);
		 		pt->SetMarkerStyle(m);
		 		pt->SetMarkerSize(2);
		 		pt->SetFillColor(0);
		 		pt->SetLineColor(0);
		 		return pt;
}

//load tree used to calculate efficicencies
void setEffTree(TString sample_dir, TString sample_tag){
	TString basedir = "/n/atlasdata1/etolley/Trigger/SAMPLES_var/";
	TFile * f = new TFile(basedir+sample_dir+"/triggerInfo_"+sample_tag+".root");
	effTree = (TTree*)f->Get("physics");
}

//get list of efficicencies for a set of threshold values and a single seed
double * getEff(TString var, TString seed, double * vals, int n){
	double * effs = new double[n];
	for (int i = 0; i < n; i++){
		TString tag = var;
		tag+= floor(vals[i]);
		float nPass = effTree->GetEntries(tag + " && " + seed + "&& 2j30");
		float nTotal = effTree->GetEntries("2j30");
		effs[i] = nPass/nTotal;
	}
	return effs;
}
