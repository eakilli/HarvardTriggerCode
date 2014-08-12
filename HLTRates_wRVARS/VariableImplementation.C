////////////////////////////////
//
// SUPER-RAZOR VARIABLES CALCULATION 
//
// Code written by Christopher Rogan <crogan@cern.ch>, 27-04-14
// Adapted from paper PRD 89, 055020 (http://arxiv.org/abs/1310.4827) written by
// Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu
//
////////////////////////////////
//
// Requires ROOT libraries:
#include "TLorentzVector.h"
#include "TVector3.h"
//
//
/////////////
//
// LAB frame
//
/////////////

//Reconstructed mega-jets and missing transverse energy (from somewhere else...)
TLorentzVector J1, J2;
TVector3 MET;

J1.SetVectM(J1.Vect(),0.0);
J2.SetVectM(J2.Vect(),0.0);

TVector3 vBETA_z = (1./(J1.E()+J2.E()))*(J1+J2).Vect();
vBETA_z.SetX(0.0);
vBETA_z.SetY(0.0);

//transformation from lab frame to approximate rest frame along beam-axis
J1.Boost(-vBETA_z);
J2.Boost(-vBETA_z);

TVector3 pT_CM = (J1+J2).Vect() + MET;
pT_CM.SetZ(0.0); //should be redundant...

double Minv2 = (J1+J2).M2();
double Einv = sqrt(MET.Mag2()+Minv2);

//////////////////////
// definition of shatR
//////////////////////
double shatR = sqrt( ((J1+J2).E()+Einv)*((J1+J2).E()+Einv) - pT_CM.Mag2() );

TVector3 vBETA_R = (1./sqrt(pT_CM.Mag2() + SHATR*SHATR))*pT_CM;

//transformation from lab frame to R frame
J1.Boost(-vBETA_R);
J2.Boost(-vBETA_R);

/////////////
//
// R-frame
//
/////////////

TVector3 vBETA_Rp1 = (1./(J1.E()+J2.E()))*(J1.Vect() - J2.Vect());

////////////////////////
// definition of gaminvR
////////////////////////
double gaminvR = sqrt(1.-vBETA_Rp1.Mag2());

//transformation from R frame to R+1 frames
J1.Boost(-vBETA_Rp1);
J2.Boost(vBETA_Rp1);
//////////////
//
// R+1-frames
//
//////////////

///////////////////////
// definition of mdeltaR
////////////////////////
double mdeltaR = J1.E()+J2.E();

///////////////////////
// definition of cosptR
////////////////////////
double cosptR = pT_CM.Mag()/sqrt(pT_CM.Mag2()+mdeltaR*mdeltaR);

