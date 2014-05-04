/* 
 * File:   CircuitRepr.h
 * Author: ph4r05
 *
 * Created on April 29, 2014, 4:29 PM
 */

#ifndef CIRCUITREPR_H
#define	CIRCUITREPR_H

#include "EACglobals.h"
#include "GA1DArrayGenome.h"
#include "circuit/ICircuit.h"
#include "GACallbacks.h"

class GateCircuit : public ICircuit {
public:
    GateCircuit();
    virtual ~GateCircuit();
    string shortDescription();
    
    // Getters for GA callbacks.
    virtual inline GAGenome::Initializer       getInitializer() { return GACallbacks::initializer; }
    virtual inline GAGenome::Evaluator         getEvaluator()   { return GACallbacks::evaluator;   }
    virtual inline GAGenome::Mutator           getMutator()     { return GACallbacks::mutator;     }
    virtual inline GAGenome::Comparator        getComparator()  { return NULL; }
    virtual inline GAGenome::SexualCrossover   getSexualCrossover()  { return GACallbacks::crossover; }
    virtual inline GAGenome::AsexualCrossover  getAsexualCrossover() { return NULL; }
    
    int executeCircuit(GAGenome* pGenome, unsigned char* inputs, unsigned char* outputs) { }

    // Constructs empty genome from settings.
    virtual GAGenome * createGenome(const SETTINGS * settings, bool setCallbacks=false);
    
    // Sets non-null GA callbacks.
    virtual GAGenome * setGACallbacks(GAGenome * g, const SETTINGS * settings);
    
    // Creates a configuration population.
    virtual GAPopulation * createConfigPopulation(const SETTINGS * settings);
    
    // Individual post-processing.
    virtual int postProcess(GAGenome &originalGenome, GAGenome &prunnedGenome);
private:

};

#endif	/* CIRCUITREPR_H */

