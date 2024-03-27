// dftmain.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// March 28, 2024

#include "dft.h"

using namespace std;

int main(int argc, char *argv[])
{
  string outputfileName;

  // set variables from cmd terminal
  string inputfileName = argv[1];
  double samplingFreq = atof(argv[2]);
  double startFreq = atof(argv[3]);
  double endFreq = atof(argv[4]);
  int nSteps = atof(argv[5]);
  if (argv[6])
  {
    outputfileName = argv[6];
  }
  else
  {
    // default output file name
    outputfileName = "dftlog.txt";

    cout << "No output log file name specified." << endl
         << "Default name used." << endl;
  }

  cout << endl;

  // initialize xData
  double *xData;

  // import data from input file and get its duration
  int xDuration = SignalImport(inputfileName, &xData);

  // check if data were imported from file name inputted
  if (xDuration == 0)
  {
    return 0;
  }

  // initialize output array containers
  double *realPart = new double[nSteps + 1];
  double *imagPart = new double[nSteps + 1];
  double *magnitude = new double[nSteps + 1];
  double *phase = new double[nSteps + 1];

  // do DFT on x
  computeDFT(xData, xDuration, samplingFreq, startFreq, endFreq,
             nSteps, realPart, imagPart, magnitude, phase);

  // export accordingly
  DFTExport(outputfileName, startFreq, endFreq, nSteps, realPart,
            imagPart, magnitude, phase);

  cout << endl;

  return 0;
}
