#include <vector>
#include <memory>

class Species;
class Environment;

using namespace std;

/**
 * @file Save.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing the definition of the Save class, which contians all methods used to save simulation results.
 */

#ifndef SAVE_HPP
#define SAVE_HPP

 /**
 *\brief Class containing methods responsible for saving any and all aspects of the simulation (except simulation parameters, handled by saveSimParams() in interface.hpp)
 *
 * This class is responsible for saving simulation results, such as Species density or interaction values. The saves generated
 * with these methods can be used for both statistical analysis of the results, as well as for rerunnning simulations containing
 * Species with the same attributes, but other simulation parameters (e.g. desactivatig evolution).
 * The name of the save files are:
 *  - "rawSave", containing all info on the Environment/Species, used by all save methods except completeSave() (file name == Environment.filename)
 *  - "completeSaveS", containing a complete save of the start of the simulation (used to reRun simulations) (generated using Environment.path)
 *  - "completeSaveE", containing a complete save of the end of the simulation (used to reRun simulations) (generated using Environment.path)
 *
 * If the simulation has multiple environments, the file names will have the string "Env<envNum>" appended (e.g. "completeSaveSEnv2").
 * For ease of use, this obect contains all methods necessary to save any and all values of the running simulation. Like Species and
 * Environment, this means that in the case of basic simulations, it uses more space in memory than strictly necessary.
 * 
 * \warning Saves are done directly to disk, and are not buffered in memory. This means high save frequency = high disk access frequency,
 * which can slow down simulations considerably.
 */



class Save{
  protected:
  	bool evo;/**<Used to determine wether to save parameters relating to evolution*/
    bool eco;/**<Used to determine wether to save parameters relating to environmental constant*/
    bool multi;/**<Used to determine wether to save parameters relating to multi-environment simulations*/
  public:
  	Save(bool _evo, bool _eco, bool _multi): evo(_evo), eco(_eco), multi(_multi){}
    /**
    * \brief Saves a header of the simulation, indicating which kind of simulation it is, as well as containing information on the Environment
    */
    void saveHeader(int runNumber, Environment* env);
    /**
    * \brief Saves all pertinent data of the current simulation
    *
    * This method calls all other pertinent Save methods to make a snapshot of the simulation
    * at a given time (e.g. if a simulation doesn't use evolution, the 'range' attribute of the Species class will
    * not be saved, since it is never used).
    *
    *\param step Step at which the save takes place
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    */
  	void saveAll(int step, vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves densities of all species
    *
    *\param step Step at which the save takes place
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    */
  	void saveDens(int step, vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves cc (carrying capacity) of all species
    *
    *\param step Step at which the save takes place
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    */
  	void saveCc(int step, vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves interaction values of all species
    *
    *\param step Step at which the save takes place
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    */
    void saveInter(int step, vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves optimums of all species
    *
    *\param step Step at which the save takes place
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    *\note This method is only called when the environmental constant influences Species density change
    */
    void saveOpts(int step, vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Makes a complete save of the simulation
    *
    * Complete saves are mainly used to reRun a simulation, perhaps after changing
    * some attributes of Species/Environment (for more information on loading/reRunning past
    * simulations, refer to the documentation of reloadModule.hpp).
    *
    *\param runNumber which run of the simulation we're at.
    *\param step Step at which the save takes place
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    *\param se Parameter indicating wheter the save happens at the start or the end of a simulation (to determine file name)
    */
    void completeSave(int runNumber, int step, vector<unique_ptr<Species>>* speciesList, Environment* env, int se);
    /**
    * \brief Saves routes of all species
    *
    *\param step Step at which the save takes place
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    *\note This method is only called when there are multiple environments
    */
    void saveRoutes(vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves evolution parameters of all species
    *
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    *\note This method is only called when evolution occurs
    *
    */
    void saveEvoParams(vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves evolution parameters in regards to optimum of all species
    *
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    
    *\note This method is only called when the environmental constant influences Species density change and evolution occurs
    */
    void saveEEParams(vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves all perturbations of the Environment (when they happened, and what value envConst had AFTER they occured)
    *
    *\param env Pointer to the Environment
    *\note This method is only called when the environmental constant influences Species density change
    */
    void savePerturbations(Environment* env);
    /**
    * \brief Saves the total amount of change in density (sum of absolute values) that happened in the envioronment during the last turn of the simulation
    *
    *\param env Pointer to the Environment
    */
    void saveLastTurnChange(Environment* env);
    /**
    * \brief Saves the density to individuals of all species
    *
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    *\note This method is only called when evolution occurs
    */
    void saveDtoI(vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves alphas of all species
    *
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    */
    void saveAlphas(vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves betas of all species
    *
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    */
    void saveBetas(vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves ID of all (live) species (mainly used to reRun simulations)
    *
    *\param speciesList List of all Species present in the Environment
    *\param env Pointer to the Environment
    */
    void saveID(vector<unique_ptr<Species>>* speciesList, Environment* env);
    /**
    * \brief Saves final densities of all species
    *
    * This method is slighlty different from saveDens(), so that the python script I use
    * to generate some basic statistics for simulations can differentiate from regular
    * density saves (dead species are also saved for the same reason)
    *
    *\param speciesList List of all Species present in the Environment
    *\param deadSpecies List of all Species where <tt>density == 0</tt>
    *\param env Pointer to the Environment
    */
    void saveFinalDens(int step, vector<unique_ptr<Species>>* speciesList, vector<unique_ptr<Species>>* deadSpecies, Environment* env);
    bool getEco(){return eco;}
};

#endif /*SAVE_HPP*/