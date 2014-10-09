/******************************
Last modified by Emma Tolley (emma.tolley@cern.ch) 8 Sept '14
    
******************************/

#include <iostream>
#include <iomanip>
#include "common_plotting_functions.hh"

TCanvas* c1;
TH2F *hDraw;

const int nBinsJ = 5;
const int nBinsX = 6;

TString tagsj[]	= {"15","20","25","30","35"};     
TString tagsx[] =	{"35","40","45","50","55","60"};						
TString tags[] = {
" 2J15_XE35 8597.33024813 ",
" 2J15_XE40 4509.15407795 ",
" 2J15_XE45 2427.56120319 ",
" 2J15_XE50 1310.91124544 ",
" 2J15_XE55 716.991163284 ",
" 2J15_XE60 389.956600497 ",
" 2J20_XE35 6213.77431606 ",
" 2J20_XE40 3490.38085536 ",
" 2J20_XE45 1964.76136305 ",
" 2J20_XE50 1087.58593402 ",
" 2J20_XE55 591.753258154 ",
" 2J20_XE60 335.57460085 ",
" 2J25_XE35 4663.88520726 ",
" 2J25_XE40 2743.89335347 ",
" 2J25_XE45 1561.59257067 ",
" 2J25_XE50 888.696930507 ",
" 2J25_XE55 498.938390632 ",
" 2J25_XE60 281.013444526 ",
" 2J30_XE35 3512.98196512 ",
" 2J30_XE40 2180.06084946 ",
" 2J30_XE45 1283.69420194 ",
" 2J30_XE50 741.485397951 ",
" 2J30_XE55 419.926582987 ",
" 2J30_XE60 240.147401176 ",
" 2J35_XE35 2681.36460857 ",
" 2J35_XE40 1744.13872245 ",
" 2J35_XE45 1060.31396527 ",
" 2J35_XE50 635.234634517 ",
" 2J35_XE55 357.266975018 ",
" 2J35_XE60 204.2683120426 "
}
   						
void plotty_plot(){
	setstyle();
	gStyle->SetOptStat(0);
	c1 = new TCanvas("c1", "First canvas", 1200, 800);
	setupPlot();
	plot();
}

//initialize 2d histogram
void setupPlot(){
	double binsj[nBinsJ+1];
	for (int i = 0; i <= nBinsJ; i++){
		binsj[i] = i*1.0;///nBins;
	}
	double binsx[nBinsX+1];
	for (int i = 0; i <= nBinsX; i++){
		binsx[i] = i*1.0;///nBins;
	}
	hDraw = new TH2F("nice map", 
	     "nice map", 
	     nBinsX, binsx,
	     nBinsJ, binsj); 
}

//make 2d plot
void plot(){
	//hDraw->Reset();
	hDraw->SetMarkerColor(kWhite);
	
	for(int j=0; j < nBinsJ; j++){
    for(int x=0; x < nBinsX; x++){   	
			TString t = tags[j*nBinsX+x];
			float value = TString(t(10,t.Length())).Atof();
			cout << tagsj[j] << " " << tagsx[x] << " " << t << endl;
      hDraw->Fill( x+0.5, j+0.5, value); 
    }
  }
  
  //add alphanumeric labels
  for (int j=1;j<=nBinsJ;j++) hDraw->GetYaxis()->SetBinLabel(j,tagsj[j-1]);
  for (int x=1;x<=nBinsX;x++) hDraw->GetXaxis()->SetBinLabel(x,tagsx[x-1]);


  gStyle->SetPaintTextFormat("5.0f");
  hDraw->GetYaxis()->SetTitle("2J*");
  hDraw->GetXaxis()->SetTitle("XE*"); 
  hDraw->GetZaxis()->SetTitle("Rate in Hz");
  hDraw->GetZaxis()->SetTitleOffset(1.3); 
  hDraw->SetMarkerSize(1.0);

 	hDraw->Draw("coltextz");
 /*
 if (filter.Length() > 1){
 	TLatex l; 
  l.SetNDC();
  l.SetTextColor(kAzure+1);
  l.SetTextSize(0.03);
  l.DrawLatex(0.2, 0.05, "Filter: 2 Jets with p_{T} > 30 GeV");
  }
  drawText(getSampleName(s_model));*/
 
}

