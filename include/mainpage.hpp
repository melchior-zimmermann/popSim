/*! \mainpage General information
 *
 * \section intro_sec Introduction
 *
 * popSim was developped as a simulation software for population dynamics. It uses generalized Lotka-Volterra equations
 * to simulate species interactions, and solves them usng the explicit Euler scheme. popSim can simulate
 * multiple species in multiple environments, with evolution, and taking into account an environmental constant
 * which represents abiotic factors of the environent.
 * popSim comes with no warranty whatsoever, and I recommend that you do some simple tests with it
 * before using it for any serious work, to make sure it suits your needs.
 * popSim is open source, and is published under a GPL licence.
 * 
 * popSim was developped on Arch Linux (www.archlinux.org)
 *
 * \section doc_sec Documentation
 *
 * This documentation describes most methods used in popSim. Getters and setters, as well as constructors, 
 * are not commented in detail, and neither are wrapper functions for library methods. Instead, this documentation
 * focues more on the role of each method, and the logic underpinning the inner workings of the pogram.
 * If you are interested by the details of implementations, refer to the source code you can find at https://github.com/melchior-zimmermann/popSim.
 * If you have questions or remarks, please send them to popsimproject \<at\> gmail.com, or post them on the github
 * comment section/forum.
 *
 *
 *\section doc_compo Class composition
 *
 *The classes documented here interact in the following manner:
 *
 *The Species class contains a pointer to a Change object to calculate its change in density form one step to the next, and a pointer to an Evo 
 *object to calculate evolution (when required).
 *
 *The Environment object contains a list of pointers to Species objects, as well as a pointer to a NextGen object that is used to update Species
 *densities in a coordinatedd manner. Environment objects also contain a pointer to a Save object that is in charge of saving results.
 *
 *The Simulation object contains a pointer to an Environment object (or a list of pointers to Environment objects), and is in charge of executing the 
 *simulation loop while calling the appropriate methods.
 *
 *
 *\section doc_files Files
 *
 *Files containing the implementaiton of a class have the same name as that class (e.g. the class Species is implemented in Species.hpp and Species.cpp).
 *
 *
 *The interface.hpp file contains methods to get simulation parameters from the user and load/save those parameters, and is also in charge of
 *launchin simulations.
 *
 *The initializers.hpp file contains methods to create Species and Environment objects from simulation parameters.
 *
 *The reloadModule.hpp contains all methods used for loading a simularion form a save file, and running that simulation again, eventually after
 *adding some species.
 *
 *The helpers.hpp file contains wrapper functions for library calls.
 *
 *The main.cpp file parses user arguments, and calls the appropriate methods.
 *
 *The basicStats.py file contains methods to output some basic statistics about simulation results, to get a quick overview of the results (this should not be 
 *considered an alternative to conducting detailed statistics).
 *
 */
/**
 * @file mainpage.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief Main page layout of this documentation
 */