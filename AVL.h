#include <iostream>
#include <map>
#include <algorithm>
#include <utility>
#include <vector>

bool operator<(std::pair<int, int> &left, std::pair<int, int> &other) {
    if (left.second - left.first + 1 == other.second - other.first + 1) {
        return left.first < other.first;
    }
    return left.second - left.first + 1 > other.second - other.first + 1;
}

bool operator==(std::pair<int, int> &left, std::pair<int, int> &other) {
    return left.first == other.first && left.second == other.second;
}

struct Avl {
    std::pair<int, int> key;
    Avl *left;
    Avl *right;
    int depth;
    Avl(std::pair<int, int> tkey) {
        key = tkey;
        left = right = nullptr;
        depth = 1;
        return;
    }
};

int depth(Avl *tr) {
    if (tr == nullptr) {
        return 0;
    }
    return tr->depth;
}

Avl *rightRotate(Avl *ty) {
    Avl *tx = ty->left;
    Avl *TR = tx->right;
    tx->right = ty;
    ty->left = TR;
    ty->depth = std::max(depth(ty->left), depth(ty->right)) + 1;
    tx->depth = std::max(depth(tx->left), depth(tx->right)) + 1;
    return tx;
}

Avl *leftRotate(Avl *tx) {
    Avl *ty = tx->right;
    Avl *TR = ty->left;
    ty->left = tx;
    tx->right = TR;
    tx->depth = std::max(depth(tx->left), depth(tx->right)) + 1;
    ty->depth = std::max(depth(ty->left), depth(ty->right)) + 1;
    return ty;
}

int getBalance(Avl *tr) {
    if (tr == nullptr)
        return 0;
    return depth(tr->left) - depth(tr->right);
}

Avl* insert(Avl* node, std::pair<int, int> key)  {
    if (node == nullptr) {
        Avl *cur = new Avl(key);
        return cur;
    }
    if (key < node->key) {
        node->left = insert(node->left, key);
    } else {
        node->right = insert(node->right, key);
    }
    node->depth = 1 + std::max(depth(node->left), depth(node->right));
    int balance = getBalance(node);
    if (balance > 1 && key < node->left->key) {
        return rightRotate(node);
    }
    if (balance < -1 && node->right->key < key) {
        return leftRotate(node);
    }
    if (balance > 1 && node->left->key < key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && node->right->key < key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Avl *minValueNode(Avl* node) {
    Avl* current = node;
    while (current->left != nullptr)
        current = current->left;

    return current;
}

Avl* erase(Avl* root, std::pair<int, int> key) {
    if (root == nullptr) {
        return root;
    }
    if (key < root->key) {
        root->left = erase(root->left, key);
    } else if (root->key < key) {
        root->right = erase(root->right, key);
    } else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Avl *temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            Avl* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = erase(root->right, temp->key);
        }
    }
    if (root == nullptr) {
        return root;
    }
    root->depth = 1 + std::max(depth(root->left), depth(root->right));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0) {
        return rightRotate(root);
    }
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) {
        return leftRotate(root);
    }
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}
