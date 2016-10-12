#ifndef LANGAGE_H_INCLUDED
#define LANGAGE_H_INCLUDED

#include "F09fonctions.h"

void resolution();
void lecture(Donnees& donnees);
bool verifierRecursiviteGauche(Donnees& donnees);
void supprimerRecursiviteGauche(Donnees& donnees);
void calculerPremiers(Donnees& donnees);
void calculerSuivants(Donnees& donnees);
void premiers(Donnees& donnees, string production, string nonTerminal, vector<string> & productionsTraitees);
void suivants(Donnees& donnees, string production, string nonTerminal);
void tableAnalyse(Donnees& donnees);
void reconnaissance(Donnees& donnees);

void afficherGrammaire(Donnees& donnees);
void afficherPremiers(Donnees& donnees);
void afficherSuivants(Donnees& donnees);

string trouverNomNonTerminal(Donnees& donnees);

#endif // LANGAGE_H_INCLUDED
