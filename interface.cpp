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
#include "initializers.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <numeric>
#include <sstream>



using namespace std;



int getSimParams(string savePath, simParams* params){
	//populates a simParams struct from user input, and saves params
	//in savePath+"/simParams"

	char eco = 'n';
	char evo = 'n';
	char multi = 'n';

	cout<<"Will there be an environmental constant in this simulation ('y' or 'n')?\n";
	eco = getChar();
	if(eco == 'y' || eco == 'Y'){
		params->eco = 'y';
		cout<<"Enter range of species optimums:\n";
		getRange(params->optRangeSpec);
		cout<<"Enter range of environmental constant:\n";
		getRange(params->optRangeEnv);
		cout<<"Enter the probability of perturbation:\n";
		bool pCheck = true;
		while(pCheck){
			params->perturbationProb = getDouble();
			if (params->perturbationProb >= 0 && params->perturbationProb <= 1){
				pCheck = false;
			}else{
				cout<<"Perturbation probability must be between 0 and 1!\nTry again...\n";
			} 
		}
		cout<<"Enter the range of perturbation intensity:\n";
		getRange(params->pertRange);

	}else{
		params->eco = 'n';
		params->optRangeSpec[0] = 0;
		params->optRangeSpec[1] = 0;
		params->optRangeEnv[0] = 0;
		params->optRangeEnv[1] = 0;
		params->pertRange[0] = 0;
		params->pertRange[1] = 0;
		params->perturbationProb = 0;
	}

	cout<<"Will there be evolution in this simulation ('y' or 'n')?\n";
	evo = getChar();
	if(evo =='y' || evo == 'Y'){
		params->evo = 'y';
		cout<<"Enter range of evolution range: (sigma parameter for generating individuals)\n";
		getRange(params->evoRangeRange);
		if(eco == 'y' || eco == 'Y'){
			cout<<"Enter range of optimum evolution range: (sigma parameter for generating individuals)\n";
			getRange(params->optEvoRange);
		}else{
			params->optEvoRange[0] = 0;
			params->optEvoRange[1] = 0;
		}
		cout<<"Enter range of generation time (each how many timeSteps an evolutionary event takes pla)ce:\n";
		getIntRange(params->genTimeRange);

		cout<<"Enter range of evolution resolution (how many individuals for each unit of density:\n";
		getIntRange(params->evoResRange);

		cout<<"Each how many time steps should we save interaction values?\n";
		params->interSaveDiv = getInt();
	}else{
		params->evo = 'n';
		params->evoRangeRange[0] = 0;
		params->evoRangeRange[1] = 0;
		params->genTimeRange[0] = 0;
		params->genTimeRange[1] = 0;
		params->interSaveDiv = 0;
		params->optEvoRange[0] = 0;
		params->optEvoRange[1] = 0;
		params->evoResRange[0] = 0;
		params->evoResRange[1] = 0;
	}
	cout<<"Will there be multiple environments in this simulation ('y' or 'n')?\n";
	multi = getChar();
	if(multi == 'y' || multi == 'Y'){
		params->multi = 'y';
		cout<<"How many environments will there be?\n";
		params->numEnvs = getInt();
		cout<<"What will be the range of migration probabilities ? (must be between 0 and 1)\n";
		bool check = true;
		while(check){
			getRange(params->migProbRange);
			if(params->migProbRange[0]<0 || params->migProbRange[1]>1){
				cout<<"Range must be between 0 and 1!\n";
			}else{
				check = false;
			}
		}
		cout<<"What will the range of migration size be? (max size must be < 1/numEnvs, min must be >=0)\n";
		check = true;
		double upper = 1.0/params->numEnvs;
		while(check){
			getRange(params->migSizeRange);
			if(params->migSizeRange[0]<0 || params->migSizeRange[1]>upper){
				cout<<"Range must be between 0 and 1/numEnvs!\n";
			}else{
				check = false;
			}
		}
	}else{
		params->multi = 'n';
		params->numEnvs = 1;
		params->migProbRange[0] = 0;
		params->migProbRange[1] = 0;
		params->migSizeRange[0] = 0;
		params->migSizeRange[1] = 0;
	
	}


	if(params->multi == 'n'){
		cout<<"Enter number of species:\n";
		params->numSpecs = getInt();
		params->specsPerEnv.push_back(params->numSpecs);
	}else{
		for(int i = 0; i<params->numEnvs; i++){
			cout<<"How many species would you like to have in environment #"<<i<<"?\n";
			params->specsPerEnv.push_back(getInt());
		}

		params->numSpecs = accumulate(params->specsPerEnv.begin(), params->specsPerEnv.end(), 0);
	}
	cout<<"Enter range of interaction values:\n";
	getRange(params->interRange);
	cout<<"Enter range of initial densities:\n";
	getRange(params->densRange);
	cout<<"Enter range of carrying capacity:\n";
	getRange(params->ccRange);
	cout<<"Enter range of alpha values:\n";
	getRange(params->alphaRange);
	cout<<"Enter range of beta values:\n";
	getRange(params->betaRange);
	cout<<"Enter number of steps for which to run the simluation:\n";
	params->numSteps = getInt();
	cout<<"Enter delta (stepsize):\n";
	params->delta = getDouble();
	cout<<"Enter deathThreshold:\n";
	params->deathThreshold = getDouble();
	cout<<"Enter each how many steps we should save density-values:\n";
	params->saveDiv = getInt();
	cout<<"Would you like to make a complete save at the start of your smulation? (used to rerun sims)"<<endl;
	cout<<"Enter 'y' for yes and 'n' for no:"<<endl;
	char comp = getChar();
	if(comp == 'y' || comp == 'Y'){
		params->completeStart = true;
	}
	cout<<"Would you like to make a complete save at the end of your smulation? (used to rerun sims)"<<endl;
	cout<<"Enter 'y' for yes and 'n' for no:"<<endl;
	comp = getChar();
	if(comp == 'y' || comp == 'Y'){
		params->completeEnd = true;
	}
	if(saveSimParams(savePath, params) != 0){
		return -1;
	}

	return 0;



}

