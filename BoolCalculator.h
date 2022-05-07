#include <iostream>
#include <vector>

using namespace std;
using namespace sf;


class BoolCalc {
    int id;
    string exp;

    bool cmb() {;
        if (id >= exp.size()) {
            error = 1;
        }
        if (error) return 0;
        if (exp[id] == '|') {
            ++id;
            return _or();
        }
        if (exp[id] == '&') {
            ++id;
            return _and();
        }
        return oper();
    }

    bool _or() {
        if (error) return 0;
        if (exp[id] == '(') {
            bool answer = false;
            while (1) {
                ++id;
                if (id >= exp.size()) {
                    break;
                }
                answer |= oper();
                if (exp[id] == ')') break;
                if (exp[id] != ',') {
                    error = 1;
                    ++id;
                    return 0;
                }
            }
            if (id == exp.size()) {
                error = 1;
                ++id;
                return 0;
            }
            ++id;
            return answer;
        }
        error = 1;
        return 0;
    }

    bool _and() {
        if (error) return 0;
        if (exp[id] == '(') {
            bool answer = true;
            while (1) {
                ++id;
                if (id >= exp.size()) {
                    break;
                }
                answer &= oper();
                if (exp[id] == ')') break;
                if (exp[id] != ',') {
                    error = 1;
                    ++id;
                    return 0;
                }
            }
            if (id == exp.size()) {
                error = 1;
                ++id;
                return 0;
            }
            ++id;
            return answer;
        }
        error = 1;
        return 0;
    }

    bool oper() {
        if (error) return 0;
        if (exp[id] == '0' || exp[id] == '1') {
            return exp[id++] - '0';
        }
        if (exp[id] == '!') {
            ++id;
            return !oper();
        }
        if (exp[id] == '(') {
            ++id;
            bool val = cmb();
            if (exp[id] == ')') {
                ++id;
                return val;
            } else {
                error = 1;
                ++id;
                return 0;
            }
        }
        error = 1;
        return 0;
    }
public:
    bool res;
    bool error;
    BoolCalc(string input) {
        exp = input;
        error = 0;
        id = 0;
        res = cmb();
        if (id != exp.size()) error = 1;
    }
};
