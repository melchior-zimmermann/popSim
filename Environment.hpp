#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <memory>
#include "Species.hpp"
#include "Save.hpp"
#include "NextGen.hpp"
#include "helpers.hpp"

using namespace std;

/**
 * @file Environment.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the Environment class, used to contain Species and control their evolution (both in the mathematical and biological sense).
 *
 */

#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

 /**
 *\brief Class containing (unique_ptrs to) all Species, as well as everything relating to updating simulation and saves.
 *
 * The Environment class is the main controller/container class of our simulations. It contains (unique_ptrs to)
 * all Species objects, and coordinates updates of Species density, as well as evolution, and changes in the
 * Environment. It also contains the methods necessary to save simulation output, and associated attributes (e.g. path to the save files).
 * As with the Species class, for ease of use the Environment class contains all information necessary for all types of simulation, and thus
 * in standard simulations will take up more space in memory than strictly necessary.
 *
 */



class Environment {
  protected:
	int initNumSpecs;/**<Initial number of live species in Environment (in single environment simulations)*/
    int numSelf;/**<ID of the Environment*/
    int numEnvs;/**<Total number of environments in simulation (only used in simulations with multiple environments)*/
	double delta;/**<Stepsize for the ecplicit Euler scheme (only used in simulations with multiple environments)*/
    double deathThreshold;/**<Threshold below which species are considered extinct (-> their density is set to 0)*/
    double envConst;/**<The value of the environmental constant of the Environment (only used when simulation takes environmental constant into account)*/
    double pertProb;/**<Probability that the Environment experiences an abiotical perturbation (= change in the value of envConst, only used when simulation takes environmental constant into account)*/
	string path;/**<Path to folder where simulation parameters/results are written to (used for complete saves)*/
    string filename;/**<Path to the file were regular saves (rawSave) are written to*/
	vector<double> change;/**<Vector containing the change in density of all (live) species of the environment*/
    vector<double> pertRange;/**<The range from which change in the value of envConst is drawn when a perturbation happens (only used when simulation takes environmental constant into account)*/
    vector<vector<double>> perturbations;/**<Vector containing information on all perturbation events (of this Environment) that happened during simulation ([pertNum][0] = step at which the 
    perturbation happened, [pertNum][1] = new value of envConst). (Only used when simulation takes environmental constant into account)*/
    vector<unique_ptr<Species>> speciesList;/**<List of unique_ptrs to each Species object present in this Environment (in single environment
    simulations, only contains Species where <tt>density != 0</tt>*/
    vector<unique_ptr<Species>> deadSpecies;/**<List of all Species where <tt>density == 0</tt> (only in single environment simulations)*/
    vector<Environment>* allEnvs;/**<Pointer to list of all environments in simulation (only used in simulations with multiple environments)*/
    unique_ptr<Save> save;/**Unique_ptr to object containing all methods to save state of Environment*/
    unique_ptr<NextGen> nextGen;/**<Unique_ptr to object containing methods to calculate change in Species density/parameters from one time step to the next*/

  public:
    Environment(int _initNumSpecs, int _numSelf, int _numEnvs, double _delta, double _deathThreshold, double _envConst, double _pertProb,
    string _path, string _filename, vector<double> _change, vector<double> _pertRange, vector<unique_ptr<Species>> _speciesList,
    vector<Environment>* _allEnvs, unique_ptr<Save> _save, unique_ptr<NextGen> _nextGen) :initNumSpecs(_initNumSpecs), numSelf(_numSelf),
    numEnvs(_numEnvs), delta(_delta), deathThreshold(_deathThreshold), envConst(_envConst), pertProb(_pertProb), path(_path), filename(_filename),
    change(_change), pertRange(_pertRange), speciesList(move(_speciesList)), allEnvs(move(_allEnvs)), save(move(_save)), nextGen(move(_nextGen)){}

  	double getDelta() {return delta;}

  	double getDeathThreshold() {return deathThreshold;}
  	void setFilename(string newVal) {filename = newVal;}
  	string getFilename() {return filename;}
    string getPath(){return path;}
    void initChange();
    double getEnvConst(){return envConst;}
    void setEnvConst(double newVal){envConst = newVal;}
    double* getEnvConstPtr(){return &envConst;}

    int getInitNumSpecs(){return initNumSpecs;}
    double getPertProb(){return pertProb;}
    int getNumEnvs(){return numEnvs;}

    int getNumSelf(){return numSelf;}
    vector<vector<double>>* getPerts(){return &perturbations;}

    vector<double>* getChange(){return &change;}
    void setChange(vector<double> newVal){change = newVal;}

    vector<double> getPertRange(){return pertRange;}

    vector<unique_ptr<Species>>* getSpeciesList() {return &speciesList;}
    vector<unique_ptr<Species>>* getDeadSpecies() {return &deadSpecies;}



    /**
    *\brief Wrapper for call to NextGen object
    */
  	void getNextGen(int stepNum){nextGen->getNextGen(stepNum, &speciesList, this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void saveHeader(int runNumber){save->saveHeader(runNumber, this);}
    /**
    *\brief Wrapper for call to Save object
    */
  	void saveAll(int step){save->saveAll(step, &speciesList, this);}
    /**
    *\brief Wrapper for call to Save object
    */
  	void saveDens(int step){save->saveDens(step, &speciesList, this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void saveFinalDens(int step){save->saveFinalDens(step, &speciesList, &deadSpecies, this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void saveCc(int step){save->saveCc(step, &speciesList, this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void saveInter(int step){save->saveInter(step, &speciesList, this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void saveOpts(int step){save->saveOpts(step, &speciesList, this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void completeSave(int runNumber, int step, int se){save->completeSave(runNumber, step, &speciesList, this, se);}
    /**
    *\brief Wrapper for call to Save object
    */
    void saveRoutes(){save->saveRoutes(&speciesList, this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void savePerturbations(){save->savePerturbations(this);}
    /**
    *\brief Wrapper for call to Save object
    */
    void saveLastTurnChange(){save->saveLastTurnChange(this);}
    /**
    *\brief Method to get size of migrations from this Environment to all others for each Species 
    *
    *This method coordinates the call to getMigration() in each Species, and returns a vector [numEnv][numSpec] = size of migration of numSpec to numEnv.
    *Migrations are then distributed to the correct Environment object by Simulation.
    *
    *\note Only used in simulations with multiple environments
    */
    vector<vector<double>> getMigrants();

    /**
    *\brief Method to integrate migrants from other environments into this one
    *
    * This method integrates all migrants from other environments into this one. Species attributes are changed, and
    * become the weighted mean of already present population and the migrating one.
    *
    *\param migrants A vector containing size of migrations: [numEnv][numSpec] = size of migration of numSpec from numEnv
    *\note Only used in simulations with multiple environments
    */

    void integrateMigrants(vector<vector<double>> migrants);
    

};



#endif /*ENVIRONMENT_HPP*/