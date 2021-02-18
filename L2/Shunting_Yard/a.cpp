#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

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

    stack<string> operations;
    for (auto it: tokens) {
        if (isdigit(it[0])) {
            cout << it << " ";
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
                            cout << operations.top() << " ";
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
                cout << operations.top() << " ";
                operations.pop();
            }
            operations.pop(); // throw out the open parenthesis
        }
    }

    while (!operations.empty()) {
        cout << operations.top() << " ";
        operations.pop();
    }

    return 0;
}
