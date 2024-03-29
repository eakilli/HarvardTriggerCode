/******************************
Last modified by Emma Tolley (emma.tolley@cern.ch) 8 Sept '14
    
******************************/

#include <iostream>
#include <iomanip>
#include "common_plotting_functions.hh"

TCanvas* c1;
TH2F *hDraw;
int color_list[10];

TString outputFlag = "all/";
TString filter="2j30";
//TString outputFlag = "best/";
bool doEfficient = false;
//bool abs_e = false;
const int nBins = 9;
TString triggers[] = { "Prod > 170 && 4J20",
												"Prod > 170 && HT > 200",
												"Prod > 140 && XE70",
												"Prod > 220 && 2J15_XE55",
		       							"5j85_4J20",
		       							"j150_xe90_J75_XE40",
		       							"xe100_XE70",
		       							"HT > 200 && ht > 1000",
		       							"all_norazor" };
		       							
TString triggernames[] = {"ProdR170_4J20",
												"ProdR170_HT200",
												"ProdR140_XE70",		
												"ProdR220_2J15_XE55",										
		       							"5jet85_4J20",
		       							"j150_xe90_J75_XE40",
		       							"xe100_XE70",
		       							"ht1000_HT200",
		       							"All jet + MET triggers" };
		       						
void trigger_overlap_plot(){
	setstyle();
	gStyle->SetOptStat(0);
	c1 = new TCanvas("c1", "First canvas", 1200, 800);
	setupTrigPlot();
	
	TString basedir = "/n/atlasdata1/etolley/Trigger/SAMPLES_var2/";
	/*parseSample(basedir+"GG_direct/","GG_direct_1125_225");
	parseSample(basedir+"GG_direct/","GG_direct_1425_975");
	parseSample(basedir+"GG_onestepCC/","GG_onestepCC_1145_785_425");
	parseSample(basedir+"SS_direct/","SS_direct_1425_225");
	parseSample(basedir+"SS_onestepCC/","SS_onestepCC_1185_705_225");*/
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM400_QCUT300");
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM400_QCUT80");
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM50_QCUT300");
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM50_QCUT80");
	parseDir(basedir+"SS_direct/");
	parseDir(basedir+"GG_direct/");
	parseDir(basedir+"WimpPair/");
	parseDir(basedir+"T1/");
	parseDir(basedir+"MONOJET_B/");
	
	//Samples with best Razor Trigger improvement
  //parseSample(basedir+"WimpPair/","WimpPair_D5_DM400_QCUT80");
 	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM50_QCUT80");
	//parseSample(basedir+"SS_direct/","SS_direct_1125_975");
	//parseSample(basedir+"GG_direct/","GG_direct_1125_975");
	//parseSample(basedir+"T1/","Tt_T700_L500");*/
}

/*************************************************************
2D Trigger AND/NOT Histogram code
**************************************************************/

//Make plots for an entire directory
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
			if (fname.EndsWith(".root") &&fname(0,12) == "triggerInfo_"){
				//cout << "Attempting to tamper with file " << fname << endl;
				TFile * f = new TFile(dirname+fname, "READ");
				if (f->IsOpen()){
					TTree * t = (TTree*)f->Get("physics");
					if (t && t ->GetEntries() > 0) makeAndOrPlots(t,fname(12,fname.Index(".")-12));
				}
				delete f;
			}
		}
	} else cout << "No files in " << dirname<<endl;
}

//Make plots for a single file
void parseSample(TString dir,TString s_model){
	TFile *f = new TFile(dir+"triggerInfo_"+s_model+".root","read");
	if (f){
	cout << "debug1" << endl;
	TTree * t = (TTree*)f->Get("physics");
	makeAndOrPlots(t, s_model );
	f->Close();
	}else cout << "ERROR, " << dir+"triggerInfo_"+s_model+".root" << " not found." << endl;
}

void makeAndOrPlots(TTree *t,TString s_model ){
	cout << "Running over " << s_model << endl;
	
	bool savePlot = plotTrigCompare(t, s_model);
	TString plotName = "plots/"+outputFlag+s_model;
	if (filter.Length() > 1) plotName+="_"+filter;
	if (savePlot){
		c1->Print(plotName+"_NOT.pdf");
		c1->Print(plotName+"_NOT.png");
	}
	savePlot = plotTrigCompare(t, s_model, true);
	if (savePlot){
		c1->Print(plotName+"_abs_NOT.pdf");
		c1->Print(plotName+"_abs_NOT.png");
	}
}

//initialize 2d histogram
void setupTrigPlot(){
	double bins[nBins+1];
	for (int i = 0; i <= nBins; i++){
		bins[i] = i*1.0;///nBins;
	}
	hDraw = new TH2F("nice map", 
	     "nice map", 
	     nBins, bins,
	     nBins, bins); 
}

