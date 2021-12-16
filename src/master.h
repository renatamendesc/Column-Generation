#ifndef MASTER_H
#define MASTER_H

#include "data.h"
#include "node.h"
#include <vector>
#include <ilcplex/ilocplex.h>

class Master {

    private:
        IloEnv env;
        IloModel model;

        IloNumVarArray lambda;

        IloObjective obj;
        IloExpr objectiveFunction; // Objective function
        IloRangeArray constraints; // Constraints

        std::vector <std::vector <bool>> A;

        Data data;

    public:
        Master (Data &data);
        void solve (Node &node);

};

#endif