void makeTemplate(string path){
	string fileName;
	if(!path.empty()){
		fileName = path+"/simParams";
	}else{
		fileName = "simParams";
	}
	ofstream saveFile(fileName);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in interface::saveSimParams:\n";
		cout<<strerror(errno)<<endl;
		return;
	}


	saveFile<<"Eco:\n";
	saveFile<<"y"<<endl;
	saveFile<<"Species optimum Range:\n";
	saveFile<<"-1"<<endl;
	saveFile<<"1"<<endl;
	saveFile<<"Environment optimum Range:\n";
	saveFile<<"-1"<<endl;
	saveFile<<"1"<<endl;
	saveFile<<"Perturbation probability:\n";
	saveFile<<"0.0001"<<endl;
	saveFile<<"Perturbation intensity range:\n";
	saveFile<<"0.0001"<<endl;
	saveFile<<"0.001"<<endl;
	saveFile<<"Evo:\n";
	saveFile<<"y"<<endl;
	saveFile<<"Species evo range Range:\n";
	saveFile<<"0.0001"<<endl;
	saveFile<<"0.01"<<endl;
	saveFile<<"Species opt evo Range:\n";
	saveFile<<"0.001"<<endl;
	saveFile<<"0.1"<<endl;
	saveFile<<"Gen time Range:\n";
	saveFile<<"100"<<endl;
	saveFile<<"2000"<<endl;
	saveFile<<"Evo resolution Range:\n";
	saveFile<<"10"<<endl;
	saveFile<<"100"<<endl;
	saveFile<<"Inter save Div:\n";
	saveFile<<"2000"<<endl;
	saveFile<<"Number species:\n";
	saveFile<<"27"<<endl;
	saveFile<<"Multi:\n";
	saveFile<<"y"<<endl;
	saveFile<<"Number of environments:\n";
	saveFile<<"3"<<endl;

	for(int i = 0; i<3; i++){
		saveFile<<"Number species in env #"<<i<<":\n";
		saveFile<<"9"<<endl;
	}
	saveFile<<"Mig prob Range:\n";
	saveFile<<"0.0001"<<endl;
	saveFile<<"0.01"<<endl;
	saveFile<<"Mig size Range:\n";
	saveFile<<"0.0001"<<endl;
	saveFile<<"0.01"<<endl;


	saveFile<<"Interaction range:\n";
	saveFile<<"-0.2"<<endl;
	saveFile<<"0.2"<<endl;
	saveFile<<"Initial density range:\n";
	saveFile<<"8"<<endl;
	saveFile<<"10"<<endl;
	saveFile<<"Carrying capacity range:\n";
	saveFile<<"8"<<endl;
	saveFile<<"10"<<endl;
	saveFile<<"Alpha range:\n";
	saveFile<<"0.05"<<endl;
	saveFile<<"0.2"<<endl;
	saveFile<<"Beta range:\n";
	saveFile<<"-0.2"<<endl;
	saveFile<<"0"<<endl;
	saveFile<<"Number of steps:\n";
	saveFile<<"200000"<<endl;
	saveFile<<"Delta (stepSize):\n";
	saveFile<<"0.0001"<<endl;
	saveFile<<"Death threshold:\n";
	saveFile<<"0.0001"<<endl;
	saveFile<<"Each how many steps density-values are saved:\n";
	saveFile<<"100000"<<endl;
	saveFile<<"CompleteStart:\n";
	saveFile<<"y"<<endl;;

	saveFile<<"CompleteEnd:\n";
	saveFile<<"y"<<endl;


	saveFile.close();
}