//make 2d plot
bool plotTrigCompare(TTree * t,TString s_model,bool doAbs=false){
	hDraw->Reset();
	hDraw->SetMarkerColor(kWhite);
	float totval = 0;
	for(int x=0; x < nBins; x++){
    for(int y=0; y < nBins; y++){   	
			float value = t->GetEntries(triggers[y]+" && !("+triggers[x]+") &&" + filter);
			if(!doAbs) value /= (float)t->GetEntries(triggers[y]+"&&"+filter);
			else value /= (float)t->GetEntries(filter);
      hDraw->Fill( x+0.5, y+0.5, value*100); 
      if (x == nBins-1) totval+=value;
      //doLogic(t, triggers[y],triggers[x],doAnd);
    }
  }
  
  //add alphanumeric labels
  for (int i=1;i<=nBins;i++) hDraw->GetXaxis()->SetBinLabel(i,triggernames[i-1]);
  for (int i=1;i<=nBins;i++) hDraw->GetYaxis()->SetBinLabel(i,triggernames[i-1]);
  
  //format plot
  gStyle->SetPaintTextFormat("5.0f");
  hDraw->GetYaxis()->SetTitle("Trigger Y");
  hDraw->GetYaxis()->SetTitleSize(0.04); 
  hDraw->GetYaxis()->SetLabelSize(0.04); 
  hDraw->GetYaxis()->SetTitleOffset(2.7);
  hDraw->GetXaxis()->SetTitle("Trigger X"); 
  hDraw->GetXaxis()->SetTitleSize(0.04); 
  hDraw->GetXaxis()->SetLabelSize(0.04); 
  hDraw->GetXaxis()->SetTitleOffset(1.7);
  if(doAbs) hDraw->GetZaxis()->SetTitle("% of events that pass Trigger Y and not Trigger X"); 
  else hDraw->GetZaxis()->SetTitle("% of events passing Trigger Y that do not pass Trigger X");
  hDraw->GetZaxis()->SetTitleSize(0.03); 
  hDraw->GetZaxis()->SetTitleOffset(1.6);
  hDraw->GetZaxis()->SetRangeUser(0., 100.);
  hDraw->SetMarkerSize(1.0);
  c1->SetTopMargin(0.072);
  c1->SetLeftMargin(0.3);
  //c1->SetRightMargin(1);
		       						
 	hDraw->Draw("coltextz");
 
 if (filter.Length() > 1){
 	TLatex l; 
  l.SetNDC();
  l.SetTextColor(kAzure+1);
  l.SetTextSize(0.03);
  l.DrawLatex(0.02, 0.13, "Filter: 2 Jets with p_{T} > 30 GeV");
  l.DrawLatex(0.02, 0.07, "Prod = ("+getVarName("shatR")+" + 200 GeV)*("+getVarName("gaminvR")+")");
  l.DrawLatex(0.02, 0.02, "ProdR threshold set so rate ~28Hz");
  }
  drawText(getSampleName(s_model));
	return totval > 0.01;
 
}

/*****************************************************
misc variable plotting functions
******************************************************/

void plotAllVars(TTree * t){
	c1->Divide(2,2);
	c1->cd(1);
	makePlot(t, "shatR/1000");
	c1->cd(2);
	makePlot(t, "cosptR");
	c1->cd(3);
	makePlot(t, "mdeltaR/1000");
	c1->cd(4);
	makePlot(t, "gaminvR");
}

void plotAllVars2D(TTree * t){
	c1->Divide(3,2);
	
	c1->cd(1);
	makePlot(t, "shatR/1000", "gaminvR");
	c1->cd(2);
	makePlot(t, "shatR/1000", "cosptR");
	c1->cd(3);
	makePlot(t, "shatR/1000", "mdeltaR/1000");
	c1->cd(4);
	makePlot(t, "cosptR", "gaminvR");
	c1->cd(5);
	makePlot(t, "cosptR", "mdeltaR/1000");
	c1->cd(6);
	makePlot(t, "mdeltaR/1000", "gaminvR");
}

void makePlot(TTree * t, TString xvar, TString yvar){
	gPad->SetLogz();
	gPad->SetGridx();
	gPad->SetGridy();
	t->Draw(yvar+":"+xvar,"","colz");
	t->GetHistogram()->SetTitle("");
	t->GetHistogram()->SetXTitle(getVarName(xvar));
	t->GetHistogram()->SetYTitle(getVarName(yvar));
	t->GetHistogram()->GetXaxis()->CenterTitle();
	t->GetHistogram()->GetYaxis()->CenterTitle();
	t->GetHistogram()->GetZaxis()->CenterTitle();
	t->GetHistogram()->GetZaxis()->SetTitle("a.u.");
	t->GetHistogram()->GetZaxis()->SetTitleOffset(1.5);
	//t->GetHistogram()->GetZaxis()->SetRangeUser(0.9/INT,1.1*t->GetHistogram()->GetMaximum());
	drawText();

}

void makePlot(TTree * t, TString xvar){
	gPad->SetLogy();
	gPad->SetGridx();
	gPad->SetGridy();
	t->Draw(xvar,"","hist");
	t->GetHistogram()->SetTitle("");
	t->GetHistogram()->GetXaxis()->CenterTitle();
	t->GetHistogram()->GetYaxis()->CenterTitle();
	t->GetHistogram()->SetXTitle(getVarName(xvar));
	drawText();
}
