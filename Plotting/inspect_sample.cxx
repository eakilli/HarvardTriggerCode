TCanvas* c1;
int color_list[10];
TString s_model;

void inspect_sample(){
	setstyle();
	gStyle->SetOptStat(0);
	c1 = new TCanvas("c1", "First canvas", 1000, 800);
	TString dir = "/afs/cern.ch/user/e/etolley/work/D3PD/TRIGGERvar_SAMPLES/DM/";
	TString s_model = "triggerInfo_WimpPair_D1MB_DM10_QCUT80";
	TFile *f = new TFile(dir+s_model+".root","read");
	TTree * t = (TTree*)f->Get("physics");
	plotAllVars2D(t);
}

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

void drawText(){
	TLatex l;
	l.SetTextFont(132);
	l.SetNDC();
	l.SetTextSize(0.04);
	l.SetTextFont(132);
	l.DrawLatex(0.44,0.943,s_model);
	l.SetTextSize(0.04);
	l.SetTextFont(42);
	l.DrawLatex(0.15,0.943,"#bf{#it{ATLAS}} Internal");
}

TString getVarName(TString in){
	if		(in == "shatR" || in == "shatR/1000")		return " #sqrt{#hat{s}_{R}} (GeV)";
	else if (in == "mdeltaR" || in == "mdeltaR/1000")	return "M_{#Delta}^{R} (GeV)";
	else if (in == "cosptR")	return "cos #theta _{pT,M#Delta}";
	else if (in == "gaminvR")	return "1/#gamma _{R+1}";
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
  gStyle->SetTitleFont(132,"xyz");  // set the all 3 axes title font
  gStyle->SetTitleFont(132," ");    // set the pad title font
  gStyle->SetTitleSize(0.06,"xyz"); // set the 3 axes title size
  gStyle->SetTitleSize(0.06," ");   // set the pad title size
  gStyle->SetLabelFont(132,"xyz");
  gStyle->SetLabelSize(0.05,"xyz");
  gStyle->SetLabelColor(1,"xyz");
  gStyle->SetTextFont(132);
  gStyle->SetTextSize(0.08);
  gStyle->SetStatFont(132);
	
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
