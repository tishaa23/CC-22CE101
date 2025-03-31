#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <stack>
using namespace std;

map<char, vector<string>> grammar;
map<char, set<char>> firstSets, followSets;
map<char, map<char, string>> parsingTable;
set<char> nonTerminals;

void computeFirst(char nonTerminal);
void computeFollow(char nonTerminal);
void constructParsingTable();
bool isLL1();
bool validateString(const string &input);

int main() {
    // Define the grammar
    grammar['S'] = {"ABC", "D"};
    grammar['A'] = {"a", ""};
    grammar['B'] = {"b", ""};
    grammar['C'] = {"(S)", "c"};
    grammar['D'] = {"AC"};

    // Identify non-terminals
    for (const auto &rule : grammar) {
        nonTerminals.insert(rule.first);
    }

    // Compute First sets
    for (char nonTerminal : nonTerminals) {
        computeFirst(nonTerminal);
    }

    // Compute Follow sets
    followSets['S'].insert('$'); // Start symbol follow set includes $
    for (char nonTerminal : nonTerminals) {
        computeFollow(nonTerminal);
    }

    // Construct Parsing Table
    constructParsingTable();

    // Display First sets
    cout << "First sets:\n";
    for (const auto &entry : firstSets) {
        cout << "First(" << entry.first << ") = {";
        for (auto it = entry.second.begin(); it != entry.second.end(); ++it) {
            if (it != entry.second.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";
    }

    // Display Follow sets
    cout << "\nFollow sets:\n";
    for (const auto &entry : followSets) {
        cout << "Follow(" << entry.first << ") = {";
        for (auto it = entry.second.begin(); it != entry.second.end(); ++it) {
            if (it != entry.second.begin()) cout << ", ";
            cout << *it;
        }
        cout << "}\n";
    }

    // Display Parsing Table
    cout << "\nPredictive Parsing Table:\n";
    for (const auto &row : parsingTable) {
        for (const auto &col : row.second) {
            cout << "M[" << row.first << ", " << col.first << "] = " << col.second << "\n";
        }
    }

    // Check if the grammar is LL(1)
    if (isLL1()) {
        cout << "\nThe grammar is LL(1)." << endl;
    } else {
        cout << "\nThe grammar is not LL(1)." << endl;
        return 0;
    }

    // Validate input strings
    vector<string> testCases = {"abc", "ac", "(abc)", "c", "(ac)", "a", "()", "(ab)", "abcabc", "b"};
    for (const string &test : testCases) {
        cout << "\nTesting string: " << test << " -> ";
        cout << (validateString(test) ? "Valid string" : "Invalid string") << endl;
    }

    return 0;
}

void computeFirst(char nonTerminal) {
    if (!firstSets[nonTerminal].empty()) return;

    for (const string &production : grammar[nonTerminal]) {
        if (production.empty()) {
            firstSets[nonTerminal].insert('ε');
            continue;
        }

        for (char symbol : production) {
            if (!nonTerminals.count(symbol)) { // Terminal character
                firstSets[nonTerminal].insert(symbol);
                break;
            } else {
                computeFirst(symbol);
                firstSets[nonTerminal].insert(firstSets[symbol].begin(), firstSets[symbol].end());
                if (firstSets[symbol].count('ε') == 0) break;
            }
        }
    }
}

void computeFollow(char nonTerminal) {
    for (const auto &rule : grammar) {
        char lhs = rule.first;
        for (const string &production : rule.second) {
            for (size_t i = 0; i < production.size(); ++i) {
                if (production[i] == nonTerminal) {
                    size_t j = i + 1;
                    while (j < production.size()) {
                        char nextSymbol = production[j];
                        if (!nonTerminals.count(nextSymbol)) { // Terminal
                            followSets[nonTerminal].insert(nextSymbol);
                            break;
                        } else {
                            followSets[nonTerminal].insert(firstSets[nextSymbol].begin(), firstSets[nextSymbol].end());
                            if (firstSets[nextSymbol].count('ε') == 0) break;
                        }
                        j++;
                    }
                    if (j == production.size() || firstSets[production[j - 1]].count('ε')) {
                        followSets[nonTerminal].insert(followSets[lhs].begin(), followSets[lhs].end());
                    }
                }
            }
        }
    }
}

void constructParsingTable() {
    for (char nonTerminal : nonTerminals) {
        for (const string &production : grammar[nonTerminal]) {
            set<char> firstSet;
            if (!production.empty()) {
                firstSet.insert(production[0]);
            }

            for (char terminal : firstSet) {
                parsingTable[nonTerminal][terminal] = production;
            }

            if (firstSet.count('ε')) {
                for (char terminal : followSets[nonTerminal]) {
                    parsingTable[nonTerminal][terminal] = "ε";
                }
            }
        }
    }
}

bool isLL1() {
    for (auto &entry : parsingTable) {
        set<char> seen;
        for (auto &cell : entry.second) {
            if (seen.count(cell.first)) {
                return false;
            }
            seen.insert(cell.first);
        }
    }
    return true;
}

bool validateString(const string &input) {
    stack<char> parseStack;
    parseStack.push('$');
    parseStack.push('S');
    size_t index = 0;
    string str = input + "$";

    while (!parseStack.empty()) {
        char top = parseStack.top();
        char current = str[index];
        parseStack.pop();

        if (top == current) {
            index++;
        } else if (parsingTable[top].count(current)) {
            string rule = parsingTable[top][current];
            for (auto it = rule.rbegin(); it != rule.rend(); ++it) {
                if (*it != 'ε') parseStack.push(*it);
            }
        } else {
            return false;
        }
    }
    return index == str.size() - 1;
}
