#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <algorithm>
#include <numeric>
#include <list>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TProfile.h>
#include <TVector.h>
#include <TLorentzVector.h>
#include <TTree.h>
#include <TBranch.h>
//#include <RooRealVar.h>
//#include <RooDataSet.h>
#include <TRandom3.h>
#include <TDatime.h>
#include <TLatex.h>
#include <TColor.h>
#include <TStyle.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TF1.h>
#include <TColorWheel.h>
#include <TEfficiency.h>

#include "common_plotting_functions.hh"
TCanvas * c1;
TLegend legend(0.7,0.6,0.85,0.85);
class TurnonObject: public TObject{
	public:
	TString triggerName;
	int threshold;

	TH1D* hist_numerator;
	TH1D* hist_denominator;
	TEfficiency *hist;

	TurnonObject(int in_triggerVal){
		threshold = in_triggerVal;
	 	triggerName = "XE" + TString::Itoa(floor(threshold),10);
		hist_numerator =  new TH1D(triggerName+"_n",triggerName+"_n",100,0.,200.);
    hist_denominator= new TH1D(triggerName+"_d",triggerName+"_d",100,0.,200.);
	}
	void Fill(float hl_val, float l1_val, float weight = 1.0){
		hist_denominator->Fill(hl_val,weight);
		if (l1_val >= threshold)
			hist_numerator->Fill(hl_val,weight);
	}
	void setEfficiency(){
	  hist = (TEfficiency*) new TEfficiency(*hist_numerator,*hist_denominator);
    hist->SetLineColor(kBlack);
    hist->SetFillColor(0);
    hist->SetLineWidth(2.0);
	}
	ClassDef(TurnonObject, 1);
};

void plot_turnon(){
  setstyle();
  legend.Clear();
	legend.SetBorderSize(1);
  legend.SetFillColor(0);
	gStyle->SetOptStat(0);
	c1 = new TCanvas("c1", "First canvas", 1200, 800);

	plot_1D_turnon();
}

void plot_1D_turnon(){


  TObjArray * turnon_curves  = new TObjArray();
  turnon_curves->Add(new TurnonObject(30));
  turnon_curves->Add(new TurnonObject(50));
  turnon_curves->Add(new TurnonObject(70));
  turnon_curves->Add(new TurnonObject(100));
  
  TString fdir = "~/data/Trigger/SAMPLES_var2/background_MC/";
  runOverTree(turnon_curves,fdir+"ttbar_bcid_re1.root",8.1226E-4*10 * 5.4274E-01);
  runOverTree(turnon_curves,fdir+"Zee_bcid_re1.root",	2.0570e-3*10 );
  runOverTree(turnon_curves,fdir+"Zmumu_bcid_re1.root",2.0580E-3*10);
  runOverTree(turnon_curves,fdir+"Wplusenu_bcid_re1.root",1.2160E-2*10);
  runOverTree(turnon_curves,fdir+"Wminusenu_bcid_re1.root",8.9770E-3*10);
  runOverTree(turnon_curves,fdir+"Wplusmunu_bcid_re1.root",1.2160E-2*10);
  runOverTree(turnon_curves,fdir+"Wminusmunu_bcid_re1.root",8.9770E-3*10);
	
	makeDummyPlot();
	for (int j = 0; j < turnon_curves->GetEntries(); j++){
		((TurnonObject*)turnon_curves->At(j))->setEfficiency();
		((TurnonObject*)turnon_curves->At(j))->hist->SetLineColor(colors[j]);
    ((TurnonObject*)turnon_curves->At(j))->hist->SetMarkerColor(colors[j]);
		((TurnonObject*)turnon_curves->At(j))->hist->Draw("p same");
		legend.AddEntry(((TurnonObject*)turnon_curves->At(j))->hist,
		((TurnonObject*)turnon_curves->At(j))->triggerName);
	}
	legend.Draw();
}

