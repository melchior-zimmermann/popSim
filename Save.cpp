#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <cmath>
#include "Environment.hpp"
#include "Species.hpp"
#include "Save.hpp"

using namespace std;

void Save::saveHeader(int runNumber, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveHeader:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	saveFile<<"\n\n"<<endl;
	if(eco && evo && multi){
		saveFile<<"E2M\n";
	}else if (eco && evo){
		saveFile<<"E2\n";
	}else if (eco && multi){
		saveFile<<"MULTIECO\n";
	}else if (evo && multi){
		saveFile<<"MULTIEVO\n";
	}else if (evo){
		saveFile<<"EVO\n";
	}else if (eco){
		saveFile<<"ECO\n";
	}else if (multi){
		saveFile<<"MULTI\n";
	}else{
		saveFile<<"NORMAL\n";
	}

	saveFile<<"Run number "<<runNumber<<endl;
	
	

	saveFile<<"numSpecs: "<<env->getSpeciesList()->size()<<endl;
	saveFile<<"delta: "<<env->getDelta()<<endl;
	saveFile<<"deathThreshold: "<<env->getDeathThreshold()<<endl;


	if(multi){
		saveFile<<"Env #"<<env->getNumSelf()<<endl;
	}
	if(eco){
		saveFile<<"EnvConst:\n";
		saveFile<<env->getEnvConst()<<endl;
		saveFile<<"Perturbation probability:\n";
		saveFile<<env->getPertProb()<<endl;
		saveFile<<"Perturbation size range:\n";
		saveFile<<env->getPertRange()[0]<<"\t"<<env->getPertRange()[1]<<endl;
	}

	saveFile.close();
}


void Save::saveAll(int step, vector<unique_ptr<Species>>* speciesList, Environment* env){

	saveDens(step, speciesList, env);
	saveCc(step, speciesList, env);
	saveAlphas(speciesList, env);
	saveBetas(speciesList, env);
	saveInter(step, speciesList, env);
	if(eco){
		saveOpts(step, speciesList, env);
	}
	if(evo){
		saveEvoParams(speciesList, env);
		saveDtoI(speciesList, env);
	}
	if(multi){
		saveRoutes(speciesList, env);
	}

	if(eco && evo){
		saveEEParams(speciesList, env);
	}



}


void Save::saveDens(int step, vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveDens:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();

	saveFile<<"Step "<<step<<endl;
	saveFile<<"Density:\n";
	for(int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getDensity()<<endl;
	}
	

	saveFile.close();


}

void Save::saveFinalDens(int step, vector<unique_ptr<Species>>* speciesList, vector<unique_ptr<Species>>* deadSpecies, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveDens:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();
	int numDead = deadSpecies->size();

	saveFile<<"Densities at step "+to_string(step)+":\n";
	for(int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getDensity()<<endl;
	}
	for(int i = 0; i<numDead; i++){
		saveFile<<"Species "<<(*deadSpecies)[i]->getNumSelf()<<": "<<(*deadSpecies)[i]->getDensity()<<endl;
	}
	

	saveFile.close();


}

void Save::saveAlphas( vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveAlpha:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();

	saveFile<<"Alphas:\n";
	for(int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getAlpha()<<endl;
	}
	

	saveFile.close();


}

void Save::saveBetas( vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveBeta:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();

	saveFile<<"Betas:\n";
	for(int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getBeta()<<endl;
	}
	

	saveFile.close();


}

void Save::saveCc(int step, vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in MsaveDens:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();

	saveFile<<"ccs:\n";
	for(int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getCc()<<endl;
	}
	
	saveFile.close();

}


void Save::saveInter(int step, vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveInter:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();
	int initSpecs = env->getInitNumSpecs();

	saveFile<<"Step "<<step<<endl;
	saveFile<<"Interactions:\n";
	for(int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": ";
		for(int j =0; j<initSpecs; j++){
			saveFile<<(*speciesList)[i]->getInteraction(j)<<"\t";
		}

		saveFile<<endl;
	}

	saveFile.close();


}


void Save::saveOpts(int step, vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveOpts:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();

	saveFile<<"Step "<<step<<endl;
	saveFile<<"Optimums:\n";
	for( int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getOptimum()<<endl;
	}

	saveFile.close();


}

void Save::saveID(vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveID:\n";
		cout<<strerror(errno)<<endl;
		return;
	}
	int numSpecs = speciesList->size();

	saveFile<<"IDs:\n";
	for( int i = 0; i<numSpecs; i++){
		saveFile<<(*speciesList)[i]->getNumSelf()<<endl;
	}

	saveFile.close();

}

