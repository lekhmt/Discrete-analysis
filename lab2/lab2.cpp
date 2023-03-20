#include <iostream>
#include <utility>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

using TPair = pair<string, unsigned long long>;

class TAVLTree;

class TNode {
public:

    TNode(TPair value, TNode *parent);

    void Print(int height);

    void Insert(TAVLTree *tree, const TPair &valueToInsert, bool &flag);

    void Remove(TAVLTree *tree, const string &key, bool &flag);

    unsigned long long Find(const string &key, bool &flag);

    void Save(ofstream &file);

    void Clear();

private:
    TPair value;

    int balance = 0;
    TNode *parent = nullptr;
    TNode *left = nullptr;

    TNode *right = nullptr;

    void Rebalance(TAVLTree *tree, const string &key);

    void RemovalRebalance(TAVLTree *tree, const string &key);

    TPair FindMinimumAndRemove(TAVLTree *tree);

    void LeftRotate(TAVLTree *tree);

    void RightRotate(TAVLTree *tree);

};


class TAVLTree {
private:

    TNode *root = nullptr;

public:

    void SetRoot(TNode *root);

    void Print();

    string Insert(TPair &value);

    string Remove(const string &key);

    string Find(const string &key);

    void Save(ofstream &file);

    void Load(ifstream &file);

    void Clear();

};

void TAVLTree::SetRoot(TNode *newRoot) {
    this->root = newRoot;
}

void TAVLTree::Print() {
    root->Print(0);
}

string TAVLTree::Insert(TPair &value) {
    bool flag = true;
    if (!root) {
        root = new TNode(value, nullptr);
    } else {
        root->Insert(this, value, flag);
    }
    return flag ? "OK" : "Exist";
}

string TAVLTree::Remove(const string& key) {
    bool flag = true;
    if (!root) {
        flag = false;
    } else {
        root->Remove(this, key, flag);
    }
    return flag ? "OK" : "NoSuchWord";
}

string TAVLTree::Find(const string &key) {
    bool flag = true;
    unsigned long long value;
    if (!root) {
        flag = false;
    } else {
        value = root->Find(key, flag);
    }
    return flag ? ("OK: " + to_string(value)) : "NoSuchWord";
}

void TAVLTree::Save(ofstream &file) {
    if (!root) {
        return;
    }
    root->Save(file);
}

void TAVLTree::Load(ifstream &file) {
    this->Clear();
    size_t size;
    while (file.read(reinterpret_cast<char*>(&size), sizeof(size))) {
        string key;
        key.resize(size);
        file.read(key.data(), size);
        unsigned long long value;
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        TPair pair = make_pair(key, value);
        this->Insert(pair);
    }
}

void TAVLTree::Clear() {
    if (!root) {
        return;
    }
    root->Clear();
    root = nullptr;
}


TNode::TNode(TPair value, TNode* parent) : value(move(value)), parent(parent) {}

void TNode::Print(int height){
    if (this->right != nullptr) {
        this->right->Print(height + 2);
    }
    cout << height / 2 << ":";
    for (int i = 0; i < height; i++) {
        cout << "-";
    }
    cout << this->value.first << ": " << this->balance << "\n";
    if (this->left != nullptr) {
        this->left->Print(height + 2);
    }
}

void TNode::Insert(TAVLTree* tree, const TPair& valueToInsert, bool& flag) {
    if (valueToInsert.first == this->value.first) {
        flag = false;
        return;
    }
    if (valueToInsert.first.compare(this->value.first) < 0) {
        if (this->left == nullptr) {
            this->left = new TNode(valueToInsert, this);
            Rebalance(tree, valueToInsert.first);
        } else {
            this->left->Insert(tree, valueToInsert, flag);
        }
    } else {
        if (this->right == nullptr) {
            this->right = new TNode(valueToInsert, this);
            Rebalance(tree, valueToInsert.first);
        } else {
            this->right->Insert(tree, valueToInsert, flag);
        }
    }
}

