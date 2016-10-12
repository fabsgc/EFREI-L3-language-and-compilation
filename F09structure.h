#ifndef STRUCTURE_H_INCLUDED
#define STRUCTURE_H_INCLUDED

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <list>
#include <map>
#include <set>

using namespace std;

typedef map<string, map<string, string>> TableAnalyse;
typedef map<string, vector<string>> Grammaire;
typedef map<string, vector<string>> Premiers;
typedef map<string, vector<string>> Suivants;

struct Donnees{
    Grammaire grammaire;
    Premiers suivants;
    Suivants premiers;
    TableAnalyse tableAnalyse;
    list<string> nonTerminaux;
    list<string> terminaux;
};

#endif // STRUCTURE_H_INCLUDED
