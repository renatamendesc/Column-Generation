#include "master.h"
#include "subproblem.h"

using namespace std;

Master::Master (Data &data) {

    this->data = data;

    this->env = IloEnv();
    this->model = IloModel(this->env);

    this->env.setName("Bin Packing Problem");
    this->model.setName("Master Problem");

    this->lambda = IloNumVarArray(this->env, this->data.getNItems(), 0, IloInfinity); // Variables
    this->objectiveFunction = IloExpr(this->env); // Objective function
    this->constraints = IloRangeArray(this->env, this->data.getNItems());

    this->A = vector <vector <bool>>(this->data.getNItems(), vector <bool> (this->data.getNItems(), false)); // Matriz que indica se o item existe no pacote em questão

    for (int i = 0; i < this->data.getNItems(); i++) {

        char name[100];

        // Adds variables to model
        sprintf(name, "lambda(%d)", i);
        this->lambda[i].setName(name);
        this->model.add(this->lambda[i]);

        this->objectiveFunction += this->lambda[i]; // Adds to objective function

        // Creates constraints
        sprintf(name, "c%d", i);
        this->constraints[i] = (this->lambda[i] == 1); // Adds to constraint
        this->constraints[i].setName(name);
        
        this->A[i][i] = true;
    }

    this->model.add(constraints);
    this->obj = IloMinimize(this->env, this->objectiveFunction);
    this->model.add(this->obj);
}

void Master::solve (Node &node) {

    IloCplex master(this->model);

    master.setParam(IloCplex::Param::TimeLimit, 60);
    master.setParam(IloCplex::Param::Threads, 1);
    master.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1e-08);
    master.setOut(this->env.getNullStream());

    // Creates subproblem
    Subproblem subproblem(this->data);

    // Enforce and exclude nodes
    node.enforcePair(subproblem.model, subproblem.x, this->A, this->lambda);
    node.excludePair(subproblem.model, subproblem.x, this->A, this->lambda);

    while (true) {

        try {
            master.solve(); // Try to solve master problem
        } catch (IloException &e) {
            cerr << e << endl;
        }

        if (master.getCplexStatus() == IloCplex::Infeasible) {
            node.prune = true;
            break;
        }

        // cout << "Objective value: " << master.getObjValue() << endl;

        // Gets dual variables
        IloNumArray duals(this->env, this->data.getNItems());
        for (int i = 0; i < this->data.getNItems(); i++) duals[i] = master.getDual(this->constraints[i]);
        
        subproblem.addObjectiveFunction(this->data, duals);

        vector <bool> col(this->data.getNItems()); // Vector with column

        double reducedCost = subproblem.solve(this->data, node, duals, col);

        duals.clear();
        duals.end();

        node.verifyFeasibleColumn(col); // Verifica se gerou coluna inviavel

        if (reducedCost >= -0.000001 || node.prune) break; // Encerra geração de colunas

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

        // cout << "GEROU COLUNA..." << endl << endl;

    }

    // Atualiza node
    if (!node.prune) {

        // cout << "Colunas geradas: " << this->lambda.getSize() - this->data.getNItems() << endl;

        IloNumArray solution (this->env, this->lambda.getSize());
        master.getValues(solution, this->lambda);

        node.updateNode(solution, this->A, master.getObjValue());
    }

    for (int i = 0; i < this->lambda.getSize(); i++) lambda[i].setUB(IloInfinity); // Reverter upper bounds

    master.clear();
    master.end();

}