#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <set>
#include <cmath>

const int Alp = 26; // число переменных

struct Polynom { // инвариант: многочлены упорядочены лексиграфически по степеням
    double cf; // коэффициент
    std::vector<int> degs; // степени
    Polynom *child, *parent;
    Polynom(std::vector<int> _degs, double _cf) {
        degs = _degs, cf = _cf;
        parent = child = nullptr;
    }
};

Polynom *rotate(Polynom *to) { // перевод многочлена в начало
    if (to == nullptr) return nullptr;
    while (to->parent != nullptr) {
        to = to->parent;
    }
    return to;
}

Polynom *to_end(Polynom *to) {
    if (to == nullptr) return nullptr;
    while (to->child != nullptr) {
        to = to->child;
    }
    return to;
}

void set(Polynom *&child, Polynom *&parent) { // перевод указателей
    child->parent = parent;
    parent->child = child;
    return;
}

void clear(Polynom *&x) {
    if (x == nullptr) return;
    clear(x->child);
    x->parent = x->child = nullptr;
    delete x;
    x = nullptr;
    return;
}

Polynom *add(Polynom *left, Polynom *right) { // сумма
    std::map<std::vector<int>, double> keys;
    while (left != nullptr) {
        keys[left->degs] += left->cf;
        left = left->child;
    }
    while (right != nullptr) {
        keys[right->degs] += right->cf;
        right = right->child;
    }
    Polynom *res = nullptr;
    for (auto [_degs, _cf] : keys) { // поддержание инварианта вследствие свойств map
        if (_cf == 0) continue;
        Polynom *to = new Polynom(_degs, _cf);
        if (res != nullptr) {
            set(to, res);
        }
        res = to;
    }
    return rotate(res);
}

Polynom *diff(Polynom *left, Polynom *right) { // вычитание
    if (right == nullptr) return left;
    while (true) {
        right->cf *= -1;
        if (right->child == nullptr) {
            break;
        }
        right = right->child;
    }
    right = rotate(right);
    return add(left, right);
}

Polynom *mul(Polynom *left, Polynom *right) { // умножение
    std::map<std::vector<int>, double> keys;
    while (left) {
        Polynom *to = right;
        while (to) {
            std::vector<int> sum(Alp, 0);
            for (int i = 0; i < Alp; ++i) {
                sum[i] = left->degs[i] + to->degs[i];
            }
            keys[sum] += left->cf * to->cf;
            to = to->child;
        }
        left = left->child;
    }
    Polynom *res = nullptr;
    for (auto [_degs, _cf] : keys) { // поддержание инварианта вследствие свойст map
        if (_cf == 0) continue;
        Polynom *to = new Polynom(_degs, _cf);
        if (res != nullptr) {
            set(to, res);
        }
        res = to;
    }
    return rotate(res);
}

std::pair<Polynom *, Polynom *> div(Polynom *left, Polynom *right) { // деление
    if (left == nullptr || right == nullptr) return {nullptr, nullptr}; // не определено деление
    std::set<int> pos1, pos2;
    while (true) {
        for (int i = 0; i < Alp; ++i) {
            if (left->degs[i] != 0) pos1.insert(i);
        }
        if (left->child == nullptr) break;
        left = left->child;
    }
    while (true) {
        for (int i = 0; i < Alp; ++i) {
            if (right->degs[i] != 0) pos2.insert(i);
        }
        if (right->child == nullptr) break;
        right = right->child;
    }
    try {
        if (pos1.size() > 1 || pos2.size() > 1) {
            throw 1;
        }
        int it;
        if (pos1.size() && pos2.size()) {
            int it1 = *pos1.begin();
            int it2 = *pos2.begin();
            if (it1 != it2) {
                throw 2;
            }
            it = it1;
        } else if (pos1.size()) {
            it = *pos1.begin();
        } else if (pos2.size()) {
            it = *pos2.begin();
        } else {
            it = 0;
        }
        Polynom *res = nullptr;
        while (left != nullptr && left->degs[it] >= right->degs[it]) {
            double ncf = left->cf / right->cf;
            int df = left->degs[it] - right->degs[it];
            std::vector<int> dg(Alp, 0);
            dg[it] = df;
            Polynom *to = new Polynom(dg, ncf);
            Polynom *cur = mul(rotate(right), to);
            if (res != nullptr) {
                set(res, to);
            }
            res = to;
            left = to_end(diff(rotate(left), cur));
            clear(cur);
        }
        return {rotate(res), rotate(left)};
    }
    catch (int e) {
        if (e == 1) {
            std::cerr << "too many arguments\n";
            return {nullptr, nullptr};
        }
        if (e == 2) {
            std::cerr << "different arguments\n";
            return {nullptr, nullptr};
        }
    }
}

