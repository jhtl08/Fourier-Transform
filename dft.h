// dft.h
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// April 01, 2024

#ifndef DFTHEADERDEF
#define DFTHEADERDEF

#include <iostream>

using namespace std;

// import function
int SignalImport(string fileName, double **xData);

// export function
void DFTExport(string fileName, double startFreq, double endFreq,
               int nSteps, double *realPart, double *imagPart,
               double *magnitude, double *phase);

// DFT function
void computeDFT(double *xData, int xDuration,
                double samplingFreq, double startFreq,
                double endFreq, int nSteps, double *realPart,
                double *imagPart, double *magnitude,
                double *phase);

#endif