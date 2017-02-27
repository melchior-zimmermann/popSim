#include "Environment.hpp"
#include <vector>

using namespace std;

void Environment::initChange(){
	//Initiates change vector at right size
	int numSpecs = speciesList.size();

	for(int i = 0; i<numSpecs; i++)
		change.push_back(0);
}



vector<vector<double>> Environment::getMigrants(){
	//This function gets a list of all the migrants, with index[speciesID][destinationEnv]
	//value is density of migration

	int numSpecs = speciesList.size();	
	//cout<<"speciesList size is: "<<numSpecs<<endl;
	//cout<<"InitNumSpecs is: "<<initNumSpecs<<endl;

	vector<vector<double>> migrants(numSpecs, vector<double>(numEnvs));


	for(int i = 0; i<numSpecs; i++){
		if(speciesList[i]->getDensity() == 0){
			vector<double> v(numEnvs);
			for(int j = 0; j<numEnvs; j++){
				v[j] = 0;
			}
			migrants[i] = v;
		}else{
			migrants[i] = speciesList[i]->getMigration(numSelf, numEnvs);
		}
	}

	return migrants;
}

void Environment::integrateMigrants(vector<vector<double>> migrants){
	//This function integrates migrants from all environments into the current one
	//indexes are migrants[sourceEnvID][speciesID], value is density of migration
	//cout<<"\n\n In env #"<<numSelf<<endl;
	int numSpecs = speciesList.size();	

	for(int i = 0; i<numEnvs; i++){
		for(int j = 0; j<numSpecs; j++){
			double totDensity = speciesList[j]->getDensity() + migrants[i][j];
			//cout<<"In second loop #"<<j<<endl;
			//cout<<"totDensity is "<<totDensity<<endl;

			if(totDensity != 0){

				for(int k = 0; k<numSpecs; k++){
					//cout<<"Env is "<<allEnvs[i].getNumSelf()<<endl;
					//cout<<"In third loop #"<<k<<endl;
					double newInter = (speciesList[j]->getInteraction(k)*speciesList[j]->getDensity() + (*(*allEnvs)[i].getSpeciesList())[j]->getInteraction(k)*migrants[i][j])/totDensity;
					
					//cout<<"Before setInteraction\n";

					speciesList[j]->setInteraction(k, newInter);

					
					//scout<<"New interaction value for spec "<<j<<" with spec "<<k<<" is "<<speciesList[i].getInteraction(k)<<endl;;
				}
				double newOpt = (speciesList[j]->getOptimum()*speciesList[j]->getDensity() + (*(*allEnvs)[i].getSpeciesList())[j]->getOptimum()*migrants[i][j])/totDensity;
				speciesList[j]->setOptimum(newOpt);
				speciesList[j]->setDensity(totDensity);
			}
		}
	}

}