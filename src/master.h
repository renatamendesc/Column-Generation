#ifndef MASTER_H
#define MASTER_H

#include "data.h"
#include <vector>
#include <ilcplex/ilocplex.h>

class Master {

    private:
        IloEnv env;
        IloModel model;

        IloNumVarArray lambda;

        IloObjective obj;
        IloExpr objExpression; // Objective function
        IloRangeArray constraints; // Constraints

        std::vector <std::vector <bool>> A;

        Data data;
        double upperBound;

    public:
        Master (Data &data, double upperBound);
        void solve ();

};

#endif