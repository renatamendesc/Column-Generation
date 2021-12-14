#ifndef NODE_H
#define NODE_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <utility> 
#include <vector>

class Node {

    public:

        // Atributos
        std::vector <double> solution;
        std::vector <std::vector <bool>> columns;

        std::vector <std::pair<int, int>> exclude;
        std::vector <std::pair<int, int>> enforce;

        bool feasible;

        // Metodos
        void updateNode (std::vector <double> &solution, std::vector <std::vector <bool>> &A);
        std::pair <int, int> getMostFractionalPair ();

};

#endif