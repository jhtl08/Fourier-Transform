// dft.cpp
// Kyle Coloma, Jason Lorenzo
// ENGG 151.01-A
// March 28, 2024

#include "dft.h"
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

int SignalImport(string fileName, double **xData)
{
  // opens and checks the files
  ifstream isignalFile;
  isignalFile.open(fileName);
  if (!isignalFile.is_open())
  { // fail import feedback
    cout << "Unable to import a valid signal from "
         << fileName << endl;
    return 0;
  }
  // parsing elements to vector
  string line;
  getline(isignalFile, line);

  vector<double> vect_elements;

  double temp;
  string word;
  stringstream ss(line);

  ss >> word;
  stringstream ss1(word);
  if (ss1 >> temp) // Ex. '-4' '-4a'
  {
    if (ss1.eof()) // Ex. '-4'
    {
      vect_elements.push_back(temp);
    }
    else // Ex. '-4a'
    {
      cout << "Encountered nonfloat as first element in " << fileName
           << "\nNo elements imported.\n"
           << endl;
      return 0;
    }
  }
  else // Ex. 'a'
  {
    cout << "Encountered nonfloat as first element in " << fileName
         << "\nNo elements imported.\n"
         << endl;
    return 0;
  }
  // Clear word to reuse it
  word.clear();

  // Check if there is another double
  ss >> word;
  stringstream ss2(word);
  if (ss2 >> temp) // Ex. '-4' '-4a'
  {
    if (ss2.eof()) // Ex. '-4'
    {
      if (vect_elements[0] != 0)
      { // Message to inform disregarding of signal file start index
        cout << "Starting index of " << vect_elements[0]
             << " ignored.\nStarting index set to 0." << endl;
      }
      vect_elements[0] = temp;
    }
    else // Ex. '-4a'
    {
      cout << "Encountered nonfloat as first element in " << fileName
           << "\nNo elements imported.\n"
           << endl;
      return 0;
    }
  }

  // Clear word to reuse it
  word.clear();

  // Check valid values
  while (getline(isignalFile, line))
  {
    if (line.empty())
    {
      cout << "Parsing of input from " << fileName
           << " stopped at duration " << vect_elements.size()
           << " due to an empty line." << endl;
      break;
    }

    stringstream ss3(line);
    ss3 >> word;

    stringstream ss4(word);
    if (ss4 >> temp)
    {
      if (ss4.eof()) // Ex. '4'
      {
        vect_elements.push_back(temp);
      }
      else // Ex. '4a'
      {
        cout << "Parsing of input from " << fileName
             << " stopped at duration " << vect_elements.size()
             << " due to invalid element." << endl;
        break;
      }
    }
    else // Ex. 'a'
    {
      cout << "Parsing of input from " << fileName
           << " stopped at duration " << vect_elements.size()
           << " due to invalid element." << endl;
      break;
    }
    word.clear();
  }

  int duration = vect_elements.size();

  // allocate memory
  *xData = new double[duration];

  // convert vector to array
  for (int i = 0; i < duration; i++)
  {
    (*xData)[i] = vect_elements[i];
  }

  // successful import feedback
  cout << "Signal with start index 0, duration " << duration
       << ", imported from " << fileName << "\n"
       << endl;

  isignalFile.close();
  return duration;
}

void DFTExport(string fileName, double startFreq, double endFreq,
               int nSteps, double *realPart, double *imagPart,
               double *magnitude, double *phase)
{
  // creates or open, then checks the file
  ofstream outFile;
  outFile.open(fileName, ios::app); // init to append
  if (!outFile.is_open())
  { // fail export feedback
    cout << "Unable to export DFT result to "
         << fileName << endl;
    return;
  }

  // exporting to file
  outFile << "frequency \t real part \t imaginary part\n";
  double freqInterval = (endFreq - startFreq) / nSteps;
  int i = 0;
  for (double w = startFreq; w <= endFreq; w += freqInterval)
  {
    outFile << w << " \t" << realPart[i] << " \t" << imagPart[i]
            << endl;
    i++;
  }
  outFile << "\nfrequency \tmagnitude \tphase(degrees)\n";
  i = 0;
  for (double w = startFreq; w <= endFreq; w += freqInterval)
  {
    outFile << w << " \t" << magnitude[i] << " \t" << phase[i]
            << endl;
    i++;
  }
  outFile << endl;

  outFile.close();

  if (nSteps < 10)
  { // exporting to command line
    cout << "frequency\treal part\timaginary part\n";
    double freqInterval = (endFreq - startFreq) / nSteps;
    int i = 0;
    for (double w = startFreq; w <= endFreq; w += freqInterval)
    {
      cout << w << "  \t" << realPart[i] << "  \t" << imagPart[i]
           << endl;
      i++;
    }
    cout << "\nfrequency\tmagnitude\tphase(degrees)\n";
    i = 0;
    for (double w = startFreq; w <= endFreq; w += freqInterval)
    {
      cout << w << "  \t" << magnitude[i] << "  \t" << phase[i]
           << endl;
      i++;
    }
  }

  // export success feedback
  cout << "\nOutput Signal exported to "
       << fileName << endl;
}

void computeDFT(double *xData, int xDuration,
                double samplingFreq, double startFreq,
                double endFreq, int nSteps, double *realPart,
                double *imagPart, double *magnitude,
                double *phase)
{
  double b_n, digitalFreq;
  double freqInterval = (endFreq - startFreq) / nSteps;
  int i = 0;
  for (double w = startFreq; w <= endFreq; w += freqInterval)
  {
    // set accumulator variables
    realPart[i] = 0.0;
    imagPart[i] = 0.0;
    // convert each w to equivalent digital frequency
    digitalFreq = (2 * M_PI * w / samplingFreq);
    for (int n = 0; n < xDuration; n++)
    {
      // calculate e's exponent for real and im components
      b_n = (-digitalFreq * n);
      // accumulate sums for the components of each discrete w
      realPart[i] += xData[n] * cos(b_n);
      imagPart[i] += xData[n] * sin(b_n);
    }
    // pythagorean of components to find magnitude
    magnitude[i] = sqrt(realPart[i] * realPart[i] +
                        imagPart[i] * imagPart[i]);
    // arctan to find phase
    phase[i] = atan2(imagPart[i], realPart[i]) * 180 / M_PI;
    i++;
  }
}