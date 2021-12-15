#include "subproblem.h"
#include "minknap.c"

using namespace std;

Subproblem::Subproblem (Data &data, IloNumArray &duals) {

    this->env = IloEnv();
    this->model = IloModel(this->env);

    // Pricing problem
    this->env.setName("Knapsack Problem");
    this->model.setName("Subproblem");

    this->x = IloBoolVarArray(this->env, data.getNItems()); // Variables

    this->objExpression = IloExpr(env); // Objective function

    // Writing objective function
    for (int i = 0; i < data.getNItems(); i++) this->objExpression -= duals[i] * x[i];
    this->model.add(IloMinimize(this->env, this->objExpression)); // Minimize objective function

    this->constraint = IloExpr(env); // Constraint
    
    // Writing constraint
    for (int i = 0; i < data.getNItems(); i++) this->constraint += data.getItemWeight(i) * x[i];
    this->model.add(this->constraint <= data.getBinCapacity()); // Adds constraint to model

}

bool Subproblem::solve (Data &data, Node &node, IloNumArray &duals, vector <bool> &column, vector <vector <bool>> &A, IloNumVarArray &lambda) {

    double objectiveValue;
    bool flag = false;

    node.enforcePair(this->model, this->x, A, lambda);
    node.excludePair(this->model, this->x, A, lambda);

    if (MIP) {

        IloCplex subproblem(this->model);
        subproblem.setOut(this->env.getNullStream());

        try {
            subproblem.solve(); // Try to solve subproblem
        } catch (IloException &e) {
            cerr << e << endl;
        }

        objectiveValue = subproblem.getObjValue();

        // Verifies if reduced cost is negative
        if (1 + objectiveValue < 0) {
    
            // IloNumArray results(this->env, data.getNItems());
            // subproblem.getValues(results);
            flag = true;

            // Gets column
            for (int i = 0; i < data.getNItems(); i++) {
                if (subproblem.getValue(this->x[i]) > 0.9) column[i] = true;
                else column[i] = false;
            } 
        }

        this->subproblem.clear();
        this->subproblem.end();

    } else {

        int results[data.getNItems()];
        int profit[data.getNItems()];
        int weight[data.getNItems()];

        for (int i = 0; i < data.getNItems(); i++) {
            profit[i] = duals[i] * 1000000;
            weight[i] = data.getItemWeight(i);
        }

        objectiveValue = minknap(data.getNItems(), profit, weight, results, data.getBinCapacity()) / 1000000;

        if (1 + objectiveValue < 0) {

            flag = true;

            // Gets column
            for (int i = 0; i < data.getNItems(); i++) {
                if (results[i] > 0.9) column[i] = true;
                else column[i] = false;
            }
        }
    }

    this->model.end();
    this->env.end();

    return flag;
}