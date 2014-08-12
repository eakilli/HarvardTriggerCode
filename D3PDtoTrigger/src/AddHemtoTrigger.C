//-------------------------------------------------------
// Description:
//    Routine to run Vecbos selection
// Authors:
//    Chiara Rovelli & Emanuele Di Marco
//    Universita' di Roma "La Sapienza" & INFN Roma
//    Maurizio Pierini
//    CERN
//-------------------------------------------------------

// C++ includes
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

// ROOT includes
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

// #define TriggerAnalysis
#include "TriggerAnalysis.hh"

using namespace std;

/// Main function that runs the analysis algorithm on the
/// specified input files
int main(int argc, char* argv[]) {

  /// Gets the list of input files and chains
  /// them into a single TChain
  char inputFileName[400];
  char outputFileName[400];

  bool DO_FILE = false;
  bool DO_LIST = false;

  if ( argc < 2 ){
    cout << "Error at Input: please specify an input file name or list of input ROOT files" << endl; 
    cout << "                and an output filename" << endl; 
    cout << "Example:        ./AddHemtoTrigger.x -ifile=input.root -ofile=output.root" << endl;
    
    return 1;
  }
  for (int i=0;i<argc;i++){
    if (strncmp(argv[i],"-ifile",6)==0){
      sscanf(argv[i],"-ifile=%s",  inputFileName);
      DO_FILE = true;
    }
    if (strncmp(argv[i],"-ofile",6)==0)  sscanf(argv[i],"-ofile=%s",  outputFileName);
  }

  char *CMD = new char[2000];
  sprintf(CMD,"cp %s %s",inputFileName,outputFileName);
  system(CMD);

  //TFile *f = new TFile(inputFileName,"READ");
  //TTree *T = (TTree*) f->Get("physics");
  
  // Trigger tree Analysis
  TriggerAnalysis analysis(outputFileName);
  analysis.AddHemLoop();
  analysis.WriteUpdatedTree();
 
  return 0;
}
