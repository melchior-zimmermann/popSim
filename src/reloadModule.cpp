#include "Environment.hpp"
#include "Species.hpp"
#include "Simulation.hpp"
#include "EcoSimulation.hpp"
#include "EvoSimulation.hpp"
#include "E2Simulation.hpp"
#include "MultiSimulation.hpp"
#include "MultiEcoSimulation.hpp"
#include "MultiEvoSimulation.hpp"
#include "E2MSimulation.hpp"
#include "helpers.hpp"
#include "interface.hpp"
#include "reloadModule.hpp"
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
#include <iostream>
#include <string>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <numeric>
#include <sstream>
#include <vector>
#include <memory>
#include <sys/stat.h>
#include <cstdlib>


using namespace std;

Environment loadEnv(string newSavePath, vector<Environment>* allEnvs, simParams* params, simValues* values, int numSelf, string savePath);
void loadSpecs(vector<unique_ptr<Species>>* speciesList, simParams* params, simValues* values, double* envConst);


int reRunSim(string oldSavePath, string newSavePath, int addSpec, int numRuns){
	//this function reloads data from a previous simulation
	//and runs it again (used to verify results or run 100% custom sims) 
	//file from which sim Values are to be loaded has to be called "/completeSave",
	//and contain a complete save of the sim at the time at which we want to reload it
	//In case of multiSims, addSpec specs will be added to EACH environment
	
	simParams params;

	struct stat st;

	if(loadSimParams(oldSavePath, &params)!= 0){
		cout<<"Failed to load params!\n";
		cout<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}

	if (stat(newSavePath.c_str(), &st) == -1) {
	    mkdir(newSavePath.c_str(), 0700);
	}

	if(stat(newSavePath.c_str(), &st) == -1){
		cout<<"Could not create save directory:\n";
		cout<<strerror(errno);
		exit(EXIT_FAILURE);
	}

	

	vector<simValues> values;


	if(params.completeStart && params.completeEnd){
		cout<<"Found two complete saves (start and end), do not know which to load!"<<endl;
		cout<<"(set one of them to anything but 'y' in "<<oldSavePath<<"/simParams)"<<endl;
		exit(EXIT_FAILURE);
	}

	if(params.completeStart){
		if(params.multi != 'y'){
			simValues vals;
			string savePath = oldSavePath + "/completeSaveS";
			loadSimValues(&params, &vals, savePath);
			//cout<<"where\n";
			vals.addSpec = addSpec;
			vals.totSpec = vals.oldSpec+addSpec;
			//cout<<"are"<<endl;
			values.push_back(vals);
			//cout<<"you?"<<endl;
		}else{
			for(int i = 0; i<params.numEnvs; i++){
				simValues vals;
				string savePath = oldSavePath + "/completeSaveEnv"+to_string(i)+"S";
				loadSimValues(&params, &vals, savePath);
				vals.addSpec = addSpec;
				vals.totSpec = vals.oldSpec+addSpec;
				values.push_back(vals);
			}
		}
	}else if(params.completeEnd){
		if(params.multi != 'y'){
			simValues vals;
			string savePath = oldSavePath + "/completeSaveE";
			loadSimValues(&params, &vals, savePath);
			vals.addSpec = addSpec;
			vals.totSpec = (vals.oldSpec+addSpec);
			//cout<<"right\n";
			values.push_back(vals);
			//cout<<"here\n";
		}else{
			for(int i = 0; i<params.numEnvs; i++){
				simValues vals;
				string savePath = oldSavePath + "/completeSaveEnv"+to_string(i)+"E";
				loadSimValues(&params, &vals, savePath);
				vals.addSpec = addSpec;
				vals.totSpec = vals.oldSpec+addSpec;
				values.push_back(vals);
			}
		}
	}else{
		cout<<"Could not find a complete save (completeStart and completeEnd != 'y' in file "<<oldSavePath<<"/simParams)"<<endl;
		cout<<"Aborting..."<<endl;
		exit(EXIT_FAILURE);
	}

	if(params.multi != 'y'){
		if(initSave(newSavePath+"/rawSave") != 0){
			cout<<"Could no init saveFile "<<newSavePath+"/rawSave"<<endl;
			exit(EXIT_FAILURE);
		}
		if(params.completeEnd){
			if(initSave(newSavePath+"/completeSaveE") != 0){
				cout<<"Could no init saveFile "<<newSavePath+"/completeSaveE"<<endl;
				exit(EXIT_FAILURE);
			}
		}
		if(params.completeStart){
			if(initSave(newSavePath+"/completeSaveS") != 0){
				cout<<"Could no init saveFile "<<newSavePath+"/completeSaveS"<<endl;
				exit(EXIT_FAILURE);
			}
		}
	}else{
		for(int i = 0; i<params.numEnvs; i++){
			if(initSave(newSavePath+"/rawSaveEnv"+to_string(i)) != 0){
				cout<<"Could no init saveFile "<<newSavePath+"/rawSaveEnv"<<i<<endl;
				exit(EXIT_FAILURE);
			}
			if(params.completeEnd){
				if(initSave(newSavePath+"/completeSaveEnv"+to_string(i)+"E") != 0){
					cout<<"Could no init saveFile "<<newSavePath+"/completeSaveE"<<endl;
					exit(EXIT_FAILURE);
				}
			}
			if(params.completeStart){
				if(initSave(newSavePath+"/completeSaveEnv"+to_string(i)+"S") != 0){
					cout<<"Could no init saveFile "<<newSavePath+"/completeSaveS"<<endl;
					exit(EXIT_FAILURE);
				}
			}

		}
	}

	for(int k = 0; k<numRuns; k++){

		if(params.evo == 'y' && params.eco == 'y' && params.multi == 'y'){
			
			vector<Environment> envs;

			E2MSimulation sim(move(envs));

			for(int i = 0; i<params.numEnvs; i++){
				sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[i]), i, newSavePath));
			}

			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setInterSaveDiv(params.interSaveDiv);
			sim.setNumEnvs(params.numEnvs);
			sim.setNumSpecs(params.numSpecs);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);

		}else if(params.evo == 'y' && params.eco == 'y' ){
			
			vector<Environment> envs;

			E2Simulation sim(move(envs));

			sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[0]), 0, newSavePath));
			

			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setInterSaveDiv(params.interSaveDiv);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);

		}else if (params.evo == 'y' && params.multi == 'y'){
			
			vector<Environment> envs;

			MultiEvoSimulation sim(move(envs));

			for(int i = 0; i<params.numEnvs; i++){
				sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[i]), i, newSavePath));
			}

			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setNumEnvs(params.numEnvs);
			sim.setNumSpecs(params.numSpecs);
			sim.setInterSaveDiv(params.interSaveDiv);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);

		}else if (params.eco == 'y' && params.multi == 'y'){
			
			vector<Environment> envs;

			MultiEcoSimulation sim(move(envs));

			for(int i = 0; i<params.numEnvs; i++){
				sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[i]), i, newSavePath));
			}

			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setNumEnvs(params.numEnvs);
			sim.setNumSpecs(params.numSpecs);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);

		}else if (params.multi == 'y'){
			
			vector<Environment> envs;

			MultiSimulation sim(move(envs));

			for(int i = 0; i<params.numEnvs; i++){
				sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[i]), i, newSavePath));
			}

			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setNumEnvs(params.numEnvs);
			sim.setNumSpecs(params.numSpecs);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);
			

		}else if (params.evo == 'y'){
			
			vector<Environment> envs;

			EvoSimulation sim(move(envs));

			sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[0]), 0, newSavePath));

			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setInterSaveDiv(params.interSaveDiv);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);

		}else if (params.eco == 'y'){
			
			vector<Environment> envs;

			EcoSimulation sim(move(envs));

			sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[0]), 0, newSavePath));

			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);

		}else{
			
			vector<Environment> envs;

			Simulation sim(move(envs));
			//cout<<"ARE"<<endl;

			sim.addEnv(loadEnv(newSavePath, sim.getEnvs(), &params, &(values[0]), 0, newSavePath));
			//cout<<"YOU"<<endl;
			sim.setNumSteps(params.numSteps);
			sim.setSaveDiv(params.saveDiv);
			sim.setCompleteStart(params.completeStart);
			sim.setCompleteEnd(params.completeEnd);
			sim.runSim(k);
			//cout<<"HERE?"<<endl;
		}
	}
}


