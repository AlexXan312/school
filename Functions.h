#include <cmath>
#include <iomanip>
#include <string>

using namespace std;

using ld = float;
const ld eps = 0.05;
const ld Exp = exp(1.0);
const ld Pi = acos(-1);

const ld INF = 1e5;

int type(char c) {
    if (c == '(') return -1;
    if (c == '+' || c == '-') return 0;
    if (c == '*' || c == '/') return 1;
    if (c == '^') return 2;
    if (c == 'q' || c == 'l' || c == 's' || c == 'c') return 3;
    return -2;
}

struct Node {
    string val;
    Node* lhs = nullptr;
    Node* rhs = nullptr;
    void Clear() {
        if (lhs != nullptr) {
            lhs->Clear();
            delete lhs;
        }
        if (rhs != nullptr) {
            rhs->Clear();
            delete rhs;
        }
    }
    Node(string operation) {
        val = operation;
    }
};

Node* Reclear(vector<Node*> &nodes) {
    while (!nodes.empty()) {
        Node* t = nodes.back();
        t->Clear();
        delete t;
        nodes.pop_back();
    }
    return nullptr;
}

void AddUnary(vector<Node*> &nodes, string opr) {
    Node *cur = new Node(opr);
    cur->lhs = nodes.back();
    nodes.pop_back();
    nodes.push_back(cur);
    return;
}

void AddBinary(vector<Node*> &nodes, std::string operation) {
    Node *cur = new Node(operation);
    Node *r = nodes.back();
    nodes.pop_back();
    Node* l = nodes.back();
    nodes.pop_back();
    cur->lhs = l;
    cur->rhs = r;
    nodes.push_back(cur);
    return;
}

string OperBy(char c) {
    if (c == 's') return "sin";
    if (c == 'c') return "cos";
    if (c == '+') return "plus";
    if (c == '-') return "minus";
    if (c == 'l') return "ln";
    if (c == 'q') return "sqrt";
    if (c == '*') return "multiply";
    if (c == '/') return "divide";
    if (c == '^') return "power";
}

Node* Polis(string redo) {
    redo = '(' + redo + ')';
    vector<Node*> ns;
    vector<char> op;
    for (int i = 0; i < redo.size(); ++i) {
        if (redo[i] == ' ') continue;
        if (redo[i] == '(') {
            op.push_back('(');
        }
        else if (redo[i] == ')') {
            while (!op.empty() && op.back() != '(') {
                char c = op.back();
                op.pop_back();
                if (c == 'q' || c == 'l'
                || c == 's' || c == 'c') {
                    if (ns.empty()) {
                        return Reclear(ns);
                    }
                    AddUnary(ns, OperBy(c));
                } else {
                    if (ns.size() < 2) {
                        return Reclear(ns);
                    }
                    AddBinary(ns, OperBy(c));
                }
            }
            if (op.empty()) {
                return Reclear(ns);
            }
            op.pop_back();
        } else if (redo[i] == 'x') {
            Node* q = new Node("x");
            ns.push_back(q);
        } else if (redo[i] >= '0' && redo[i] <= '9') {
            int res = redo[i] - '0';
            while (redo[i + 1] >= '0' && redo[i + 1] <= '9') {
                ++i;
                res = res * 10 + redo[i] - '0';
            }
            Node* q = new Node(to_string(res));
            ns.push_back(q);
        }
        else if (redo[i] == '+' || redo[i] == '-' ||
        redo[i] == '*' || redo[i] == '/' ||
        redo[i] == '*' || redo[i] == '^') {
            while (!op.empty() && type(redo[i]) < type(op.back())) {
                char c = op.back();
                op.pop_back();
                if (c == 'q' || c == 'l' || c == 's' || c == 'c') {
                    if (ns.size() < 1) {
                        return Reclear(ns);
                    }
                    AddUnary(ns, OperBy(c));
                }
                else {
                    if (ns.size() < 2) {
                        return Reclear(ns);
                    }
                    AddBinary(ns, OperBy(c));
                }
            }
            op.push_back(redo[i]);
        }
        else if (redo[i] == 's') {
            if (redo.size() - i >= 5 && redo[i + 1] == 'q' && redo[i + 2] == 'r'
            && redo[i + 3] == 't' && redo[i + 4] == '(') {
                op.push_back('q');
                i += 3;
            }
            else if (redo.size() - i >= 4 && redo[i + 1] == 'i' && redo[i + 2] == 'n' && redo[i + 3] == '(') {
                op.push_back('s');
                i += 2;
            }
            else {
                return Reclear(ns);
            }
        }
        else if (redo[i] == 'l') {
            if (redo.size() - i >= 3 && redo[i + 1] == 'n' && redo[i + 2] == '(') {
                op.push_back('l');
                i += 1;
            }
            else {
                return Reclear(ns);
            }
        }
        else if (redo[i] == 'c') {
            if (redo.size() - i >= 4 && redo[i + 1] == 'o' && redo[i + 2] == 's' && redo[i + 3] == '(') {
                op.push_back('c');
                i += 2;
            }
            else {
                return Reclear(ns);
            }
        }
    }
    if (op.size() != 0 || ns.size() != 1) {
        return Reclear(ns);
    }
    return ns.back();
}

ld Get(ld x, Node* T) {
    if (T == nullptr) return INF;
    if (T->val == "x") {
        return x;
    }
    if (T->val[0] >= '0' && T->val[0] <= '9') {
        return 1.0 * stoi(T->val);
    }
    ld a = Get(x, T->lhs);
    ld b = Get(x, T->rhs);
    if (T->val == "plus") {
        return a + b;
    }
    if (T->val == "minus") {
        return a - b;
    }
    if (T->val == "multiply") {
        return a * b;
    }
    if (T->val == "divide") {
        if (b != 0) return 1.0 * a / b;
        return INF;
    }
    if (T->val == "sin") {
        return sin(a);
    }
    if (T->val == "cos") {
        return cos(a);
    }
    if (T->val == "ln") {
        if (a > 0) return log(a);
        return INF;
    }
    if (T->val == "sqrt") {
        if (a >= 0) return sqrt(a);
        return INF;
    }
    if (T->val == "power") {
        return pow(a, b);
    }
}
