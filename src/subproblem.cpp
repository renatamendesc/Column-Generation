#include "subproblem.h"

using namespace std;

Subproblem::Subproblem (Data &data) {

    this->env = IloEnv();
    this->model = IloModel(this->env);

    // Pricing problem
    this->env.setName("Knapsack Problem");
    this->model.setName("Subproblem");

    int numberItems = data.getNItems();

    this->x = IloBoolVarArray(this->env, numberItems); // Variables
    this->constraint = IloExpr(env); // Constraint

    // Falta função objetivo
    
    // Writing constraint
    for (int i = 0; i < numberItems; i++) this->constraint += data.getItemWeight(i) * x[i];

    this->model.add(this->constraint <= data.getBinCapacity()); // Adds constraint to model
    this->model.add(IloMinimize(this->env)); // Minimize objective function

}

void Subproblem::solve () {

}