#include "data.h"
#include "master.h"
#include "node.h"
#include <iostream>
#include <cstdio>
#include <ilcplex/ilocplex.h>

using namespace std;

void search (Master &master) {

    Node root;

    vector <Node> tree;
    tree.push_back(root);

    while (!tree.empty()) {

        Node node;

        // Depth search
        node = tree.back();
		tree.erase(tree.end());

        master.solve(Node &node);

        if (!node.feasible) { // Branching

            pair <int, int> branching = node.getMostFractionalPair();

            Node newNodeEnforce, newNodeExclude;

            newNodeEnforce.exclude = node.exclude;
            newNodeEnforce.enforce = node.enforce;
            newNodeEnforce.enforce.push_back(branching);

            newNodeExclude.exclude = node.exclude;
            newNodeExclude.enforce = node.enforce;
            newNodeExclude.exclude.push_back(branching);

            tree.push_back(newNodeEnforce);
            tree.push_back(newNodeExclude);

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