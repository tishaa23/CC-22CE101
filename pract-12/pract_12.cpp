//  #include <iostream>
// #include <sstream>
// #include <vector>
// #include <stack>
// #include <algorithm>
// #include <cctype>
// using namespace std;

// bool isNumber(const string &s) {
//     return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
// }

// int precedence(char op) {
//     if (op == '+' || op == '-') return 1;
//     if (op == '*' || op == '/') return 2;
//     return 0;
// }

// // Evaluates a simple expression with two numbers and an operator
// double applyOp(double a, double b, char op) {
//     if (op == '+') return a + b;
//     if (op == '-') return a - b;
//     if (op == '*') return a * b;
//     if (op == '/') return a / b; // Assuming valid input (no divide by zero)
//     return 0;
// }

// // Convert infix expression to postfix (handling precedence)
// vector<string> infixToPostfix(const string &expr) {
//     vector<string> output;
//     stack<char> opStack;
//     string num = "";

//     for (char ch : expr) {
//         if (isdigit(ch)) {
//             num += ch;
//         } else {
//             if (!num.empty()) {
//                 output.push_back(num);
//                 num = "";
//             }
//             if (ch == '(') {
//                 opStack.push(ch);
//             } else if (ch == ')') {
//                 while (!opStack.empty() && opStack.top() != '(') {
//                     output.push_back(string(1, opStack.top()));
//                     opStack.pop();
//                 }
//                 opStack.pop(); // Remove '('
//             } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
//                 while (!opStack.empty() && precedence(opStack.top()) >= precedence(ch)) {
//                     output.push_back(string(1, opStack.top()));
//                     opStack.pop();
//                 }
//                 opStack.push(ch);
//             } else if (isalpha(ch)) { // Handle variables
//                 output.push_back(string(1, ch));
//             }
//         }
//     }
//     if (!num.empty()) output.push_back(num);
//     while (!opStack.empty()) {
//         output.push_back(string(1, opStack.top()));
//         opStack.pop();
//     }
//     return output;
// }

// // Evaluate the postfix expression and fold constant expressions
// string optimizeExpression(vector<string> postfix) {
//     stack<string> evalStack;

//     for (const string &token : postfix) {
//         if (isNumber(token)) {
//             evalStack.push(token);
//         } else if (isalpha(token[0])) { // If it's a variable, keep it
//             evalStack.push(token);
//         } else {
//             string op2 = evalStack.top(); evalStack.pop();
//             string op1 = evalStack.top(); evalStack.pop();

//             if (isNumber(op1) && isNumber(op2)) {
//                 double val = applyOp(stod(op1), stod(op2), token[0]);
//                 evalStack.push(to_string(val)); // Push computed value
//             } else { // If variables are involved, keep the expression
//                 evalStack.push(op1 + " " + token + " " + op2);
//             }
//         }
//     }
//     return evalStack.top();
// }

// int main() {
//     string expr;
//     cout << "Enter an arithmetic expression: ";
//     getline(cin, expr);

//     vector<string> postfix = infixToPostfix(expr);
//     string optimized = optimizeExpression(postfix);
    
//     cout << "Optimized Expression: " << optimized << endl;
//     return 0;
// } 



#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;

// Helper function to check if a string is a number
bool isNumber(const string &s) {
    for (char ch : s) {
        if (!isdigit(ch) && ch != '.') return false;
    }
    return !s.empty();
}

// Perform the operation on two numbers
string applyOp(double a, double b, char op) {
    double result;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/': result = b != 0 ? a / b : 0; break;
    }
    ostringstream oss;
    oss << result;
    return oss.str();
}

// Evaluate the expression using precedence: / > * > + > -
string simplifyExpression(string expr) {
    string operators = "/\n*\n+\n-";
    bool modified;
    
    do {
        modified = false;
        for (char op : operators) {
            size_t pos = expr.find(op);
            while (pos != string::npos) {
                size_t left = pos - 1;
                size_t right = pos + 1;

                // Find the left number
                while (left >= 0 && (isdigit(expr[left]) || expr[left] == '.')) left--;
                size_t numStart = left + 1;

                // Find the right number
                while (right < expr.size() && (isdigit(expr[right]) || expr[right] == '.')) right++;
                size_t numEnd = right - 1;

                string leftNum = expr.substr(numStart, pos - numStart);
                string rightNum = expr.substr(pos + 1, numEnd - pos);

                if (isNumber(leftNum) && isNumber(rightNum)) {
                    double num1 = stod(leftNum);
                    double num2 = stod(rightNum);
                    string result = applyOp(num1, num2, op);

                    // Replace the evaluated part
                    expr.replace(numStart, numEnd - numStart + 1, result);
                    modified = true;
                    break;
                }
                pos = expr.find(op, pos + 1);
            }
        }
    } while (modified);

    return expr;
}

int main() {
    string expr;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expr);

    string result = simplifyExpression(expr);
    cout << "Simplified Expression: " << result << endl;

    return 0;
}
