#include <iostream>
#include <iomanip>

TCanvas* c1;
TH2F *hDraw;
int color_list[10];
const int nBins = 11;
TString outputFlag = "best/";
//TString outputFlag = "all/";
TString triggers[] = { "Prod > 200 && HT > 200 ", 
												"Prod > 250 && HT > 200 ",
												"Prod > 200 && TRIG_4J20 ", 
												"Prod > 250 && TRIG_4J20 ",
		       							"TRIG_6jet70_4J20", 
		       							"TRIG_5jet85_4J20",
		       							"TRIG_J75_XE40_j150_xe90",
		       							"TRIG_J100_j200_xe80",
		       							"TRIG_XE70_xe100",
		       							"HT > 200 && ht > 1000",
		       							"All" };
		       							
TString triggernames[] = { "ProdR200_HT200 ", 
												"ProdR250_HT200 ",
												"ProdR200_4J20 ", 
												"ProdR250_4J20 ",
		       							"6jet70_4J20", 
		       							"5jet85_4J20",
		       							"J75_XE40_j150_xe90",
		       							"J100_j200_xe80",
		       							"XE70_xe100",
		       							"HT200_ht1000",
		       							"All non-razor" };

void inspect_sample(){
	setstyle();
	gStyle->SetOptStat(0);
	c1 = new TCanvas("c1", "First canvas", 1200, 800);
	setupTrigPlot();
	
	TString basedir = "/n/atlasdata1/etolley/Trigger/SAMPLES_var/";
	/*parseSample(basedir+"GG_direct/","GG_direct_1125_225");
	parseSample(basedir+"GG_direct/","GG_direct_1425_975");
	parseSample(basedir+"GG_onestepCC/","GG_onestepCC_1145_785_425");
	parseSample(basedir+"SS_direct/","SS_direct_1425_225");
	parseSample(basedir+"SS_onestepCC/","SS_onestepCC_1185_705_225");*/
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM400_QCUT300");
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM400_QCUT80");
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM50_QCUT300");
	//parseSample(basedir+"WimpPair/","WimpPair_D5_DM50_QCUT80");
	//parseDir(basedir+"SS_direct/");
	//parseDir(basedir+"GG_direct/");
	//parseDir(basedir+"WimpPair/");
	//parseDir(basedir+"D5/");
	
	//Samples with best Razor Trigger improvement
  parseSample(basedir+"WimpPair/","WimpPair_D5_DM400_QCUT80");
	parseSample(basedir+"WimpPair/","WimpPair_D5_DM50_QCUT80");
	parseSample(basedir+"SS_direct/","SS_direct_1125_975");
	parseSample(basedir+"GG_direct/","GG_direct_1125_975");
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
	plotTrigCompare(t, s_model);
	c1->Print("plots/"+outputFlag+s_model+"_NOT.pdf");
	c1->Print("plots/"+outputFlag+s_model+"_NOT.png");
	plotTrigCompare(t, s_model, true);
	c1->Print("plots/"+outputFlag+s_model+"_AND.pdf");
	c1->Print("plots/"+outputFlag+s_model+"_AND.png");
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
void plotTrigCompare(TTree * t,TString s_model,bool doAnd=false){
	hDraw->Reset();
	
	//fille histogram
	for(int x=0; x < nBins; x++){
    for(int y=0; y < nBins; y++){
    	if(doAnd) t->Draw("Prod",triggers[y]+"&& ("+triggers[x]+")","goff");
    	else 			t->Draw("Prod",triggers[y]+"&& !("+triggers[x]+")","goff");
    	
			float value = t->GetHistogram()->Integral();
			t->Draw("Prod",triggers[y],"goff");
			value /= t->GetHistogram()->Integral();
      hDraw->Fill( x+0.5, y+0.5, value*100); 
    }
  }
  
  //add alphanumeric labels
  for (int i=1;i<=nBins;i++) hDraw->GetXaxis()->SetBinLabel(i,triggernames[i-1]);
  for (int i=1;i<=nBins;i++) hDraw->GetYaxis()->SetBinLabel(i,triggernames[i-1]);
  
  //format plot
  gStyle->SetPaintTextFormat("5.0f");
  hDraw->GetYaxis()->SetTitle("Trigger Y");
  hDraw->GetYaxis()->SetTitleSize(0.05); 
  hDraw->GetYaxis()->SetLabelSize(0.04); 
  hDraw->GetYaxis()->SetTitleOffset(2.0);
  hDraw->GetXaxis()->SetTitle("Trigger X"); 
  hDraw->GetXaxis()->SetTitleSize(0.05); 
  hDraw->GetXaxis()->SetLabelSize(0.04); 
  hDraw->GetXaxis()->SetTitleOffset(1.6);
  if(doAnd) hDraw->GetZaxis()->SetTitle("% of events passing Trigger Y that also pass Trigger X"); 
  else hDraw->GetZaxis()->SetTitle("% of events passing Trigger Y that do not pass Trigger X");
  hDraw->GetZaxis()->SetTitleSize(0.03); 
  hDraw->GetZaxis()->SetTitleOffset(1.5);
  hDraw->GetZaxis()->SetRangeUser(0., 100.);
  hDraw->SetMarkerSize(1.0);
  c1->SetTopMargin(0.072);
  c1->SetLeftMargin(0.3);
  //c1->SetRightMargin(1);
		       						
 	hDraw->Draw("coltextz");
 
 	/*TLatex l; 
  l.SetNDC();
  l.SetTextColor(1);
  l.SetTextSize(0.06);
  l.DrawLatex(0.39, 0.945, getSampleName(s_model));*/
  drawText(getSampleName(s_model));
 
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

/******************************
beautification functions
*******************************/

void drawText(TString title = ""){
	TLatex l;
	l.SetTextFont(132);
	l.SetNDC();
	l.SetTextSize(0.03);
	l.SetTextFont(132);
	l.DrawLatex(0.5,0.95,title);
	l.SetTextSize(0.03);
	l.SetTextFont(42);
	l.DrawLatex(0.30,0.95,"#bf{#it{ATLAS}} Internal");
}

TString getVarName(TString in){
	if		(in == "shatR" || in == "shatR/1000")		return " #sqrt{#hat{s}_{R}} (GeV)";
	else if (in == "mdeltaR" || in == "mdeltaR/1000")	return "M_{#Delta}^{R} (GeV)";
	else if (in == "cosptR")	return "cos #theta _{pT,M#Delta}";
	else if (in == "gaminvR")	return "1/#gamma _{R+1}";
	else return in;
}

TString getSampleName(TString in){
	if (in == "SS_direct_1125_975") return "pp #rightarrow #tilde{q} #tilde{q}, m_{#tilde{q}} = 1125 GeV, m_{#tilde{#chi}} = 975 GeV";
	else if (in == "GG_direct_1125_975") return "pp #rightarrow #tilde{g} #tilde{g}, m_{#tilde{q}} = 1125 GeV, m_{#tilde{#chi}} = 975 GeV";
	else if (in == "WimpPair_D5_DM50_QCUT80") return "pp #rightarrow #tilde{#chi} #tilde{#chi}, m_{#tilde{#chi}} = 50 GeV, QCUT = 80 GeV, D5";
		else if (in == "WimpPair_D5_DM400_QCUT80") return "pp #rightarrow #tilde{#chi} #tilde{#chi}, m_{#tilde{#chi}} = 400 GeV, QCUT = 80 GeV, D5";

	else return in;
}

void setstyle() {
	
  // For the canvas:
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasDefH(300); //Height of canvas
  gStyle->SetCanvasDefW(600); //Width of canvas
  gStyle->SetCanvasDefX(0);   //POsition on screen
  gStyle->SetCanvasDefY(0);
	
  // For the Pad:
  gStyle->SetPadBorderMode(0);
  // gStyle->SetPadBorderSize(Width_t size = 1);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadGridX(false);
  gStyle->SetPadGridY(false);
  gStyle->SetGridColor(0);
  gStyle->SetGridStyle(3);
  gStyle->SetGridWidth(1);
	
  // For the frame:
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameBorderSize(1);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineColor(1);
  gStyle->SetFrameLineStyle(1);
  gStyle->SetFrameLineWidth(1);
	
  // set the paper & margin sizes
  gStyle->SetPaperSize(20,26);
  gStyle->SetPadTopMargin(0.09);
  gStyle->SetPadRightMargin(0.25);
  gStyle->SetPadBottomMargin(0.18);
  gStyle->SetPadLeftMargin(0.15);
	
  // use large Times-Roman fonts
  /*gStyle->SetTitleFont(132,"xyz");  // set the all 3 axes title font
  gStyle->SetTitleFont(132," ");    // set the pad title font
  gStyle->SetTitleSize(0.06,"xyz"); // set the 3 axes title size
  gStyle->SetTitleSize(0.06," ");   // set the pad title size
  gStyle->SetLabelFont(132,"xyz");
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetLabelColor(1,"xyz");
  gStyle->SetTextFont(132);
  gStyle->SetTextSize(0.08);
  gStyle->SetStatFont(132);*/
	
  // use bold lines and markers
  gStyle->SetMarkerStyle(8);
  gStyle->SetHistLineWidth(1.85);
  gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
	
  //..Get rid of X error bars
  gStyle->SetErrorX(0.001);
	
  // do not display any of the standard histogram decorations
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(11111111);
	
  // put tick marks on top and RHS of plots
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
	
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
	
  gStyle->cd();
	
  color_list[0] = kBlue-2;
  color_list[1] = kGreen+3;
  color_list[2] = kOrange-1;
  color_list[3] = kCyan+2;
  color_list[4] = kRed+1;
  color_list[5] = kMagenta+1;
  color_list[6] = kYellow+1;
  color_list[7] = kCyan+3;
  color_list[8] = 1397;
  color_list[9] = 1407;
	
}
