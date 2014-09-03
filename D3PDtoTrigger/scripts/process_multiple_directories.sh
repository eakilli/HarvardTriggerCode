#!/bin/sh

# make Trigger Ntuple skim with:
# process_directory INDIR OUTDIR
function process_directory(){
	for f in $1/*.root
	do
		echo "*** Processing "$filename
		./D3PDtoTrigger.x -ifile=$f -ofile=$2/$filename
	done
}

# list multiple directories here
process_directory /n/atlasdata1/crogan/SAMPLES_ATLAS/D3PD/SS_direct /n/atlasdata1/etolley/Trigger


