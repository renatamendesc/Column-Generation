#include "master.h"
#include "subproblem.h"

using namespace std;

Master::Master (Data &data, double upperBound) {

    this->data = data;
    this->upperBound = upperBound;

    this->env = IloEnv();
    this->model = IloModel(this->env);

    this->env.setName("Bin Packing Problem");
    this->model.setName("Master Problem");

    int numberItems = data.getNItems();

    this->lambda = IloNumVarArray(this->env, numberItems, 0, IloInfinity); // Variables
    this->objExpression = IloExpr(this->env); // Objective function

    this->A = vector <vector <bool>>(numberItems, vector <bool> (numberItems, false)); // Matriz que indica se o item existe no pacote em questão

    for (int i = 0; i < numberItems; i++) {

        char name[100];

        // Adds variables to model
        sprintf(name, "lambda(%d)", i);
        this->lambda[i].setName(name);
        this->model.add(this->lambda[i]);

        this->objExpression += this->lambda[i]; // Adds to objective function

        sprintf(name, "c%d", i);
        this->constraints[i].setName(name);

        this->constraints[i] = (this->lambda[i] == 1); // Adds to constraint

        this->A[i][i] = true;
    }

    this->model.add(constraints);
    this->obj = IloMinimize(this->env, this->objExpression);
    this->model.add(this->obj);
}

void Master::solve () {

    IloCplex master(this->model);

    master.solve(); // Depois adicionar exception

    // Gets dual variables
    IloNumArray duals(this->env, this->data.getNItems());

    for (int i = 0; i < this->data.getNItems(); i++) duals[i] = master.getDual(this->constraints[i]);

    vector <bool> col(this->data.getNItems());
    Subproblem subproblem(data, duals);
    subproblem.solve(data, duals, col);

    // Creates column
    IloNumColumn column = this->obj(1);
    for (int i = 0; i < data.getNItems(); i++) column += this->constraints[i](col[i]);
    
    IloNumVar var(column, 0, IloInfinity);

    // Sets name
    char name[100];
    sprintf(name, "lambda(%d)", this->A.size());
    var.setName(name);

    // Adds variable
    lambda.add(var);
    this->A.push_back(col);

}