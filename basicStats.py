import numpy as np
from matplotlib import pyplot as plt
import re
import sys

#first arg is path to saveFile (same as for sim), second is number of runs, third one is # of added species (0 if none)

savePath = sys.argv[1]

runs = int(sys.argv[2])

extraSpecs = int(sys.argv[3])

simParams = open(savePath+'/simParams', 'r')

lines = []
for line in simParams:
	lines.append(line.strip('\n'))

#getting sim params to know how to parse rawSaves

evo = False
eco = False
multi = False

"""
if lines[1] == 'y':
	eco = True

if lines[14] == 'y':
	evo = True

if lines[32] == 'y':
	multi = True

numEnvs = int(lines[34])

numSpecs = int(lines[30])


numSteps = int(lines[-11])
"""

numEnvs = 1
numSpecs = 17
numSteps = 100000



def parseOne(fileName, savePath, numSteps, numSpecs, eco, evo, runs, multi, numEnv):	
	#initiating np.arrays to hold values

	fDens = np.zeros((runs, numSpecs), dtype=float)

	iDens = np.zeros((runs, numSpecs), dtype=float)

	ccs = np.zeros((runs, numSpecs), dtype=float)

	alphas = np.zeros((runs, numSpecs), dtype=float)

	betas = np.zeros((runs, numSpecs), dtype=float)
	if evo:
 		evoRates = np.zeros((runs, numSpecs), dtype=int)

	if eco:
		optimas = np.zeros((runs, numSpecs), dtype = float)

	imMax = np.zeros((runs, numSpecs), dtype=float)
	imMin = np.zeros((runs, numSpecs), dtype=float)
	imMean = np.zeros((runs, numSpecs), dtype=float)
	imMed = np.zeros((runs, numSpecs), dtype=float)
	imStdev = np.zeros((runs, numSpecs), dtype=float)
	rankedInMean = [0]*runs
	rankedIden = [0]*runs
	rankedAlpha = [0]*runs
	rankedBeta = [0]*runs
	rankedER = [0]*runs

	ltc = np.zeros((runs), dtype=float)




	fDen = False					#bools to know which kind of value we're parsing
	iDen = False
	cc = False
	alpha = False
	beta = False
	interactions = False
	opt = False
	new = False
	eRates = False

	#creating incremental variables to get numpy.array indexes

	count = -1			#keeps track of which run we're on
	specs = 0			#keeps track of how many species have been processed



	toParse = open(fileName, 'r')

	for line in toParse:
									#setting bools in accordance with the kind of value we're parsing
		if line == 'Interactions:\n' and new:
			interactions = True
			new = False
			toRank = []
		elif line == 'ccs:\n':
			cc = True
		elif line == 'Alphas:\n':
			alpha = True
			new = True
			aRank = []
		elif line == 'Betas:\n':
			beta = True
			bRank = []
		elif line == 'Densities at step ' +str(numSteps)+':\n':
			fDen = True
		elif line == 'Optimums:\n':
			opt = True
		elif line[0:3] == 'Run':
			count+=1
		elif line == 'Density:\n':
			iDen = True
			iRank = []
		elif line[0] == 'L':
			temp = line.strip('Last turn change: \n')
			if temp != '':
				ltc[count] = float(temp)
			else:
				ltc[count] = -1
		elif line == "Evo Rate:\n":
			eRates = True
			erRank = []

		elif interactions:			#parsing lines according to bools
			specs += 1
			
			thisI = line.strip('Species \n').split(': ')

			thisI2 = np.array(thisI[1].split('\t')[0:-1], dtype = float)



			imMin[count][int(thisI[0])] =thisI2.min()
			imMax[count][int(thisI[0])] =thisI2.max()
			imMean[count][int(thisI[0])] =thisI2.mean()
			imMed[count][int(thisI[0])] =np.median(thisI2)
			imStdev[count][int(thisI[0])] =thisI2.std()

			toRank.append([int(thisI[0]), thisI2.mean()])

			if specs == numSpecs:
				specs = 0
				interactions = False
				toRank.sort(key = lambda meanInt:meanInt[1]) 
				rankedInMean[count] = toRank
		elif iDen:
			specs +=1
			temp = line.strip('Species \n').split(': ')
			iRank.append([int(temp[0]), float(temp[1])])
			iDens[count][int(temp[0])]=float(temp[1])
			if specs == numSpecs:
				specs = 0
				iDen = False
				iRank.sort(key = lambda meanInt:meanInt[1])
				rankedIden[count] = iRank

		elif fDen:
			specs +=1
			temp = line.strip('Species \n').split(': ')
			fDens[count][int(temp[0])]=float(temp[1])
			if specs == numSpecs:
				specs = 0
				fDen = False

		elif cc:
			specs +=1
			temp = line.strip('Species \n').split(': ')
			ccs[count][int(temp[0])]=float(temp[1])
			if specs == numSpecs:
				specs = 0
				cc = False
		elif alpha:
			specs +=1
			temp = line.strip('Species \n').split(': ')
			aRank.append([int(temp[0]), float(temp[1])])
			alphas[count][int(temp[0])]=float(temp[1])
			if specs == numSpecs:
				specs = 0
				alpha = False
				aRank.sort(key = lambda meanInt:meanInt[1])
				rankedAlpha[count] = aRank
		elif opt:
			specs +=1
			temp = line.strip('Species \n').split(': ')
			optimas[count][int(temp[0])]=float(temp[1])
			if specs == numSpecs:
				specs = 0
				opt = False

		elif beta:
			specs +=1
			temp = line.strip('Species \n').split(': ')
			bRank.append([int(temp[0]), float(temp[1])])
			betas[count][int(temp[0])]=float(temp[1])
			if specs == numSpecs:
				specs = 0
				beta = False
				bRank.sort(key = lambda meanInt:meanInt[1])
				rankedBeta[count] = bRank

		else:
			pass


	toParse.close()
	res = [alphas, betas, ccs, iDens, fDens, imMin, imMax, imMean, imMed, imStdev, ltc]



	if eco:
		res.append(optimas)

	return res

