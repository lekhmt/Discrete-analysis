#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

const int SIZE = 32;

using TPair = pair<char[SIZE], char*>;
using TVector = vector<TPair>;

/**
 * @key - телефонные номера в формате +<код страны>-<код города>-<телефон>
 * @value - строки переменной длины (до 2048 символов)
 */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    TPair pair;
    TVector elements;
    string key;
    string value;

    size_t maxLength = 0;
    while (cin >> key >> value) {
        pair.first[0] = key.size();
        for (size_t i = SIZE - key.size(); i < SIZE; i++) {
            pair.first[i] = key[key.size() - (SIZE - i)];
        }
        pair.second = new char[value.length()];
        strcpy(pair.second, value.c_str());
        maxLength = max(key.size(), maxLength);
        elements.push_back(pair);
    }

    TVector result(elements.size());

    for (int pos = SIZE - 1; pos > SIZE - maxLength - 1; pos--) {

        vector<int> tmp(10, 0);

        for (TPair &element: elements) {
            int num = (SIZE - pos <= element.first[0]) ? max(element.first[pos] - '0', 0) : 0;
            tmp[num]++;
        }

        for (int i = 1; i < 10; i++) {
            tmp[i] = tmp[i] + tmp[i - 1];
        }

        for (int i = elements.size() - 1; i >= 0; i--) {
            int num = SIZE - pos <= elements[i].first[0] ? max(elements[i].first[pos] - '0', 0) : 0;
            tmp[num] -= 1;
            result[tmp[num]].first[0] = elements[i].first[0];
            for (int j = SIZE - elements[i].first[0]; j < SIZE; j++) {
                result[tmp[num]].first[j] = elements[i].first[j];
            }
            result[tmp[num]].second = elements[i].second;
        }

        swap(elements, result);

    }

    for (TPair & element : elements) {
        for (int j = SIZE - element.first[0]; j < SIZE; j++) {
            cout << element.first[j];
        }
        cout << '\t' << element.second << '\n';
        delete element.second;
    }
}