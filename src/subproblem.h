#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include "data.h"
#include <vector>
#include <ilcplex/ilocplex.h>

#define MIP true

class Subproblem {

    private:
        IloEnv env;
        IloModel model;

        IloBoolVarArray x;

        IloExpr obj;
        IloExpr constraint;

    public:
        Subproblem (Data &data, IloNumArray &duals);
        double solve (Data &data, IloNumArray &duals, std::vector <bool> &column);

};

#endif