def makeStats(toStat, eco, numEnv, savePath, extraSpecs):
	"""eco is a bool, toPlot is the list
	with all the np.arrays to plot."""

	alphas = res[0]
	betas = res[1]
	ccs = res[2]
	iDens = res[3]	
	fDens = res[4]
	imMin = res[5]		
	imMax = res[6]
	imMean = res[7]
	imMed = res[8]
	imStdev = res[9]
	ltc = res[10]
	if eco:
		optimas = res[11]										


	parsed = open(savePath+'/baseStats'+str(numEnv), 'w+')

	parsed.write('Number of runs: '+str(runs)+'\n')

	parsed.write('Number of iterations per run: '+str(numSteps)+'\n')

	parsed.write('Number of species: '+str(numSpecs)+'\n')

	deathToll = np.zeros((runs), dtype=int)

	for i in range(runs):
		dt = 0
		for j in range(numSpecs):
			if fDens[i][j] == 0:
				dt +=1
		deathToll[i] = dt


	parsed.write('\t\t\t\t\tMin\t\t\t\tMax\t\t\t\tMean\t\t\t\tMedian\t\t\t\tstdev\n\n')
	parsed.write('Death toll:\t\t\t '+str(deathToll.min())+'\t\t\t\t'+str(deathToll.max())+'\t\t\t\t'+str(deathToll.mean())+'\t\t\t\t'+str(np.median(deathToll))+'\t\t\t\t'+str(deathToll.std())+'\n')

	parsed.write('Last turn change: '+str(ltc.min())+'\t'+str(ltc.max())+'\t'+str(ltc.mean())+'\t'+str(np.median(ltc))+'\t'+str(ltc.std())+'\n')

	parsed.write('\nInitial Densities: \n')
	parsed.write('Initial Density mins: '+str(iDens.min(1).min())+'\t'+str(iDens.min(1).max())+'\t'+str(iDens.min(1).mean())+
		'\t'+str(np.median(iDens.min(1)))+'\t'+str(iDens.min(1).std())+'\n')
	parsed.write('Initial Density maxs: '+str(iDens.max(1).min())+'\t'+str(iDens.max(1).max())+'\t'+str(iDens.max(1).mean())+
		'\t'+str(np.median(iDens.max(1)))+'\t'+str(iDens.max(1).std())+'\n')
	parsed.write('Initial Density mean: '+str(iDens.mean(1).min())+'\t'+str(iDens.mean(1).max())+'\t'+str(iDens.mean(1).mean())+
		'\t'+str(np.median(iDens.mean(1)))+'\t'+str(iDens.mean(1).std())+'\n')
	idMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		idMed[i] = np.median(iDens[i])
	parsed.write('Initial Density median: '+str(idMed.min())+'\t'+str(idMed.max())+'\t'+str(idMed.mean())+
		'\t'+str(np.median(idMed))+'\t'+str(idMed.std())+'\n')
	parsed.write('Initial Density stdev: '+str(iDens.std(1).min())+'\t'+str(iDens.std(1).max())+'\t'+str(iDens.std(1).mean())+
		'\t'+str(np.median(iDens.std(1)))+'\t'+str(iDens.std(1).std())+'\n')


	parsed.write('\nFinal Densities: \n')
	parsed.write('Final Density mins: '+str(fDens.min(1).min())+'\t'+str(fDens.min(1).max())+'\t'+str(fDens.min(1).mean())+
		'\t'+str(np.median(fDens.min(1)))+'\t'+str(fDens.min(1).std())+'\n')
	parsed.write('Final Density maxs: '+str(fDens.max(1).min())+'\t'+str(fDens.max(1).max())+'\t'+str(fDens.max(1).mean())+
		'\t'+str(np.median(fDens.max(1)))+'\t'+str(fDens.max(1).std())+'\n')
	parsed.write('Final Density mean: '+str(fDens.mean(1).min())+'\t'+str(fDens.mean(1).max())+'\t'+str(fDens.mean(1).mean())+
		'\t'+str(np.median(fDens.mean(1)))+'\t'+str(fDens.mean(1).std())+'\n')
	fdMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		fdMed[i] = np.median(fDens[i])
	parsed.write('Final Density median: '+str(fdMed.min())+'\t'+str(fdMed.max())+'\t'+str(fdMed.mean())+
		'\t'+str(np.median(fdMed))+'\t'+str(fdMed.std())+'\n')
	parsed.write('final Density stdev: '+str(fDens.std(1).min())+'\t'+str(fDens.std(1).max())+'\t'+str(fDens.std(1).mean())+
		'\t'+str(np.median(fDens.std(1)))+'\t'+str(fDens.std(1).std())+'\n')


	parsed.write('\nCarrying Capacities: \n')
	parsed.write('Carrying Capacity mins: '+str(ccs.min(1).min())+'\t'+str(ccs.min(1).max())+'\t'+str(ccs.min(1).mean())+
		'\t'+str(np.median(ccs.min(1)))+'\t'+str(ccs.min(1).std())+'\n')
	parsed.write('Carrying Capacity maxs: '+str(ccs.max(1).min())+'\t'+str(ccs.max(1).max())+'\t'+str(ccs.max(1).mean())+
		'\t'+str(np.median(ccs.max(1)))+'\t'+str(ccs.max(1).std())+'\n')
	parsed.write('Carrying Capacity mean: '+str(ccs.mean(1).min())+'\t'+str(ccs.mean(1).max())+'\t'+str(ccs.mean(1).mean())+
		'\t'+str(np.median(ccs.mean(1)))+'\t'+str(ccs.mean(1).std())+'\n')
	ccMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		ccMed[i] = np.median(ccs[i])
	parsed.write('Carrying Capacity median: '+str(ccMed.min())+'\t'+str(ccMed.max())+'\t'+str(ccMed.mean())+
		'\t'+str(np.median(ccMed))+'\t'+str(ccMed.std())+'\n')
	parsed.write('Carrying Capacity stdev: '+str(ccs.std(1).min())+'\t'+str(ccs.std(1).max())+'\t'+str(ccs.std(1).mean())+
		'\t'+str(np.median(ccs.std(1)))+'\t'+str(ccs.std(1).std())+'\n')


	parsed.write('\nAlphas: \n')
	parsed.write('Alpha mins: '+str(alphas.min(1).min())+'\t'+str(alphas.min(1).max())+'\t'+str(alphas.min(1).mean())+
		'\t'+str(np.median(alphas.min(1)))+'\t'+str(alphas.min(1).std())+'\n')
	parsed.write('Alpha maxs: '+str(alphas.max(1).min())+'\t'+str(alphas.max(1).max())+'\t'+str(alphas.max(1).mean())+
		'\t'+str(np.median(alphas.max(1)))+'\t'+str(alphas.max(1).std())+'\n')
	parsed.write('Alpha mean: '+str(alphas.mean(1).min())+'\t'+str(alphas.mean(1).max())+'\t'+str(alphas.mean(1).mean())+
		'\t'+str(np.median(alphas.mean(1)))+'\t'+str(alphas.mean(1).std())+'\n')
	alphasMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		alphasMed[i] = np.median(alphas[i])
	parsed.write('Alpha median: '+str(alphasMed.min())+'\t'+str(alphasMed.max())+'\t'+str(alphasMed.mean())+
		'\t'+str(np.median(alphasMed))+'\t'+str(alphasMed.std())+'\n')
	parsed.write('Alpha stdev: '+str(alphas.std(1).min())+'\t'+str(alphas.std(1).max())+'\t'+str(alphas.std(1).mean())+
		'\t'+str(np.median(alphas.std(1)))+'\t'+str(alphas.std(1).std())+'\n')


	parsed.write('\nBetas: \n')
	parsed.write('Beta mins: '+str(betas.min(1).min())+'\t'+str(betas.min(1).max())+'\t'+str(betas.min(1).mean())+
		'\t'+str(np.median(betas.min(1)))+'\t'+str(betas.min(1).std())+'\n')
	parsed.write('Beta maxs: '+str(betas.max(1).min())+'\t'+str(betas.max(1).max())+'\t'+str(betas.max(1).mean())+
		'\t'+str(np.median(betas.max(1)))+'\t'+str(betas.max(1).std())+'\n')
	parsed.write('Beta mean: '+str(betas.mean(1).min())+'\t'+str(betas.mean(1).max())+'\t'+str(betas.mean(1).mean())+
		'\t'+str(np.median(betas.mean(1)))+'\t'+str(betas.mean(1).std())+'\n')
	betasMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		betasMed[i] = np.median(betas[i])
	parsed.write('Beta median: '+str(betasMed.min())+'\t'+str(betasMed.max())+'\t'+str(betasMed.mean())+
		'\t'+str(np.median(betasMed))+'\t'+str(betasMed.std())+'\n')
	parsed.write('Beta stdev: '+str(betas.std(1).min())+'\t'+str(betas.std(1).max())+'\t'+str(betas.std(1).mean())+
		'\t'+str(np.median(betas.std(1)))+'\t'+str(betas.std(1).std())+'\n')


	if eco:
		parsed.write('\noptimas: \n')
		parsed.write('opt mins: '+str(optimas.min(1).min())+'\t'+str(optimas.min(1).max())+'\t'+str(optimas.min(1).mean())+
			'\t'+str(np.median(optimas.min(1)))+'\t'+str(optimas.min(1).std())+'\n')
		parsed.write('opt maxs: '+str(optimas.max(1).min())+'\t'+str(optimas.max(1).max())+'\t'+str(optimas.max(1).mean())+
			'\t'+str(np.median(optimas.max(1)))+'\t'+str(optimas.max(1).std())+'\n')
		parsed.write('opt mean: '+str(optimas.mean(1).min())+'\t'+str(optimas.mean(1).max())+'\t'+str(optimas.mean(1).mean())+
			'\t'+str(np.median(optimas.mean(1)))+'\t'+str(optimas.mean(1).std())+'\n')
		optimasMed = np.zeros(runs, dtype=float)
		for i in range(runs):
			optimasMed[i] = np.median(optimas[i])
		parsed.write('opt median: '+str(optimasMed.min())+'\t'+str(optimasMed.max())+'\t'+str(optimasMed.mean())+
			'\t'+str(np.median(optimasMed))+'\t'+str(optimasMed.std())+'\n')
		parsed.write('opt stdev: '+str(optimas.std(1).min())+'\t'+str(optimas.std(1).max())+'\t'+str(optimas.std(1).mean())+
			'\t'+str(np.median(optimas.std(1)))+'\t'+str(optimas.std(1).std())+'\n')


	parsed.write('\nInteraction mins: \n')
	parsed.write('InteractionMin mins: '+str(imMin.min(1).min())+'\t'+str(imMin.min(1).max())+'\t'+str(imMin.min(1).mean())+
		'\t'+str(np.median(imMin.min(1)))+'\t'+str(imMin.min(1).std())+'\n')
	parsed.write('InteractionMin maxs: '+str(imMin.max(1).min())+'\t'+str(imMin.max(1).max())+'\t'+str(imMin.max(1).mean())+
		'\t'+str(np.median(imMin.max(1)))+'\t'+str(imMin.max(1).std())+'\n')
	parsed.write('InteractionMin mean: '+str(imMin.mean(1).min())+'\t'+str(imMin.mean(1).max())+'\t'+str(imMin.mean(1).mean())+
		'\t'+str(np.median(imMin.mean(1)))+'\t'+str(imMin.mean(1).std())+'\n')
	imMinMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		imMinMed[i] = np.median(imMin[i])
	parsed.write('InteractionMin median: '+str(imMinMed.min())+'\t'+str(imMinMed.max())+'\t'+str(imMinMed.mean())+
		'\t'+str(np.median(imMinMed))+'\t'+str(imMinMed.std())+'\n')
	parsed.write('InteractionMin stdev: '+str(imMin.std(1).min())+'\t'+str(imMin.std(1).max())+'\t'+str(imMin.std(1).mean())+
		'\t'+str(np.median(imMin.std(1)))+'\t'+str(imMin.std(1).std())+'\n')


	parsed.write('\nInteraction maxs: \n')
	parsed.write('InteractionMax mins: '+str(imMax.max(1).min())+'\t'+str(imMax.max(1).max())+'\t'+str(imMax.min(1).mean())+
		'\t'+str(np.median(imMax.min(1)))+'\t'+str(imMax.min(1).std())+'\n')
	parsed.write('InteractionMax maxs: '+str(imMax.max(1).min())+'\t'+str(imMax.max(1).max())+'\t'+str(imMax.max(1).mean())+
		'\t'+str(np.median(imMax.max(1)))+'\t'+str(imMax.max(1).std())+'\n')
	parsed.write('InteractionMax mean: '+str(imMax.mean(1).min())+'\t'+str(imMax.mean(1).max())+'\t'+str(imMax.mean(1).mean())+
		'\t'+str(np.median(imMax.mean(1)))+'\t'+str(imMax.mean(1).std())+'\n')
	imMaxMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		imMaxMed[i] = np.median(imMax[i])
	parsed.write('InteractionMax median: '+str(imMaxMed.min())+'\t'+str(imMaxMed.max())+'\t'+str(imMaxMed.mean())+
		'\t'+str(np.median(imMaxMed))+'\t'+str(imMaxMed.std())+'\n')
	parsed.write('InteractionMax stdev: '+str(imMax.std(1).min())+'\t'+str(imMax.std(1).max())+'\t'+str(imMax.std(1).mean())+
		'\t'+str(np.median(imMax.std(1)))+'\t'+str(imMax.std(1).std())+'\n')


	parsed.write('\nInteraction means: \n')
	parsed.write('InteractionMean mins: '+str(imMean.max(1).min())+'\t'+str(imMean.max(1).max())+'\t'+str(imMean.min(1).mean())+
		'\t'+str(np.median(imMean.min(1)))+'\t'+str(imMean.min(1).std())+'\n')
	parsed.write('InteractionMean maxs: '+str(imMean.max(1).min())+'\t'+str(imMean.max(1).max())+'\t'+str(imMean.max(1).mean())+
		'\t'+str(np.median(imMean.max(1)))+'\t'+str(imMean.max(1).std())+'\n')
	parsed.write('InteractionMean mean: '+str(imMean.mean(1).min())+'\t'+str(imMean.mean(1).max())+'\t'+str(imMean.mean(1).mean())+
		'\t'+str(np.median(imMean.mean(1)))+'\t'+str(imMean.mean(1).std())+'\n')
	imMeanMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		imMeanMed[i] = np.median(imMean[i])
	parsed.write('InteractionMean median: '+str(imMeanMed.min())+'\t'+str(imMeanMed.max())+'\t'+str(imMeanMed.mean())+
		'\t'+str(np.median(imMeanMed))+'\t'+str(imMeanMed.std())+'\n')
	parsed.write('InteractionMean stdev: '+str(imMean.std(1).min())+'\t'+str(imMean.std(1).max())+'\t'+str(imMean.std(1).mean())+
		'\t'+str(np.median(imMean.std(1)))+'\t'+str(imMean.std(1).std())+'\n')

	parsed.write('\nInteraction meds: \n')
	parsed.write('InteractionMed mins: '+str(imMed.max(1).min())+'\t'+str(imMed.max(1).max())+'\t'+str(imMed.min(1).mean())+
		'\t'+str(np.median(imMed.min(1)))+'\t'+str(imMed.min(1).std())+'\n')
	parsed.write('InteractionMed maxs: '+str(imMed.max(1).min())+'\t'+str(imMed.max(1).max())+'\t'+str(imMed.max(1).mean())+
		'\t'+str(np.median(imMed.max(1)))+'\t'+str(imMed.max(1).std())+'\n')
	parsed.write('InteractionMed mean: '+str(imMed.mean(1).min())+'\t'+str(imMed.mean(1).max())+'\t'+str(imMed.mean(1).mean())+
		'\t'+str(np.median(imMed.mean(1)))+'\t'+str(imMed.mean(1).std())+'\n')
	imMedMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		imMedMed[i] = np.median(imMed[i])
	parsed.write('InteractionMed median: '+str(imMedMed.min())+'\t'+str(imMedMed.max())+'\t'+str(imMedMed.mean())+
		'\t'+str(np.median(imMedMed))+'\t'+str(imMedMed.std())+'\n')
	parsed.write('InteractionMed stdev: '+str(imMed.std(1).min())+'\t'+str(imMed.std(1).max())+'\t'+str(imMed.std(1).mean())+
		'\t'+str(np.median(imMed.std(1)))+'\t'+str(imMed.std(1).std())+'\n')

	parsed.write('\nInteraction stdevs: \n')
	parsed.write('InteractionStdev mins: '+str(imStdev.max(1).min())+'\t'+str(imStdev.max(1).max())+'\t'+str(imStdev.min(1).mean())+
		'\t'+str(np.median(imStdev.min(1)))+'\t'+str(imStdev.min(1).std())+'\n')
	parsed.write('InteractionStdev maxs: '+str(imStdev.max(1).min())+'\t'+str(imStdev.max(1).max())+'\t'+str(imStdev.max(1).mean())+
		'\t'+str(np.median(imStdev.max(1)))+'\t'+str(imStdev.max(1).std())+'\n')
	parsed.write('InteractionStdev mean: '+str(imStdev.mean(1).min())+'\t'+str(imStdev.mean(1).max())+'\t'+str(imStdev.mean(1).mean())+
		'\t'+str(np.median(imStdev.mean(1)))+'\t'+str(imStdev.mean(1).std())+'\n')
	imStdevMed = np.zeros(runs, dtype=float)
	for i in range(runs):
		imStdevMed[i] = np.median(imStdev[i])
	parsed.write('Interactionstddev median: '+str(imStdevMed.min())+'\t'+str(imStdevMed.max())+'\t'+str(imStdevMed.mean())+
		'\t'+str(np.median(imStdevMed))+'\t'+str(imStdevMed.std())+'\n')
	parsed.write('InteractionStdev stdev: '+str(imStdev.std(1).min())+'\t'+str(imStdev.std(1).max())+'\t'+str(imStdev.std(1).mean())+
		'\t'+str(np.median(imStdev.std(1)))+'\t'+str(imStdev.std(1).std())+'\n')


	extraSurvival = [0]*extraSpecs

	for j in range(len(fDens)):
		for i in range(extraSpecs):
			if fDens[j][-i-1] != 0:
				extraSurvival[-i-1] += 1

	parsed.write("\nExtra survivals: "+str(extraSurvival)+"\n")


	parsed.close()

	return 0


