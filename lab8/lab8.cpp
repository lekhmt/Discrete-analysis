#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

using Matrix = vector<vector<bool>>;

bool dfs(int v, const Matrix& matrix, vector<int>& color, vector<int>& result) {

    color[v] = 1;
    for (int to = 0; to < matrix[v].size(); to++) {
        if (matrix[v][to]) {
            if (color[to] == 1) {
                return false;
            }
            if (color[to] == 0) {
                if (!dfs(to, matrix, color, result)) {
                    return false;
                }
                color[to] = 2;
                result.push_back(to);
            }
        }
    }
    return true;

}

vector<int> TopologicalSort(const Matrix& matrix) {

    vector<int> color(matrix.size(), 0);
    vector<int> result;

    for (int i = 0; i < matrix.size(); i++) {
        if (!color[i]) {
            bool isAcyclic = dfs(i, matrix, color, result);
            if (!isAcyclic) {
                return {};
            }
            color[i] = 2;
            result.push_back(i);
        }
    }

    reverse(result.begin(), result.end());
    return result;

}

int main() {

    int n, m;
    cin >> n >> m;
    Matrix matrix(n, vector<bool>(n, false));

    int a, b;
    for (int i = 0; i < m; i++) {
        cin >> a >> b;
        matrix[a - 1][b - 1] = true;
    }

    vector<int> result = TopologicalSort(matrix);

    if (result.empty()) {
        cout << -1;
    } else {
        for (int e : result) {
            cout << e + 1 << " ";
        }
    }

}