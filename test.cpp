#include <iostream>
#include <string>
#include <stack>

using namespace std;

typedef stack<int> stack_num;
typedef stack<char> stack_op;


int prededence(char op);
int eval(int num1, int num2, char op);
int evaluate(string exp);

int main() {
    string exp;
    getline(cin, exp);
    cout << evaluate(exp) << endl;
    return 0;
}

int precedence(char op) {
    if (op == '*' || op == '/') return 1;
    if (op == '+' || op == '-') return 0;
    return -1;
}

int eval(int num1, int num2, char op) {
    switch (op) {
        case '+':
        return num1 + num2;
        case '-':
        return num1 - num2;
        case '*':
        return num1 * num2;
        case '/':
        return num1 / num2;
    }
}

int evaluate(string exp) {
    stack_num num;
    stack_op op;

    for (int i = 0; i < exp.length(); i++) {
        if (exp[i] == ' ') continue;

        else if (exp[i] == '(') op.push(exp[i]);

        else if (isdigit(exp[i])) {
            string str = "";
            while (i < exp.length() && isdigit(exp[i])) {
                str += exp[i];
                i++;
            }
            num.push(stoi(str));
            i--;
        }
        
        else if (exp[i] == ')') {
            while (!op.empty() && op.top() != '(') {
                int num2 = num.top();
                num.pop();

                int num1 = num.top();
                num.pop();

                char ope = op.top();
                op.pop();

                num.push(eval(num1, num2, ope));
            }
            if (!op.empty()) {
                op.pop();
            }
        }
        
        else {
            while (!op.empty() && precedence(op.top()) >= precedence(exp[i])) {
                int num2 = num.top();
                num.pop();

                int num1 = num.top();
                num.pop();

                char ope = op.top();
                op.pop();

                num.push(eval(num1, num2, ope));
                
            }
            op.push(exp[i]);
        }
    }

    while (!op.empty()) {
        int num2 = num.top();
        num.pop();

        int num1 = num.top();
        num.pop();

        char ope = op.top();
        op.pop();

        num.push(eval(num1, num2, ope));
    }

    return num.top();
}