void TNode::Rebalance(TAVLTree* tree, const string& key) {
    if (key.compare(this->value.first) < 0) {
        this->balance++;
    } else {
        this->balance--;
    }
    if (abs(this->balance) == 1) {
        if (this->parent != nullptr) {
            this->parent->Rebalance(tree, this->value.first);
        }
        return;
    }
    if (this->balance == -2) {
        if (this->right->balance == -1) {
            this->right->balance = 0;
            this->balance = 0;
            this->LeftRotate(tree);
        } else {
            switch (this->right->left->balance) {
                case -1:
                    this->balance = 1;
                    this->right->balance = 0;
                    break;
                case 0:
                    this->balance = 0;
                    this->right->balance = 0;
                    break;
                case 1:
                    this->balance = 0;
                    this->right->balance = -1;
                    break;
            }
            this->right->left->balance = 0;
            this->right->RightRotate(tree);
            this->LeftRotate(tree);
        }
        return;
    }
    if (this->balance == 2) {
        if (this->left->balance == 1) {
            this->left->balance = 0;
            this->balance = 0;
            this->RightRotate(tree);
        } else {
            switch (this->left->right->balance) {
                case -1:
                    this->balance = 0;
                    this->left->balance = 1;
                    break;
                case 0:
                    this->balance = 0;
                    this->left->balance = 0;
                    break;
                case 1:
                    this->balance = -1;
                    this->left->balance = 0;
                    break;
            }
            this->left->right->balance = 0;
            this->left->LeftRotate(tree);
            this->RightRotate(tree);
        }
        return;
    }
}

void TNode::LeftRotate(TAVLTree* tree) {
    TNode* new_root = this->right;
    this->right = new_root->left;
    if (new_root->left != nullptr) {
        new_root->left->parent = this;
    }
    new_root->parent = this->parent;
    if (this->parent == nullptr) {
        tree->SetRoot(new_root);
    } else if (this == this->parent->left) {
        this->parent->left = new_root;
    } else {
        this->parent->right = new_root;
    }
    new_root->left = this;
    this->parent = new_root;
}

void TNode::RightRotate(TAVLTree* tree) {
    TNode* new_root = this->left;
    this->left = new_root->right;
    if (new_root->right != nullptr) {
        new_root->right->parent = this;
    }
    new_root->parent = this->parent;
    if (this->parent == nullptr) {
        tree->SetRoot(new_root);
    } else if (this == this->parent->right) {
        this->parent->right = new_root;
    } else {
        this->parent->left = new_root;
    }
    new_root->right = this;
    this->parent = new_root;
}

void TNode::Remove(TAVLTree *tree, const string &key, bool &flag) {
    if (key.compare(this->value.first) < 0) {
        if (this->left != nullptr) {
            this->left->Remove(tree, key, flag);
        } else {
            flag = false;
            return;
        }
    } else if (key.compare(this->value.first) > 0){
        if (this->right != nullptr) {
            this->right->Remove(tree, key, flag);
        } else {
            flag = false;
            return;
        }
    } else {
        if (this->left == nullptr and this->right == nullptr) {
            if (this->parent == nullptr) {
                tree->SetRoot(nullptr);
                delete this;
                return;
            }
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = nullptr;
            } else {
                this->parent->right = nullptr;
            }
            this->parent->RemovalRebalance(tree, key);
            delete this;
            return;
        } else if (this->left != nullptr and this->right == nullptr) {
            if (this->parent == nullptr) {
                tree->SetRoot(this->left);
                this->left->parent = nullptr;
                delete this;
                return;
            }
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = this->left;
            } else {
                this->parent->right = this->left;
            }
            this->left->parent = this->parent;
            this->parent->RemovalRebalance(tree, key);
            delete this;
            return;
        } else if (this->left == nullptr and this->right != nullptr) {
            if (this->parent == nullptr) {
                tree->SetRoot(this->right);
                this->right->parent = nullptr;
                delete this;
                return;
            }
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = this->right;
            } else {
                this->parent->right = this->right;
            }
            this->right->parent = this->parent;
            this->parent->RemovalRebalance(tree, key);
            delete this;
            return;
        } else {
            this->value = this->right->FindMinimumAndRemove(tree);
            return;
        }
    }
}

