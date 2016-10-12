#include "F09langage.h"

/**
 * @brief Fonction principale ? partir de laquelle se lance toute la r?solution
 * @return void
*/

void resolution()
{
    Donnees donnees;
    lecture(donnees);
    afficherGrammaire(donnees);

    if(verifierRecursiviteGauche(donnees))
    {
        cout << "La grammaire n'est pas recursive" << endl;
    }
    else
    {
        cout << "La grammaire est recursive" << endl;
        supprimerRecursiviteGauche(donnees);
        afficherGrammaire(donnees);
    }

    calculerPremiers(donnees);
    cout << "Affichage des premiers" << endl;
    afficherPremiers(donnees);

    calculerSuivants(donnees);
    cout << "Affichage des suivants" << endl;
    afficherSuivants(donnees);
}

/**
 * @brief Fonction de lecture du fichier. Lit le fichier et enregistre dans deux vector la liste des terminaux et des nons terminaux
 * @param Donnees& donnees : r?f?rence vers les donn?es
 * @return void
*/

void lecture(Donnees& donnees)
{
    string nom;
    string ligne;
    string caractere;
    string nonTerminal;

    /*cout << "Nom du fichier : ";
    cin >> nom;
    cout << endl;

    string nomFichier = nom + ".txt";*/
    string nomFichier = "F09grammaire.txt";
    ifstream fichier(nomFichier);

    if(fichier.good())
    {
        while(getline(fichier, ligne))
        {
            istringstream ss(ligne);
            ss >> nonTerminal;
            vector<string> ligneGrammaire;

            while(ss >> caractere)
            {
                regex regNonTerminal ("([A-Z])");

                for(auto i : caractere)
                {
                    if(regex_match(var2String(i), regNonTerminal))
                    {
                        donnees.nonTerminaux.push_back(var2String(i));
                    }
                    else
                    {
                        donnees.terminaux.push_back(var2String(i));
                    }
                }

                ligneGrammaire.push_back(caractere);
            }

            donnees.grammaire[nonTerminal] = ligneGrammaire;
        }
    }
}

/**
 * @brief Parcourt la d?finition de chaque terminal. Regarde si pour un terminal A, on a une cha?ne Ab avec A ? gauche. Si c'est le cas alors la grammaire est r?cursive ? gauche
 * @param Donnees& donnees : r?f?rence vers les donn?es
 * @return bool
*/

bool verifierRecursiviteGauche(Donnees& donnees)
{
    //On parcourt toutes les d?finitions des non terminaux
    for(auto i : donnees.grammaire)
    {
        //On parcourt ensuite chaque possibilit?
        for(auto j : i.second)
        {
            if(i.first == var2String(*(j.begin())))
            {
                return false;
            }
        }
    }

    return true;
}

/**
 * @brief Parcourt la grammaire et supprime la r?cursivit? ? gauche
 * @param Donnees& donnees : r?f?rence vers les donn?es
 * @return void
*/

void supprimerRecursiviteGauche(Donnees& donnees)
{
    //on parcourt la grammaire
    for(auto i = donnees.grammaire.begin(); i != donnees.grammaire.end(); i++)
    {
        string nouveauNonTerminal = "";

        //pour chaque production, on parcourt toutes les possibilit?s
        for(auto j = i->second.begin(); j != i->second.end(); j++)
        {
            //Si on a A->Ab
            if(i->first == var2String(*(j->begin())))
            {
                //On cree un nouveau non terminal la premi?re fois qu'on d?tecte une r?cursivit? ? gauche
                if(nouveauNonTerminal == "")
                {
                    nouveauNonTerminal = trouverNomNonTerminal(donnees);
                }

                //Pour chaque A->Ab, on cr?e un A'->bA'
                donnees.grammaire[nouveauNonTerminal].push_back(j->substr(1, j->size()) + nouveauNonTerminal);
                donnees.grammaire[nouveauNonTerminal].push_back("#");

                //On supprime A->Ab
                donnees.grammaire[i->first].erase(j);

                //On ajoute les nouveaux non terminaux ? la liste
                donnees.nonTerminaux.push_back(nouveauNonTerminal);
            }
        }

        //Si on a eu une r?cursivit? ? gauche, on corrige les possiblit?s non r?cursives : A->B devient A->bA'
        if(nouveauNonTerminal != "")
        {
            for(auto j = i->second.begin(); j != i->second.end(); j++)
            {
                *j += nouveauNonTerminal;
            }
        }
    }
}

