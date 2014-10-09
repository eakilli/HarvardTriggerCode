/******************************
Last modified by Emma Tolley (emma.tolley@cern.ch) 15 Sept '14
    
******************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include "common_plotting_functions.hh"
#include <TString>
#include <locale>
#include <sstream>
#include <pair>
#include <string>

TCanvas* c1;
const int nSeeds = 3;
const int nSeeds2j = 6;
TString seeds[] = {"HT200", "XE70","2J15_XE55"};
TString seeds_2j[] = {"2J15_XE55","2J20_XE50","2J25_XE45","2J30_XE40","2J40_XE35","2J45_XE30"};
int seeds_thresholds[] = {170, 140, 220};
//int seeds_thresholds[] = {200, 200, 200, 220};
float loweff = 1.0;
float higheff = 0.0;

TNtuple * razor_rates;
TNtuple * met_rates;
TNtuple * metjet_rates;
vector<TGraph*> graphs;
TGraph * met;
TGraph * metjet;
map<TGraph*,TGraph*> points;
TTree * effTree;
TGraph * dummy ;
//TLegend legend(0.15,0.7,0.38,0.95);
TLegend legend(0.60,0.6,0.90,0.85);
bool unique = false;
bool saveImg = true;
		       							
void rate_eff_plot(){
	//setstyle();
	legend.Clear();
	legend.SetBorderSize(1);
	
  legend.SetFillColor(0);
	gStyle->SetOptStat(0);
	//
	
	c1 = new TCanvas("c1", "First canvas", 1200, 800);
	c1->SetBottomMargin(0.15);
	c1->SetGridx();
	c1->SetGridy();
	
	//makeRatePlot("WimpPair","WimpPair_D5_DM400_QCUT80");
	//makeRatePlot("WimpPair","WimpPair_D5_DM50_QCUT80");
 	makeRatePlot("WimpPair","WimpPair_D5_DM50_QCUT300");
 	//makeRatePlot("SS_direct","SS_direct_1125_975");
	//makeRatePlot("GG_direct","GG_direct_1125_975");
	//makeRatePlot("GG_direct","GG_direct_900_750");
	//makeRatePlot("GG_direct","GG_direct_1050_900");
	makeRatePlot("GG_direct","GG_direct_1425_225");
	//makeRatePlot("T1","Tt_T700_L500");
	//makeRatePlot("MONOJET_B","WimpPair_D1MB_DM10_QCUT80");
	//makeRatePlot("MONOJET_B","WimpPair_D1TT_DM10_QCUT80");
}

/*************************************************************
Rate vs Efficiency code
**************************************************************/
void makeRatePlot(TString dir,TString samplename){
	legend.Clear();
	graphs.clear();
	points.clear();
	loweff = 1.0;
	higheff = 0.0;
	setEffTree(dir, samplename);
	TString seed = "2J15_XE55";//"XE70";
	//makeVarRatePlot(seed);
	makeSeedRatePlot();
	//make2JSeedRatePlot();
	legend.Draw();
	//filter string
 	TLatex l; 
  l.SetNDC();
  l.SetTextColor(kAzure+1);
  l.SetTextSize(0.03);
  l.DrawLatex(0.1, 0.02, "Filter: 2 Jets with p_{T} > 30 GeV");
  
  //TLatex l; 
  l.SetNDC();
  l.SetTextColor(kAzure+1);//l.SetTextColor(kViolet-5);
  l.SetTextSize(0.03);
  //l.DrawLatex(0.1, 0.07, "L1 seed: " + seed);
  l.DrawLatex(0.1, 0.07, "Prod = ("+getVarName("shatR")+" + 200 GeV ) * ( "+getVarName("gaminvR")+" )");
  
  //title
  TLatex lt; 
  lt.SetNDC();
  lt.SetTextColor(kBlack);
  lt.SetTextSize(0.03);
  lt.DrawLatex(0.20, 0.92, "Efficiency vs Rate for " +getSampleName(samplename));
  
  if (saveImg){
  	c1->Print("plots/rate_v_eff_seed_"+samplename+".pdf");
		c1->Print("plots/rate_v_eff_seed_"+samplename+".png");
  }
}

