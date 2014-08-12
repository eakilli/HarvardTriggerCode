//-------------------------------------------------------
// Description:
//    Class for DiLeptonAnalysis search analyses
// Authors:
//
//-------------------------------------------------------

/// The DiLeptonAnalysis class can be used to perform fast check
/// on input ntuples (in a format compatible to VecbosBase)

#ifndef DiLeptonAnalysis_h
#define DiLeptonAnalysis_h

#include <TTree.h>
#include <TFile.h>
#include <TH1D.h>
#include <RooFit.h>
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooKeysPdf.h"
#include "RooNDKeysPdf.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "mt2_bisect.hh"
#include "LHCOBase.hh"

const int N_ANALYSIS_MAX = 4; //maximum number of 'analyses' (or the ones implemented...)
const int N_CAT0 = 3;          //number of categories type '0' : ee, mumu, emu
const int N_CAT1 = 3;          //number of categories type '1' : 0, 1, >=2 jets

const int EVENT_LIMIT = 10000; //maximum number of events in RooDataSet that can be passed to RooNDKeysPdf

// for MET smearing and shape systematic
const float JES_uncertainty = 0.1;

using namespace std;
using namespace RooFit;

class DiLeptonAnalysis : public LHCOBase{
public:

	DiLeptonAnalysis(TTree *tree=0); /// Class Constructor
	virtual ~DiLeptonAnalysis();     /// Class Destructor
	/// The function to run on each events
  
	// Build the PDFs for the analyses listed in ANA;
	void BuildPDFs(vector<int> ANA, char *output_file, int N_EVENT_MAX, bool b_SYS);
	
private:
	
	int N_EVT_MAX;
	bool DO_ANA[N_ANALYSIS_MAX];
	char *s_f_out;
	bool DO_SYS; 
	
	int N_EVT_tot;
	int N_EVT_pass[N_ANALYSIS_MAX][N_CAT0][N_CAT1];   
	float EFF[N_ANALYSIS_MAX][N_CAT0][N_CAT1]; 
	
	int INIT_ALL(vector<int> ANA);
	int Loop();
	int WRITE_OUTPUT();
	
	bool PASS_BASELINE();
	bool PASS_CMS_BASELINE();
	bool PASS_ATLAS_BASELINE();
	
	vector<TLorentzVector> MY_LEP;
	vector<TVector3> MY_JET;
	int GET_CAT0();
	int iCAT0;
	
	int GET_CAT1();
	int iCAT1;
	
	TVector3 MY_MET;
	
	/////////////////////////////////////////////////////
	// INFORMATION FOR DIFFERENT ANALYSIS IMPLEMENTATIONS
	/////////////////////////////////////////////////////
	
	/////////////////////////////////////
	// type 0: MDELTA 1D analysis w CMS baseline
	/////////////////////////////////////
	void DO_TYPE0_ANA(TVector3 vMET,vector<TLorentzVector> LEPs, int iSYS);
	void WRITE_TYPE0_ANA(TFile *fout);
	int NBIN_MDELTAR_type0;
	float MDELTAR_MAX_type0;
	float MDELTAR_MIN_type0;
	RooRealVar *MDELTAR_type0;
	RooDataSet *DATA_type0[N_CAT0][N_CAT1][3];
	int DATA_type0_count[N_CAT0][N_CAT1][3];
	void INIT_TYPE0_ANA(){
		//variable bounds and binning
		NBIN_MDELTAR_type0 = 200;
		MDELTAR_MAX_type0 = 800.;
		MDELTAR_MIN_type0 = 10.;
		//variable for PDF
		MDELTAR_type0 = (RooRealVar*) new RooRealVar("MDELTAR_0","MDELTAR_0",MDELTAR_MIN_type0,MDELTAR_MAX_type0);
		//RooDataSets for non-parametric pdfs
		for(int icat0 = 0; icat0 < N_CAT0; icat0++){
			for(int icat1 = 0; icat1 < N_CAT1; icat1++){
				for(int i = 0; i < 3; i++){
					DATA_type0[icat0][icat1][i] = (RooDataSet*) new RooDataSet(Form("data0_%d_%d_%d", icat0,icat1,i),
																			   Form("data0_%d_%d_%d", icat0,icat1,i),
																			   RooArgSet(*MDELTAR_type0));
					DATA_type0_count[icat0][icat1][i] = 0;
				}
			}
		}
	}
	
