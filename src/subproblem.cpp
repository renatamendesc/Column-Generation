#include "subproblem.h"
#include "minknap.c"

using namespace std;

Subproblem::Subproblem (Data &data) {

    this->env = IloEnv();
    this->model = IloModel(this->env);

    // Pricing problem
    this->env.setName("Knapsack Problem");
    this->model.setName("Subproblem");

    this->x = IloBoolVarArray(this->env, data.getNItems()); // Variables

    this->constraint = IloExpr(env); // Constraint
    
    // Writing constraint
    for (int i = 0; i < data.getNItems(); i++) this->constraint += data.getItemWeight(i) * x[i];
    this->model.add(this->constraint <= data.getBinCapacity()); // Adds constraint to model

    this->objective = IloMinimize(this->env);
    this->model.add(this->objective); // Minimize objective function

}

void Subproblem::addObjectiveFunction (Data &data, IloNumArray &duals) {

    IloExpr objectiveFunction (this->env);

    // Writing objective function
    for (int i = 0; i < data.getNItems(); i++) objectiveFunction -= duals[i] * x[i];

    this->objective.setExpr(objectiveFunction);
    
}

double Subproblem::solve (Data &data, Node &node, IloNumArray &duals, vector <bool> &column) {

    double objectiveValue;

    if (CPLEX) {

        IloCplex subproblem(this->model);

        subproblem.setParam(IloCplex::Param::TimeLimit, 60);
        subproblem.setParam(IloCplex::Param::Threads, 1);
        subproblem.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1e-08);
        subproblem.setOut(this->env.getNullStream());

        try {
            subproblem.solve(); // Try to solve subproblem
        } catch (IloException &e) {
            cerr << e << endl;
        }

        if (subproblem.getCplexStatus() == IloCplex::Infeasible) {
            node.prune = true;
        }

        objectiveValue = 1 + subproblem.getObjValue();

        // Gets column
        for (int i = 0; i < data.getNItems(); i++) {
            if (subproblem.getValue(this->x[i]) >= 0.9) column[i] = true;
            else column[i] = false;

            //cout << "Coluna " << i << ": " << column[i] << endl;
        }

        subproblem.clear();
        subproblem.end();

    } else {

        int results[data.getNItems()];
        int profit[data.getNItems()];
        int weight[data.getNItems()];

        for (int i = 0; i < data.getNItems(); i++) {
            profit[i] = duals[i] * 1e6;
            weight[i] = data.getItemWeight(i);
        }

        objectiveValue = 1 -(minknap(data.getNItems(), profit, weight, results, data.getBinCapacity())) * 1e-6;

        // Gets column
        for (int i = 0; i < data.getNItems(); i++) {
            if (results[i] >= 0.9) column[i] = true;
            else column[i] = false;
        }

    }

    // cout << "Pricing " << objectiveValue << endl;

    return objectiveValue;

}