int saveSimParams(string savePath, simParams* params){

	string fileName = savePath+"/simParams";

	ofstream saveFile(fileName);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in interface::saveSimParams:\n";
		cout<<strerror(errno)<<endl;
		return -1;
	}


	saveFile<<"Eco:\n";
	saveFile<<params->eco<<endl;
	saveFile<<"Species optimum Range:\n";
	saveFile<<params->optRangeSpec[0]<<endl;
	saveFile<<params->optRangeSpec[1]<<endl;
	saveFile<<"Environment optimum Range:\n";
	saveFile<<params->optRangeEnv[0]<<endl;
	saveFile<<params->optRangeEnv[1]<<endl;
	saveFile<<"Perturbation probability:\n";
	saveFile<<params->perturbationProb<<endl;
	saveFile<<"Perturbation intensity range:\n";
	saveFile<<params->pertRange[0]<<endl;
	saveFile<<params->pertRange[1]<<endl;
	saveFile<<"Evo:\n";
	saveFile<<params->evo<<endl;
	saveFile<<"Species evo range Range:\n";
	saveFile<<params->evoRangeRange[0]<<endl;
	saveFile<<params->evoRangeRange[1]<<endl;
	saveFile<<"Species opt evo Range:\n";
	saveFile<<params->optEvoRange[0]<<endl;
	saveFile<<params->optEvoRange[1]<<endl;
	saveFile<<"Gen time Range:\n";
	saveFile<<params->genTimeRange[0]<<endl;
	saveFile<<params->genTimeRange[1]<<endl;
	saveFile<<"Evo resolution Range:\n";
	saveFile<<params->evoResRange[0]<<endl;
	saveFile<<params->evoResRange[1]<<endl;
	saveFile<<"Inter save Div:\n";
	saveFile<<params->interSaveDiv<<endl;
	saveFile<<"Number species:\n";
	saveFile<<params->numSpecs<<endl;
	saveFile<<"Multi:\n";
	saveFile<<params->multi<<endl;
	saveFile<<"Number of environments:\n";
	saveFile<<params->numEnvs<<endl;

	for(int i = 0; i<params->numEnvs; i++){
		saveFile<<"Number species in env #"<<i<<":\n";
		saveFile<<params->specsPerEnv[i]<<endl;
	}
	saveFile<<"Mig prob Range:\n";
	saveFile<<params->migProbRange[0]<<endl;
	saveFile<<params->migProbRange[1]<<endl;
	saveFile<<"Mig size Range:\n";
	saveFile<<params->migSizeRange[0]<<endl;
	saveFile<<params->migSizeRange[1]<<endl;


	saveFile<<"Interaction range:\n";
	saveFile<<params->interRange[0]<<endl;
	saveFile<<params->interRange[1]<<endl;
	saveFile<<"Initial density range:\n";
	saveFile<<params->densRange[0]<<endl;
	saveFile<<params->densRange[1]<<endl;
	saveFile<<"Carrying capacity range:\n";
	saveFile<<params->ccRange[0]<<endl;
	saveFile<<params->ccRange[1]<<endl;
	saveFile<<"Alpha range:\n";
	saveFile<<params->alphaRange[0]<<endl;
	saveFile<<params->alphaRange[1]<<endl;
	saveFile<<"Beta range:\n";
	saveFile<<params->betaRange[0]<<endl;
	saveFile<<params->betaRange[1]<<endl;
	saveFile<<"Number of steps:\n";
	saveFile<<params->numSteps<<endl;
	saveFile<<"Delta (stepSize):\n";
	saveFile<<params->delta<<endl;
	saveFile<<"Death threshold:\n";
	saveFile<<params->deathThreshold<<endl;
	saveFile<<"Each how many steps density-values are saved:\n";
	saveFile<<params->saveDiv<<endl;
	saveFile<<"CompleteStart:\n";
	if(params->completeStart){
		saveFile<<"y\n";
	}else{
		saveFile<<"n\n";
	}
	saveFile<<"CompleteEnd:\n";
	if(params->completeEnd){
		saveFile<<"y\n";
	}else{
		saveFile<<"n\n";
	}

	saveFile.close();

	return 0;
}



