/******************************
Last modified by Emma Tolley (emma.tolley@cern.ch) 15 Sept '14
    
******************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include "common_plotting_functions.hh"

TCanvas* c1;
const int nSeeds = 6;
TString seeds[] = {"4J20", "2J30_XE40", "HT200", "XE70","2J10_XE60","2J15_XE55"};
int colors[] = {kRed, kAzure+1, kTeal-8, kOrange-3,kViolet+1,kBlack};
TNtuple * rates;
vector<TGraph*> graphs;
vector<TGraph*> points200;
vector<TGraph*> points225;
vector<TGraph*> points175;
TTree * effTree;
TGraph * dummy ;
TLegend legend(0.15,0.7,0.38,0.95);
bool unique = true;
		       							
void rate_eff_plot(){
	//setstyle();
	legend.Clear();
	legend.SetBorderSize(1);
  legend.SetFillColor(0);
	gStyle->SetOptStat(0);
	
	c1 = new TCanvas("c1", "First canvas", 1200, 800);
	
	 makeRatePlot("WimpPair","WimpPair_D5_DM400_QCUT80");
 	//makeRatePlot("WimpPair","WimpPair_D5_DM50_QCUT80");
	//makeRatePlot("SS_direct","SS_direct_1125_975");
	//makeRatePlot("GG_direct","GG_direct_1125_975");
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
	rates = (TNtuple*)fRate->Get("rates");
	
	for (int i = 0; i < nSeeds; i++){
		 drawTag = "v:" + seeds[i];
		 rates->Draw(drawTag,"","goff");
		 int n = rates->GetSelectedRows();
		 double * eff = getEff(seeds[i],rates->GetV1(),n);
		 
		 //add markers denoting ProdR threshold values
		 for (int j = 0; j < n; j++){
		 	float r = rates->GetV2()[j];
		 	float e = eff[j];
		 	if (rates->GetV1()[j] == 200)
		 		points200 = makeMarkerPoint(points200, r, e, colors[i], 22);
		 	else if (rates->GetV1()[j] == 225)
		 		points225 = makeMarkerPoint(points225, r, e, colors[i], 21);
		 	else if (rates->GetV1()[j] == 175)
		 		points175 = makeMarkerPoint(points175, r, e, colors[i], 20);	
		 }
		 
		 //make eff vs rate plot
		 TGraph *gr = new TGraph(n,rates->GetV2(),eff);
		 gr->SetLineColor(colors[i]);
		 gr->SetFillColor(0);
		 gr->SetLineWidth(2);
		 graphs.push_back(gr);
	}  
	
	//dummy plot to set up canvas
	float x_dummy [] = {0,150};
	float y_dummy [] = {0.009,1.0};
	dummy = new TGraph(2,x_dummy,y_dummy);
	dummy->SetTitle("");
	dummy->Draw("p a");
	dummy->SetMarkerColor(0);
	dummy->GetYaxis()->SetTitle("Efficiency");
	if (unique) dummy->GetXaxis()->SetTitle("Unique Rate (Hz)");
	else dummy->GetXaxis()->SetTitle("Rate (Hz)");
	
	//draw the plots & markers
	for (int i = 0; i < graphs.size(); i++){
		graphs.at(i)->Draw("l same")  ;
		points200.at(i)->Draw("p same")  ;
		points225.at(i)->Draw("p same")  ;
		points175.at(i)->Draw("p same")  ;
		legend.AddEntry(graphs.at(i),seeds[i]);
	}
	legend.AddEntry(points200.at(points200.size()-1),"ProdR > 200 GeV");
	legend.AddEntry(points225.at(points225.size()-1),"ProdR > 225 GeV");
	legend.AddEntry(points175.at(points175.size()-1),"ProdR > 175 GeV");
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

//plot helper function
vector<TGraph*> makeMarkerPoint(vector<TGraph*>  points, float rate, float eff, int c, int m){
		 		float r_d [] = {rate};
		 		float r_e [] = {eff};
		 		TGraph *pt = new TGraph(1,r_d,r_e);
		 		//formatMarker(pt, colors[i], 20);
		 		pt->SetMarkerColor(c);
		 		pt->SetMarkerStyle(m);
		 		pt->SetMarkerSize(2);
		 		pt->SetFillColor(0);
		 		pt->SetLineColor(0);
		 		points.push_back(pt);
		 		return points;
}

//load tree used to calculate efficicencies
void setEffTree(TString sample_dir, TString sample_tag){
	TString basedir = "/n/atlasdata1/etolley/Trigger/SAMPLES_var/";
	TFile * f = new TFile(basedir+sample_dir+"/triggerInfo_"+sample_tag+".root");
	effTree = (TTree*)f->Get("physics");
}

//get list of efficicencies for a set of threshold values and a single seed
double * getEff(TString seed, double * vals, int n){
	double * effs = new double[n];
	for (int i = 0; i < n; i++){
		TString tag = "Prod >= ";
		tag+= floor(vals[i]);
		effs[i] = effTree->GetEntries(tag + " && " + seed + "&& 2J30")*1.0/effTree->GetEntries("2J30");
	}
	return effs;
}