void makeVarRatePlot(TString seed){
	TFile * fRate = new TFile("../HLTRates_wRVARS/rate_var"+seed+"_info.root","read");
	razor_rates = (TNtuple*)fRate->Get("razor_rates");
	if (!razor_rates) cout << "No rate data found" << endl;
	vector<TString>pars;
	vector<TString>par1;		vector<TString>par2;
	par1.push_back("0");		par2.push_back("0");
	par1.push_back("85");		par2.push_back("48");
	par1.push_back("200");		par2.push_back("-50");
	par1.push_back("300");		par2.push_back("-50");
	par1.push_back("400");		par2.push_back("-50");
	//par1.push_back("0");		par2.push_back("-50");
	par1.push_back("200");		par2.push_back("0");
	par1.push_back("300");		par2.push_back("0");
	par1.push_back("400");		par2.push_back("0");
	//par1.push_back("300");		par2.push_back("-100");
	//par1.push_back("500");		par2.push_back("0");
	//par1.push_back("500");		par2.push_back("-50");
	//par1.push_back("500");		par2.push_back("-100");

	for (int i = 0; i < par1.size(); i++){
			TString p1 = par1[i];
			TString p2 = par2[i];
			pars.push_back(p1+"_"+p2);
			TString param = seed+" && (shatR/1000+(" + p1 + "))*(gaminvR/1000+ (" + p2 + ")/1000.)>=";
			std::string tag = p1+"_"+p2;
			if (tag.find("-") !=std::string::npos)
				tag[tag.find("-")] = 'n';
			graphs.push_back(makeRateGraph(razor_rates,param,tag,colors[(i)%11],200,22,false));
		
	}
	for (int i = 0; i < pars.size(); i++)
	

	float deff=higheff-loweff;
	if (higheff > 0.9){ higheff += 0.05; loweff -= 0.05;
	}
	makeDummyPlot(60,loweff-0.05-1.0*deff,higheff+0.01+0.5*deff);
	cout << loweff<<" " << higheff << endl;
	
	for (int i = 0; i < graphs.size(); i++){
		graphs.at(i)->Draw("l same")  ;
		//points[graphs.at(i)]->Draw("p same");
		legend.AddEntry(graphs.at(i),"ProdR*_"+pars[i]);
	}
	
	legend.SetX1(0.12);
	legend.SetX2(0.37);
  legend.SetY1(0.85);
	legend.SetY2(0.5);

}

void makeSeedRatePlot(){
	legend.SetNColumns(2);
	TFile * fRate;
	if (unique) fRate = new TFile("../HLTRates_wRVARS/unique_rate_info.root","read");
	else fRate = new TFile("../HLTRates_wRVARS/rate_info.root","read");
	razor_rates = (TNtuple*)fRate->Get("razor_rates");
	met_rates = 	 (TNtuple*)fRate->Get("met_rates");
	metjet_rates = (TNtuple*)fRate->Get("metjet_rates");
	
	met    = makeRateGraph(met_rates,   "xe_pt/1000 >= ",												 "XE70",		kAzure+7,100,20);
	metjet = makeRateGraph(metjet_rates,"jet_pt[0]/1000 > 150 && xe_pt/1000 >= ","J75_XE40",kBlue+2, 90, 20);
	
	for (int i = 0; i < nSeeds; i++)
		 graphs.push_back(makeRateGraph(razor_rates,"Prod >= ",seeds[i],colors[i],seeds_thresholds[i]));


	//makeDummyPlot(60,loweff-0.1-1.0*deff,min(1.0,higheff+0.15+0.5*deff));
	makeDummyPlot(60);
	
	//makeDummyPlot(70);
	//draw the plots & markers
	for (int i = 0; i < graphs.size(); i++){
		graphs.at(i)->Draw("l same")  ;
		points[graphs.at(i)]->Draw("p same");

		//temp_tag += seeds_thresholds[i];
		//cout << seeds_thresholds[i] << endl;
		//temp_tag += "_"+seeds[i];
		legend.AddEntry(graphs.at(i),"ProdR*_"+seeds[i]);
		legend.AddEntry(points[graphs.at(i)],"ProdR"+TString::Itoa(seeds_thresholds[i],10)+"_"+seeds[i]);
		
	}
	met->Draw("l same");
	points[met]->Draw("p same");
	legend.AddEntry(met,"xe*_XE70");
	legend.AddEntry(points[met],"xe100_XE70");
	
	metjet->Draw("l same");
	points[metjet]->Draw("p same");
	legend.AddEntry(metjet,"j150_xe*_J75_XE40");
	legend.AddEntry(points[metjet],"j150_xe90_J75_XE40");
	
	
	if (loweff > 0.5){
		legend.SetX1(0.12);
		legend.SetX2(0.50);
	  legend.SetY1(0.85);
		legend.SetY2(0.65);
		cout << loweff << endl;
	}else{
		legend.SetX1(0.12+0.49);
		legend.SetX2(0.50+0.49);
	  legend.SetY1(0.85-0.45);
		legend.SetY2(0.65-0.45);
	}
	legend.SetTextSize(0.022);
}