int loadSimParams(string savePath, simParams* params){
	//populates a simParam struct from params save 
	//in savePath+"/simParams"

	string throwAway;


	string fileName = savePath+"/simParams";
	

	ifstream saveFile(fileName);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in interface::loadSimParams:\n";
		cout<<strerror(errno)<<endl;
		return -1;
	}

	getline(saveFile, throwAway);
	saveFile>>params->eco;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->optRangeSpec[0];
	saveFile>>params->optRangeSpec[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->optRangeEnv[0];
	saveFile>>params->optRangeEnv[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->perturbationProb;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->pertRange[0];
	saveFile>>params->pertRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->evo;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->evoRangeRange[0];
	saveFile>>params->evoRangeRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->optEvoRange[0];
	saveFile>>params->optEvoRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->genTimeRange[0];
	saveFile>>params->genTimeRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->evoResRange[0];
	saveFile>>params->evoResRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->interSaveDiv;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	//cout<<throwAway<<endl;
	saveFile>>params->numSpecs;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->multi;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->numEnvs;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	int temp;
	for(int i = 0; i<params->numEnvs; i++){
		saveFile>>temp;
		params->specsPerEnv.push_back(temp);
		getline(saveFile, throwAway);
		getline(saveFile, throwAway);

	}
	saveFile>>params->migProbRange[0];
	saveFile>>params->migProbRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->migSizeRange[0];
	saveFile>>params->migSizeRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	//cout<<throwAway<<endl;
	saveFile>>params->interRange[0];
	saveFile>>params->interRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	//cout<<throwAway<<endl;
	saveFile>>params->densRange[0];
	saveFile>>params->densRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->ccRange[0];
	saveFile>>params->ccRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->alphaRange[0];
	saveFile>>params->alphaRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->betaRange[0];
	saveFile>>params->betaRange[1];
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->numSteps;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->delta;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->deathThreshold;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>params->saveDiv;
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	string complete;
	saveFile>>complete;
	if(complete == "y"){
		params->completeStart = true;
	}else{
		params->completeStart = false;
	}
	getline(saveFile, throwAway);
	getline(saveFile, throwAway);
	saveFile>>complete;
	if(complete == "y"){
		params->completeEnd = true;
	}else{
		params->completeEnd = false;
	}

	saveFile.close();




	return 0;

}



int launchSim(string savePath, int runNumber, simParams params){

	//instantiates all the necessary objects and runs simulations
	//defined by the parameters in 'params',
	//prefixing saves with the runNumber


	/* --- Multi-Environments ---
	For mutli-environment simulations, the initial interaction values of
	species which are not present in the environment is irrevelant, since
	the weighted average of interaction values is calculated when a migration
	occures, and species with density=0 will therefore be ignored in regards
	to the new interaction values (see env::integrateMigrants())
	The same goes for optimum*/


	if(params.evo == 'y' && params.eco == 'y' && params.multi == 'y'){

		vector<Environment> envs;

		E2MSimulation sim(move(envs));

		for(int i = 0;i < params.numEnvs; i++){
			sim.addEnv(move(getEnv(savePath, params, i, sim.getEnvs())));
		}

		vector<int> inEnv(params.numSpecs);

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(!(j<before) && !(j>max)){
					inEnv[j] = i;
				}
			}

		}

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(j<before || j>max){
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setDensity(0);
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setAllRoutes((*(*sim.getEnvs())[inEnv[j]].getSpeciesList())[j]->getAllRoutes());
				}
			}
		}

		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		sim.setInterSaveDiv(params.interSaveDiv);
		sim.setNumEnvs(params.numEnvs);
		sim.setNumSpecs(params.numSpecs);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//cout<<"Sim num 0 is "<<sim.getEnvs()[0].getNumSelf()<<endl;

		sim.runSim(runNumber);


	}else if (params.evo == 'y' && params.eco == 'y'){
		vector<Environment> envs;

		E2Simulation sim(move(envs));

		sim.addEnv(move(getEnv(savePath,params, 0, sim.getEnvs())));
		//cout<<"eco Got envs!\n";



		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		sim.setInterSaveDiv(params.interSaveDiv);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//sim.setEnv(env);

		//cout<<"eco Gonna run!\n";

		sim.runSim(runNumber);


	}else if (params.eco == 'y' && params.multi == 'y'){
		vector<Environment> envs;

		MultiEcoSimulation sim(move(envs));

		for(int i = 0;i < params.numEnvs; i++){
			sim.addEnv(move(getEnv(savePath, params, i, sim.getEnvs())));
		}

		vector<int> inEnv(params.numSpecs);

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(!(j<before) && !(j>max)){
					inEnv[j] = i;
				}
			}
		}

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(j<before || j>max){
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setDensity(0);
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setAllRoutes((*(*sim.getEnvs())[inEnv[j]].getSpeciesList())[j]->getAllRoutes());
				}
			}
		}

		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		sim.setNumEnvs(params.numEnvs);
		sim.setNumSpecs(params.numSpecs);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//cout<<"Sim num 0 is "<<sim.getEnvs()[0].getNumSelf()<<endl;

		sim.runSim(runNumber);


	}else if (params.evo == 'y' && params.multi == 'y'){
		vector<Environment> envs;

		MultiEvoSimulation sim(move(envs));

		for(int i = 0;i < params.numEnvs; i++){
			sim.addEnv(move(getEnv(savePath, params, i, sim.getEnvs())));
		}

		vector<int> inEnv(params.numSpecs);

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(!(j<before) && !(j>max)){
					inEnv[j] = i;
				}
			}
		}

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(j<before || j>max){
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setDensity(0);
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setAllRoutes((*(*sim.getEnvs())[inEnv[j]].getSpeciesList())[j]->getAllRoutes());
				}
			}
		}

		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		sim.setInterSaveDiv(params.interSaveDiv);
		sim.setNumEnvs(params.numEnvs);
		sim.setNumSpecs(params.numSpecs);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//cout<<"Sim num 0 is "<<sim.getEnvs()[0].getNumSelf()<<endl;

		sim.runSim(runNumber);


	}else if(params.multi == 'y'){
		vector<Environment> envs;

		MultiSimulation sim(move(envs));

		for(int i = 0;i < params.numEnvs; i++){
			sim.addEnv(move(getEnv(savePath, params, i, sim.getEnvs())));
		}

		vector<int> inEnv(params.numSpecs);

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(!(j<before) && !(j>max)){
					inEnv[j] = i;
				}
			}
		}

		for(int i = 0; i<params.numEnvs; i++){
			int before = 0;
			for(int j =0; j<i; j++)
				before += params.specsPerEnv[j];

			int max = before + params.specsPerEnv[i] -1;

			
			for(int j = 0; j<params.numSpecs; j++){
				if(j<before || j>max){
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setDensity(0);
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setAllRoutes((*(*sim.getEnvs())[inEnv[j]].getSpeciesList())[j]->getAllRoutes());
				}
			}
		}

		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		//sim.setInterSaveDiv(params.interSaveDiv);
		sim.setNumEnvs(params.numEnvs);
		sim.setNumSpecs(params.numSpecs);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//cout<<"Sim num 0 is "<<sim.getEnvs()[0].getNumSelf()<<endl;

		sim.runSim(runNumber);


	}else if(params.evo == 'y'){
		vector<Environment> envs;

		EvoSimulation sim(move(envs));

		sim.addEnv(move(getEnv(savePath,params, 0, sim.getEnvs())));
		//cout<<"eco Got envs!\n";



		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		sim.setInterSaveDiv(params.interSaveDiv);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//sim.setEnv(env);

		//cout<<"eco Gonna run!\n";

		sim.runSim(runNumber);

	}else if(params.eco == 'y'){
		vector<Environment> envs;

		EcoSimulation sim(move(envs));

		sim.addEnv(move(getEnv(savePath,params, 0, sim.getEnvs())));
		//cout<<"eco Got envs!\n";



		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//sim.setInterSaveDiv(params.interSaveDiv);
		//sim.setEnv(env);

		//cout<<"eco Gonna run!\n";

		sim.runSim(runNumber);

	}else{
		vector<Environment> envs;

		Simulation sim(move(envs));

		sim.addEnv(move(getEnv(savePath,params, 0, sim.getEnvs())));

		//cout<<(*(*sim.getEnvs())[0].getSpeciesList())[0]->getDensity()<<endl;
		//cout<<"eco Got envs!\n";



		sim.setNumSteps(params.numSteps);
		sim.setSaveDiv(params.saveDiv);
		sim.setCompleteStart(params.completeStart);
		sim.setCompleteEnd(params.completeEnd);
		//sim.setInterSaveDiv(params.interSaveDiv);
		//sim.setEnv(env);

		//cout<<"eco Gonna run!\n";

		sim.runSim(runNumber);
	}

	return 0;
}

