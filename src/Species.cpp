#include "Species.hpp"
#include "helpers.hpp"
#include <math.h>
#include <random>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <vector>
#include <memory>

using namespace std;


vector<double> Species::getMigration(int envNum, int numEnvs){
    //This function returns a vector for the amount (density)
    //of this species that will migrate to a given environment
    //(envNum corresponds to the index for the vectors)
    //envNum is the ID of the environment the species is in, 
    //numEnvs is the total number of environments

    vector<double> migrants(numEnvs, 0);    //used to store the size of migrations (index is envNum)
    if (density <= 0) {
        return migrants;
    }

    vector<vector<double>> routes = migrationRoutes[envNum];

    // double effectiveCc = cc * exp(-(*envConst - optimum)*(*envConst - optimum));
    // cout<<effectiveCc<<endl;
    // cout<<density<<endl;

    double quotient = density/cc;           //used to calculate migration probability

    int i = 0;

    for (i = 0; i<numEnvs; i++){
        if(i!=envNum){
            if(getRandom()<routes[i][0]*quotient){
                //cout<<"Species "<<numSelf<<" migrating from env "<<envNum<<" to env "<<i<<endl;
                migrants[i] = quotient*density*routes[i][1];
                //cout<<"Size: "<<migrants[i]<<endl;
            }
        }

    }

    for (i = 0; i<numEnvs; i++){
        density -= migrants[i];
        // if (migrants[i] > density) {
        //     cout<<"m>d !\n";
        //     cout<<"d = "<<density<<endl;
        //     cout<<"m = "<<migrants[i]<<endl;
        //     exit(EXIT_FAILURE);
        // }
    }

    if(density<0){
        density = 0;
    }

    return migrants;

}