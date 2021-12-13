#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <utility> 
#include <vector>

class Node {

    public:

        // Atributos
        std::vector <std::pair<int, int>> exclude;
        std::vector <std::pair<int, int>> enforce;

        bool feasible;

        // Metodos
        std::pair <int, int> getMostFractionalPair ();

};

#endif