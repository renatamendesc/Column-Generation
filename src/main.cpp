#include "data.h"
#include "master.h"
#include "node.h"
#include <iostream>
#include <cstdio>
#include <ilcplex/ilocplex.h>

using namespace std;

void search (Master &master) {

    Node root;

    master.solve(); // Column generation
    pair <int, int> branching = root.getMostFractionedPair();

    vector <Node> tree;

    while (!tree.empty()) {

        Node node;

        // Depth search
        node = tree.back();
		tree.erase(tree.end());

        if (!node.feasible) {

            // Branching

        } else {

            // Solucao viavel

        }
    }
}

int main (int argc, char **argv) {

    Data data;
    data.readData(argv[1]);

    // cout << "Number of items: " << data.getNItems() << endl << endl;
    // for (int i = 0; i < data.getNItems(); i++) cout << "Weight " << i+1 << ": " << data.getItemWeight(i) << endl;
    // cout << endl << "Bin capacity: " << data.getBinCapacity() << endl;

    Master master(data, __DBL_MAX__);
    search(master);

    return 0;
}