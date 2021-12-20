#ifndef NODE_H
#define NODE_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <utility> 
#include <vector>

class Node {

    public:

        // Atributos
        double lowerBound = __DBL_MAX__;

        std::vector <double> solution;
        std::vector <std::vector <bool>> columns;

        std::vector <std::pair<int, int>> exclude;
        std::vector <std::pair<int, int>> enforce;

        bool feasible = false;
        bool prune = false;

        // Metodos
        std::pair <int, int> getMostFractionalPair ();
        void updateNode (std::vector <double> &solution, std::vector <std::vector <bool>> &A, double lowerBound);

        void verifyFeasibleColumn (std::vector <bool> &column);

        void enforcePair (IloModel &pricing, IloBoolVarArray &x, std::vector <std::vector <bool>> &A, IloNumVarArray &lambda);
        void excludePair (IloModel &pricing, IloBoolVarArray &x, std::vector <std::vector <bool>> &A, IloNumVarArray &lambda);

};

#endif