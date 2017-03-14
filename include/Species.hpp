#include <vector>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "Change.hpp"
#include "Evo.hpp"


using namespace std;


#ifndef SPECIES_HPP
#define SPECIES_HPP


/**
 * @file Species.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the Species class, which is the basic building block of simulations.
 *
 */

 /**
 *\brief Class containing all parameters&methods that define a species & it's interactions with other species/the environment.
 *
 * The species class is the most important class of this program, since it contains all the information
 * necessary to evolve the system over time. To simplify the use of polymorphism in conjunction with c++ vectors,
 * species contain all attributes and methods needed for every type of simulation, which means that species used in basic
 * simulations use more memory than strictly necessary. If you want to strip down the species definition to limit
 * memory usage, adapting the initSpecs() function in initializers.cpp is necessary, but the rest should work fine.
 *
 */


class Species {
  protected:
    int numSelf; /**<ID of species*/
    int numTotal; /**<Total number of species (all envrionments)*/
    int genTime; /**<Each how many timeSteps an evolution event occurs (only needed in simulations with evolution)*/
    int indToDens;/**<How many individuals are generated per unit of density (only needed in simulations with evolution)*/
    double alpha;/**<Species growth rate*/
    double beta;/**<Species mortality rate*/
    double density;/**<Species density*/
    double previousDensity;/**<Species density in the previous timeStep*/
    double cc;/**<Species carrying capacity (can be exceeded due to interactions with other species)*/
    double optimum; /**<Value of the environmental constant at which species thrives best (only needed in simulations with an environmental constant)**/
    double optRange; /**<Sigma parameter for generating optimums in indivudals (only needed in simulations with both evolution and environmental constant)**/
    double *const envConst;/**<Points to the environmental constant of the environment (only needed in simulations with an environmental constant)**/
    vector<double> interactions;/**<Vector containing interaction values for all species, index = species ID**/
    vector<double> range;/**<sigma parameter for generating interactions in individuals (only needed in simulations with evolution)**/
    vector<vector<vector<double>>> migrationRoutes; /**<Routes from each env to each other, containing migration probability ([env][env][0]) and size([env][env][1]). Only needed in simulations with multiple environments.**/
    unique_ptr<Change> change;/**<Points to class that defines the way in which density change from one step to the next is calculated (see class Change and its children for details)**/
    unique_ptr<Evo> evo;/**<Points to class that defines how evolution happens (only needed in simulations with evolution). See class Evo and its children for more details.**/
  public:
    Species(int _numSelf, int _numTotal, int _genTime, int _indToDens, double _alpha, double _beta, double _dens, double _cc, double _opt, double _optRange,
        double* _envConst, vector<double> _interactions, vector<double> _range, vector<vector<vector<double>>> _migrationRoutes, unique_ptr<Change> _change, unique_ptr<Evo> _evo):
        numSelf(_numSelf), numTotal(_numTotal), genTime(_genTime), indToDens(_indToDens), alpha(_alpha), beta(_beta), density(_dens), previousDensity(_dens), cc(_cc), optimum(_opt), optRange(_optRange),
        envConst(_envConst), interactions(_interactions), range(_range), migrationRoutes(_migrationRoutes), change(move(_change)), evo(move(_evo)){}
    int getNumSelf() {return numSelf;}
    //void setNumSelf(int newVal) {numSelf = newVal;}
    int getNumTotal() {return numTotal;}
    //void setNumTotal(int newVal) {numTotal = newVal;}
    int getGenTime() {return genTime;}
    //void setEvoRate(int newVal) {evoRate = newVal;}
    double getAlpha() {return alpha;}
    //void setAlpha(double newVal) {alpha = newVal;}
    double getBeta() {return beta;}
    //void setBeta(double newVal) {beta = newVal;}
    double getDensity() {return density;}
    void setDensity(double newVal) {density = newVal;}
    double getPreviousDensity() {return previousDensity;}
    void setPreviousDensity(double newVal) {previousDensity = newVal;}
    double getCc() {return cc;}
    //void setCc(double newVal) {cc = newVal;}
    double getOptRange(){return optRange;}

    int getIndToDens(){return indToDens;}

    vector<double> getRange(){return range;}
    double *getEnvConst(){return envConst;}
    double getOptimum(){return optimum;}
    void setOptimum(double newVal) {optimum = newVal;}
    vector<double>* getInteractions() {return &interactions;}
    double getInteraction(int index) {return interactions[index];}
    void setInteractions(vector<double> newVal) {interactions = newVal;}
    void setInteraction(int index, double newVal) { interactions[index] = newVal;}
    //void deleteInteraction(int index);
    void deleteInteraction(int index){interactions.erase(interactions.begin()+index);}
    double getOneRange(int index){return range[index];}

    /**
    *\brief Index of return value is the ID of environment, value is the size of the migration to that environment.
    *
    * Calculates size of migrations of this species to each envrionment (up to but not including numEnvs) from envNum.
    * Total migration size can, depending on migration parameters, be bigger than species density (but species density
    * won't become negative as a result). Co-ordinated by getMigrants() in Environment class.
    * \param envNum ID of the environment in which the species resides
    * \param numEnvs Number of environments in the simulation in total
    * \note This method is only called when there are multiple environments
    */
    vector<double> getMigration(int envNum, int numEnvs);
    vector<vector<double>> getRoutesOneEnv(int envNum) {return migrationRoutes[envNum];}            //indexes are: targetEnv, 0:migProb, 1:migSize
    //void setRoutesOneEnv(vector<vector<double>> routes, int envNum) {migrationRoutes[envNum] = routes;}
    vector<vector<vector<double>>> getAllRoutes() {return migrationRoutes;}
    void setAllRoutes( vector<vector<vector<double>>> routes) {migrationRoutes = routes;}

    /**
    *\brief Calls function of the Change object with the required parameters to calculate species density change (density remains unchanged after call).
    */
    double getChange(double delta, vector<unique_ptr<Species>>* speciesList){
        double densityChange = change->getChange(this, delta, speciesList);
        previousDensity = density;
        return densityChange;
    }
    //~Species(){cout<<"~Species\n";}
    /**
    *\brief Calls function of the Evo object with the required parameters to calculate the change in interction values/optimums (interaction/optimum values changed after call). This method is only needed in simulations with evolution.
    */
    void getEvo(vector<unique_ptr<Species>>* speciesList, int resolution){evo->getEvo(speciesList, this, resolution);}

};


#endif /*SPECIES_HPP*/