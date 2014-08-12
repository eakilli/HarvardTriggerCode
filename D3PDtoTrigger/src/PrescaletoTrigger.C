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

// #define D3PDAnalysis
#include "PrescaleAnalysis.hh"

using namespace std;

/// Main function that runs the analysis algorithm on the
/// specified input files
int main(int argc, char* argv[]) {

  /// Gets the list of input files and chains
  /// them into a single TChain
  char inputFileName[400];
  char inputListName[400];
  char outputFileName[400];
  char TreeName[400];

  bool DO_FILE = false;
  bool DO_LIST = false;
  bool GOT_TREE = false;

  if ( argc < 2 ){
    cout << "Error at Input: please specify an input file name or list of input ROOT files" << endl; 
    cout << "                and an output filename" << endl; 
    cout << "Example:        ./PrescaletoTrigger.x -ifile=input.root -ofile=output.root" << endl;
    cout << "Example:        ./PrescaletoTrigger.x -ilist=input.list -ofile=output.root" << endl;
    
    return 1;
  }
  for (int i=0;i<argc;i++){
    if (strncmp(argv[i],"-ifile",6)==0){
      sscanf(argv[i],"-ifile=%s",  inputFileName);
      DO_FILE = true;
    }
    if (strncmp(argv[i],"-ilist",6)==0){
      sscanf(argv[i],"-ilist=%s",  inputListName);
      DO_LIST = true;
    }
    if (strncmp(argv[i],"-ofile",6)==0)  sscanf(argv[i],"-ofile=%s",  outputFileName);
    if (strncmp(argv[i],"-tree",5)==0){
      sscanf(argv[i],"-tree=%s",  TreeName);
      GOT_TREE = true;
    }
  }


  TChain *theChain = new TChain("susy");
 
  char Buffer[500];
  char MyRootFile[2000];  

  if(DO_LIST){
    std::cout << "Chaining trees from input list: " << inputListName << std::endl;
    ifstream *inputFile = new ifstream(inputListName);

    while( !(inputFile->eof()) ){
      inputFile->getline(Buffer,500);
      if (!strstr(Buffer,"#") && !(strspn(Buffer," ") == strlen(Buffer))){
	sscanf(Buffer,"%s",MyRootFile);
	if(string(MyRootFile).find("eos") != std::string::npos){
	  theChain->Add("root:/"+TString(MyRootFile));
	} else {
	  theChain->Add(MyRootFile);
	}

	std::cout << "chaining " << MyRootFile << std::endl;
      }
    }
    inputFile->close();
    delete inputFile;
  }

  if(DO_FILE){
    std::cout << "Chaining tree from input file: " << inputFileName << std::endl;
    if(string(inputFileName).find("eos") != std::string::npos){
      theChain->Add("root:/"+TString(inputFileName));
    } else {
      theChain->Add(inputFileName);
    }
    
  }
  
  // Prescale Analysis
  PrescaleAnalysis analysis(theChain);
  analysis.CreateTriggerTreeFile(outputFileName);
 
  return 0;
}
