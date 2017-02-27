#include "MultiEcoSimulation.hpp"
#include "MultiSimulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;


int MultiEcoSimulation::runSim(int runNumber){
	//cout<<"multiEco Start\n";
	//cout<<"Size of vector 'envs' is "<<envs.size()<<endl;

		int j = 0;

	vector< vector< vector <double> > > migrants(numEnvs, vector< vector<double> >(numEnvs, vector<double>(numSpecs)));
	//cout<<"Intiated migrant vector.\n";
	//vector index is [destEnv][sourceEnv][specID]
	vector< vector<double> > tempMigrants;

	//cout<<"Initiated tempMigrants vector.\n";



	for(j = 0; j<numEnvs; j++){

	//	cout<<"J is "<<j<<endl;

	//	cout<<"In save, going for env #"<<envs[j].getNumSelf()<<endl;
		envs[j].saveHeader(runNumber);

		envs[j].saveAll(-1); 
	}

	if(completeStart){
		for( j = 0; j<numEnvs; j++){
			envs[j].completeSave(runNumber, -1, 0);
		}
	}

	//cout<<"multi After first save\n";

	for(int i = 1; i<=numSteps; i++){

		for(j = 0; j<numEnvs; j++){

			envs[j].getNextGen(i);
		}

		//cout<<"multi Got next gen\n";

		for(j = 0; j<numEnvs; j++){
			tempMigrants = envs[j].getMigrants();
			//cout<<"Got Temp\n";
			//cout<<tempMigrants.size()<<endl;
			//cout<<tempMigrants[0].size()<<endl;
			//cout<<numSpecs<<endl;
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

		//cout<<"Got migrants\n";

*/
		//cout<<"Got migrants\n";

		for(j = 0; j<numEnvs; j++){
			envs[j].integrateMigrants(migrants[j]);
		}
/*
		for(int i = 0; i<3; i++ ){
			cout<<"After integration, dens is "<<envs[0].getSpeciesList()[i].getDensity()<<endl;
		}
*/

		//cout<<"integrated migrants\n";

		if(i%saveDiv == 0){

			for( j = 0; j<numEnvs; j++){

				envs[j].saveDens(i);
			}

//			cout<<"multi after Dens save\n";
		}


		//cout<<"multi At step "<<i<<endl;



	}

	for( j = 0; j<numEnvs; j++){

		envs[j].saveFinalDens(numSteps);
	}

	//cout<<"multi After main loop\n";

	for( j = 0; j<numEnvs; j++){

		envs[j].saveLastTurnChange();
	}

	
	for(int j = 0; j<numEnvs; j++){

		envs[j].savePerturbations();
	}


	if(completeEnd){
		for( j = 0; j<numEnvs; j++){
			envs[j].completeSave(runNumber, -1, 1);
		}
	}



	//cout<<"MultiEco all done!\n";

	return 0;

}