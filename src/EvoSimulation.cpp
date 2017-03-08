#include "EvoSimulation.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

int EvoSimulation::runSim(int runNumber){
	//Runs the simulation

	//cout<<"evo Start\n";

	envs[0].saveHeader(runNumber);


	envs[0].saveAll(-1);

	if(completeStart){
		envs[0].completeSave(runNumber, -1, 0);
	}

	//cout<<"evo After first save\n";

	for(int i = 1; i<=numSteps; i++){
		envs[0].getNextGen(i);

		//cout<<"evo Got next gen\n";

		if(i%saveDiv == 0){
			//cout<<"test\n";

			envs[0].saveDens(i);

			//cout<<"Evo after Dens save\n";
		}

		
		//cout<<"Inbetween\n";

		if(i%interSaveDiv == 0){
			//cout<<"test2\n";

			envs[0].saveInter(i);
		}


		//cout<<"evo At step "<<i<<endl;



	}

	envs[0].saveFinalDens(numSteps);

	//cout<<"evo After main loop\n";

	//env.saveDens(-1);

	envs[0].saveLastTurnChange();


	if(completeEnd){
		envs[0].completeSave(runNumber, -1, 1);
	}

	//cout<<"All Done\n";

	return 0;
}