Environment loadEnv(string newSavePath, vector<Environment>* allEnvs, simParams* params, simValues* values, int numSelf, string savePath){
	vector<unique_ptr<Species>> speciesList;
	bool eco = false;
	bool evo = false;
	bool multi = false;
	string saveFile = savePath + "/rawSave";
	
	unique_ptr<NextGen> nextGen;


	if(params->evo == 'y' && params->eco == 'y' && params->multi == 'y'){
		//save = make_unique<E2Save>();
		nextGen = make_unique<E2MultiNextGen>();
		eco = true;
		evo = true;
		multi = true;
		saveFile = saveFile + "Env" + to_string(numSelf);

	}else if(params->evo == 'y' && params->eco == 'y' ){
		//save = make_unique<E2Save>();
		nextGen = make_unique<E2NextGen>();
		eco = true;
		evo = true;

	}else if (params->evo == 'y' && params->multi == 'y'){
		//save = make_unique<EvoSave>();
		nextGen = make_unique<EvoMultiNextGen>();
		evo = true;
		multi = true;
		saveFile = saveFile + "Env" + to_string(numSelf);

	}else if (params->eco == 'y' && params->multi == 'y'){
		//save = make_unique<EvoSave>();
		nextGen = make_unique<EcoMultiNextGen>();
		eco = true;
		multi = true;
		saveFile = saveFile + "Env" + to_string(numSelf);

	}else if (params->multi == 'y'){
		//save = make_unique<EvoSave>();
		nextGen = make_unique<StdMultiNextGen>();
		multi = true;
		saveFile = saveFile + "Env" + to_string(numSelf);
		

	}else if (params->evo == 'y'){
		//save = make_unique<EvoSave>();
		nextGen = make_unique<EvoNextGen>();
		evo = true;

	}else if (params->eco == 'y'){
		//save = make_unique<EcoSave>();
		nextGen = make_unique<EcoNextGen>();
		eco = true;

	}else{
		//save = make_unique<StdSave>();
		nextGen = make_unique<StdNextGen>();

	}
	unique_ptr<Save> save(new Save(evo, eco, multi));
	//cout<<save.get()<<endl;
	vector<double> change;
	vector<double> pertRange (2);
	pertRange[0] = params->pertRange[0];
	pertRange[1] = params->pertRange[1];

	Environment env((values->addSpec + params->numSpecs), numSelf, params->numEnvs, params->delta, params->deathThreshold, values->envConst, params->perturbationProb,
		savePath, saveFile, change, pertRange, move(speciesList),
		allEnvs, move(save), move(nextGen));


	//cout<<"Specs"<<endl;
	loadSpecs(env.getSpeciesList(), params, values, env.getEnvConstPtr());
	//cout<<"?"<<endl;
	//env->setSpeciesList(speciesList);
	env.initChange();
	
	return move(env);

}