/**
 * @brief Calcule les premiers de toutes les productions
 * @param Donnees& donnees : r?f?rence vers les donn?es
 * @return void
*/

void calculerPremiers(Donnees& donnees)
{
    //On parcourt toutes les productions
    for(auto i = donnees.grammaire.begin(); i != donnees.grammaire.end(); i++)
    {
        vector<string> productionsTraitees;
        premiers(donnees, i->first, i->first, productionsTraitees);
    }
}

/**
 * @brief Calcule les suivants de toutes les productions
 * @param Donnees& donnees : r?f?rence vers les donn?es
 * @return void
*/

void calculerSuivants(Donnees& donnees)
{
    //On parcourt toutes les productions
    for(auto i = donnees.grammaire.begin(); i != donnees.grammaire.end(); i++)
    {
        vector<string> productionsTraitees;
        suivants(donnees, i->first, i->first);
    }

    //On a r?cup?rer tous les suivants mais il reste des r?f?rences manquantes : WX
    //On parcourt tous les suivants et on remplace
    for(auto i = donnees.suivants.begin(); i != donnees.suivants.end(); i++){
        for(auto j = donnees.suivants.begin(); j != donnees.suivants.end(); j++){
            if(i != j){
                //Pour chaque reference manquante dans i, on regarde si elle peut ?tre remplac?e par le contenu de j
                string referenceManquante = "W" + j->first;
                string ssdf = i->first;

                for(auto k = i->second.begin(); k != i->second.end(); k++){
                    string tmp = *k;

                    if(tmp == referenceManquante){
                        i->second.push_back("ff");
                        //assemblerSansDoublons(donnees.suivants[i->first], donnees.suivants[j->first]);
                    }
                }
            }
        }
    }
}

/*bool contientReferenceManquante(Suivants suivant){
    regex refManquante ("W(.+)");

    for(auto i : suivant){
        if(regex_match(var2String(i.second), refManquante))
            return true;
    }
}*/

/**
 * @brief Pour chaque production, calcule ses premiers de mani?re r?cursive
 * @param Donnees& donnees : r?f?rence vers les donn?es
 * @param string production : le nom de la production que l'on nest en train de traiter
 * @param string nonTerminal : ? chaque fois que pour une production on tombe sur un nonTerminal, on est oblig? de relancer la fonction pour trouver les "sous" terminaux
 * @param vector<string> & productionsTraitees : ? chaque fois qu'on traite une production, on l'ajoute ? la liste pour ne pas la retraiter une deuxi?me fois
 * @return void
*/

void premiers(Donnees& donnees, string production, string nonTerminal, vector<string> & productionsTraitees)
{
    //On parcourt toutes les combinaisons dans la production en question
    //Pour chaque, on regarde quel est le premier caract?re, si c'est un non terminal on l'ajoute aux suivants
    //Sinon, on relance l'algorithme
    for(auto i = donnees.grammaire[nonTerminal].begin(); i != donnees.grammaire[nonTerminal].end(); i++)
    {
        string caractere = var2String(*i->begin());
        productionsTraitees.push_back(nonTerminal);

        //Si c'est un terminal, on l'ajoute aux premiers
        if(!existe(donnees.nonTerminaux, caractere))
        {
            if(!existe(donnees.premiers[production], caractere))
                donnees.premiers[production].push_back(caractere);
        }
        else
        {
            if(!existe(productionsTraitees, caractere))
                premiers(donnees, production, caractere, productionsTraitees);
        }
    }
}

