#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
using namespace std;

struct SymbolTableEntry {
    string name;
};

unordered_set<string> keywords = {"int", "char", "return", "if", "else", "while", "for", "void", "float", "double", "struct"};
unordered_set<char> operators = {'+', '-', '*', '/', '%', '=', '<', '>', '!', '&', '|'};
unordered_set<char> punctuation = {';', ',', '(', ')', '{', '}', '[', ']', '.'};

void tokenizeLine(string line, vector<string> &tokens, vector<SymbolTableEntry> &symbolTable, unordered_map<string, bool> &identifiers, vector<string> &errors) {
    string token = "";
    bool inComment = false;

    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '/' && i + 1 < line.size() && (line[i + 1] == '/' || line[i + 1] == '*')) {
            inComment = true;
            break; // Ignore comments
        }

        if (isspace(line[i])) {
            continue;
        } else if (isalpha(line[i]) || line[i] == '') { // Starting with a letter or ''
            token.clear();
            while (i < line.size() && (isalnum(line[i]) || line[i] == '_')) {
                token += line[i++];
            }
            --i;

            if (keywords.count(token)) {
                tokens.push_back("Keyword: " + token);
            } else if (token != "main") { // Exclude function names
                tokens.push_back("Identifier: " + token);
                if (!identifiers[token]) {
                    symbolTable.push_back({token});
                    identifiers[token] = true;
                }
            }
        } else if (isdigit(line[i])) { // Starting with a digit
            token.clear();
            while (i < line.size() && (isalnum(line[i]) || line[i] == '.')) {
                token += line[i++];
            }
            --i;

            // Check if token contains invalid characters
            bool invalidToken = false;
            for (size_t j = 0; j < token.size(); ++j) {
                if (isalpha(token[j])) { // Letters in a numeric literal are invalid
                    invalidToken = true;
                    break;
                }
            }

            if (invalidToken) {
                errors.push_back(token + " invalid lexeme");
            } else {
                tokens.push_back("Constant: " + token);
            }
        } else if (line[i] == '\'' || line[i] == '"') { // Strings and character literals
            char quote = line[i];
            token.clear();
            token += line[i++];
            while (i < line.size() && line[i] != quote) {
                token += line[i++];
            }
            if (i < line.size()) token += line[i];
            tokens.push_back("String: " + token);
        } else if (operators.count(line[i])) {
            tokens.push_back("Operator: " + string(1, line[i]));
        } else if (punctuation.count(line[i])) {
            tokens.push_back("Punctuation: " + string(1, line[i]));
        } else { // Handle any other invalid character sequences
            token.clear();
            while (i < line.size() && !isspace(line[i]) && !isalnum(line[i])) {
                token += line[i++];
            }
            --i;
            errors.push_back(token + " invalid lexeme");
        }
    }
}

int main() {
    ifstream inputFile("input.c");
    if (!inputFile.is_open()) {
        cerr << "Failed to open input file." << endl;
        return 1;
    }

    vector<string> tokens;
    vector<SymbolTableEntry> symbolTable;
    unordered_map<string, bool> identifiers;
    vector<string> errors;

    string line;
    while (getline(inputFile, line)) {
        tokenizeLine(line, tokens, symbolTable, identifiers, errors);
    }

    inputFile.close();

    cout << "TOKENS" << endl;
    for (const auto &token : tokens) {
        cout << token << endl;
    }

    cout << "\nLEXICAL ERRORS" << endl;
    for (const auto &error : errors) {
        cout << error << endl;
    }

    cout << "\nSYMBOL TABLE ENTRIES" << endl;
    for (const auto &entry : symbolTable) {
        cout << entry.name << endl;
    }

    return 0;
}
