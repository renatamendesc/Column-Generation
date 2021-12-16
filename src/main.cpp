#include "data.h"
#include "master.h"
#include "node.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <ilcplex/ilocplex.h>

using namespace std;

void search (Master &master) {

    clock_t start = clock(); // Starts time counting
    double time;

    Node root, bestNode;

    master.solve(root);
    bestNode = root;

    vector <Node> tree;
    tree.push_back(root);

    while (!tree.empty()) {

        Node node;

        // Depth search
        node = tree.back();
        tree.erase(tree.end());

        if (!node.feasible) { // Branching

            pair <int, int> branching = node.getMostFractionalPair();

            for (int i = 0; i < 2; i++) {

                Node newNode;

                newNode.exclude = node.exclude;
                newNode.enforce = node.enforce;

                if (i == 0) { // Enforce

                    newNode.enforce.push_back(branching);

                } else { // Exclude

                    newNode.exclude.push_back(branching);
                }

                master.solve(newNode);

                if (newNode.lowerBound < bestNode.lowerBound) {

                    tree.push_back(newNode);
                }
            }

        } else { // Verifies best node

            if (node.lowerBound < bestNode.lowerBound) {

                bestNode = node;
            }
        }

		clock_t end = clock();
		time = ((double) (end - start)) / CLOCKS_PER_SEC;
		// if(time > 600) break;
    }
}

int main (int argc, char **argv) {

    Data data;
    data.readData(argv[1]);

    // cout << "Number of items: " << data.getNItems() << endl << endl;
    // for (int i = 0; i < data.getNItems(); i++) cout << "Weight " << i+1 << ": " << data.getItemWeight(i) << endl;
    // cout << endl << "Bin capacity: " << data.getBinCapacity() << endl;

    Master master(data);
    search(master);

    return 0;
}