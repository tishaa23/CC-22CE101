#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <cctype>
using namespace std;

// Function to check if a token is an operator
bool isOperator(const string &op) {
    return op == "+" || op == "-" || op == "*" || op == "/";
}

// Function to check if a token is a number
bool isNumber(const string &s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Function to perform arithmetic operations
int applyOperator(int a, int b, const string &op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return b != 0 ? a / b : 0; // Avoid division by zero
    return 0;
}

// Function to convert infix expression to postfix
vector<string> infixToPostfix(const vector<string> &tokens) {
    vector<string> output;
    stack<string> operators;
    unordered_map<string, int> precedence = {{"+", 1}, {"-", 1}, {"*", 2}, {"/", 2}};

    for (const string &token : tokens) {
        if (isNumber(token) || isalpha(token[0])) {
            output.push_back(token);
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Remove the opening parenthesis
        } else if (isOperator(token)) {
            while (!operators.empty() && operators.top() != "(" &&
                   precedence[operators.top()] >= precedence[token]) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return output;
}

// Function to evaluate constant expressions in postfix notation
vector<string> evaluatePostfix(const vector<string> &postfix) {
    stack<int> evalStack;
    vector<string> optimizedExpression;

    for (const string &token : postfix) {
        if (isNumber(token)) {
            evalStack.push(stoi(token));
        } else if (isOperator(token)) {
            if (evalStack.size() >= 2) {
                int b = evalStack.top(); evalStack.pop();
                int a = evalStack.top(); evalStack.pop();

                // Compute result for constant values
                int result = applyOperator(a, b, token);
                evalStack.push(result);
                optimizedExpression.pop_back(); // Remove 'b'
                optimizedExpression.pop_back(); // Remove 'a'
                optimizedExpression.push_back(to_string(result)); // Add result
            } else {
                optimizedExpression.push_back(token); // Push operator if variables are present
            }
        } else {
            optimizedExpression.push_back(token);
        }
    }

    return optimizedExpression;
}

// Function to parse input expression into tokens
vector<string> tokenize(const string &expr) {
    vector<string> tokens;
    string token;
    for (size_t i = 0; i < expr.size(); ++i) {
        if (isspace(expr[i])) {
            continue;
        } else if (isdigit(expr[i])) {
            token += expr[i];
            while (i + 1 < expr.size() && isdigit(expr[i + 1])) {
                token += expr[++i];
            }
            tokens.push_back(token);
            token.clear();
        } else if (isalpha(expr[i])) {
            token += expr[i];
            while (i + 1 < expr.size() && isalpha(expr[i + 1])) {
                token += expr[++i];
            }
            tokens.push_back(token);
            token.clear();
        } else if (isOperator(string(1, expr[i])) || expr[i] == '(' || expr[i] == ')') {
            tokens.push_back(string(1, expr[i]));
        }
    }
    return tokens;
}

int main() {
    string input;
    cout << "Enter an arithmetic expression: ";
    getline(cin, input);

    vector<string> tokens = tokenize(input);
    vector<string> postfix = infixToPostfix(tokens);
    vector<string> optimizedTokens = evaluatePostfix(postfix);

    for (const string &token : optimizedTokens) {
        cout << token << " ";
    }
    cout << endl;

    return 0;
}
