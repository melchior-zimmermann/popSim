#include <vector>
using namespace std;



#ifndef INTERFACE_HPP
#define INTERFACE_HPP

/**
 * @file interface.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing simulation parameters wizard, as well as methods to load/save simulation parameters and execute simulations.
 *
 * The two methods of this file are used to generate Environment/Species with attributes specified in
 * simParams. getEnv() calls initSpecs() to populate the Environment's speciesList.
 */

/**
*\brief A structure that contains all the information necessary to generate a simulation.
*/

struct simParams{
	bool completeEnd = false;/**<Bool deciding wether or not there are complete saves made at the end of a simulation*/
	bool completeStart = false;/**<Bool deciding wether or not there are complete saves made at the start of a simulation*/
	bool rk = false;
	double interRange[2];/**<Unfirom range from which Species interaction values are drawn*/
	double densRange[2];/**<Unfirom range from which Species density values are drawn*/
	double alphaRange[2];/**<Unfirom range from which Species alpha values are drawn*/
	double betaRange[2];/**<Unfirom range from which Species beta values are drawn*/
	double ccRange[2];/**<Unfirom range from which Species carrying capacity (cc) values are drawn*/
	double optRangeSpec[2];/**<Unfirom range from which Species optiumum values are drawn*/
	double optRangeEnv[2];/**<Unfirom range from which Environment environmental constant (envConst) values are drawn*/
	double evoRangeRange[2];/**<Unfirom range from which evolution ranges for interaction values are drawn (see Evo)*/
	double optEvoRange[2];/**<Unfirom range from which evolution range for Species optimum values are drawn*/
	double migProbRange[2];/**<Unfirom range from which Species migration probability values are drawn*/
	double migSizeRange[2];/**<Unfirom range from which Species migration size values are drawn*/
	double pertRange[2];/**<Unfirom range from which values for perturbations (changes in envConst) are drawn*/
	double delta;/**<Step size in the explicit Euler Scheme*/
	double deathThreshold;/**<Density below which species aSe considered extinct (-> Species density is set to 0)*/
	double perturbationProb;/**<Porbability of perturbation of environment in a given timestep*/
	int genTimeRange[2];/**<Unfirom range from which Species evolution rate (each how many steps an evolution event occurs) are drawn*/
	int evoResRange[2];/**<Unfirom range from which Species resolution values are drawn*/
	vector<int> specsPerEnv;/**<Number of Species in each Environment (indexed by ID of environments)*/
	int numSpecs;/**<Total number of species in the simulation*/
	int numSteps;/**<Number of steps for which to run the simulation*/
	int saveDiv;/**<Each how many steps Species densities are saved*/
	int interSaveDiv;/**<Each how many steps Species interaction values are saved*/
	int numEnvs;/**<Number of environments in the simulation*/
	char eco;/**<Wether or not the environmental constant is taken into account ('y' or 'n')*/
	char evo;/**<Wether or not evolution is on in this simulation ('y' or 'n')*/
	char multi;/**<Wether or not this simulation has multiple environments ('y' or 'n')*/
};
/**
*\brief The simulation parameter wizard, guides the user through defining simulation parametersS
*/
int getSimParams(string savePath, simParams* params);
/**
*\brief Saves a template of simulation parameters in file simParams in folder described by path (current folder if path is an empty string)
*/
void makeTemplate(string path);
/**
*\brief Loads the simulation parameters contained in the file simParams in folder path into a simParams struct
*/
int loadSimParams(string savePath, simParams* params);
/**
*\brief Saves the simulation parameters contained in params into a file simParams in folder path
*/
int saveSimParams(string savePath, simParams* params);
/**
*\brief Launches a sim defined by params
*
* This method takes care of creating a Simulaiton object, initiating all its Environment's with their
* respective Species, and running the Simulation for the appropriate number of steps.
*/
int launchSim(string savePath, int runNumber, simParams params);




#endif /*INTERFACE_HPP*/