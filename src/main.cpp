#include "data.h"
#include "master.h"
#include <iostream>
#include <cstdio>
#include <ilcplex/ilocplex.h>

using namespace std;

// void solve (Data * data, double upperBound) {

//     ILoEnv env;
//     IloModel model;

//     env.setName("Bin Packing Problem");
//     model.setName("Master Problem");

//     int numberItems = data->getNItems();

//     IloNumVarArray lambda(this->end, numberItems, 0, IloInfinity); // Variables
//     IloExpr obj(env); // Objective function

//     itemExists = vector <vector <bool>>(numberItems, vector <bool> (numberItems, false)); // Matriz que indica se o item existe no pacote em questão

//     for (int i = 0; i < numberItems; i++) {

//         char name[100];

//         // Adds variables to model
//         sprintf(name, "lambda(%d)", i);
//         lambda[i].setName(name);
//         model.add(lambda[i]);

//         obj += this->lambda[i]; // Adds to objective function

//         IloRange constraint;

//         sprintf(name, "c%d", i);
//         constraint.setName(name, );

//         constraint = (lambda[i] == 1); // Adds to constraint
//         model.add(constraint);

//         itemExists[i][i] = true;

//     }

//     model.add(IloMinimize(env, obj));

// }

int main (int argc, char **argv) {

    Data data;
    data.readData(argv[1]);

    // cout << "Number of items: " << data.getNItems() << endl << endl;
    // for (int i = 0; i < data.getNItems(); i++) cout << "Weight " << i+1 << ": " << data.getItemWeight(i) << endl;
    // cout << endl << "Bin capacity: " << data.getBinCapacity() << endl;

    // problem(data, __DBL_MAX__);

    Master master(data, __DBL_MAX__);

    return 0;
}