double get(Polynom *p, std::vector<double> &pp) { // получение значения в точке
    double val = 0;
    while (p) {
        int key = p->cf;
        for (int i = 0; i < Alp; ++i) {
            key *= pow(pp[i], p->degs[i]);
        }
        val += key;
        p = p->child;
    }
    return val;
}

Polynom *derivative(Polynom *p, int pos) { // получение частной производной
    Polynom *res = nullptr;
    while (p) {
        std::vector<int> keys = p->degs;
        if (keys[pos] != 0) {
            double ncf = p->cf * keys[pos];
            --keys[pos];
            Polynom *dp = new Polynom(keys, ncf);
            if (res) {
                set(dp, res);
            }
            res = dp;
        }
        p = p->child;
    }
    return rotate(res);
}

std::set<int> get_roots(Polynom *p) { // получение целых корней
    std::set<int> s;
    std::set<int> cfs;
    Polynom *tp = p;
    bool check = 1;
    while (tp) {
        for (int i = 0; i < Alp; ++i) {
            if (tp->degs[i] != 0) s.insert(i);
        }
        check &= (p->cf == (int)p->cf);
        if (!check) break;
        cfs.insert((int)p->cf);
        tp = tp->child;
    }
    try {
        if (check == 0) {
            throw 1;
        }
        if (s.size() == 0) {
            throw 2;
        }
        if (s.size() > 2) {
            throw 3;
        } else {
            int pos = *s.begin();
            std::set<int> ans;
            std::vector<double> num(Alp);
            if (get(p, num) == 0) ans.insert(0);
            for (int x : cfs) {
                for (int j = 1; j * j <= x; ++j) {
                    if (x % j) continue;
                    std::vector<int> keys = {j, -j, x / j, -x / j};
                    for (int r : keys) {
                        num[pos] = r;
                        if (get(p, num) == 0) {
                            ans.insert(r);
                        }
                    }
                }
            }
            return ans;
        }
    }
    catch(int e) {
        if (e == 1) {
            std::cerr << "float coefficient\n";
        }
        if (e == 2) {
            std::cerr << "no arguments\n";
        }
        if (e == 3) {
            std::cerr << "too many arguments\n";
        }
    }
    return {};
}

Polynom * read(std::string &s) {
    int was = -1;
    int sign = 1; // по дефолту знак +
    int n = s.size();
    std::map<std::vector<int>, double> keys; // поддeржка инварианта
    for (int i = 0; i < n;) {
        if (s[i] == ' ') {
            ++i;
            continue;
        }
        if (is_letter(s[i]) || is_digit(s[i])) {
            try {
                if (was == 1) throw 1; // наличие существенных пробелов
            }
            catch (int e) {
                if (e == 1) {
                    std::cerr << "no sign after a term\n";
                    return nullptr;
                }
            }
            was = 1;
            double cf;
            if (s[i] >= '0' && s[i] <= '9') {
                cf = 0;
                while (i < n && is_digit(s[i])) {
                    cf *= 10;
                    cf += s[i] - '0';
                    ++i;
                }
            } else {
                cf = 1;
            }
            cf *= sign;
            std::vector<int> degs(Alp, 0);
            while (i < n && is_letter(s[i])) {
                int pos = s[i] - 'a';
                ++i;
                int deg;
                if (i < n && s[i] == '^') {
                    bool is = 0;
                    deg = 0;
                    ++i;
                    while (i < n && is_digit(s[i])) {
                        is = 1;
                        deg *= 10;
                        deg += s[i] - '0';
                        ++i;
                    }
                    try {
                        if (!is) {
                            throw 1;
                        }
                    }
                    catch (int e) {
                        std::cerr << "no pow after ^\n";
                        return nullptr;
                    }
                } else {
                    deg = 1;
                }
                degs[pos] += deg;
            }
            keys[degs] += cf;
            continue;
        }
        if (s[i] == '+' || s[i] == '-') {
            try {
                if (was == 0) {
                    throw 1;
                }
            }
            catch(int e) {
                if (e == 1) {
                    std::cerr << "too many signs\n";
                    return nullptr;
                }
            }
            sign = (s[i] == '+' ? 1 : -1);
            was = 0;
            ++i;
            continue;
        }
        std::cerr << "unexpected character\n";
        return nullptr;
    }
    try {
        if (was == 0) {
            throw 1;
        }
    }
    catch(int e) {
        if (e == 1) {
            std::cerr << "no terms after a sign\n";
            return nullptr;
        }
    }
    Polynom *res = nullptr;
    for (auto [_degs, _cf] : keys) { // поддержание инварианта вследствие свойст map
        if (_cf == 0) continue;
        Polynom *to = new Polynom(_degs, _cf);
        if (res != nullptr) {
            set(to, res);
        }
        res = to;
    }
    return rotate(res);
}

