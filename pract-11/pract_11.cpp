#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
using namespace std;

struct Quadruple {
    string op, arg1, arg2, result;
};

vector<Quadruple> quadruples;
int tempCount = 1;

string getTemp() {
    return "t" + to_string(tempCount++);
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

vector<string> infixToPostfix(string expr) {
    stack<char> opStack;
    vector<string> output;
    string num = "";
    
    for (char ch : expr) {
        if (isdigit(ch)) {
            num += ch;
        } else {
            if (!num.empty()) {
                output.push_back(num);
                num = "";
            }
            if (ch == '(') {
                opStack.push(ch);
            } else if (ch == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    output.push_back(string(1, opStack.top()));
                    opStack.pop();
                }
                opStack.pop();
            } else {
                while (!opStack.empty() && precedence(opStack.top()) >= precedence(ch)) {
                    output.push_back(string(1, opStack.top()));
                    opStack.pop();
                }
                opStack.push(ch);
            }
        }
    }
    if (!num.empty()) output.push_back(num);
    while (!opStack.empty()) {
        output.push_back(string(1, opStack.top()));
        opStack.pop();
    }
    return output;
}

string generateQuadruples(vector<string> postfix) {
    stack<string> evalStack;
    
    for (string token : postfix) {
        if (isdigit(token[0])) {
            evalStack.push(token);
        } else {
            string op2 = evalStack.top(); evalStack.pop();
            string op1 = evalStack.top(); evalStack.pop();
            string temp = getTemp();
            quadruples.push_back({token, op1, op2, temp});
            evalStack.push(temp);
        }
    }
    return evalStack.top();
}

void printQuadruples() {
    cout << "Operator\tOperand1\tOperand2\tResult\n";
    for (auto q : quadruples) {
        cout << q.op << "\t\t" << q.arg1 << "\t\t" << q.arg2 << "\t\t" << q.result << "\n";
    }
}

void printPostfix(const vector<string>& postfix) {
    cout << "Postfix Expression: ";
    for (const string& token : postfix) {
        cout << token << " ";
    }
    cout << endl;
}

int main() {
    string expr;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expr);
    
    vector<string> postfix = infixToPostfix(expr);
    printPostfix(postfix);
    generateQuadruples(postfix);
    printQuadruples();
    
    return 0;
}
