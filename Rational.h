#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

using namespace std;

class Rational {
    friend class Expression;
    long long int m_num, m_den;

    public:
    Rational(long long int num = 0, long long int den = 1) {
        m_num = num;
        m_den = den;
    }

    Rational getResult();
    void simplify(); // simplifies rational numbers to their lowest forms
    // overloaded operators: +, -, *, /
    Rational operator+(Rational rat);
    Rational operator-(Rational rat);
    Rational operator*(Rational rat);
    Rational operator/(Rational rat);
};

class Expression {
    string exp;
    //Rational obj;
    public:
    Expression(string exp);
    string getExpression(); // returns this->exp
    int precedence(char op); // defines and checks precedence
    Rational eval(Rational rat1, Rational rat2, char op); // evaluates a single operation
    Rational evaluate(); // evaluates the entire expression
    void getResult(); // prints the result of expression
};

// predefining containers

typedef stack<Rational> stack_rat;
typedef stack<char> stack_op;

Expression::Expression(string exp) {
    this->exp = exp;
}

string Expression::getExpression() {
    return this->exp;
}

void Expression::getResult() {
    Rational res = evaluate();
    if (res.m_den != 1)
        cout << res.m_num << "/" << res.m_den << endl;
    else
        cout << res.m_num << endl;
}

int Expression::precedence(char op) {
    if (op == '*' || op == '/') return 1;
    if (op == '+' || op == '-') return 0;
    return -1;
}

void Rational::simplify() {
    // solves negative denominator
    if (m_den < 0) {
        m_num *= -1;
        m_den *= -1;
    }

    long long int den = __gcd(m_num, m_den); // __gcd found in algorithm
    m_num /= den;
    m_den /= den;
}

Rational Rational::getResult() {
    Rational res;
    res.m_num = this->m_num;
    res.m_den = this->m_den;
    return res;
}

Rational Rational::operator+(Rational rat) {
    Rational res;
    res.m_num = this->m_num * rat.m_den + rat.m_num * this->m_den;
    res.m_den = this->m_den * rat.m_den;
    return res;
}

Rational Rational::operator-(Rational rat) {
    Rational res;
    res.m_num = this->m_num * rat.m_den - rat.m_num * this->m_den;
    res.m_den = this->m_den * rat.m_den;
    return res;
}

Rational Rational::operator*(Rational rat) {
    Rational res;
    res.m_num = this->m_num * rat.m_num;
    res.m_den = this->m_den * rat.m_den;
    return res;
}

Rational Rational::operator/(Rational rat) {
    Rational res;
    res.m_num = this->m_num * rat.m_den;
    res.m_den = this->m_den * rat.m_num;
    return res;
}

Rational Expression::eval(Rational rat1, Rational rat2, char op) {
    Rational res;
    switch (op) {
        case '+':
        res = rat1 + rat2;
        break;
        case '-':
        res = rat1 - rat2;
        break;
        case '*':
        res = rat1 * rat2;
        break;
        case '/':
        res = rat1 / rat2;
        break;
    }
    res.simplify();
    return res;
}

Rational Expression::evaluate() {
    stack_rat rat; // ratinal number stack
    stack_op op; // operator stack

    for (int i = 0; i < exp.length(); i++) {
        if (exp[i] == ' ') continue; // ignore whitespaces

        else if (exp[i] == '(') op.push(exp[i]);

        else if (isdigit(exp[i])) {
            string str = "";
            // continues until you encounter anything other than numbers
            while (i < exp.length() && isdigit(exp[i])) {
                str += exp[i];
                i++;
            }
            Rational e(stoi(str));
            rat.push(e);
            i--;
        }
        // this block evaluates in between parentheses; starts when a closing parentheses is found
        else if (exp[i] == ')') {
            while (!op.empty() && op.top() != '(') {
                Rational rat2(rat.top());
                rat.pop();

                Rational rat1(rat.top());
                rat.pop();

                char ope = op.top();
                op.pop();

                rat.push(eval(rat1, rat2, ope));
            }
            if (!op.empty()) {
                op.pop();
            }
        }
        // checks for precedence and evaluates using rational stack and operator stack
        else {
            while (!op.empty() && precedence(op.top()) >= precedence(exp[i])) {
                Rational rat2(rat.top());
                rat.pop();

                Rational rat1(rat.top());
                rat.pop();

                char ope = op.top();
                op.pop();

                rat.push(eval(rat1, rat2, ope));
            }
            op.push(exp[i]);
        }
    }
    // evaluates with the leftover operands and operators
    while (!op.empty()) {
        Rational rat2(rat.top());
        rat.pop();

        Rational rat1(rat.top());
        rat.pop();

        char ope = op.top();
        op.pop();

        rat.push(eval(rat1, rat2, ope));
    }
    return rat.top();
}
