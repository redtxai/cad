#ifndef AAGREADER_H
#define AAGREADER_H

#include "aig.h"

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include <string.h>

using namespace std;

class AAGReader {
  private:
  	string filename;
    ifstream source;
    ofstream debug;
    string word;
    char buf[250];
    istringstream line;

  public:
    AAGReader(string sourcePath);
    Aig* readFile();
};

#endif // AAGREADER_H
