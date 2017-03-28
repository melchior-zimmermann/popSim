
#include <iostream>
#include <string>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <numeric>
#include <sstream>
#include <memory>
#include <vector>
#include <cmath>

using namespace std;

#include "Environment.hpp"
#include "Species.hpp"

#include "Change.hpp"
#include "StdChange.hpp"
#include "EcoChange.hpp"

#include "Evo.hpp"
#include "StdEvo.hpp"
#include "EcoEvo.hpp"
#include "NoEvo.hpp"

#include "Save.hpp"

#include "NextGen.hpp"
#include "StdNextGen.hpp"
#include "EcoNextGen.hpp"
#include "EvoNextGen.hpp"
#include "E2NextGen.hpp"
#include "StdMultiNextGen.hpp"
#include "EcoMultiNextGen.hpp"
#include "EvoMultiNextGen.hpp"
#include "E2MultiNextGen.hpp"
#include "RKStdNextGen.hpp"
#include "RKEcoNextGen.hpp"
#include "RKEvoNextGen.hpp"
#include "RKE2NextGen.hpp"
#include "RKStdMultiNextGen.hpp"
#include "RKEcoMultiNextGen.hpp"
#include "RKEvoMultiNextGen.hpp"
#include "RKE2MultiNextGen.hpp"

#include "helpers.hpp"
#include "interface.hpp"
#include "initializers.hpp"


int initSpecs(vector<unique_ptr<Species>>* speciesList, simParams& params, double* envConst){

	for(int i = 0; i<params.numSpecs; i++){
		int numSelf = i;
		int numTotal = params.numSpecs;
		int genTime = getUniformInt(params.genTimeRange);
		int indToDens = getUniformInt(params.evoResRange);
		double alpha = getUniform(params.alphaRange);
		double beta = getUniform(params.betaRange);
		double density = getUniform(params.densRange);
		double cc = getUniform(params.ccRange);
		double optimum = getUniform(params.optRangeSpec);
		double optRange = getUniform(params.optEvoRange);


		vector<double> interactions (params.numSpecs, 0);

		for(int j = 0; j<params.numSpecs; j++){
			interactions[j] = getUniform(params.interRange);
			if (abs(interactions[j]) < params.cutoffThreshold) {
				interactions[j] = 0;
			}
		}

		interactions[i] = 0;

		vector<double> range (params.numSpecs, 0);

		for(int j = 0; j<params.numSpecs; j++){
			range[j] = getUniform(params.evoRangeRange);
		}

		range[i] = 0;


		vector<vector<vector<double>>> routes(params.numEnvs);
		for(int j = 0; j<params.numEnvs; j++){
			vector<vector<double>> routesTo(params.numEnvs);
			for(int l =0; l<params.numEnvs; l++){
				vector<double> migParams(2);
				migParams[0] = getUniform(params.migProbRange);
				migParams[1] = getUniform(params.migSizeRange);

				routesTo[l] = migParams;

			}

			routes[j] = routesTo;
			
		}

		unique_ptr<Change> change;
		if(params.eco == 'y'){
			change = make_unique<EcoChange>();

		}else{
			change = make_unique<StdChange>();
		}

		unique_ptr<Evo> evo;
		if(params.eco == 'y' && params.evo == 'y'){
			evo = make_unique<EcoEvo>();
		}else if(params.evo == 'y'){
			evo = make_unique<StdEvo>();
		}else{
			evo = make_unique<NoEvo>();
		}

		evo->setDelta(params.delta);

		unique_ptr<Species> spec (new Species(numSelf, numTotal, genTime, indToDens, alpha, beta, density, cc, optimum, optRange,
			envConst, interactions, range, routes, move(change), move(evo)));

		speciesList->push_back(move(spec));
	}
	return 0;
}



Environment getEnv(string savePath, simParams params, int numSelf, vector<Environment>* allEnvs){

	vector<unique_ptr<Species>> speciesList;
	bool eco = false;
	bool evo = false;
	bool multi = false;
	string saveFile = savePath + "/rawSave";
	
	unique_ptr<NextGen> nextGen;

	if (params.rk) {
		if(params.evo == 'y' && params.eco == 'y' && params.multi == 'y'){
			//save = make_unique<E2Save>();
			nextGen = make_unique<RKE2MultiNextGen>();
			eco = true;
			evo = true;
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);

		}else if(params.evo == 'y' && params.eco == 'y' ){
			//save = make_unique<E2Save>();
			nextGen = make_unique<RKE2NextGen>();
			eco = true;
			evo = true;

		}else if (params.evo == 'y' && params.multi == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<RKEvoMultiNextGen>();
			evo = true;
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);

		}else if (params.eco == 'y' && params.multi == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<RKEcoMultiNextGen>();
			eco = true;
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);

		}else if (params.multi == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<RKStdMultiNextGen>();
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);
			

		}else if (params.evo == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<RKEvoNextGen>();
			evo = true;

		}else if (params.eco == 'y'){
			//save = make_unique<EcoSave>();
			nextGen = make_unique<RKEcoNextGen>();
			eco = true;

		}else{
			//save = make_unique<StdSave>();
			nextGen = make_unique<RKStdNextGen>();
		}
	}
	else {
		if(params.evo == 'y' && params.eco == 'y' && params.multi == 'y'){
			//save = make_unique<E2Save>();
			nextGen = make_unique<E2MultiNextGen>();
			eco = true;
			evo = true;
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);

		}else if(params.evo == 'y' && params.eco == 'y' ){
			//save = make_unique<E2Save>();
			nextGen = make_unique<E2NextGen>();
			eco = true;
			evo = true;

		}else if (params.evo == 'y' && params.multi == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<EvoMultiNextGen>();
			evo = true;
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);

		}else if (params.eco == 'y' && params.multi == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<EcoMultiNextGen>();
			eco = true;
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);

		}else if (params.multi == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<StdMultiNextGen>();
			multi = true;
			saveFile = saveFile + "Env" + to_string(numSelf);
			

		}else if (params.evo == 'y'){
			//save = make_unique<EvoSave>();
			nextGen = make_unique<EvoNextGen>();
			evo = true;

		}else if (params.eco == 'y'){
			//save = make_unique<EcoSave>();
			nextGen = make_unique<EcoNextGen>();
			eco = true;

		}else{
			//save = make_unique<StdSave>();
			nextGen = make_unique<StdNextGen>();
		}
	}
	unique_ptr<Save> save(new Save(evo, eco, multi));
	//cout<<save.get()<<endl;
	vector<double> change;
	vector<double> pertRange (2);
	pertRange[0] = params.pertRange[0];
	pertRange[1] = params.pertRange[1];

	Environment env(params.numSpecs, numSelf, params.numEnvs, params.delta, params.deathThreshold, getUniform(params.optRangeEnv), params.perturbationProb,
		savePath, saveFile, change, pertRange, move(speciesList),
		allEnvs, move(save), move(nextGen));



	initSpecs(env.getSpeciesList(), params, env.getEnvConstPtr());

	if (params.ws) {
		vector<vector<double>> interactionGraph = getWSGraph(params);
		vector<unique_ptr<Species>>* speciesList = env.getSpeciesList();
		for (int i = 0; i<params.numSpecs; i++) {
			(*speciesList)[i]->setInteractions(interactionGraph[i]);
		}
	}
	//env->setSpeciesList(speciesList);
	env.initChange();
	
	return move(env);

}
 
