#include "subproblem.h"

using namespace std;

Subproblem::Subproblem (Data &data, IloNumArray &duals) {

    this->env = IloEnv();
    this->model = IloModel(this->env);

    // Pricing problem
    this->env.setName("Knapsack Problem");
    this->model.setName("Subproblem");

    this->numberItems = data.getNItems();
    this->x = IloBoolVarArray(this->env, numberItems); // Variables

    this->obj = IloExpr(env); // Objective function

    // Writing objective function
    for (int i = 0; i < this->numberItems; i++) obj -= duals[i] * x[i];
    this->model.add(IloMinimize(this->env, this->obj)); // Minimize objective function

    this->constraint = IloExpr(env); // Constraint
    
    // Writing constraint
    for (int i = 0; i < this->numberItems; i++) this->constraint += data.getItemWeight(i) * x[i];
    this->model.add(this->constraint <= data.getBinCapacity()); // Adds constraint to model

}

void Subproblem::solve () {

    IloCplex subproblem(this->model);

    subproblem.solve(); // Adicionar exception

    // Verifies if reduced cost is negative
    if (1 + subproblem.getObjValue() < 0) {

        // Adds column
        IloNumArray results(this->env, this->numberItems);
        subproblem.getValues(results);

        for (int i = 0; i < this->numberItems; i++) {  
            if (results[i] >= 0.9) results[i] = 1;
            else results[i] = 0;
        }

        // Creates new variable
        // IloNumVar
        
        // Updates matrix

    }

}

// IloModel Subproblem::getModel() {

//     return this->model;
// }