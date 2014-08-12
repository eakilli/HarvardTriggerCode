Last modified by Emma Tolley Aug 12 2014
*********************
General GIT Stuff
*********************
Checkout:
git clone https://github.com/etolley/HarvardTriggerCode.git

Update:
git pull

Add files:
git add FILENAME

Commit:
git commit -m "MESSAGE"

Actually commit for real:
git push -u origin master

I also needed to run this command to get the commit working on lxplus:
git remote set-url origin https://etolley@github.com/etolley/HarvardTriggerCode.git

*********************
D3PDtoTrigger
*********************
Be sure to run make to compile the code
Scripts to run over multiple files reside in the scripts folder

D3PDtoTrigger.x
 -> runs with D3PDAnalysis, makes skimmed trigger ntuple from susy D3PDs
AddHemtoTrigger.x
 -> runs with TriggerAnalysis, adds HEM* branches to trigger ntuple
AddVartoTrigger.x
 -> runs with TriggerAnalysis, adds the razor variables to trigger ntuple
root -l add_new_vars.cxx (WIP)
 -> adds trigger pass/fail variables to trigger ntuple

*********************
HLTRates_wRVARS
*********************
Twiki page at:
https://twiki.cern.ch/twiki/bin/view/Atlas/RateEstimator
Useful presentation at:
https://indico.cern.ch/event/264337/material/slides/0?contribId=0