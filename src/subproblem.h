#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include "data.h"
#include <ilcplex/ilocplex.h>

class Subproblem {

    private:
        IloEnv env;
        IloModel model;

        IloBoolVarArray x;
        IloExpr obj;

    public:
        Subproblem (Data &data);
        void solve ();

};

#endif