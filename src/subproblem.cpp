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

}

void Subproblem::addObjectiveFunction (Data &data, IloNumArray &duals) {

    this->objectiveFunction = IloExpr(env); // Objective function

    // Writing objective function
    for (int i = 0; i < data.getNItems(); i++) this->objectiveFunction -= duals[i] * x[i];
    this->model.add(IloMinimize(this->env, this->objectiveFunction)); // Minimize objective function

}

bool Subproblem::solve (Data &data, IloNumArray &duals, vector <bool> &column) {

    double objectiveValue;
    bool flag = false;

    if (MIP) {

        IloCplex subproblem(this->model);
        subproblem.setOut(this->env.getNullStream());

        try {
            subproblem.solve(); // Try to solve subproblem
        } catch (IloException &e) {
            cerr << e << endl;
        }

        // podar se for inviavel

        objectiveValue = 1 + subproblem.getObjValue();

        // Verifies if reduced cost is negative (gerar coluna)
        if (objectiveValue < 0) {
    
            // IloNumArray results(this->env, data.getNItems());
            // subproblem.getValues(results);
            flag = true;

            // Gets column
            for (int i = 0; i < data.getNItems(); i++) {

                if (subproblem.getValue(this->x[i]) > 0.9) {

                    // this->x[i] = true;
                    column[i] = true;

                } else {
                    
                    // this->x[i] = false;
                    column[i] = false;
                } 
            } 
        }

        subproblem.clear();
        subproblem.end();

    } else {

        int results[data.getNItems()];
        int profit[data.getNItems()];
        int weight[data.getNItems()];

        for (int i = 0; i < data.getNItems(); i++) {
            profit[i] = duals[i] * 1000000;
            weight[i] = data.getItemWeight(i);
        }

        objectiveValue = 1 + minknap(data.getNItems(), profit, weight, results, data.getBinCapacity()) / 1000000;

        if (objectiveValue < 0) {

            // flag = true;

            // Gets column
            for (int i = 0; i < data.getNItems(); i++) {
                if (results[i] > 0.9) column[i] = true;
                else column[i] = false;
            }
        }
    }

    this->model.end();
    this->env.end();

    return objectiveValue;

}