void Save::completeSave(int runNumber, int step, vector<unique_ptr<Species>>* speciesList, Environment* env, int se){
	string rawSave = env->getFilename();

	if(se == 0){

		if(multi){
			env->setFilename(env->getPath() + "/completeSaveEnv"+to_string(env->getNumSelf())+"S");	
		}else{
			env->setFilename(env->getPath() + "/completeSaveS");
		}
	}else{

		if(multi){
			env->setFilename(env->getPath() + "/completeSaveEnv"+to_string(env->getNumSelf())+"E");	
		}else{
			env->setFilename(env->getPath() + "/completeSaveE");
		}
	}
	

	saveHeader(runNumber, env);
	saveID(speciesList, env);
	saveDens(step, speciesList, env);
	saveCc(step, speciesList, env);
	saveAlphas(speciesList, env);
	saveBetas(speciesList, env);
	saveInter(step, speciesList, env);
	if(eco){
		saveOpts(step, speciesList, env);
	}
	if(evo){
		saveEvoParams(speciesList, env);
		saveDtoI(speciesList, env);
	}
	if(multi){
		saveRoutes(speciesList, env);
	}

	if(eco && evo){
		saveEEParams(speciesList, env);
	}

	env->setFilename(rawSave);


}


void Save::saveRoutes(vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveRoutes:\n";
		cout<<strerror(errno)<<endl;
		return;
	}


	int numSpecs = speciesList->size();
	int numEnvs = env->getNumEnvs();

	saveFile<<"Routes:\n";
	//cout<<"Going for route loop\n";
	for(int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": ";
		//cout<<"In first loop #"<<i<<"\n";
		for(int k = 0; k<numEnvs; k++){
			//cout<<"In second loop #"<<k<<"\n";
			saveFile<<"From environment "<<k<<" to environment\n";
			for(int l = 0; l<numEnvs; l++){
				//cout<<"In third loop #"<<l<<"\n";
				//cout<<speciesList[i].getAllRoutes()[k][l][0]<<endl;
				saveFile<<l<<" = "<<(*speciesList)[i]->getAllRoutes()[k][l][0]<<" , "<<(*speciesList)[i]->getAllRoutes()[k][l][1]<<endl;

			}
		}
	}


	saveFile.close();


}


void Save::savePerturbations(Environment* env){


	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in savePerturbations:\n";
		cout<<strerror(errno)<<endl;
		return;
	}

	saveFile<<"Perturbations:\n";

	vector<vector<double>>* perturbations = env->getPerts();

	int max =  perturbations->size();

	for(int i = 0; i<max; i++){
		saveFile<<"Step "<<(*perturbations)[i][0]<<": "<<(*perturbations)[i][1]<<endl;
	}
	saveFile<<endl;

}


void Save::saveLastTurnChange(Environment* env){


	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveLastTurnChange:\n";
		cout<<strerror(errno)<<endl;
		return;
	}

	saveFile<<"Last turn change: ";
	int numSpecs = env->getSpeciesList()->size();

	vector<double>* change = env->getChange();

	double ltc = 0;
	for(int i = 0; i<numSpecs;i++){
		ltc += abs((*change)[i]);
	}

	saveFile<<ltc<<endl;

	saveFile<<"\n\n";

	saveFile.close();

}

void Save::saveEvoParams(vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveLastTurnChange:\n";
		cout<<strerror(errno)<<endl;
		return;
	}

	int numSpecs = speciesList->size();	
	int i = 0;

	saveFile<<"Evo Rate:\n";
	for( i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getGenTime()<<endl;
	}

	int initSpecs = env->getInitNumSpecs();
	saveFile<<"Evo Range:\n";
	for( i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": ";
		for(int j =0;j<initSpecs; j++){
			saveFile<<(*speciesList)[i]->getOneRange(j)<<"\t";;
		}
		saveFile<<endl;
	}


	saveFile.close();
}

void Save::saveEEParams(vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveEEParams:\n";
		cout<<strerror(errno)<<endl;
		return;
	}

	int numSpecs = speciesList->size();	

	saveFile<<"Optimum Evo Range:\n";
	for( int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getOptRange()<<endl;
	}

	saveFile.close();
}

void Save::saveDtoI(vector<unique_ptr<Species>>* speciesList, Environment* env){

	ofstream saveFile(env->getFilename(), ofstream::app);
	if (! saveFile.is_open()){
		cout<<"Failed to open file in saveDtoI:\n";
		cout<<strerror(errno)<<endl;
		return;
	}

	int numSpecs = speciesList->size();	

	saveFile<<"DensToInds:\n";
	for( int i = 0; i<numSpecs; i++){
		saveFile<<"Species "<<(*speciesList)[i]->getNumSelf()<<": "<<(*speciesList)[i]->getIndToDens()<<endl;
	}

	saveFile.close();

}