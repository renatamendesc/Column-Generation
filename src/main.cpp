#include "data.h"
#include <iostream>
#include <cstdio>
#include <ilcplex/ilocplex.h>

using namespace std;

void problem (Data * data, double upperBound) {

    IloEnv env;
    IloModel master(env);

    env.setName("Bin Packing Problem");
    master.setName("Master Problem");

    int numberItems = data->getNItems();

    IloNumVarArray lambda(env, numberItems, 0, IloInfinity); // Variables
    IloExpr obj(env); // Objective function

    vector <vector <bool>> itemExists (numberItems, vector <bool> (numberItems, false)); // Matriz que indica se o item existe no pacote em questão

    for (int i = 0; i < numberItems; i++) {

        char name[100];

        // Adds variables to model
        sprintf(name, "lambda(%d)", i);
        lambda[i].setName(name);
        master.add(lambda[i]);

        obj += lambda[i]; // Adds to objective function

        IloRange constraint;

        sprintf(name, "c%d", i);
        constraint.setName(name);

        constraint = (lambda[i] == 1); // Adds to constraint
        master.add(constraint);

        itemsExists[i][i] = true;

    }

    master.add(IloMinimize(env, obj));

}

int main (int argc, char **argv) {

    Data data;
    data.readData(argv[1]);

    cout << "Number of items: " << data.getNItems() << endl << endl;

    for (int i = 0; i < data.getNItems(); i++) cout << "Weight " << i+1 << ": " << data.getItemWeight(i) << endl;

    cout << endl << "Bin capacity: " << data.getBinCapacity() << endl;

    return 0;

}