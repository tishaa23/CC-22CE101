#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<char, vector<string>> grammar = {
    {'S', {"ABC", "D"}},
    {'A', {"a", "^"}},
    {'B', {"b", "^"}},
    {'C', {"(S)", "c"}},
    {'D', {"AC"}}
};

map<char, set<char>> first_sets;
map<char, set<char>> follow_sets;

set<char> compute_first(char symbol) {
    if (first_sets.find(symbol) != first_sets.end()) {
        return first_sets[symbol];
    }

    set<char> first;
    if (!isupper(symbol)) {  // Terminal or epsilon
        first.insert(symbol);
        return first;
    }

    for (const string &production : grammar[symbol]) {
        bool has_epsilon = true;
        for (char item : production) {
            set<char> item_first = compute_first(item);
            first.insert(item_first.begin(), item_first.end());
            if (item_first.find('^') == item_first.end()) {
                has_epsilon = false;
                break;
            }
        }
        if (has_epsilon) {
            first.insert('^');
        }
    }
    first_sets[symbol] = first;
    return first;
}

set<char> compute_follow(char symbol) {
    if (follow_sets.find(symbol) != follow_sets.end()) {
        return follow_sets[symbol];
    }

    set<char> follow;
    if (symbol == 'S') {
        follow.insert('$');
    }

    for (auto &[lhs, productions] : grammar) {
        for (const string &production : productions) {
            for (size_t i = 0; i < production.size(); ++i) {
                if (production[i] == symbol) {
                    size_t j = i + 1;
                    bool epsilon_flag = true;
                    while (j < production.size()) {
                        set<char> first_next = compute_first(production[j]);
                        follow.insert(first_next.begin(), first_next.end());
                        follow.erase('^');
                        if (first_next.find('^') == first_next.end()) {
                            epsilon_flag = false;
                            break;
                        }
                        j++;
                    }
                    if (epsilon_flag || j == production.size()) {
                        set<char> follow_lhs = compute_follow(lhs);
                        follow.insert(follow_lhs.begin(), follow_lhs.end());
                    }
                }
            }
        }
    }
    follow_sets[symbol] = follow;
    return follow;
}

int main() {
    for (auto &[nt, _] : grammar) {
        compute_first(nt);
    }

    for (auto &[nt, _] : grammar) {
        compute_follow(nt);
    }

    for (auto &[nt, first] : first_sets) {
        cout << "First(" << nt << ") = { ";
        for (char ch : first) {
            cout << ch << " ";
        }
        cout << "}\n";
    }

    for (auto &[nt, follow] : follow_sets) {
        cout << "Follow(" << nt << ") = { ";
        for (char ch : follow) {
            cout << ch << " ";
        }
        cout << "}\n";
    }

    return 0;
}
