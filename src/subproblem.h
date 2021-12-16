#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include "data.h"
#include <vector>
#include <ilcplex/ilocplex.h>

#define MIP true

class Subproblem {

    public:
        // Atributos
        IloEnv env;
        IloModel model;

        IloBoolVarArray x;

        IloExpr objectiveFunction;
        IloExpr constraint;

        // Metodos
        Subproblem (Data &data);
        void addObjectiveFunction (Data &data, IloNumArray &duals);
        bool solve (Data &data, IloNumArray &duals, std::vector <bool> &column);

};

#endif