#ifndef FUNCTIONS_H
#define FUNCTIONS_H 1

#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>	// stat() depends
#include <getopt.h>	// getoptlong() depends
#include <cstdlib>
#include <map>
#include <utility>

#include "global.h"
#include "Position.h"

using std::fstream;
using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::atoi;
using std::to_string;
using std::stoi;
using std::map;
using std::pair;

bool exists(const string&);

int readOptions(int&, char**&);
void readConfig(const string&);
bool checkDependencies();
void printConfiguration(const int&);
void makeNewFiles(const string&, string&, string&, const int&);
int makeFile(const string&, const string&, const char&, const int&);
int findGene(const string&, const string&, const int&);
int findChr(const string&, const string&, const int&);
void fillMap(map<string, pair<Position, string>>&, const string&);
void writeMap(const map<string, pair<Position, string>>&, const string& fileName = "");

#endif
