#include <vector>
#include <memory>
#include <cmath>
#include <random>
#include "Species.hpp"
#include "Individual.hpp"
#include "StdEvo.hpp"
#include "helpers.hpp"

using namespace std;

vector<unique_ptr<Individual>> StdEvo::getInds(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution){
	vector<unique_ptr<Individual>> inds(resolution);
	int numTotal = spec->getNumTotal();
	int ID = spec->getNumSelf();

	int i = 0;
	for(i = 0;i<resolution;i++){

		unique_ptr<Individual> ind(new Individual());
		ind->specID = ID;
		ind->numTotal = numTotal;
		vector<double> inter(numTotal);
		ind->interactions = inter;
		inds[i] = move(ind);
	}

	default_random_engine generator;

	for(int j =0; j<numTotal; j++){
		normal_distribution<double> distribution((*spec->getInteractions())[j], spec->getRange()[j]);
		for(i=0;i<resolution;i++){
			inds[i]->interactions[j] = distribution(generator);
		}
	}

	for(i =0;i<resolution;i++){
		//setting interaction with self to 0
		inds[i]->interactions[ID] = 0;
		//cout<<"Interaction with 1: "<<inds[i]->interactions[1]<<endl;

	}

	return move(inds);
}

vector<int> StdEvo::runSelection(vector<unique_ptr<Individual>>* inds, vector<unique_ptr<Species>>* speciesList){
	int resolution = inds->size();
	vector<int> descendants(resolution);
	for(int i = 0; i<resolution; i++){
		if((*inds)[i]->getSurvival(speciesList, delta)<getRandom()){
			descendants[i] = 0;
		}else{
			descendants[i] = 2;
		}
	}

	return descendants;
}

void StdEvo::getEvo(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution){
	if(resolution<2){
		return;
	}
	int ID = spec->getNumSelf();
	vector<unique_ptr<Individual>> inds = getInds( speciesList, spec, resolution);
	vector<int> descendants = runSelection( &inds, speciesList);
	int numTotal = spec->getNumTotal();
	vector<double> newInter(numTotal, 0);
	int survivors = 0;

	for(int i = 0; i<resolution; i++){
		if(descendants[i] != 0){
			survivors += descendants[i];
			for(int j = 0; j<numTotal; j++){
				newInter[j] += descendants[i]*inds[i]->interactions[j];
			}
		}
	}

	if(survivors==0){
		return;
	}

	for(int i = 0; i<numTotal; i++){
		//cout<<"New inter "<<i<<" before: "<<newInter[i]<<endl;
		newInter[i] /= survivors;
		//cout<<"New inter "<<i<<" after: "<<newInter[i]<<endl;
	}

	spec->setInteractions(newInter);
}


