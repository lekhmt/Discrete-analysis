#include <iostream>
#include <cmath>
#include <chrono>

using namespace std;

size_t length(long long n) {
    return to_string(n).length();
}

long long numberOfDivisibleInRange(long long leftBound, long long rightBound, int divider) {

    long long l = leftBound / divider + (leftBound % divider == 0 ? 0 : 1);
    long long r = rightBound / divider;

    return r - l + 1;

}

long long dp(long long currentNumber, int divider) {

    if (currentNumber < divider) {
        return 0;
    }

    return numberOfDivisibleInRange(pow(10, length(currentNumber) - 1), currentNumber, divider) +
           dp(currentNumber / 10, divider);

}

int main() {


    long long n;
    int m;
    cin >> n >> m;

    auto begin = chrono::steady_clock::now();
    cout << "DP: " << dp(n, m) - (n % m == 0 ? 1 : 0) << "\n";
    auto end = chrono::steady_clock::now();
    cout << "DP: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " mcs.\n";

    begin = chrono::steady_clock::now();
    int res = 0;
    for (int i = 1; i < n; i++) {
        if (n / i == 0 and to_string(i) < to_string(n)) {
            res += 1;
        }
    }
    cout << "Naive: " << res << "\n";
    end = chrono::steady_clock::now();
    cout << "Naive: " << chrono::duration_cast<chrono::microseconds >(end - begin).count() << " mcs.\n";

}