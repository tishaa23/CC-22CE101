#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Function to find the longest common prefix
string findCommonPrefix(vector<string> &productions) {
    if (productions.empty()) return "";

    string prefix = productions[0];
    for (size_t i = 1; i < productions.size(); i++) {
        size_t j = 0;
        while (j < prefix.size() && j < productions[i].size() && prefix[j] == productions[i][j]) {
            j++;
        }
        prefix = prefix.substr(0, j); // Update prefix
        if (prefix.empty()) break;
    }
    return prefix;
}

// Function to remove left factoring
void removeLeftFactoring(string nonTerminal, vector<string> &productions) {
    string commonPrefix = findCommonPrefix(productions);

    if (commonPrefix.empty()) {
        cout << nonTerminal << " → ";
        for (size_t i = 0; i < productions.size(); i++) {
            cout << productions[i];
            if (i != productions.size() - 1) cout << " | ";
        }
        cout << endl;
        return;
    }

    // New non-terminal A'
    string newNonTerminal = nonTerminal + "'";

    // Print the transformed productions
    cout << nonTerminal << " → " << commonPrefix << newNonTerminal << endl;
    cout << newNonTerminal << " → ";

    bool first = true;
    for (string &prod : productions) {
        if (prod != commonPrefix) {
            if (!first) cout << " | ";
            cout << prod.substr(commonPrefix.size());
            first = false;
        }
    }
    cout << endl;
}

int main() {
    string nonTerminal;
    int numProductions;

    cout << "Enter the non-terminal: ";
    cin >> nonTerminal;

    cout << "Enter the number of productions: ";
    cin >> numProductions;

    vector<string> productions(numProductions);
    cout << "Enter the productions separated by space:\n";
    for (int i = 0; i < numProductions; i++) {
        cin >> productions[i];
    }

    removeLeftFactoring(nonTerminal, productions);

    return 0;
}
