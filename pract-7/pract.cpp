#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;
set<char> visitedFirst, visitedFollow;

void computeFirst(char nonTerminal);
void computeFollow(char nonTerminal);
void printFirstSet(char nonTerminal);

void computeFirst(char nonTerminal) {
    if (visitedFirst.find(nonTerminal) != visitedFirst.end()) return;
    visitedFirst.insert(nonTerminal);

    for (const string& production : grammar[nonTerminal]) {
        bool epsilonPresent = true;

        for (char symbol : production) {
            if (islower(symbol) || symbol == '(' || symbol == ')') {
                firstSet[nonTerminal].insert(symbol);
                epsilonPresent = false;
                break;
            } else if (isupper(symbol)) {
                computeFirst(symbol);
                bool allEpsilon = true;

                for (char f : firstSet[symbol]) {
                    if (f != '@') firstSet[nonTerminal].insert(f);
                    if (f == '@') allEpsilon = true;
                }

                if (!allEpsilon) {
                    epsilonPresent = false;
                    break;
                }
            } else if (symbol == '@') {
                firstSet[nonTerminal].insert('@');
            }
        }

        if (!epsilonPresent) firstSet[nonTerminal].erase('@');
    }
}

void computeFollow(char nonTerminal) {
    if (visitedFollow.find(nonTerminal) != visitedFollow.end()) return;
    visitedFollow.insert(nonTerminal);

    if (nonTerminal == 'S') followSet[nonTerminal].insert('$');

    for (const auto& entry : grammar) {
        char lhs = entry.first;
        for (const string& production : entry.second) {
            for (size_t i = 0; i < production.size(); i++) {
                if (production[i] == nonTerminal) {
                    bool inheritFollow = false;

                    for (size_t j = i + 1; j < production.size(); j++) {
                        char nextSymbol = production[j];
                        if (isupper(nextSymbol)) {
                            followSet[nonTerminal].insert(firstSet[nextSymbol].begin(), firstSet[nextSymbol].end());
                            followSet[nonTerminal].erase('@');

                            if (firstSet[nextSymbol].find('@') != firstSet[nextSymbol].end()) {
                                inheritFollow = true;
                            } else {
                                inheritFollow = false;
                                break;
                            }
                        } else {
                            followSet[nonTerminal].insert(nextSymbol);
                            inheritFollow = false;
                            break;
                        }
                    }

                    if (inheritFollow || i + 1 == production.size()) {
                        if (lhs != nonTerminal) {
                            computeFollow(lhs);
                            followSet[nonTerminal].insert(followSet[lhs].begin(), followSet[lhs].end());
                        }
                    }
                }
            }
        }
    }
}

void printFirstSet(char nonTerminal) {
    cout << "First(" << nonTerminal << ") = { ";
    for (char c : firstSet[nonTerminal]) {
        cout << (c == '@' ? "ε " : string(1, c) + " ");
    }
    cout << "}" << endl;
}

int main() {
    grammar['S'] = {"ABC", "D"};
    grammar['A'] = {"a", "@"};
    grammar['B'] = {"b", "@"};
    grammar['C'] = {"(S)", "c"};
    grammar['D'] = {"AC"};

    for (const auto& entry : grammar) computeFirst(entry.first);
    for (const auto& entry : grammar) computeFollow(entry.first);

    cout << "First and Follow Sets:" << endl;

    printFirstSet('S');
    for (const auto& entry : firstSet) {
        if (entry.first != 'S') {
            printFirstSet(entry.first);
        }
    }

    for (const auto& entry : followSet) {
        cout << "Follow(" << entry.first << ") = { ";
        for (char c : entry.second) {
            cout << c << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}