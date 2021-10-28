#include "subproblem.h"

using namespace std;

Subproblem::Subproblem (Data &data) {

    this->env = IloEnv();
    this->model = IloModel(this->env);

    this->env.setName("Knapsack Problem");
    this->model.setName("Subproblem");

    int numberItems = data.getNItems();

    this->x = IloBoolVarArray(this->env, numberItems); // Variables
    this->obj = IloExpr(env); // Objective function
    
    // Continuar com funcao objetivo e restricoes

}

void Subproblem::solve () {

}