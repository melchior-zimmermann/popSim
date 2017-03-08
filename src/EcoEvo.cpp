#include <vector>
#include <memory>
#include <cmath>
#include <random>
#include "Species.hpp"
#include "Individual.hpp"
#include "I2.hpp"
#include "EcoEvo.hpp"
#include "helpers.hpp"

using namespace std;

vector<unique_ptr<Individual>> EcoEvo::getInds(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution){
	vector<unique_ptr<Individual>> inds(resolution);
	int numTotal = spec->getNumTotal();
	default_random_engine generator;
	int ID = spec->getNumSelf();

	int i = 0;
	for(i = 0;i<resolution;i++){

		unique_ptr<Individual> ind(new I2(spec->getEnvConst()));
		ind->specID = ID;
		ind->numTotal = numTotal;
		vector<double> inter(numTotal);
		ind->interactions = inter;

		normal_distribution<double> optDist(spec->getOptimum(), spec->getOptRange());
		ind->optimum = optDist(generator);

		inds[i] = move(ind);
	}

	

	for(int j =0; j<numTotal; j++){
		normal_distribution<double> interDist((*spec->getInteractions())[j], spec->getRange()[j]);
		for(i=0;i<resolution;i++){
			inds[i]->interactions[j] = interDist(generator);
		}
	}

	for(i =0;i<resolution;i++){
		//setting interaction with self to 0
		inds[i]->interactions[ID] = 0;

	}

	return move(inds);
}

vector<int> EcoEvo::runSelection(vector<unique_ptr<Individual>>* inds, vector<unique_ptr<Species>>* speciesList){
	int resolution = inds->size();
	vector<int> descendants(resolution);
	for(int i = 0; i<resolution; i++){
		//cout<<"Survival:\n";
		//cout<<(*inds)[i]->getSurvival(speciesList)<<endl;;
		if((*inds)[i]->getSurvival(speciesList, delta)<getRandom()){
			descendants[i] = 0;
		}else{
			descendants[i] = 2;
			//cout<<"We have descendants!\n";
		}
	}

	return descendants;
}

void EcoEvo::getEvo(vector<unique_ptr<Species>>* speciesList, Species* spec, int resolution){
	if(resolution<2){
		return;
	}
	int ID = spec->getNumSelf();
	vector<unique_ptr<Individual>> inds = getInds( speciesList, spec, resolution);
	vector<int> descendants = runSelection( &inds, speciesList);
	int numTotal = spec->getNumTotal();
	vector<double> newInter(numTotal, 0);
	double newOpt = 0;
	int survivors = 0;

	for(int i = 0; i<resolution; i++){
		if(descendants[i] != 0){
			survivors += descendants[i];
			//cout<<"Added to survivors\n";
			//cout<<survivors<<endl;
			newOpt += inds[i]->optimum * descendants[i];
			for(int j = 0; j<numTotal; j++){
				newInter[j] += descendants[i]*inds[i]->interactions[j];
			}
		}
	}
	//cout<<survivors<<endl;
	if(survivors == 0){
		//cout<<survivors<<endl;
		//cout<<"No survivors!\n\n\n\n\n\n";
		return;
	}

	newOpt /= survivors;

	for(int i = 0; i<numTotal; i++){
		newInter[i] /= survivors;
	}

	spec->setOptimum(newOpt);

	spec->setInteractions(newInter);
}
