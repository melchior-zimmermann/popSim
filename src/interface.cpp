
#include <iostream>
#include <string>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <numeric>
#include <sstream>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

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
#include "parserHelpers.hpp"
#include "interface.hpp"
#include "initializers.hpp"

int getSimParams(string savePath, simParams* params){
	//populates a simParams struct from user input, and saves params
	//in savePath+"/simParams"

	char eco = 'n';
	char evo = 'n';
	char multi = 'n';
	char method = 'x';
	char ws = 'n';
	cout<<"Do you want to use the explicit euler scheme or RK4 ('e' or 'r')?\n";
	method = getChar();
	while(method != 'e' && method != 'r'){
		cout<<"Invalid answer. Please type 'e' or 'r':\n";
		method = getChar();
	}
	if(method == 'r'){
		params->rk = true;
	}
	cout<<"Do you want to construct the interaction graph using the Watts-Strogatz model? ('y' or  'n', otherwise graph will be random & complete):\n";
	ws = getChar();
	if (ws == 'y') {
		params->ws = true;
		cout<<"Enter the mean degree of vertices (K):\n";
		params->K = getInt();
		cout<<"Enter the value for the beta paramter (between 0 and 1):\n";
		params->wsBeta = getDouble();
	}
	else {
		params->K = 0;
		params->wsBeta = 0;
	}
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
	if (!params->ws) {
		cout<<"Enter interaction cutoff threshold (interactions with an absolute value below this threshold"<<endl<<"will be set to 0):\n";
		params->cutoffThreshold = getDouble();
		if (params->cutoffThreshold < 0) {
			params->cutoffThreshold = 0;
		}
	}
	else {
		params->cutoffThreshold = 0;
	}
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

	simParams params;

	saveSimParams(path, &params);
}

void printParams(simParams* params) {
	if(params->rk){
		cout<<"y"<<endl;
	}
	else {
		cout<<"n"<<endl;
	}
	if(params->ws){
		cout<<"y"<<endl;
	}
	else {
		cout<<"n"<<endl;
	}
	cout<<params->wsBeta<<endl;;
	cout<<params->K<<endl;
	cout<<params->eco<<endl;
	cout<<params->optRangeSpec[0]<<", ";
	cout<<params->optRangeSpec[1]<<endl;
	cout<<params->optRangeEnv[0]<<", ";
	cout<<params->optRangeEnv[1]<<endl;
	cout<<params->perturbationProb<<endl;
	cout<<params->pertRange[0]<<", ";
	cout<<params->pertRange[1]<<endl;
	cout<<params->evo<<endl;
	cout<<params->evoRangeRange[0]<<", ";
	cout<<params->evoRangeRange[1]<<endl;
	cout<<params->optEvoRange[0]<<", ";
	cout<<params->optEvoRange[1]<<endl;
	cout<<params->genTimeRange[0]<<", ";
	cout<<params->genTimeRange[1]<<endl;
	cout<<params->evoResRange[0]<<", ";
	cout<<params->evoResRange[1]<<endl;
	cout<<params->interSaveDiv<<endl;
	cout<<params->numSpecs<<endl;
	cout<<params->multi<<endl;
	cout<<params->numEnvs<<endl;

	for(int i = 0; i<params->numEnvs; i++){
		cout<<params->specsPerEnv[i];
		if (i != (params->numEnvs - 1)) {
			cout<<", ";
		}
	}
	cout<<endl;

	cout<<params->migProbRange[0]<<", ";
	cout<<params->migProbRange[1]<<endl;
	cout<<params->migSizeRange[0]<<", ";
	cout<<params->migSizeRange[1]<<endl;


	cout<<params->interRange[0]<<", ";
	cout<<params->interRange[1]<<endl;
	cout<<params->cutoffThreshold<<endl;
	cout<<params->densRange[0]<<", ";
	cout<<params->densRange[1]<<endl;
	cout<<params->ccRange[0]<<", ";
	cout<<params->ccRange[1]<<endl;
	cout<<params->alphaRange[0]<<", ";
	cout<<params->alphaRange[1]<<endl;
	cout<<params->betaRange[0]<<", ";
	cout<<params->betaRange[1]<<endl;
	cout<<params->numSteps<<endl;
	cout<<params->delta<<endl;
	cout<<params->deathThreshold<<endl;
	cout<<params->saveDiv<<endl;
	if(params->completeStart){
		cout<<"y\n";
	}else{
		cout<<"n\n";
	}
	if(params->completeEnd){
		cout<<"y\n";
	}else{
		cout<<"n\n";
	}
}