void loadSpecs(vector<unique_ptr<Species>>* speciesList, simParams* params, simValues* values, double* envConst){

	for(int i = 0; i<values->oldSpec; i++){
		//cout<<i<<endl;
		int numSelf = values->IDs[i];
		int numTotal = values->totSpec;
		int genTime = values->genTimes[i];
		int indToDens = values->densToInds[i];
		double alpha = values->alphas[i];
		double beta = values->betas[i];
		double density = values->densities[i];
		double cc =values->ccs[i];
		double optimum = values->optimums[i];
		double optRange = values->optRange[i];



		vector<double> interactions = values->interactions[i];


		for(int j = 0; j<values->addSpec; j++){
			interactions.push_back(getUniform(params->interRange));
		}

		interactions[numSelf] = 0;

		vector<double> range = values->evoRanges[i];

		for(int j = 0; j<values->addSpec; j++){
			range.push_back(getUniform(params->evoRangeRange));
		}

		range[numSelf] = 0;

		//cout<<"t\n";
		vector<vector<vector<double>>> routes = values->routes[i];
		//cout<<"s\n";

		unique_ptr<Change> change;
		if(params->eco == 'y'){
			change = make_unique<EcoChange>();

		}else{
			change = make_unique<StdChange>();
		}

		unique_ptr<Evo> evo;
		if(params->eco == 'y' && params->evo == 'y'){
			evo = make_unique<EcoEvo>();
		}else if(params->evo == 'y'){
			evo = make_unique<StdEvo>();
		}else{
			evo = make_unique<NoEvo>();
		}

		//cout<<"u"<<endl;
		unique_ptr<Species> spec (new Species(numSelf, numTotal, genTime, indToDens, alpha, beta, density, cc, optimum, optRange,
			envConst, interactions, range, routes, move(change), move(evo)));
		//cout<<"r"<<endl;
		speciesList->push_back(move(spec));
		//cout<<"v"<<endl;

	}

	for(int i = 0; i<values->addSpec; i++){
		int numSelf = (params->numSpecs + i);
		int numTotal = values->totSpec;
		int genTime = getUniformInt(params->genTimeRange);
		int indToDens = getUniformInt(params->evoResRange);
		double alpha = getUniform(params->alphaRange);
		double beta = getUniform(params->betaRange);
		double density = getUniform(params->densRange);
		double cc = getUniform(params->ccRange);
		double optimum = getUniform(params->optRangeSpec);
		double optRange = getUniform(params->optEvoRange);


		vector<double> interactions ((params->numSpecs + values->addSpec), 0);

		for(int j = 0; j<(params->numSpecs + values->addSpec); j++){
			interactions[j] = getUniform(params->interRange);
		}

		interactions[i] = 0;

		vector<double> range ((params->numSpecs + values->addSpec), 0);

		for(int j = 0; j<(params->numSpecs + values->addSpec); j++){
			range[j] = getUniform(params->evoRangeRange);
		}

		range[i] = 0;


		vector<vector<vector<double>>> routes(params->numEnvs);
		for(int j = 0; j<params->numEnvs; j++){
			vector<vector<double>> routesTo(params->numEnvs);
			for(int l =0; l<params->numEnvs; l++){
				vector<double> migParams(2);
				migParams[0] = getUniform(params->migProbRange);
				migParams[1] = getUniform(params->migSizeRange);

				routesTo[l] = migParams;

			}

			routes[j] = routesTo;
			
		}

		unique_ptr<Change> change;
		if(params->eco == 'y'){
			change = make_unique<EcoChange>();

		}else{
			change = make_unique<StdChange>();
		}

		unique_ptr<Evo> evo;
		if(params->eco == 'y' && params->evo == 'y'){
			evo = make_unique<EcoEvo>();
		}else if(params->evo == 'y'){
			evo = make_unique<StdEvo>();
		}else{
			evo = make_unique<NoEvo>();
		}

		//cout<<"right"<<endl;
		unique_ptr<Species> spec (new Species(numSelf, numTotal, genTime, indToDens, alpha, beta, density, cc, optimum, optRange,
			envConst, interactions, range, routes, move(change), move(evo)));
		//cout<<"here!"<<endl;

		speciesList->push_back(move(spec));


	}
}

