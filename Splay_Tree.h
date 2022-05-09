#include <iostream>
#include <map>
#include <algorithm>
#include <utility>
#include <utility>
#include <vector>
#include <random>

struct SplayTree {
    int key;
    SplayTree *left, *right, *parent;
    SplayTree(int tkey) {
        key = tkey;
        left = right = parent = nullptr;
    }
};


void set_parent(SplayTree *&child, SplayTree *&parent) {
    if (child != nullptr) {
        child->parent = parent;
    }
    return;
}

void keep_parent(SplayTree *&tr) {
    set_parent(tr->left, tr);
    set_parent(tr->right, tr);
    return;
}

SplayTree *zig(SplayTree *parent, SplayTree *tr) {
    SplayTree *gparent = parent->parent;
    if (gparent != nullptr) {
        if (gparent->left == parent) {
            gparent->left = tr;
        } else {
            gparent->right = tr;
        }
    }
    if (parent->left == tr) {
        SplayTree *tmp = tr->right;
        tr->right = parent;
        parent->left = tmp;
    }
    else {
        SplayTree *tmp = tr->left;
        tr->left = parent;
        parent->right = tmp;
    }
    keep_parent(tr);
    keep_parent(parent);
    set_parent(tr, gparent);
    return tr;
}

SplayTree *splay(SplayTree *tr) {
    if (tr->parent == nullptr) return tr;
    SplayTree *parent = tr->parent;
    SplayTree *gparent = parent->parent;
    if (gparent == nullptr) {
        tr = zig(parent, tr);
        return tr;
    }
    bool zigzig = (gparent->left == parent) == (parent->left == tr);
    if (zigzig) {
        parent = zig(gparent, parent);
        tr = zig(parent, tr);
    } else {
        tr = zig(parent, tr);
        tr = zig(gparent, tr);
    }
    return splay(tr);
}

SplayTree *insert(SplayTree *root, int key) {
    SplayTree *tmp = root;
    if (tmp) {
        while (true) {
            if (tmp->key == key) return root;
            if (tmp->key > key) {
                if (tmp->left == nullptr) {
                    tmp->left = new SplayTree(key);
                    set_parent(tmp->left, tmp);
                    root = splay(tmp->left);
                    return root;
                } else {
                    tmp = tmp->left;
                }
            } else {
                if (tmp->right == nullptr) {
                    tmp->right = new SplayTree(key);
                    set_parent(tmp->right, tmp);
                    root = splay(tmp->right);
                    return root;
                } else {
                    tmp = tmp->right;
                }
            }
        }
    }
    root = new SplayTree(key);
    return root;
}

SplayTree *merge(SplayTree *l, SplayTree *r) {
    if (l == nullptr) return r;
    if (r == nullptr) return l;
    SplayTree *tmp = l;
    while (tmp->right != nullptr) {
        tmp = tmp->right;
    }
    l = splay(tmp);
    l->right = r;
    set_parent(r, l);
    return l;
}

SplayTree *erase(SplayTree *root, int key) {
    SplayTree *tmp = root;
    while (tmp != nullptr) {
        if (tmp->key == key) {
            root = splay(tmp);
            SplayTree *tl = root->left, *tr = root->right;
            root = nullptr;
            delete root;
            if (tl) {
                tl->parent = nullptr;
            }
            if (tr) {
                tr->parent = nullptr;
            }
            return merge(tl, tr);
        } else if (tmp->key > key) {
            tmp = tmp->left;
        } else {
            tmp = tmp->right;
        }
    }
    return root;
}