void make2JSeedRatePlot(){
	TFile * fRate;
	if (unique) fRate = new TFile("../HLTRates_wRVARS/unique_rate_info.root","read");
	else fRate = new TFile("../HLTRates_wRVARS/rate_info.root","read");
	razor_rates = (TNtuple*)fRate->Get("razor_rates");
	
	for (int i = 0; i < nSeeds2j; i++)
		 graphs.push_back(makeRateGraph(razor_rates,"Prod >= ",seeds_2j[i],colors[i],200));



	makeDummyPlot(70);
	//draw the plots & markers
	for (int i = 0; i < graphs.size(); i++){
		graphs.at(i)->Draw("l same")  ;
		legend.AddEntry(graphs.at(i),"ProdR*_"+seeds_2j[i]);
	}
}

void makeDummyPlot(float y_high = 200, float x_low = 0, float x_high = 1.0){
	//dummy plot to set up canvas
	float y_dummy [] = {0,y_high};
	float x_dummy [] = { x_low,x_high};
	dummy = new TGraph(2,x_dummy,y_dummy);
	dummy->SetTitle("");
	dummy->Draw("p a");
	dummy->SetMarkerColor(0);
	dummy->GetXaxis()->SetTitle("Efficiency");
	if (unique) dummy->GetYaxis()->SetTitle("Unique Rate (Hz)");
	else dummy->GetYaxis()->SetTitle("Rate (Hz)");
}

TGraph * makeRateGraph(TNtuple * rates, TString tag, TString seed, int color, int num, int mark = 22,bool useSeed = true){
	 TString drawTag = "v:" + seed;
	 rates->Draw(drawTag,"","goff");
	 int n = rates->GetSelectedRows();
	 double * eff;
	 if (!useSeed)
	 	eff = getEff(tag, "1",rates->GetV1(),n);
	 else
	  eff= getEff(tag, seed,rates->GetV1(),n);
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
			 	// << seed << " th=" << rates->GetV1()[j] <<" r="<< r << " e=" << e << endl;
			 	if (rates->GetV1()[j] == num){
			 		marker = makeMarkerPoint( r, e, color, mark);
			 		loweff = min(e,loweff);
			 		higheff = max(e,higheff);
			 		cout << tag << " " << num<<", e=" <<e << ", r=" <<r << endl;
			 	}

			 	if (!useSeed && r > 10 && r < 50){
			 		loweff = min(e,loweff);
			 		higheff=max(e,higheff);
			 	}
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
	TString basedir = "/n/atlasdata1/etolley/Trigger/SAMPLES_var2/";
	TFile * f = new TFile(basedir+sample_dir+"/triggerInfo_"+sample_tag+".root");
	effTree = (TTree*)f->Get("physics");
}

//get list of efficicencies for a set of threshold values and a single seed
double * getEff(TString var, TString seed, double * vals, int n){
	double * effs = new double[n];
	for (int i = 0; i < n; i++){
		TString tag = var + TString::Itoa(floor(vals[i]),10);
		float nPass = effTree->GetEntries(tag + " && " + seed + "&& 2j30");
		float nTotal = effTree->GetEntries("2j30");
		effs[i] = nPass/nTotal;
	}
	return effs;
}
