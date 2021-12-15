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

        IloExpr objExpression;
        IloExpr constraint;

    public:
        Subproblem (Data &data, IloNumArray &duals);
        bool solve (Data &data, Node &node, IloNumArray &duals, std::vector <bool> &column, std::vector <std::vector <bool>> &A, IloNumVarArray &lambda);

};

#endif