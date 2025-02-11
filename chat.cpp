#include <iostream>
#include <string>
using namespace std;

// Function to handle FSM based on the transition table
bool process_string(const string& str, int transition[][10], int num_symbols, int num_states, int initial_state, int accept_state) {
    int current_state = initial_state;

    // Process each symbol in the string
    for (int i = 0; i < str.length(); i++) {
        char current_symbol = str[i];
        int symbol_index = -1;

        // Find symbol index based on the user's input
        for (int j = 0; j < num_symbols; j++) {
            if (current_symbol == 'a' + j) {
                symbol_index = j;
                break;
            }
        }

        if (symbol_index == -1) {
            cout << "Invalid symbol in the string!" << endl;
            return false;
        }

        // Transition to the next state
        current_state = transition[symbol_index][current_state];
        cout << "After processing symbol '" << current_symbol << "', current state is: " << current_state << endl;
    }

    // Return whether the current state is the accepting state
    return current_state == accept_state;
}

int main() {
    int num_states, num_symbols;
    cout << "Enter the number of states: ";
    cin >> num_states;

    cout << "Enter the number of symbols: ";
    cin >> num_symbols;

    char symbols[num_symbols];
    cout << "Enter the symbols: ";
    for (int i = 0; i < num_symbols; i++) {
        cin >> symbols[i];
    }

    int transition[num_symbols][num_states];
    cout << "Enter the transition table (row by row):" << endl;
    for (int i = 0; i < num_symbols; i++) {
        for (int j = 0; j < num_states; j++) {
            cout << "Transition for symbol '" << symbols[i] << "' from state " << j << ": ";
            cin >> transition[i][j];
        }
    }

    int initial_state, accept_state;
    cout << "Enter the initial state: ";
    cin >> initial_state;

    cout << "Enter the accepting state: ";
    cin >> accept_state;

    string str;
    cout << "Enter the string to process: ";
    cin >> str;

    bool is_accepted = process_string(str, transition, num_symbols, num_states, initial_state, accept_state);

    if (is_accepted) {
        cout << "The input string is accepted." << endl;
    } else {
        cout << "The input string is not accepted." << endl;
    }

    return 0;
}

