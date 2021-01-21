//
// Created by Артём Черница on 21.01.21.
//

#include <iostream>
#include <vector>

using namespace std;

void insertion_sort(vector<int>& v) {
    for (int i = 1; i < v.size(); ++i) {
        int key = v[i];
        int j = i - 1;
        while (j >= 0 && key < v[j]) {
            v[j + 1] = v[j]; // antecedent move to the place of consequent
            j -= 1;
        }
        v[j + 1] = key;
    }
}

int main() {

    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) cin >> v[i];

    insertion_sort(v);

    for (int i = 0; i < n; ++i) cout << v[i] << " ";

    return 0;
}
