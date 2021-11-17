#include "subproblem.h"
#include "minknap.c"

using namespace std;

Subproblem::Subproblem (Data &data, IloNumArray &duals) {

    this->env = IloEnv();
    this->model = IloModel(this->env);

    // Pricing problem
    this->env.setName("Knapsack Problem");
    this->model.setName("Subproblem");

    this->x = IloBoolVarArray(this->env, numberItems); // Variables

    this->obj = IloExpr(env); // Objective function

    // Writing objective function
    for (int i = 0; i < data.getNItems(); i++) obj -= duals[i] * x[i];
    this->model.add(IloMinimize(this->env, this->obj)); // Minimize objective function

    this->constraint = IloExpr(env); // Constraint
    
    // Writing constraint
    for (int i = 0; i < data.getNItems(); i++) this->constraint += data.getItemWeight(i) * x[i];
    this->model.add(this->constraint <= data.getBinCapacity()); // Adds constraint to model

}

void Subproblem::solve (Data &data, IloNumArray &duals, vector <int> &column) {

    double objectiveValue;

    if (MIP) {

        IloCplex subproblem(this->model);
        subproblem.solve(); // Adicionar exception

        objectiveValue = subproblem.getObjValue();

        // Verifies if reduced cost is negative
        if (1 + objectiveValue < 0) {

            // Gets column
            IloNumArray results(this->env, data.getNItems());
            subproblem.getValues(results);

            for (int i = 0; i < data.getNItems(); i++) {  
                if (results[i] >= 0.9) column[i] = 1;
                else column[i] = 0;
            }

            return objectiveValue;
        }

    } else {

        int results[data.getNItems()];
        int profit[data.getNItems()];
        int weight[data.getNItems()];

        for (int i = 0; i < data.getNItems(); i++) {

            profit[i] = duals[i];
            weight[i] = data.getItemWeight(i);
        }

        objectiveValue = minknap(data.getNItems(), profit, weight, results, data.getBinCapacity());

        // Falta pegar resultados e adicionar coluna

    }

}