int loadSimValues(simParams* params, simValues* values, string savePath ){
	//file from which sim Values are to be loaded has to be called "/completeSave",
	//and contain a complete save of the sim at the time at which we want to reload it
	


	ifstream toParse(savePath);
	if (! toParse.is_open()){
		cout<<"Failed to open file in reloadModule::loadSimValues:\n";
		cout<<strerror(errno)<<endl;
		return -1;
	}

	string currentLine;
	getline(toParse, currentLine);
	getline(toParse, currentLine);
	getline(toParse, currentLine);
	getline(toParse, currentLine);
	getline(toParse, currentLine);
	string simType = currentLine;
	getline(toParse, currentLine);
	getline(toParse, currentLine);
	stringstream ss;
	ss.str(currentLine);
	string numS;
	getline(ss, numS, ' ');
	getline(ss, numS, ' ');
	int numSpecs = stoi(numS);
	//cout<<"Found numSpecs "<<numSpecs<<endl;

	values->oldSpec = numSpecs;
	//cout<<params->numSpecs<<endl;

	vector<int> genTimes (numSpecs, 0);
	vector<int> densToInds (numSpecs, 0);
	vector<int> IDs (numSpecs, 0);
	vector<double> alphas (numSpecs, 0);
	vector<double> betas (numSpecs, 0);
	vector<double> ccs (numSpecs, 0);
	vector<double> densities (numSpecs, 0);
	vector<double> optimums (numSpecs, 0);
	vector<double> optRange (numSpecs, 0);
	vector<vector <double> > interactions (numSpecs, vector<double> (params->numSpecs, 0));
	vector<vector <double> > evoRange (numSpecs, vector<double> (params->numSpecs, 0));
	vector<vector<vector<vector<double>>>> routes (numSpecs, vector<vector<vector<double>>> (params->numEnvs, vector<vector<double>> (params->numEnvs, vector<double> (2, 0))));


	values->genTimes = genTimes;
	values->densToInds = densToInds;
	values->IDs = IDs;
	values->alphas = alphas;
	values->betas = betas;
	values->ccs = ccs;
	values->densities = densities;
	values->optimums = optimums;
	values->optRange = optRange;
	values->evoRanges = evoRange;
	values->interactions = interactions;
	values->routes = routes;

	int whereAt = 0; //1 = alpha, 2 = beta, 3 = cc, 4 = density, 5 = interaction, 6 = Opt, 7 = envConst, 8 = genTime, 9 = evoRange, 10 = densToInds, 
	//11 = optRange, 12 = routes, 13 = IDs, 0 = nothing

	int count = 0;
	int count2 = 0;
	//cout<<currentLine<<endl;

	vector<vector <string> > temps(numSpecs, vector<string> (3));
	vector<vector<vector<vector<string> > > >rTemps(numSpecs, vector<vector<vector<string>>> (params->numEnvs, vector<vector<string>> (params->numEnvs, vector<string> (5))));

	while(!currentLine.empty()){

		//cout<<currentLine<<endl;

		
		if(currentLine == "Alphas:\0"){
			whereAt = 1;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Betas:\0"){
			whereAt = 2;
			//cout<<whereAt<<endl;
		}else if(currentLine == "ccs:\0"){
			whereAt = 3;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Density:\0"){
			whereAt = 4;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Interactions:\0"){
			whereAt = 5;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Optimums:\0"){
			whereAt = 6;
			//cout<<whereAt<<endl;
		}else if(currentLine == "EnvConst:\0"){
			whereAt = 7;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Evo Rate:\0"){
			whereAt = 8;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Evo Range:\0"){
			whereAt = 9;
			//cout<<whereAt<<endl;
		}else if(currentLine == "DensToInds:\0"){
			whereAt = 10;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Optimum Evo Range:\0"){
			whereAt = 11;
			//cout<<whereAt<<endl;
		}else if(currentLine == "Routes:\0"){
			whereAt = 12;
			//cout<<whereAt<<endl;
		}else if(currentLine == "IDs:\0"){
			whereAt = 13;
			//cout<<whereAt<<endl;
		}else if(whereAt == 13){
			for(int i = 0; i<values->oldSpec; i++){
				values->IDs[i] = stoi(currentLine);
				if(i != (values->oldSpec - 1)){
					getline(toParse, currentLine);
				}
			}
			whereAt = 0;
		}else if(whereAt == 12){
			
			
			//cout<<"NumEnvs: "<<params->numEnvs<<endl;
			for(int j = 0; j<params->numEnvs; j++){
				//cout<<"in first loop "<<j<<endl;
				
				for(int i = 0; i < params->numEnvs; i++){
					//cout<<"in second loop "<<i<<endl;
					getline(toParse, currentLine);
					//cout<<"currentLine: "<<currentLine<<endl;
					stringstream ss;
					ss.str(currentLine);
					string temp;
					//getline(ss, temp, ' ');
					//cout<<temp<<endl;
					while(getline(ss, temp, ' ')){
						//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;
						//cout<<temp<<endl;

						if(!currentLine.empty()){
							//cout<<j<<'\t'<<i<<'\t'<<count<<'\t'<<count2<<endl;
							rTemps[count][j][i][count2] = temp;
							count2 ++;

							
						}

					}
					count2 = 0;
				}
				if(j != params->numEnvs-1){
					getline(toParse, currentLine);
				}			
			}
			//cout<<"hay!"<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					for(int j = 0; j < params->numEnvs; j++){
						for(int k = 0; k<params->numEnvs; k++){
					//cout<<"Converting "<<temps[i][2]<<endl;
							values->routes[i][j][k][0] = stod(rTemps[i][j][k][2]);
							//cout<<rTemps[i][j][k][2]<<endl;
							values->routes[i][j][k][1] = stod(rTemps[i][j][k][4]);
							//cout<<rTemps[i][j][k][4]<<endl;
						}
					}
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}
		}else if(whereAt == 11){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->optRange[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}
		}else if(whereAt == 10){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->densToInds[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}
		}else if(whereAt == 1){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->alphas[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}
		}else if (whereAt == 2){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->betas[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;
			}else{
				count ++;
			}

		}else if (whereAt == 3){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->ccs[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}

		}else if (whereAt == 4){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->densities[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}

		}else if(whereAt == 5){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					//cout<<"A"<<endl;
					temps[count][count2] = temp;
					//cout<<"B"<<endl;
					count2 ++;
					
				}

			}
			count2 = 0;

			//cout<<temps[0][1]<<endl;
			if(count == (numSpecs-1)){
				int count3 = 0;
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					stringstream ss2;
					ss2.str(temps[i][2]);
					
					while(getline(ss2, temp, '\t')){
						values->interactions[i][count3] = stod(temp);
						count3 ++;
					}
					count3 = 0;
					//densities[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;
				//cout<<"done\n";

			}else{
				count ++;
			}

		}else if (whereAt == 6){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->optimums[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}
		}else if (whereAt == 7){
			values->envConst = stod(currentLine);
			whereAt = 0;
		}else if(whereAt == 8){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;
			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					values->genTimes[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}
		}else if(whereAt == 9){
			stringstream ss;
			ss.str(currentLine);
			string temp;
			while(getline(ss, temp, ' ')){
				//cout<<"In second loop!\n"<<temp<<endl<<count<<endl;;

				if(!currentLine.empty()){
					temps[count][count2] = temp;
					count2 ++;
					
				}

			}
			count2 = 0;

			//cout<<temps[0][0]<<endl;
			if(count == (numSpecs-1)){
				int count3 = 0;
				for(int i = 0; i < numSpecs; i++){
					//cout<<"Converting "<<temps[i][2]<<endl;
					stringstream ss2;
					ss2.str(temps[i][2]);
					
					while(getline(ss2, temp, '\t')){
						values->evoRanges[i][count3] = stod(temp);
						count3 ++;
					}
					count3 = 0;
					//densities[i] = stod(temps[i][2]);
				}
				count = 0;
				whereAt = 0;

			}else{
				count ++;
			}

		}

		//cout<<"Found nothing!\n";
		//cout<<currentLine<<endl;
		getline(toParse, currentLine);
		

	}
	//cout<<values->alphas.size()<<endl;
	//cout<<values->densToInds[0]<<endl;
	//cout<<values->routes[9][0][0][1]<<endl;
	return 0;

}