int saveSimParams(string savePath, simParams* params){
	string fileName;
	if(!savePath.empty()){
		fileName = savePath+"/simParams";
	}else{
		fileName = "simParams";
	}

	ofstream saveFile(fileName);
	if (! saveFile.is_open()){
		cout<<"Failed to open file '"<<fileName<<"' in interface::saveSimParams:\n";
		cout<<strerror(errno)<<endl;
		return -1;
	}

	saveFile<<"rk, ";
	if(params->rk){
		saveFile<<"y"<<endl;
	}
	else {
		saveFile<<"n"<<endl;
	}
	saveFile<<"ws, ";
	if(params->ws){
		saveFile<<"y"<<endl;
	}
	else {
		saveFile<<"n"<<endl;
	}
	saveFile<<"ws-Beta, ";
	saveFile<<params->wsBeta<<endl;;
	saveFile<<"ws-K, ";
	saveFile<<params->K<<endl;
	saveFile<<"Eco, ";
	saveFile<<params->eco<<endl;
	saveFile<<"Species optimum Range, ";
	saveFile<<params->optRangeSpec[0]<<", ";
	saveFile<<params->optRangeSpec[1]<<endl;
	saveFile<<"Environment optimum Range, ";
	saveFile<<params->optRangeEnv[0]<<", ";
	saveFile<<params->optRangeEnv[1]<<endl;
	saveFile<<"Perturbation probability, ";
	saveFile<<params->perturbationProb<<endl;
	saveFile<<"Perturbation intensity range, ";
	saveFile<<params->pertRange[0]<<", ";
	saveFile<<params->pertRange[1]<<endl;
	saveFile<<"Evo, ";
	saveFile<<params->evo<<endl;
	saveFile<<"Species evo range Range, ";
	saveFile<<params->evoRangeRange[0]<<", ";
	saveFile<<params->evoRangeRange[1]<<endl;
	saveFile<<"Species opt evo Range, ";
	saveFile<<params->optEvoRange[0]<<", ";
	saveFile<<params->optEvoRange[1]<<endl;
	saveFile<<"Gen time Range, ";
	saveFile<<params->genTimeRange[0]<<", ";
	saveFile<<params->genTimeRange[1]<<endl;
	saveFile<<"Evo resolution Range, ";
	saveFile<<params->evoResRange[0]<<", ";
	saveFile<<params->evoResRange[1]<<endl;
	saveFile<<"Inter save Div, ";
	saveFile<<params->interSaveDiv<<endl;
	saveFile<<"Number species, ";
	saveFile<<params->numSpecs<<endl;
	saveFile<<"Multi, ";
	saveFile<<params->multi<<endl;
	saveFile<<"Number of environments, ";
	saveFile<<params->numEnvs<<endl;

	saveFile<<"Specs per env, ";
	for(int i = 0; i<params->numEnvs; i++){
		saveFile<<params->specsPerEnv[i];
		if (i != (params->numEnvs - 1)) {
			saveFile<<", ";
		}
	}
	saveFile<<endl;

	saveFile<<"Mig prob Range, ";
	saveFile<<params->migProbRange[0]<<", ";
	saveFile<<params->migProbRange[1]<<endl;
	saveFile<<"Mig size Range, ";
	saveFile<<params->migSizeRange[0]<<", ";
	saveFile<<params->migSizeRange[1]<<endl;


	saveFile<<"Interaction Range, ";
	saveFile<<params->interRange[0]<<", ";
	saveFile<<params->interRange[1]<<endl;
	saveFile<<"Interaction cutoff threshold, ";
	saveFile<<params->cutoffThreshold<<endl;
	saveFile<<"Initial density Range, ";
	saveFile<<params->densRange[0]<<", ";
	saveFile<<params->densRange[1]<<endl;
	saveFile<<"Carrying capacity Range, ";
	saveFile<<params->ccRange[0]<<", ";
	saveFile<<params->ccRange[1]<<endl;
	saveFile<<"Alpha Range, ";
	saveFile<<params->alphaRange[0]<<", ";
	saveFile<<params->alphaRange[1]<<endl;
	saveFile<<"Beta Range, ";
	saveFile<<params->betaRange[0]<<", ";
	saveFile<<params->betaRange[1]<<endl;
	saveFile<<"Number of steps, ";
	saveFile<<params->numSteps<<endl;
	saveFile<<"Delta (stepSize), ";
	saveFile<<params->delta<<endl;
	saveFile<<"Death threshold, ";
	saveFile<<params->deathThreshold<<endl;
	saveFile<<"Each how many steps density-values are saved, ";
	saveFile<<params->saveDiv<<endl;
	saveFile<<"CompleteStart, ";
	if(params->completeStart){
		saveFile<<"y\n";
	}else{
		saveFile<<"n\n";
	}
	saveFile<<"CompleteEnd, ";
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

	string fileName;

	if(!savePath.empty()){
		fileName = savePath+"/simParams";
	}else{
		fileName = "simParams";
	}
	

	ifstream saveFile(fileName);
	if (! saveFile.is_open()){
		cout<<"Failed to open file "<<fileName<<" in interface::loadSimParams:\n";
		cout<<strerror(errno)<<endl;
		return -1;
	}
	string line;
	// while (getline(saveFile, line) && saveFile.good()) {
	// 	cout<<line<<endl;
	// }
	// rewind(saveFile);
	vector<int> tempVecI;
	vector<double> tempVecD;

	if(!findName(saveFile, "rk", line)) {
		cout<<"Could not find parameter 'rk'!\nAborting...\n";
		cout<<line<<endl;
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->rk);

	if(!findName(saveFile, "ws", line)) {
		cout<<"Could not find parameter 'ws'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->ws);

	if(!findName(saveFile, "ws-Beta", line)) {
		cout<<"Could not find parameter 'ws-Beta'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->wsBeta);

	if(!findName(saveFile, "ws-K", line)) {
		cout<<"Could not find parameter 'ws-K'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->K);

	if(!findName(saveFile, "Eco", line)) {
		cout<<"Could not find parameter 'Eco'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->eco);

	if(!findName(saveFile, "Species optimum Range", line)) {
		cout<<"Could not find parameter 'Species optimum Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->optRangeSpec[0] = tempVecD[0];
	params->optRangeSpec[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Environment optimum Range", line)) {
		cout<<"Could not find parameter 'Environment optimum Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->optRangeEnv[0] = tempVecD[0];
	params->optRangeEnv[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Perturbation probability", line)) {
		cout<<"Could not find parameter 'Environment optimum range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->perturbationProb);

	if(!findName(saveFile, "Perturbation intensity range", line)) {
		cout<<"Could not find parameter 'Perturbation intensity range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->pertRange[0] = tempVecD[0];
	params->pertRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Evo", line)) {
		cout<<"Could not find parameter 'Evo'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->evo);

	if(!findName(saveFile, "Species evo range Range", line)) {
		cout<<"Could not find parameter 'Species evo range Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->evoRangeRange[0] = tempVecD[0];
	params->evoRangeRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Species opt evo Range", line)) {
		cout<<"Could not find parameter 'Species opt evo Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->optEvoRange[0] = tempVecD[0];
	params->optEvoRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Gen time Range", line)) {
		cout<<"Could not find parameter 'Gen time Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecI);
	params->genTimeRange[0] = tempVecI[0];
	params->genTimeRange[1] = tempVecI[1];
	tempVecI.clear();

	if(!findName(saveFile, "Evo resolution Range", line)) {
		cout<<"Could not find parameter 'Evo resolution Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecI);
	params->evoResRange[0] = tempVecI[0];
	params->evoResRange[1] = tempVecI[1];
	tempVecI.clear();

	if(!findName(saveFile, "Inter save Div", line)) {
		cout<<"Could not find parameter 'Inter save Div'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->interSaveDiv);

	if(!findName(saveFile, "Number species", line)) {
		cout<<"Could not find parameter 'Number species'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->numSpecs);

	if(!findName(saveFile, "Multi", line)) {
		cout<<"Could not find parameter 'Multi'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->multi);

	if(!findName(saveFile, "Number of environments", line)) {
		cout<<"Could not find parameter 'Number of environments'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->numEnvs);

	if(!findName(saveFile, "Specs per env", line)) {
		cout<<"Could not find parameter 'Specs per env'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->specsPerEnv);
	if (params->specsPerEnv.size() < params->numEnvs) {
		cout<<"Nbr of environments does not match between params->numEnvs & params->specsPerEnv!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	int totSpecs = 0;
	for (size_t i = 0; i<params->specsPerEnv.size(); i++) {
		totSpecs += params->specsPerEnv[i];
	}
	if (totSpecs < params->numSpecs) {
		cout<<"Too few species intialized in environments! (specsPerEnv.sum() < numSpecs)\nAborting...\n";
		exit(EXIT_FAILURE);
	}

	if(!findName(saveFile, "Mig prob Range", line)) {
		cout<<"Could not find parameter 'Mig prob Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->migProbRange[0] = tempVecD[0];
	params->migProbRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Mig size Range", line)) {
		cout<<"Could not find parameter 'Mig size Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->migSizeRange[0] = tempVecD[0];
	params->migSizeRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Interaction Range", line)) {
		cout<<"Could not find parameter 'Interaction Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->interRange[0] = tempVecD[0];
	params->interRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Interaction cutoff threshold", line)) {
		cout<<"Could not find parameter 'Interaction cutoff threshold'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->cutoffThreshold);

	if(!findName(saveFile, "Initial density Range", line)) {
		cout<<"Could not find parameter 'Initial density Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->densRange[0] = tempVecD[0];
	params->densRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Carrying capacity Range", line)) {
		cout<<"Could not find parameter 'Carrying capacity Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->ccRange[0] = tempVecD[0];
	params->ccRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Alpha Range", line)) {
		cout<<"Could not find parameter 'Alpha Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->alphaRange[0] = tempVecD[0];
	params->alphaRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Beta Range", line)) {
		cout<<"Could not find parameter 'Beta Range'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), tempVecD);
	params->betaRange[0] = tempVecD[0];
	params->betaRange[1] = tempVecD[1];
	tempVecD.clear();

	if(!findName(saveFile, "Number of steps", line)) {
		cout<<"Could not find parameter 'Number of steps'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->numSteps);

	if(!findName(saveFile, "Delta (stepSize)", line)) {
		cout<<"Could not find parameter 'Delta (stepSize)'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->delta);

	if(!findName(saveFile, "Death threshold", line)) {
		cout<<"Could not find parameter 'Death threshold'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->deathThreshold);

	if(!findName(saveFile, "Each how many steps density-values are saved", line)) {
		cout<<"Could not find parameter 'Each how many steps density-values are saved'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->saveDiv);

	if(!findName(saveFile, "CompleteStart", line)) {
		cout<<"Could not find parameter 'CompleteStart'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->completeStart);

	if(!findName(saveFile, "CompleteEnd", line)) {
		cout<<"Could not find parameter 'CompleteEnd'!\nAborting...\n";
		exit(EXIT_FAILURE);
	}
	readValue(prune(line, ' '), params->completeEnd);

	saveFile.close();

	//printParams(params);

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
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setPreviousDensity(0);
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
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setPreviousDensity(0);
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
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setPreviousDensity(0);
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
					(*(*sim.getEnvs())[i].getSpeciesList())[j]->setPreviousDensity(0);
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

