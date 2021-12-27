#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include "data.h"
#include "node.h"
#include <vector>
#include <ilcplex/ilocplex.h>

#define CPLEX true

class Subproblem {

    public:
        // Atributos
        IloEnv env;
        IloModel model;

        IloBoolVarArray x;

        IloObjective objective;

        IloExpr constraint;

        // Metodos
        Subproblem (Data &data);
        void addObjectiveFunction (Data &data, IloNumArray &duals);
        double solve (Data &data, Node &node, IloNumArray &duals, std::vector <bool> &column);

};

#endif