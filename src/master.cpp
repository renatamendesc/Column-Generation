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

    this->lambda = IloNumVarArray(this->env, this->data.getNItems(), 0, IloInfinity); // Variables
    this->objExpression = IloExpr(this->env); // Objective function

    this->A = vector <vector <bool>>(this->data.getNItems(), vector <bool> (this->data.getNItems(), false)); // Matriz que indica se o item existe no pacote em questão

    for (int i = 0; i < this->data.getNItems(); i++) {

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

void Master::solve (Node &node) {

    IloCplex master(this->model);
    master.setOut(this->env.getNullStream());

    try {
        master.solve(); // Try to solve master problem
    } catch (IloException &e) {
        cerr << e << endl;
    }


    while (true) {

        // if (master.getCplexStatus() == IloCplex::Infeasible) break;

        // Gets dual variables
        IloNumArray duals(this->env, this->data.getNItems());

        for (int i = 0; i < this->data.getNItems(); i++) duals[i] = master.getDual(this->constraints[i]);

        // Creates subproblem
        vector <bool> col(this->data.getNItems());
        Subproblem subproblem(this->data, duals);

        if (subproblem.solve(this->data, node, duals, col, this->A, this->lambda)) {

            // adiconar break se col tiver tamanho 0

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

        } else {
            break;
        }

        duals.clear();
        duals.end();

        // Apagar subproblema

        try {
            master.solve(); // Try to solve master problem
        } catch (IloException &e) {
            cerr << e << endl;
        }

    }

    // obs -> talvez essa parte esteja errada
    vector <double> solution (this->lambda.getSize())

    for (int i = 0; i < data.getNItems(); i++) solution[i] = master.getValue(this->lambda[i]);

    // Atualiza node
    node.updateNode(solution, this->A);

}