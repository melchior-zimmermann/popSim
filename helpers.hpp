#include <string>
#include <vector>
/**
 * @file helpers.hpp
 * @author Melchior Zimmermann
 * @date 24 Dec 2016
 * @brief File containing helper functions used throughout the pogramm.
 *
 * Most of the methods in this file are concerned on safely getting user input and generating random numbers.
 * The first set of methods are wrapper functions for iostream functions, whereas the
 * second set of methods are wrappers to the rand() function of the standard library.
 */


#ifndef HELPERS_HPP
#define HELPERS_HPP

int getInt();
double getDouble();
char getChar();
double getRandom();
int getUniformInt(int range[2]);
void getRange(double range[2]);
void getIntRange(int range[2]);
double getUniform(double range[2]);
double getUniformV(std::vector<double> range);
/**
*\brief Method used to initiate save files makes sure they exist and are empty before launching simulations.
*/
int initSave(std::string fileName);

#endif /*HELPERS_HPP*/