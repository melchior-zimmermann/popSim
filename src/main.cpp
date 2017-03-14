#include "helpers.hpp"
#include "interface.hpp"
#include "reloadModule.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

/**
 * @file main.cpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief Main function of the program, parses user arguments and launches appropriate functions.
 *
 * Controller of the different types of activities (e.g. launching the simulation parameter
 * console wizard or creating a template file for simulation parameters).
 * Argument parsing is rather crude for now, and error messages are minimal, but it gets
 * the job done. For a detailed explication of arguments/options, please refer to the user manual.
 *
 */


int main(int argc , char* argv[]){

	// simParams testParams;
	// loadSimParams(".", &testParams);
	// saveSimParams("test", &testParams);
	// return 0;


	if(argv[1] == NULL){
		cout<<"Usage:\n 'popSim <choice> <savePath> <numRuns> [loadPath] [numSpecsToAdd]'\n";
		exit(EXIT_SUCCESS);
	}
	int choice1;

	choice1 = atoi(argv[1]);
	//argv:
	//progName, choice1, savePath, numRuns, paramPath, addSpec
	if(choice1 == 0){
		string tempPath;
		if(argv[2] != NULL){
			tempPath.assign(argv[2]);
		
		} 
		makeTemplate(tempPath);
		exit(EXIT_SUCCESS);
			
	}
	srand(time(NULL));

	

	if(argv[2] == NULL ){
		cout<<"Missing arg 2 (savePath)!\n(Need at least 2 args)\n";
		exit(EXIT_FAILURE);
	}
	string paramPath;
	string savePath;
	savePath.assign(argv[2]);
	int numRuns, addSpec;
	numRuns = atoi(argv[3]);

    if (choice1 == 1){
    	if (argc != 4){
    		cout<<"Usage:\n 'popSim <choice> <savePath> <numRuns> [loadPath] [numSpecsToAdd]'\n";
			exit(EXIT_FAILURE);
    	}
    	paramPath.assign(argv[2]);
    }
	else if(choice1 == 2){
		if(argv[4] == NULL ){
			cout<<"Missing arg 4 (loadPath)!\n(Need at least 4 argv)\n";
			exit(EXIT_FAILURE);
		}
		paramPath.assign(argv[4]);
	}else if(choice1 ==3){
		if(argv[4] == NULL ){
			cout<<"Missing arg 4 (loadPath)!\n(Need at least 5 argv)\n";
			exit(EXIT_FAILURE);
		}
		paramPath.assign(argv[4]);
		addSpec = atoi(argv[5]);

		reRunSim(paramPath, savePath, addSpec, numRuns);
		exit(EXIT_SUCCESS);
	}else{
		cout<<"Invalid number for arg 1! (must be 1, 2 or 3)"<<endl;
		exit(EXIT_FAILURE);
	}


	simParams params;

	struct stat st;

	if(choice1 == 1){
		
		if (stat(savePath.c_str(), &st) == -1) {
		    cout<<mkdir(paramPath.c_str(), 0700)<<endl;
		    cout<<strerror(errno)<<endl;
		}

		if(stat(savePath.c_str(), &st) == -1){
			cout<<"Could not create param directory:\n";
			cout<<strerror(errno)<<endl;
			exit(EXIT_FAILURE);

		}
		if(getSimParams(savePath, &params)!=0){
			cout<<"Could not save simulation parameters.\n";
			exit(EXIT_FAILURE);
		}
	}else if(loadSimParams(paramPath, &params)!= 0){
		cout<<"Failed to load params!\n";
		cout<<strerror(errno)<<endl;
		exit(EXIT_FAILURE);
	}

	if (stat(savePath.c_str(), &st) == -1) {
	    mkdir(savePath.c_str(), 0700);
	}

	if(stat(savePath.c_str(), &st) == -1){
		cout<<"Could not create save directory:\n";
		cout<<strerror(errno);
		exit(EXIT_FAILURE);
	}

	if(params.multi == 'n'){
		if(initSave(savePath+"/rawSave") != 0){
			cout<<"Could not init saveFile "<<savePath+"/rawSave"<<endl;
			exit(EXIT_FAILURE);
		}
		if(params.completeEnd){
			if(initSave(savePath+"/completeSaveE") != 0){
				cout<<"Could no init saveFile "<<savePath+"/completeSaveE"<<endl;
				exit(EXIT_FAILURE);
			}
		}
		if(params.completeStart){
			if(initSave(savePath+"/completeSaveS") != 0){
				cout<<"Could no init saveFile "<<savePath+"/completeSaveS"<<endl;
				exit(EXIT_FAILURE);
			}
		}
	}else{
		for(int i = 0; i<params.numEnvs; i++){
			if(initSave(savePath+"/rawSaveEnv"+to_string(i)) != 0){
				cout<<"Could no init saveFile "<<savePath+"/rawSaveEnv"<<i<<endl;
				exit(EXIT_FAILURE);
			}
			if(params.completeEnd){
				if(initSave(savePath+"/completeSaveEnv"+to_string(i)+"E") != 0){
					cout<<"Could no init saveFile "<<savePath+"/completeSaveE"<<endl;
					exit(EXIT_FAILURE);
				}
			}
			if(params.completeStart){
				if(initSave(savePath+"/completeSaveEnv"+to_string(i)+"S") != 0){
					cout<<"Could no init saveFile "<<savePath+"/completeSaveS"<<endl;
					exit(EXIT_FAILURE);
				}
			}

		}
	}



	for(int i = 0;i<numRuns;i++){			

		if(launchSim(savePath, i, params)!=0){
			cout<<"Could not successfully launch & save simulation #"<<i<<".\n";
		}
	}
	


//	Would want to automatically launch pyhton script to make base stats from here...



	exit(EXIT_SUCCESS);

}