	/////////////////////////////////////
	// type 1: MDELTA 1D analysis w ATLAS baseline
	/////////////////////////////////////
	void DO_TYPE1_ANA(TVector3 vMET,vector<TLorentzVector> LEPs, int iSYS);
	void WRITE_TYPE1_ANA(TFile *fout);
	int NBIN_MDELTAR_type1;
	float MDELTAR_MAX_type1;
	float MDELTAR_MIN_type1;
	RooRealVar *MDELTAR_type1;
	RooDataSet *DATA_type1[N_CAT0][N_CAT1][3];
	int DATA_type1_count[N_CAT0][N_CAT1][3];
	void INIT_TYPE1_ANA(){
		//variable bounds and binning
		NBIN_MDELTAR_type1 = 200;
		MDELTAR_MAX_type1 = 800.;
		MDELTAR_MIN_type1 = 10.0;
		//variable for PDF
		MDELTAR_type1 = (RooRealVar*) new RooRealVar("MDELTAR_1","MDELTAR_1",MDELTAR_MIN_type1,MDELTAR_MAX_type1);
		//RooDataSets for non-parametric pdfs
		for(int icat0 = 0; icat0 < N_CAT0; icat0++){
			for(int icat1 = 0; icat1 < N_CAT1; icat1++){
				for(int i = 0; i < 3; i++){
					DATA_type1[icat0][icat1][i] = (RooDataSet*) new RooDataSet(Form("data1_%d_%d_%d", icat0,icat1,i),
																			   Form("data1_%d_%d_%d", icat0,icat1,i),
																			   RooArgSet(*MDELTAR_type1));
					DATA_type1_count[icat0][icat1][i] = 0;
				}
			}
		}
	}
	
	/////////////////////////////////////
	// type 2: MCTperp analysis w CMS baseline
	/////////////////////////////////////
	void DO_TYPE2_ANA(TVector3 vMET,vector<TLorentzVector> LEPs, int iSYS);
	void WRITE_TYPE2_ANA(TFile *fout);
	int NBIN_MCT_type2;
	float MCT_MAX_type2;
	float MCT_MIN_type2;
	RooRealVar *MCT_type2;
	RooDataSet *DATA_type2[N_CAT0][N_CAT1][3];
	int DATA_type2_count[N_CAT0][N_CAT1][3];
	void INIT_TYPE2_ANA(){
		//variable bounds and binning
		NBIN_MCT_type2 = 200;
		MCT_MAX_type2 = 800.;
		MCT_MIN_type2 = 10.0;
		//variable for PDF
		MCT_type2 = (RooRealVar*) new RooRealVar("MCT_2","MCT_2",MCT_MIN_type2,MCT_MAX_type2);
		//RooDataSets for non-parametric pdfs
		for(int icat0 = 0; icat0 < N_CAT0; icat0++){
			for(int icat1 = 0; icat1 < N_CAT1; icat1++){
				for(int i = 0; i < 3; i++){
					DATA_type2[icat0][icat1][i] = (RooDataSet*) new RooDataSet(Form("data2_%d_%d_%d", icat0,icat1,i),
																			   Form("data2_%d_%d_%d", icat0,icat1,i),
																			   RooArgSet(*MCT_type2));
					DATA_type2_count[icat0][icat1][i] = 0;
				}
			}
		}
	}
	
	/////////////////////////////////////
	// type 3: MT2 1D analysis w ATLAS baseline
	/////////////////////////////////////
	void DO_TYPE3_ANA(TVector3 vMET,vector<TLorentzVector> LEPs, int iSYS);
	void WRITE_TYPE3_ANA(TFile *fout);
	int NBIN_MT2_type3;
	float MT2_MAX_type3;
	float MT2_MIN_type3;
	RooRealVar *MT2_type3;
	RooDataSet *DATA_type3[N_CAT0][N_CAT1][3];
	int DATA_type3_count[N_CAT0][N_CAT1][3];
	void INIT_TYPE3_ANA(){
		//variable bounds and binning
		NBIN_MT2_type3 = 200;
		MT2_MAX_type3 = 800.;
		MT2_MIN_type3 = 10.0;
		//variable for PDF
		MT2_type3 = (RooRealVar*) new RooRealVar("MT2_3","MT2_3",MT2_MIN_type3,MT2_MAX_type3);
		//RooDataSets for non-parametric pdfs
		for(int icat0 = 0; icat0 < N_CAT0; icat0++){
			for(int icat1 = 0; icat1 < N_CAT1; icat1++){
				for(int i = 0; i < 3; i++){
					DATA_type3[icat0][icat1][i] = (RooDataSet*) new RooDataSet(Form("data3_%d_%d_%d", icat0,icat1,i),
																			   Form("data3_%d_%d_%d", icat0,icat1,i),
																			   RooArgSet(*MT2_type3));
					DATA_type3_count[icat0][icat1][i] = 0;
				}
			}
		}
	}
	
	////////////////////////////////////////////////////
	// INFORMATION FOR SHAPE SYSTEMATICS IMPLEMENTATION
	////////////////////////////////////////////////////
	
	TVector3 GET_MET_SYS_UP();
	TVector3 GET_MET_SYS_DOWN();
	
	////////////////////////////////////////////////////
	// Kinematic variable functions
	////////////////////////////////////////////////////
	double Calc_MT2(TLorentzVector p1, TLorentzVector p2, TVector3 met);
	double Calc_MCTperp(TLorentzVector p1, TLorentzVector p2, TVector3 met);
	double Calc_ETmissRel(TLorentzVector p1, TLorentzVector p2, vector<TVector3> vjets, TVector3 met);
};
#endif
