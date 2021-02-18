#include <iostream>
#include <stack>
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

using namespace std;

template <typename T>
stack<T> reverse_stack(stack<T> s) {
    stack<T> ss;
    while (!s.empty()) {
        ss.push(s.top());
        s.pop();
    }
    return ss;
}

long long calc_operation(const long long& lhs, const long long& rhs, const char& operation) {
    long long res = 0;
    switch (operation) {
        case '/': res = lhs / rhs; break;
        case '*': res = lhs * rhs; break;
        case '+': res = lhs + rhs; break;
        case '-': res = lhs - rhs; break;
    }
    return res;
}

bool is_left_higher(const char& lhs, const char& rhs) {
    if (lhs == '*' || lhs == '/') {
        return !(rhs == '*' || rhs == '/');
    }
    return false;
}

int main() {

    vector<string> tokens;
    string s;
    getline(cin, s);
    stringstream ss(s);

    while (getline(ss, s, ' ')) {
        tokens.push_back(s);
    }

    queue<string> output;
    stack<string> operations;
    for (auto it: tokens) {
        if (isdigit(it[0])) {
            output.push(it);
        }
        else if (it[0] != '(' && it[0] != ')') {
            if (!operations.empty()) {
                if (operations.top()[0] != '(' && operations.top()[0] != ')') {
                    if (!is_left_higher(it[0], operations.top()[0])) {
                        // pop all previous operations till (
                        // pop all operations strictly of the same priority
                        while (!operations.empty() &&
                               operations.top()[0] != '(' &&
                               !is_left_higher(it[0],operations.top()[0])) {
                            output.push(operations.top());
                            operations.pop();
                        }
                    }
                }
            }
            operations.push(it);
        }
        else if (it[0] == '(') {
            operations.push(it);
        }
        else if (it[0] == ')') {
            while (operations.top()[0] != '(') {
                output.push(operations.top());
                operations.pop();
            }
            operations.pop(); // throw out the open parenthesis
        }
    }

    while (!operations.empty()) {
        output.push(operations.top());
        operations.pop();
    }

    long long res;
    stack<long long> numbers;
    while (!output.empty()) {
        string token = output.front();
        if (!isdigit(token[0])) {
            long long t1 = numbers.top();
            numbers.pop();
            long long t2 = numbers.top();
            numbers.pop();
            // WARNING! Order of the t2 and t1 as arguments is reversed!
            res = calc_operation(t2, t1, token[0]);
            numbers.push(res);
        }
        else {
            numbers.push(atoll(token.c_str()));
        }
        output.pop();
    }

    cout << numbers.top() << " ";

    return 0;
}