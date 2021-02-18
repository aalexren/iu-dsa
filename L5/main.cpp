/*
 * Knapsnack problem
 * solution with O(n * 2^n) time complexity
 * Brute-force: exclude and include every item,
 * check all possible variants (2^n),
 * find best solution that satisfies the condition (maximum weight)
 *
 * Created by Artem Chernitsa 18.02.2020
 */

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
   int N, W;
   cin >> N >> W; // number of items, max weight

   vector<int> weights(N);
   vector<int> prices(N);

   for (int i = 0; i < N; ++i) {
      cin >> weights[i];
   }

   for (int i = 0; i < N; ++i) {
      cin >> prices[i];
   }

   vector<int> answers; // indexes of needed items
   int variants = pow(2, N);
   int max = 0;
   for (int i = 0; i < variants; ++i) {
      int temp_p = 0;
      int temp_w = 0;
      vector<int> temp_i; // temp indexes
      for (int j = 0; j < N; ++j) {
         if (((1 << j) & i) > 0) { // should we take j-th element?
            temp_w += weights[j];
            temp_p += prices[j];
            temp_i.push_back(j);
         }
         if (temp_w <= W) {
            if (max < temp_p) {
               max = temp_p;
               answers = temp_i;
            }
         }
      }
   }

   cout << answers.size() << endl;
   for (auto i: answers) {
      cout << i + 1 << " ";
   }

   return 0;
}
