#include "Simulation.hpp"
#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

int Simulation::runSim(int runNumber){
	//Runs the simulation

	//cout<<"base Start\n";
	//cout<<envs.size()<<endl;

    envs.at(0).saveHeader(runNumber);

	envs[0].saveAll(-1);
	if(completeStart){
		envs[0].completeSave(runNumber,-1,0);
	}

	//cout<<"base After first save\n";

	for(int i = 0; i<numSteps; i++){
		envs[0].getNextGen(i);

		//cout<<"base Got next gen\n";

		if(i%saveDiv == 0){

			envs[0].saveDens(i);
		}

		//cout<<"base At step "<<i<<endl;

	}

	envs[0].saveFinalDens(numSteps);

	//cout<<"base After main loop\n";

	//envs[0].saveDens(-1);

	envs[0].saveLastTurnChange();

	if(completeEnd){
		envs[0].completeSave(runNumber,-1,1);
	}

	//cout<<"All Done\n";
	return 0;
}