def survivalDiff(savePath, fDens, numEnvs, numSpecs, runs):
	"""Checks the difference in surviving species between
	different environments (only used for multi-env simulations)"""

	survDiffSave = savePath+'/survDiff'
	save = open(survDiffSave, 'w+')

	totCount = 0
	totDiff = []

	for k in range(runs):
		save.write("Run #"+str(k)+" :\n")
		atLeastOne = False
		for i in range(numEnvs):
			save.write("Env #"+str(i)+" :\n")
			interDiff = 0

			for j in range(numEnvs):
				diff = 0
				for l in range(numSpecs):
					if(fDens[i][k][l] == 0 and fDens[j][k][l] != 0):
						diff += 1
						atLeastOne = True
					elif(fDens[i][k][l] != 0 and fDens[j][k][l] == 0):
						diff += 1
						atLeastOne = True
					else:
						pass
				save.write(str(diff)+"\t")

			save.write("\n")

			for j in range(numEnvs):
				diff2 = 0
				for l in range(numSpecs):
					diff2 += abs(fDens[i][k][l] - fDens[j][k][l])
					#print(diff2)
	
				save.write(str(diff2)+"\t")
				interDiff += diff2
			totDiff.append(interDiff/numEnvs)
			save.write("\n")

		if atLeastOne:
			totCount += 1
	totDiff = np.array(totDiff)
	save.write("Total sims with at least one survivalDiff: "+str(totCount)+"\n")
	save.write("Mean density diff between sims: "+str(totDiff.mean())+"\n")
	save.write("Median density diff between sims: "+str(np.median(totDiff))+"\n")
	save.write("Stdev: "+str(totDiff.std())+"\n")

	save.close();



if not multi:
	fileName = savePath + '/rawSave'
	res = parseOne(fileName, savePath, numSteps, numSpecs, eco, evo, runs, multi, 0)
	makeStats(res, eco, 0, savePath, extraSpecs)

else:
	allFDens = []
	for i in range(numEnvs):
		fileName = savePath + '/rawSaveEnv' + str(i)
		res = parseOne(fileName, savePath, numSteps, numSpecs, eco, evo, runs, multi, i)
		allFDens.append(res[4])
		makeStats(res, eco, i, savePath, extraSpecs)

	survivalDiff(savePath, allFDens, numEnvs, numSpecs, runs)
