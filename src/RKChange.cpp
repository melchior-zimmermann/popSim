
#include <vector>
#include <memory>
#include <iostream>
#include "RKChange.hpp"
#include "Species.hpp"

using std::vector;
using std::unique_ptr;
using std::cout;

RKChange::RKChange(unique_ptr<Change> _change)
	: change(move(_change)) {}

double RKChange::getChange(Species* spec, double delta, vector<unique_ptr <Species>>* speciesList) {
	double currentDensity = spec->getDensity();
	double t1, t2, t3;
	double u, u1, u2, u3, u0 = spec->getDensity();
	double f0, f1, f2, f3;

	f0 = (u0 - spec->getPreviousDensity())/delta;

	t1 = delta/2.0;
	u1 = u0 + delta * f0/2.0;
	spec->setDensity(u1);
	f1 = change->getChange(spec, t1, speciesList)/t1;

	t2 = t1;
	u2 =  u0 + delta * f1/2.0;
	spec->setDensity(u2);
	f2 = change->getChange(spec, t2, speciesList)/t2; 

	t3 = delta;
	u3 = u0 + delta*f2;
	spec->setDensity(u3);
	f3 = change->getChange(spec, t3, speciesList)/t3; 	

	u = delta * (f0 + 2.0*f1 + 2.0*f2 + f3)/6.0;

	spec->setDensity(currentDensity);

	// cout<<"f0: "<<f0<<endl;
	// cout<<f1<<endl;
	// cout<<f2<<endl;
	// cout<<f3<<endl;

	// cout<<"u0: "<<u0<<endl;
	// cout<<u1<<endl;
	// cout<<u2<<endl;
	// cout<<u3<<endl;
	// cout<<u<<endl;

	// cout<<"Current density: "<<spec->getDensity()<<endl;
	// cout<<"Previous density: "<<spec->getPreviousDensity()<<endl;



	return u;
}