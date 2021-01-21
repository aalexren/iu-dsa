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

//    selection_sort(v);
    insertion_sort(v);

    for (int i = 0; i < n; ++i) cout << v[i] << " ";

    return 0;
}
