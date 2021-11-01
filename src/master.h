#ifndef MASTER_H
#define MASTER_H

#include "data.h"
#include <ilcplex/ilocplex.h>

class Master {

    private:
        IloEnv env;
        IloModel model;

        IloNumVarArray lambda;

        IloExpr obj; // Objective function
        IloRangeArray constraints; // Constraints

        std::vector <std::vector <bool>> itemExists;

        Data data;
        double upperBound;

    public:
        Master (Data &data, double upperBound);
        void solve ();

};

#endif