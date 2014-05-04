/* 
 * File:   PolyRepr.cpp
 * Author: ph4r05
 * 
 * Created on April 29, 2014, 4:20 PM
 */

#include "PolynomialCircuit.h"
#include "circuit/ICircuit.h"
#include "PolynomialCircuitIO.h"
#include "GAPolyCallbacks.h"
#include "Term.h"
#include <math.h>

PolynomialCircuit::PolynomialCircuit() : ICircuit(CIRCUIT_POLYNOMIAL) { }

PolynomialCircuit::~PolynomialCircuit() { }

GAGenome* PolynomialCircuit::createGenome(const SETTINGS* settings, bool setCallbacks) {
    // Has to compute genome dimensions.
    int numVariables = settings->circuit.sizeInput;
    int numPolynomials = settings->circuit.sizeOutput;
    unsigned int   termSize = Term::getTermSize(numVariables); // Length of one term in terms of POLY_GENOME_ITEM_TYPE.    
    
    GA2DArrayGenome<POLY_GENOME_ITEM_TYPE> * g = new GA2DArrayGenome<POLY_GENOME_ITEM_TYPE>(
            numPolynomials, 
            1 + termSize * settings->polydist.genomeInitMaxTerms,               // number of terms N + N terms.
            this->getEvaluator());
    
    if (setCallbacks){
        setGACallbacks(g, settings);
    }
    
    return g;
}

GAGenome* PolynomialCircuit::setGACallbacks(GAGenome* g, const SETTINGS* settings) {
    g->initializer(getInitializer());
    g->evaluator(getEvaluator());
    g->mutator(getMutator());
    g->crossover(getSexualCrossover());
    return g;
}

GAPopulation* PolynomialCircuit::createConfigPopulation(const SETTINGS* settings) {
    int numVariables = settings->circuit.sizeInput;
    int numPolynomials = settings->circuit.sizeOutput;
    unsigned int   termSize = Term::getTermSize(numVariables);   // Length of one term in terms of POLY_GENOME_ITEM_TYPE.    
    
    GA2DArrayGenome<POLY_GENOME_ITEM_TYPE> g(
            numPolynomials, 
            1 + termSize * settings->polydist.genomeInitMaxTerms,               // number of terms N + N terms.
            this->getEvaluator());
    setGACallbacks(&g, settings);
    
    GAPopulation * population = new GAPopulation(g, settings->ga.popupationSize);
    return population;
}

bool PolynomialCircuit::postProcess(GAGenome& originalGenome, GAGenome& prunnedGenome) {
    return false;
}