void TNode::RemovalRebalance(TAVLTree *tree, const string &key) {
    if (key.compare(this->value.first) < 0) {
        this->balance--;
    } else {
        this->balance++;
    }
    if (this->balance == 0) {
        if (this->parent != nullptr) {
            this->parent->RemovalRebalance(tree, this->value.first);
        }
        return;
    }
    if (this->balance == -2) {
        if (this->right->balance == 0) {
            this->right->balance = 1;
            this->balance = -1;
            this->LeftRotate(tree);
        } else if (this->right->balance == -1) {
            this->right->balance = 0;
            this->balance = 0;
            this->LeftRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemovalRebalance(tree, this->value.first);
            }
        } else {
            switch (this->right->left->balance) {
                case -1:
                    this->balance = 1;
                    this->right->balance = 0;
                    break;
                case 0:
                    this->balance = 0;
                    this->right->balance = 0;
                    break;
                case 1:
                    this->balance = 0;
                    this->right->balance = -1;
                    break;
            }
            this->right->left->balance = 0;
            this->right->RightRotate(tree);
            this->LeftRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemovalRebalance(tree, this->value.first);
            }
        }
        return;
    }
    if (this->balance == 2) {
        if (this->left->balance == 0) {
            this->left->balance = -1;
            this->balance = 1;
            this->RightRotate(tree);
        } else if (this->left->balance == 1) {
            this->left->balance = 0;
            this->balance = 0;
            this->RightRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemovalRebalance(tree, this->value.first);
            }
        } else {
            switch (this->left->right->balance) {
                case -1:
                    this->balance = 0;
                    this->left->balance = 1;
                    break;
                case 0:
                    this->balance = 0;
                    this->left->balance = 0;
                    break;
                case 1:
                    this->balance = -1;
                    this->left->balance = 0;
                    break;
            }
            this->left->right->balance = 0;
            this->left->LeftRotate(tree);
            this->RightRotate(tree);
            if (this->parent->parent != nullptr) {
                this->parent->parent->RemovalRebalance(tree, this->value.first);
            }
        }
        return;
    }
}

TPair TNode::FindMinimumAndRemove(TAVLTree* tree) {
    if (this->left == nullptr) {
        TPair new_value = this->value;
        if (this->right != nullptr) {
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = this->right;
                this->right->parent = this->parent;
            } else {
                this->parent->right = this->right;
                this->right->parent = this->parent;
            }
        } else {
            if (this->value.first.compare(this->parent->value.first) < 0) {
                this->parent->left = nullptr;
            } else {
                this->parent->right = nullptr;
            }
        }
        this->parent->RemovalRebalance(tree, new_value.first);
        delete this;
        return new_value;
    }
    return this->left->FindMinimumAndRemove(tree);
}

unsigned long long TNode::Find(const string& key, bool& flag) {
    if (this->value.first == key) {
        return this->value.second;
    } else if (this->value.first.compare(key) > 0) {
        if (this->left != nullptr) {
            return this->left->Find(key, flag);
        } else {
            flag = false;
        }
    } else {
        if (this->right != nullptr) {
            return this->right->Find(key, flag);
        } else {
            flag = false;
        }
    }
}

void TNode::Save(ofstream &file) {
    size_t size = this->value.first.length();
    file.write(reinterpret_cast<char*>(&size), sizeof(size));
    file.write(this->value.first.c_str(), size);
    file.write(reinterpret_cast<char*>(&(this->value.second)), sizeof(this->value.second));
    if (this->left != nullptr) {
        this->left->Save(file);
    }
    if (this->right != nullptr) {
        this->right->Save(file);
    }
}

void TNode::Clear() {
    if (this->left != nullptr) {
        this->left->Clear();
    }
    if (this->right != nullptr) {
        this->right->Clear();
    }
    delete this;
}



int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    TAVLTree tree;
    TPair pair;
    string command;


    while (cin >> command) {
        if (command == "+") {
            string key;
            unsigned long long value;
            cin >> key >> value;
            transform(key.begin(), key.end(), key.begin(),
                      [](unsigned char c){ return tolower(c); });
            pair = make_pair(key, value);
            cout << tree.Insert(pair) << "\n";
        } else if (command == "-") {
            string key;
            cin >> key;
            transform(key.begin(), key.end(), key.begin(),
                      [](unsigned char c){ return tolower(c); });
            cout << tree.Remove(key) << "\n";
        } else if (command == "!") {
            string action, path;
            cin >> action >> path;
            if (action == "Save") {
                ofstream file(path, ios_base::binary | ios_base::trunc);
                tree.Save(file);
                file.close();
                cout << "OK\n";
            } else {
                ifstream file(path, ios_base::binary);
                tree.Load(file);
                file.close();
                cout << "OK\n";
            }
        } else {
            transform(command.begin(), command.end(), command.begin(),
                      [](unsigned char c){ return tolower(c); });
            cout << tree.Find(command) << "\n";
        }
    }

    tree.Clear();

}



