#include <iostream>
#include "ArrayList.h"

using namespace std;

int main() {
    ArrayList<int> stack = ArrayList<int>(5);

    for (int i = 0; i < 33; ++i) {
        stack.push(i + 1);
        cout << stack.peek() << endl;
    }

    for (int i = 0; i < 33; ++i) {
        cout << stack.pop() << "\n";
    }

    return 0;
}
