#include "MultiEvoSimulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;


int MultiEvoSimulation::runSim(int runNumber){
	//cout<<"multiEco Start\n";
	//cout<<"Size of vector 'envs' is "<<envs.size()<<endl;

	int j = 0;

	vector< vector< vector <double> > > migrants(numEnvs, vector< vector<double> >(numEnvs, vector<double>(numSpecs)));
	//cout<<"Intiated migrant vector.\n";
	//vector index is [destEnv][sourceEnv][specID]
	vector< vector<double> > tempMigrants;

	//cout<<"Initiated tempMigrants vector.\n";
	//cout<<"NumEnvs is "<<numEnvs<<endl;



	for(j = 0; j<numEnvs; j++){

		//cout<<"J is "<<j<<endl;

		//cout<<"multiEvo In save, going for env #"<<envs[j].getNumSelf()<<endl;
		envs[j].saveHeader(runNumber);

		envs[j].saveAll(-1);
	}

	if(completeStart){
		for( j = 0; j<numEnvs; j++){
			envs[j].completeSave(runNumber, -1, 0);
		}
	}

	//cout<<"multiEvo After first save\n";

	for(int i = 1; i<=numSteps; i++){

		for(j = 0; j<numEnvs; j++){

			envs[j].getNextGen(i);
		}

		//cout<<"multiEvo Got next gen\n";

		for(j = 0; j<numEnvs; j++){
			tempMigrants = envs[j].getMigrants();
			for(int k = 0; k<numSpecs; k++){
				for(int l = 0; l<numEnvs; l++){
					migrants[l][j][k] = tempMigrants[k][l];
				}
			}
		}

/*
		for(int i = 0; i<3; i++ ){
			cout<<"After migration, dens is "<<envs[0].getSpeciesList()[i].getDensity()<<endl;
		}


*/
		//cout<<"Got evo migrants\n";

		for(j = 0; j<numEnvs; j++){
			envs[j].integrateMigrants(migrants[j]);
		}
/*
		for(int i = 0; i<3; i++ ){
			cout<<"After integration, dens is "<<envs[0].getSpeciesList()[i].getDensity()<<endl;
		}
*/

		//cout<<"integrated evo migrants\n";

		if(i%saveDiv == 0){

			for( j = 0; j<numEnvs; j++){

				envs[j].saveDens(i);
			}

			//cout<<"multi evo after Dens save\n";
		}

		//cout<<"EvoM Inbetween\n";

		if(i%interSaveDiv == 0){
			//cout<<"EvoM test2\n";
			for(j = 0; j<numEnvs; j++){
				//cout<<"Before save attempt.\n";

				envs[j].saveInter(i);
			}
		}

		//cout<<"multi evo At step "<<i<<endl;



	}

	for( j = 0; j<numEnvs; j++){

		envs[j].saveFinalDens(numSteps);
	}

	//cout<<"multi evo After main loop\n";

	for( j = 0; j<numEnvs; j++){

		envs[j].saveLastTurnChange();
	}

	if(completeEnd){
		for( j = 0; j<numEnvs; j++){
			envs[j].completeSave(runNumber, -1, 1);
		}
	}

	//cout<<"MultiEvo all done!\n";

	return 0;

}