void runOverTree(TObjArray * turnon_curves, TString fname, float weight = 1.0){
  TFile * f = new TFile(fname,"read");
	TTree * t = (TTree*)f->Get("physics");
  
  Float_t xe_pt;
	Float_t XE_pt;
	t->SetBranchAddress("xe_pt",&xe_pt);
	t->SetBranchAddress("XE_pt",&XE_pt);

	int nEntries = t->GetEntries();
	for (int i = 0; i < nEntries; i++){
		t->GetEntry(i);
		//cout << xe_pt << " " << XE_pt << endl;
		printProgress(nEntries,i);
		for (int j = 0; j < turnon_curves->GetEntries(); j++){
			((TurnonObject*)turnon_curves->At(j))->Fill(xe_pt/1000, XE_pt/1000,weight);
		}
	}cout << endl;
}
//dummy plot to set up canvas
void makeDummyPlot(){
	float x_dummy [] = {0,220};
	float y_dummy [] = {0.0,1.0};
	dummy = new TGraph(2,x_dummy,y_dummy);
	dummy->SetTitle("");
	dummy->Draw("p a");
	dummy->SetMarkerColor(0);
	dummy->GetXaxis()->SetTitle("xe_pt (GeV)");
	dummy->GetYaxis()->SetTitle("Efficiency");
}
/*
TCanvas* Plot_Me_2D(char *titlecan, TH2D* histo){
  TCanvas *c1 = new TCanvas(titlecan,titlecan,600,500);
  c1->Draw();
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLogz();
  double INT = histo->Integral();
  histo->Scale(1./histo->Integral());
  histo->Draw("COLZ");
  histo->GetYaxis()->SetTitle("1/#gamma_{R+1}");
  histo->GetXaxis()->SetTitleOffset(1.27);
  histo->GetXaxis()->CenterTitle();
  histo->GetXaxis()->SetTitle("#sqrt{#hat{s}}_{R} [GeV]");
  histo->GetYaxis()->SetTitleOffset(1.2);
  histo->GetYaxis()->CenterTitle();
  histo->GetZaxis()->CenterTitle();
  histo->GetZaxis()->SetTitle("a.u.");
  histo->GetZaxis()->SetTitleOffset(1.5);
  histo->GetZaxis()->SetRangeUser(0.9/INT,1.1*histo->GetMaximum());
  histo->Draw("COLZ");

  TLatex l;
  l.SetTextFont(132);
	
  l.SetNDC();
	
  l.SetTextSize(0.04);
  l.SetTextFont(132);
  l.DrawLatex(0.44,0.953,s_model);

  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.15,0.943,"#bf{#it{ATLAS}} Internal");
	
  return c1;
	
}

TCanvas* Plot_Me_1D(char *titlecan, TH1D* histo){
  TCanvas *c1 = new TCanvas(titlecan,titlecan,700,500);
  c1->SetRightMargin(0.06);
  c1->Draw();
  c1->SetGridx();
  c1->SetGridy();
  c1->SetLogy();
  double INT = histo->Integral();
  histo->Scale(1./histo->Integral());
  
  histo->GetXaxis()->CenterTitle();
  histo->GetYaxis()->CenterTitle();
  histo->GetXaxis()->SetTitle("1/#gamma_{R+1}");
  histo->Draw();

  TLatex l;
  l.SetTextFont(132);
	
  l.SetNDC();
	
  l.SetTextSize(0.04);
  l.SetTextFont(132);
  l.DrawLatex(0.44,0.943,s_model);

  l.SetTextSize(0.04);
  l.SetTextFont(42);
  l.DrawLatex(0.15,0.943,"#bf{#it{ATLAS}} Internal");
	
  return c1;
	
}
 

double ONEoGAMMA(float SHATR,float SHATR_0, float G_0,float K){
  //if(SHATR < 300) return 1.1;
  double ret = K/(SHATR+SHATR_0) - G_0;
  //return ret;
  //if(ret < 0.15) return 0.15;
  return ret;

}
*/

