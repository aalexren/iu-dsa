//
// Created by Артём Черница on 21.01.21.
//

#include <iostream>
#include <vector>

using namespace std;

void selection_sort(vector<int>& v) {
    for (int i = 0; i < v.size() - 1; ++i) {
        int min = i;
        for (int j = i + 1; j < v.size(); ++j) {
            if (v[j] < v[min])
                min = j;
        }
        if (min != i)
            swap(v[i], v[min]);
    }
}

int main() {

    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) cin >> v[i];

    selection_sort(v);

    for (int i = 0; i < n; ++i) cout << v[i] << " ";

    return 0;
}
