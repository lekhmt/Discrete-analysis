#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <sstream>

using namespace std;

using TText = vector<pair<string, pair<int, int>>>;
using TPattern = vector<string>;
using TPosition = pair<int, int>;

int BadSymbolShift(unordered_map<string, int> &table, int i, const string &s) {
    if (table.count(s) == 0) {
        return i + 1;
    }
    int shift = i - table[s];
    return shift > 0 ? shift : 1;
}

vector<TPosition> Find(const TText &text, const TPattern &pattern) {
    vector<TPosition> answer;
    if (text.size() < pattern.size() or pattern.size() == 0) {
        return answer;
    }

    unordered_map<string, int> stopTable;
    for (int i = 0; i < pattern.size(); i++) {
        stopTable[pattern[i]] = i;
    }

    int i = pattern.size() - 1;
    while (i < text.size()) {
        int textIterator = i;
        int patternIterator = pattern.size() - 1;
        while (text[textIterator].first == pattern[patternIterator]) {
            if (patternIterator == 0) {
                answer.emplace_back(text[textIterator].second.first, text[textIterator].second.second);
                break;
            }
            textIterator--;
            patternIterator--;
        }
        i += max(1, BadSymbolShift(stopTable, patternIterator, text[textIterator].first));
    }

    return answer;
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    TText text;
    TPattern pattern;

    string str;
    getline(cin, str);
    istringstream patternStream(str);

    while (patternStream >> str) {
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        pattern.push_back(str);
    }

    int line = 0;
    while (getline(cin, str)) {
        line++;
        if (str.empty()) {
            continue;
        }
        istringstream textStream(str);
        int counter = 1;
        while (textStream >> str) {
            std::transform(str.begin(), str.end(), str.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            text.emplace_back(str, make_pair(line, counter));
            counter++;
        }
    }

    for (TPosition tp: Find(text, pattern)) {
        cout << tp.first << ", " << tp.second << "\n";
    }

}
