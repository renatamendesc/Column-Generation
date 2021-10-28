#include "master.h"

using namespace std;

Master::Master (Data &data, double upperBound) {

    this->upperBound = upperBound;

    this->env = IloEnv();
    this->model = IloModel(this->env);

    this->env.setName("Bin Packing Problem");
    this->model.setName("Master Problem");

    int numberItems = data.getNItems();

    this->lambda = IloNumVarArray(this->env, numberItems, 0, IloInfinity); // Variables
    this->obj = IloExpr(env); // Objective function

    this->itemExists = vector <vector <bool>>(numberItems, vector <bool> (numberItems, false)); // Matriz que indica se o item existe no pacote em questão

    for (int i = 0; i < numberItems; i++) {

        char name[100];

        // Adds variables to model
        sprintf(name, "lambda(%d)", i);
        this->lambda[i].setName(name);
        this->model.add(this->lambda[i]);

        this->obj += this->lambda[i]; // Adds to objective function

        IloRange constraint;

        sprintf(name, "c%d", i);
        constraint.setName(name);

        constraint = (this->lambda[i] == 1); // Adds to constraint
        this->model.add(constraint);

        this->itemExists[i][i] = true;

    }

    this->model.add(IloMinimize(env, obj));
}

void Master::solve () {

    IloCplex master(this->model);

    // Criar o subproblema

}