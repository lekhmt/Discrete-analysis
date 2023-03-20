#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class TSufArr;

class TNode {
public:
    map<char, TNode *> dest;
    string::iterator begin, end;
    TNode *sufLink;

    TNode(string::iterator start, string::iterator end) : begin(start), end(end), sufLink(nullptr) {};

    ~TNode() = default;
};

class TSufTree {
public:

    explicit TSufTree(string s) : text(move(s)), root(new TNode(text.end(), text.end())), isReady(0) {
        activeEdge = text.begin();
        activeLen = 0;

        activeNode = root;
        SLmarker = root;
        root->sufLink = root;

        for (string::iterator i = text.begin(); i != text.end(); ++i) {
            TrExtend(i);
        }
    }

    ~TSufTree() = default;

    friend TSufArr;

private:
    string text;
    TNode *root;
    int isReady;
    TNode *SLmarker, *activeNode;
    int activeLen;
    string::iterator activeEdge;

    int EdgeLen(TNode *node, string::iterator cur_pos) {
        return static_cast<int>(min(node->end, cur_pos + 1) - node->begin);
    }

    void TreeDestroy(TNode *node) {
        for (auto &it: node->dest) {
            TreeDestroy(it.second);
        }
        delete node;
    }

    bool WalkDown(string::iterator cur_pos, TNode *node) {
        if (activeLen >= EdgeLen(node, cur_pos)) {
            activeEdge += EdgeLen(node, cur_pos);
            activeLen -= EdgeLen(node, cur_pos);
            activeNode = node;
            return true;
        }
        return false;
    }

    void SufLinkActivate(TNode *node) {
        if (SLmarker != root) {
            SLmarker->sufLink = node;
        }

        SLmarker = node;
    }

    void TrExtend(string::iterator current) {
        SLmarker = root;
        ++isReady;

        while (isReady) {
            if (!activeLen) {
                activeEdge = current;
            }
            auto tree_it = activeNode->dest.find(*activeEdge);
            TNode *next = (tree_it == activeNode->dest.end()) ? NULL : tree_it->second;
            if (!next) {
                TNode *leaf = new TNode(current, text.end());
                activeNode->dest[*activeEdge] = leaf;
                SufLinkActivate(activeNode);
            } else {
                if (WalkDown(current, next)) {
                    continue;
                }

                if (*(next->begin + activeLen) == *current) {
                    ++activeLen;
                    SufLinkActivate(activeNode);
                    break;
                }

                TNode *split = new TNode(next->begin, next->begin + activeLen);
                TNode *leaf = new TNode(current, text.end());
                activeNode->dest[*activeEdge] = split;

                split->dest[*current] = leaf;
                next->begin += activeLen;
                split->dest[*next->begin] = next;
                SufLinkActivate(split);
            }
            --isReady;
            if (activeNode == root && activeLen) {
                --activeLen;
                activeEdge = current - isReady + 1;
            } else {
                activeNode = (activeNode->sufLink) ? activeNode->sufLink : root;
            }
        }
    }

    void DFS(TNode *node, vector<int> &result, int depth) {
        if (node->dest.empty()) {
            result.push_back(static_cast<int &&>(text.size() - depth));//found
            return;
        }
        for (auto &it: node->dest) {
            int tmp = depth;
            tmp += it.second->end - it.second->begin;
            DFS(it.second, result, tmp);
        }
    }
};

class TSufArr {
public:

    explicit TSufArr(TSufTree tree) : text(tree.text), array() {
        tree.DFS(tree.root, array, 0);
        tree.TreeDestroy(tree.root);
    }

    vector<int> Find(string pattern) {
        pair<vector<int>::iterator, vector<int>::iterator> bounds(array.begin(), array.end());
        for (int i = 0; i < pattern.size() && bounds.first != bounds.second; ++i) {
            bounds = equal_range(bounds.first, bounds.second, numeric_limits<int>::max(),
                                 [this, &pattern, &i](int l, int r) -> bool {
                                     bool tmp;
                                     (l == numeric_limits<int>::max()) ? tmp = (pattern[i] < text[i + r]) : tmp = (
                                             text[i + l] < pattern[i]);
                                     return tmp;
                                 });
        }

        vector<int> result(bounds.first, bounds.second);
        sort(result.begin(), result.end());

        return result;
    }

    ~TSufArr() = default;

private:
    string text;
    vector<int> array;
};

int main() {
    string text, pattern;
    cin >> text;

    TSufTree tree(text + "$");
    TSufArr array(tree);

    for (int pattern_num = 1; cin >> text; ++pattern_num) {
        vector<int> result = array.Find(text);
        if (!result.empty()) {
            cout << pattern_num << ": ";
            for (int i = 0; i < result.size(); ++i) {
                cout << result[i] + 1; // from 0
                if (i < result.size() - 1) cout << ", ";
            }
            cout << '\n';
        }
    }
    return 0;
}