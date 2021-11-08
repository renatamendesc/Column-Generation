#include "subproblem.h"

using namespace std;

Subproblem::Subproblem (Data &data, IloNumArray &duals) {

    this->env = IloEnv();
    this->model = IloModel(this->env);

    // Pricing problem
    this->env.setName("Knapsack Problem");
    this->model.setName("Subproblem");

    int numberItems = data.getNItems();
    this->x = IloBoolVarArray(this->env, numberItems); // Variables

    this->obj = IloExpr(env); // Objective function

    // Writing objective function
    for (int i = 0; i < numberItems; i++) obj -= duals[i] * x[i];
    this->model.add(IloMinimize(this->env, this->obj)); // Minimize objective function

    this->constraint = IloExpr(env); // Constraint
    
    // Writing constraint
    for (int i = 0; i < numberItems; i++) this->constraint += data.getItemWeight(i) * x[i];
    this->model.add(this->constraint <= data.getBinCapacity()); // Adds constraint to model

}

void Subproblem::solve () {

    IloCplex subproblem(this->model);

    subproblem.solve();

}