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

    // Objective function
    IloNumVarArray lambda(env, numberItems);
    IloExpr obj(env);

    char name[100];

    for (int i = 0; i < numberItems; i++) { // Adds variables to model

        sprintf(name, "lambda(%d)", i);
        lambda[i].setName(name);

        master.add(lambda[i]);

        // obj += lambda[i]; // Falta adicionar expressao e range das variaveis
    
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