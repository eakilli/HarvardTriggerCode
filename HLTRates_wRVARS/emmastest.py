import embedded_menuFast_wRVARS
import ROOT as r
import array
from math import floor
from multiprocessing import Pool
import os

verbose = True
doUnique = True;

#hpc/openmpi-1.5.3_gcc-4.6.1

trigger_menu = ["3j175_J100",'J400_j100',"4j100_3J50", 										"5j85_4J20","6J70_4J20","7J45_6J15","8J40_6J15",
								"bjet175_bjet60_J100","2bjet55_2j55_4J20","bjet75_3j75_4J20",
								"xe100_XE70", "ht1000_HT200",
								"xe80_j200_J100", "j150_xe90_J75_XE40",
								"bjet120_xe70_J75_XE40", "j175_bjet70_xe70_J100"
								]

def main():
	p = Pool(16)
	
	thresholds = list(set(range(0,1000,20) + [200] + [175] + [225] + [70] + [100]))
	thresholds.sort()
	seeds = ["4J20", "2J30_XE40", "HT200", "XE70","2J10_XE60","2J15_XE55"]
	
	print("Calculating rates")
	
	met_rates = [0]
	razor_rates = [0]
	filename = "rate_info.root"
	if doUnique:
		met_rates = p.map(getRate,[("xe",th,"XE70",True) for th in thresholds])
		razor_rates = [p.map(getRate,[("prodR",th,seed,True) for th in thresholds]) for seed in seeds]
		filename = "unique_" + filename;
	else:
		met_rates = p.map(getRate,[("xe",th,"XE70",False) for th in thresholds])
		razor_rates = [p.map(getRate,[("prodR",th,seed,False) for th in thresholds]) for seed in seeds]

	print("Filling trees")
	f = r.TFile(filename,"recreate")
	t  =  r.TNtuple("razor_rates","rates","v"+"".join([":"+i for i in seeds]))
	tm =  r.TNtuple("met_rates","rates","v:XE70")
	
	for i in range(len(thresholds)):
		t.Fill(array.array('f',[thresholds[i]]+[razor_rates[j][i] for j in range(len(seeds))]))
		tm.Fill(thresholds[i],met_rates[i])
	t.Write()
	tm.Write()
	f.Close()
	
def getRate(inputArg):
	(varTrig,thres, L1seed, getUnique) = inputArg
	triggers = [varTrig+str(int(thres))+"_" + L1seed]
	if (getUnique):
		triggers += trigger_menu
		
	( dataRate,dataRate_unique,mcRate,mcRate_unique) = embedded_menuFast_wRVARS.getRate(triggers)
	
	print (varTrig + " > " + str(int(thres)) + " ("+L1seed+") rate: " + str(dataRate))
	if (getUnique):
		return dataRate_unique
	return dataRate
	
if __name__ == "__main__":
	print "Running multirate calculator"
	main()
	
