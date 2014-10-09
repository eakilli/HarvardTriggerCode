import embedded_menuFast_wRVARS
import ROOT as r
import array
from math import floor
from multiprocessing import Pool
import os

verbose = True
doUnique = False;

#hpc/openmpi-1.5.3_gcc-4.6.1

trigger_menu = ["3j175_J100",'J400_j100',"4j100_3J50", 										"5j85_4J20","6J70_4J20","7J45_6J15","8J40_6J15",
								"bjet175_bjet60_J100","2bjet55_2j55_4J20","bjet75_3j75_4J20",
								"xe100_XE70", "ht1000_HT200",
								"xe80_j200_J100", "j150_xe90_J75_XE40",
								"bjet120_xe70_J75_XE40", "j175_bjet70_xe70_J100"
								]

def main():
	#varRates(True)
	#getSimpleRates()
	seedRates(True)


def getThresholds():
	thresholds = list(set(range(0,1000,100) + range(100,300,20)+ [200] + [140] + [220] +[170]+[165]+ [70] +[90]+ [100]))
	thresholds.sort()
	return thresholds

def varRates(doParallel = False):
	if doParallel:
		print "Running in parallel mode"
		p = Pool(8)
	else:
		print "Running in serial mode"
	thresholds = getThresholds()
	offsets = [('0','0'),
						 ('85','48'),
						 ('200','-50'),
						 ('300','-50'),
						 ('400','-50'),
						 #('0','-50'),
						 ('200','0'),
						 ('300','0'),
						 ('400','0'),
						 #('300','-100'),
						 #('500','0'),
						 #('500','-50'),
						 #('500','-100')
						 ]
	razor_rates = dict()
	tags = []
	seed = ""
	for (p1,p2) in offsets:
		tag = str(p1) + "_" + str(p2)
		tags += [tag]
		input_list = [(seed+"prodR",th,p1,p2,True) for th in thresholds]
		if doParallel:
			razor_rates[tag] = p.map(getParamRate, input_list)
		else:
			razor_rates[tag] = [getParamRate(i) for i in input_list]
			
	print("Filling trees")
	f = r.TFile("rate_var"+seed+"_info.root","recreate")
	t  =  r.TNtuple("razor_rates","rates","v"+"".join([":"+i.replace("-","n") for i in tags]))
	for tag in tags:
		print razor_rates[tag]
	for i in range(len(thresholds)):
		t.Fill(array.array('f',[thresholds[i]] + [razor_rates[tag][i] for tag in tags]))
	t.Write()
	f.Close()

def getSimpleRates():
	for s1 in range(15,40,5):
		for s2 in range(35,65,5):
			seed = "2J"+str(s1)+"_XE" +str(s2)
			( dataRate,dataRate_unique,mcRate,mcRate_unique) = embedded_menuFast_wRVARS.getRate([seed])
			print "\"",seed, str(dataRate),"\","

def seedRates(doParallel = False):
	if (doParallel):
		p = Pool(4)
	thresholds = getThresholds()
	seeds = ["4J20", "HT200", "XE70","2J15_XE55","2J45_XE30","2J40_XE35","2J30_XE40","2J25_XE45","2J20_XE50"]
	
	print("Calculating rates")
	
	met_rates = [0]
	metjet_rates = [0]
	razor_rates = dict()
	filename = "rate_info.root"
	if doUnique:
		filename = "unique_" + filename;

	if doParallel:
		for seed in seeds:
			razor_rates[seed] = p.map(getRate,[("prodR",th,seed,doUnique) for th in thresholds])
		met_rates =    p.map(getRate,[("xe",     th,"XE70",    doUnique) for th in thresholds])
		metjet_rates = p.map(getRate,[("j150_xe",th,"J75_XE40",doUnique) for th in thresholds])
	else:
		for seed in seeds:
			razor_rates[seed] = [getRate(("prodR",th,seed,doUnique)) for th in thresholds]
		met_rates =    [getRate(("xe",     th,"XE70",    doUnique)) for th in thresholds]
		metjet_rates = [getRate(("j150_xe",th,"J75_XE40",doUnique)) for th in thresholds]

	print("Filling trees")
	f = r.TFile(filename,"recreate")
	t  =  r.TNtuple("razor_rates","rates","v"+"".join([":"+i for i in seeds]))
	tm =  r.TNtuple("met_rates","rates","v:XE70")
	tmj =  r.TNtuple("metjet_rates","rates","v:J75_XE40")
	
	for i in range(len(thresholds)):
		t.Fill(array.array('f',[thresholds[i]] +	[razor_rates[seed][i] for seed in seeds]))
		tm.Fill(thresholds[i],met_rates[i])
		tmj.Fill(thresholds[i],metjet_rates[i])
	t.Write()
	tm.Write()
	tmj.Write()
	f.Close()

def getParamRate(inputArg):
	(varTrig,thres, p1, p2, getUnique) = inputArg
	triggers = [varTrig+str(int(thres))+"." + p1 + "." + p2]
	if (getUnique):
		triggers += trigger_menu
		
	( dataRate,dataRate_unique,mcRate,mcRate_unique) = embedded_menuFast_wRVARS.getRate(triggers)
	
	print (varTrig + " > " + str(int(thres)) + " ("+p1+", "+p2+") rate: " + str(dataRate))
	if (getUnique):
		return dataRate_unique
	return dataRate

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
	
