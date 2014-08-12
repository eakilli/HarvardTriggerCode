#!/bin/sh
#dmdir="TRIGGERpHEM_SAMPLES/DM/"
#dmdir_new="TRIGGERvar_SAMPLES/DM/"
#./AddVartoTrigger.x -ifile=$dmdir/WimpPair_D1MB_DM10_QCUT80.root -ofile=$dmdir_new/WimpPair_D1MB_DM10_QCUT80.root

#ggdir="TRIGGERpHEM_SAMPLES/GG_direct/"
#ggdir_new="TRIGGERvar_SAMPLES/GG_direct/"
#ggarray=( GG_direct_1125_225.root GG_direct_1125_75.root GG_direct_1425_1125.root GG_direct_1425_525.root GG_direct_1425_825.root GG_direct_1125_525.root GG_direct_1125_825.root GG_direct_1425_225.root GG_direct_1425_675.root GG_direct_1425_975.root GG_direct_1125_675.root GG_direct_1125_975.root GG_direct_1425_375.root GG_direct_1425_75.root)
#for i in "${ggarray[@]}"
#do
#    ./AddVartoTrigger.x -ifile=$ggdir/$i -ofile=$ggdir_new/$i
#done

ssdir="TRIGGERpHEM_SAMPLES/SS_direct/"
ssdir_new="TRIGGERvar_SAMPLES/SS_direct/"
ssarray=(SS_direct_1125_225.root SS_direct_1125_675.root SS_direct_1125_975.root SS_direct_1425_525.root SS_direct_1425_825.root SS_direct_1125_375.root SS_direct_1125_75.root SS_direct_1425_1125.root SS_direct_1425_675.root SS_direct_1425_975.root SS_direct_1125_525.root SS_direct_1125_825.root SS_direct_1425_225.root SS_direct_1425_75.root)
for i in "${ssarray[@]}"
do
	echo $i
    ./AddVartoTrigger.x -ifile=$ssdir/$i -ofile=$ssdir_new/$i
done

