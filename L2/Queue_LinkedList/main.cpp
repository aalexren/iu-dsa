#include <iostream>
#include "Queue.h"

using namespace std;

int main() {
    LinkedList<int> list = LinkedList<int>();
    list.addFirst(5);
    list.addFirst(10);
    cout << list.get(1) << endl;

    return 0;
}
