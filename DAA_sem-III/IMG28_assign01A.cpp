#include <iostream>
#include <vector>
using namespace std;

int reverseLinearSearch(const vector<int>& prices, int target) {
    for (int i = prices.size() - 1; i >= 0; i--) {
        if (prices[i] == target) {
            return i;
        }
    }
    return -1;
}

int main() {
    vector<int> prices = {100, 120, 120, 130, 130, 130, 125, 110, 90};
    int target = 130;
    int result = reverseLinearSearch(prices, target);
    cout << "Last occurrence of " << target << " is at index: " << result << endl;
    return 0;
}