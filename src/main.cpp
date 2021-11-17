#include "data.h"
#include "master.h"
#include <iostream>
#include <cstdio>
#include <ilcplex/ilocplex.h>

using namespace std;

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