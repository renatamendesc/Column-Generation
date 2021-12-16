#include "node.h"

using namespace std;

pair <int, int> Node::getMostFractionalPair () {

    pair <int, int> fractionalPair;
    double delta, minDelta = __DBL_MAX__;

    int items = this->columns[0].size();

    vector <vector <double>> pairValues (items, vector <double> (items, 0));

    for (int k = 0; k < this->solution.size(); k++) { // percorre pacote k

        vector <int> index;

        for (int i = 0; i < items; i++) {
            if (this->columns[k][i] == 1) index.push_back(i); // verifica itens existentes no pacote k
        }

        for (int i = 0; i < index.size()-1; i++) {
            for (int j = i + 1; j < index.size()-1; j++) {
                pairValues[index[i]][index[j]] += this->solution[k];
                // pairValues[index[j]][index[i]] += this->solution[k];
            }
        }
    }

    for (int i = 0; i < items-1; i++) {
        for (int j = i + 1; j < items; j++) {

            delta = abs(0.5 - pairValues[i][j]);

            if (delta < minDelta) {
                minDelta = delta;
                fractionalPair = {i, j};
            }
        }
    }

    return fractionalPair;
}

void Node::updateNode (vector <double> &solution, vector <vector <bool>> &A) {

    this->solution = solution;
    this->columns = A;

}

void Node::enforcePair (IloModel &pricing, IloBoolVarArray &x, vector <vector <bool>> &A, IloNumVarArray &lambda) {

    for (int i = 0; i < this->enforce.size(); i++) {

        pricing.add(x[this->enforce[i].first] == x[this->enforce[i].second]); // Adds constraint

        for (int j = 0; j < A.size(); j++) {
            if ((A[j][x[this->enforce[i].first]] == false && A[j][x[this->enforce[i].second]] == true) || (A[j][x[this->enforce[i].first]] == true && A[j][x[this->enforce[i].second]] == false))
                lambda[j].setUB(0);
        }
    }
}

void Node::excludePair (IloModel &pricing, IloBoolVarArray &x, vector <vector <bool>> &A, IloNumVarArray &lambda) {

    for (int i = 0; i < this->enforce.size(); i++) {

        pricing.add(x[this->enforce[i].first] + x[this->enforce[i].second] <= 1); // Adds constraint

        for (int j = 0; j < A.size(); j++) {
            if (A[j][x[this->enforce[i].first]] == true && A[j][x[this->enforce[i].second]] == true) lambda[j].setUB(0);
        }
    }
}