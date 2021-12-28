#include "node.h"

using namespace std;

pair <int, int> Node::getMostFractionalPair () {

    pair <int, int> fractionalPair;
    double delta, minDelta = __DBL_MAX__;

    int items = this->columns[0].size();
    int bins = this->columns.size();

    vector <vector <double>> pairValues (items, vector <double> (items, 0));

    for (int k = 0; k < bins; k++) { // percorre pacote k

        vector <int> insideBin;

        for (int i = 0; i < items; i++) {
            if (this->columns[k][i] == 1) insideBin.push_back(i); // verifica itens existentes no pacote k
        }

        for (int i = 0; i < insideBin.size()-1; i++) {
            for (int j = i + 1; j < insideBin.size(); j++) {
                pairValues[insideBin[i]][insideBin[j]] += this->solution[k];
            }
        }
    }

    for (int i = 0; i < items-1; i++) {
        for (int j = i + 1; j < items; j++) {

            delta = abs(pairValues[i][j] - 0.5);

            if (delta < minDelta) {
                minDelta = delta;
                fractionalPair = {i, j};
            }
        }
    }

    return fractionalPair;
}

void Node::updateNode (IloNumArray &solution, vector <vector <bool>> &A, double lowerBound) {

    this->lowerBound = lowerBound;
    this->solution = solution;
    this->columns = A;

    if (this->lowerBound - (int)this->lowerBound == 0) { // Verifica se solucao eh inteira
        this->feasible = true;
    } else {
        this->feasible = false;
    }
}

void Node::verifyFeasibleColumn (vector <bool> &column) {

    for (int i = 0; i < this->enforce.size(); i++) {
        if ((column[this->enforce[i].first] == false && column[this->enforce[i].second] == true) || (column[this->enforce[i].first] == true && column[this->enforce[i].second] == false)) {
            this->prune = true;
        }
    }

    for (int i = 0; i < this->exclude.size(); i++) {
        if (column[this->exclude[i].first] == true && column[this->exclude[i].second] == true) {
            this->prune = true;
        }
    }
}

void Node::enforcePair (IloModel &pricing, IloBoolVarArray &x, vector <vector <bool>> &A, IloNumVarArray &lambda) {

    for (int i = 0; i < this->enforce.size(); i++) {

        pricing.add(x[this->enforce[i].first] == x[this->enforce[i].second]); // Adds constraint

        for (int j = 0; j < A.size(); j++) {
            if ((A[j][this->enforce[i].first] == false && A[j][this->enforce[i].second] == true) || (A[j][this->enforce[i].first] == true && A[j][this->enforce[i].second] == false))
                lambda[j].setUB(0);
        }
    }
}

void Node::excludePair (IloModel &pricing, IloBoolVarArray &x, vector <vector <bool>> &A, IloNumVarArray &lambda) {

    for (int i = 0; i < this->exclude.size(); i++) {

        pricing.add(x[this->exclude[i].first] + x[this->exclude[i].second] <= 1); // Adds constraint

        for (int j = 0; j < A.size(); j++) {
            if (A[j][this->exclude[i].first] == true && A[j][this->exclude[i].second] == true) lambda[j].setUB(0);
        }
    }
}

void Node::printNode (double time) {

    if (this->feasible) {

        cout << endl << "Cost: " << this->lowerBound << " bins" << endl;
        cout << "Time: " << time << endl;

    } else {

        cout << "No feasible solution was found!" << endl; 
    }
}