/**
 * @brief Pour chaque production, calcule ses suivants
 * @param Donnees& donnees : r?f?rence vers les donn?es
 * @param string courant : le nom du non terminal courant dont on veut calculer les suivants
 * @param string nonTerminal : ? chaque fois que pour une production on tombe sur un nonTerminal, on est oblig? de relancer la fonction pour trouver les "sous" terminaux
 * @return void
*/

void suivants(Donnees& donnees, string courant, string nonTerminal)
{
    donnees.suivants[courant].push_back("$");

    cout << "-------------- " << courant << endl;

    for(auto productions = donnees.grammaire.begin(); productions != donnees.grammaire.end(); productions++){
        if(productions->first != courant){
            for(auto production = productions->second.begin(); production != productions->second.end(); production++){
                //Dans chaque prodution, on cherche le caract?re courant
                cout << "------- " << *production << endl;

                for(auto caractereProdChar = production->begin(); caractereProdChar != production->end(); caractereProdChar++){
                    string caractere = var2String(*caractereProdChar);

                    //Si dans une production, on a trouv? le caract?re courant, alors, on va appliquer les r?gles 1, 2 et 3
                    if(caractere == courant){
                        auto caractereProdCharTmp = caractereProdChar;
                        caractereProdCharTmp++;

                        cout << "--- " << caractere << endl;

                        //A -> aBb : SUIVANT(B) += PREMIER(b)
                        //A -> aBb : SUIVANT(B) += SUIVANT(A) SI # dans PREMIER(b)
                        if(caractereProdCharTmp != production->end()){
                            string caractereProdSuivants = var2String(*caractereProdCharTmp);

                            cout << "--------------------------" << caractereProdSuivants << endl;

                            //A -> aBb : SUIVANT(B) += PREMIER(b)
                            if(existe(donnees.nonTerminaux,caractereProdSuivants))
                                assemblerSansDoublons(donnees.suivants[courant], donnees.premiers[caractereProdSuivants]);
                            else
                                assemblerSansDoublons(donnees.suivants[courant], caractereProdSuivants);

                            //A -> aBb : SUIVANT(B) += SUIVANT(A) SI # dans PREMIER(b)
                            if(existe(donnees.premiers[caractereProdSuivants], var2String("#")))
                                assemblerSansDoublons(donnees.suivants[courant], donnees.suivants[productions->first]);

                            /*//On parcourt les caract?res suivants
                            //Tant que ce sont des non terminaux
                            //Tant que le non terminal pr?c?dent contient #

                            for(;caractereProdSuivantsChar != production->end(); caractereProdSuivantsChar++){
                                string caractereProdSuivants = var2String(*caractereProdSuivantsChar);

                                if(existe(donnees.nonTerminaux, caractereProdSuivants)){

                                }
                                else{
                                    break;
                                }
                            }*/
                        }
                        //A -> aB  : SUIVANT(B) += SUIVANT(A)
                        else{
                            string suivantTmp = "W" + var2String(productions->first);
                            donnees.suivants[courant].push_back(suivantTmp);
                        }
                    }
                }
            }
        }
    }
}

void tableAnalyse(Donnees& donnees)
{

}

void reconnaissance(Donnees& donnees)
{

}

void afficherGrammaire(Donnees& donnees)
{
    for(auto i : donnees.grammaire)
    {
        cout << i.first << "=";

        for(auto j : i.second)
        {
            cout << j << "|";
        }

        cout << endl;
    }

    cout << endl << endl;
}

void afficherPremiers(Donnees& donnees)
{
    for(auto i : donnees.premiers)
    {
        cout << i.first << ": ";

        for(auto j : i.second)
        {
            cout << j << ",";
        }

        cout << endl;
    }
}

void afficherSuivants(Donnees& donnees)
{
    for(auto i : donnees.suivants)
    {
        cout << i.first << ": ";

        for(auto j : i.second)
        {
            if(j != "#")
                cout << j << " ";
        }

        cout << endl;
    }
}

string trouverNomNonTerminal(Donnees& donnees)
{
    vector<string> nonTerminauxNom = {
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"
    };

    for(auto i : nonTerminauxNom)
    {
        if(!existe(donnees.grammaire, i))
        {
            return i;
        }
    }

    return "A";
}
