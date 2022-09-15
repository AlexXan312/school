#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>

std::fstream start_text, decode_text, tabl, tree, end_text;

struct list {
    list *prev, *next;
    int app, smb;
    std::vector<list *> ch;

    list(int _app, int _smb, list *_prev, list *_next) {
        app = _app, smb = _smb;
        prev = _prev, next = _next;
        ch.resize(2, nullptr);
    }
};

const int gym = 126;
std::string app;
bool was = false;
bool getted = false;

void read_generate(std::string &text) {
    if (getted) {
        text = app;
        return;
    }
    start_text.seekg(0);
    text = "";
    char c;
    while ((c = start_text.get()) != EOF) {
        if (c != '\r') text += c;
    }
    app = text;
    getted = true;
}

void generate() {

    std::string text;
    read_generate(text);
    std::map<char, int> cnt;

    struct list {
        list *prev, *next;
        int app, smb;
        std::vector<list *> ch;

        list(int _app, int _smb, list *_prev, list *_next) {
            app = _app, smb = _smb;
            prev = _prev, next = _next;
            ch.resize(2, nullptr);
        }
    };

    auto erase = [&](list *v) {
        if (v == nullptr) return;
        if (v->prev != nullptr) {
            (v->prev)->next = nullptr;
        }
        if (v->next != nullptr) {
            (v->next)->prev = nullptr;
        }
    };

    auto insert = [&](list *v, list *parent, list *child) {
        v->prev = parent;
        v->next = child;
        if (parent != nullptr) {
            parent->next = v;
        }
        if (child != nullptr) {
            child->prev = v;
        }
    };

    for (char &x : text) {
        ++cnt[x];
    }

    std::vector<std::pair<int, int>> arr;
    for (auto[smb, app] : cnt) {
        arr.push_back({app, (int) smb});
    }

    std::sort(arr.rbegin(), arr.rend());
    list *st = nullptr;
    for (auto[app, smb] : arr) {
        list *add = new list(app, smb, st, nullptr);
        if (st != nullptr) {
            st->next = add;
        }
        st = add;
    }
    int ans = 0;
    while (st->prev != nullptr) {
        list *last = (st->prev)->prev;
        list *left = st->prev, *right = st;
        erase(left), erase(right);
        ans += left->app + right->app;
        list *add = new list(left->app + right->app, gym, nullptr, nullptr);
        add->ch[0] = left, add->ch[1] = right;
        list *prev_last = nullptr;
        while (last != nullptr && last->app < add->app) {
            prev_last = last;
            last = last->prev;
        }
        insert(add, last, prev_last);
        while (add->next != nullptr) add = add->next;
        st = add;
    }

    std::function<void(list *)> dfs1 = [&](list *root) {
        if (root == nullptr) return;
        tree << char(root->smb);
        for (int i = 0; i < 2; ++i) {
            dfs1(root->ch[i]);
        }
    };

    dfs1(st);
    //tree << '\n';

    bool was = false;
    std::function<void(list *, std::string&)> dfs2 = [&](list *root, std::string &s) {
        if (root == nullptr) return;
        if (root->smb != gym) {
            if (was) tabl << '\n';
            was = true;
            tabl << char(root->smb) << s;
        }
        for (int i = 0; i < 2; ++i) {
            s += char('0' + i);
            dfs2(root->ch[i], s);
            s.pop_back();
        }
    };

    std::string s;
    dfs2(st, s);
    //tabl << '\n';
}

void read_decode(std::string &text, std::map<char, std::string> &tb) {
    read_generate(text);
    tabl.seekg(0);
    char c;
    while ((c = tabl.get()) != EOF) {
        std::string cur;
        cur += c;
        c = tabl.get();
        while (c != EOF && c != '\n') {
            if (c != '\r') cur += c;
            c = tabl.get();
        }
        tb[cur[0]] = cur.substr(1, cur.size() - 1);
    }
}

void decode() {
    std::string text;
    std::map<char, std::string> tb;
    read_decode(text, tb);
    for (char &ch : text) {
        decode_text << tb[ch];
    }
    //decode_text << '\n';
}

void read_encode(std::string &text, list *&tr) {
    decode_text.seekg(0);
    tree.seekg(0);
    decode_text >> text;
    std::string pre_order;
    char c;
    while ((c = tree.get()) != EOF) {
        if (c != '\r') pre_order += c;
    }
    std::vector<list *> st;
    tr = new list(0, pre_order[0], nullptr, nullptr);
    st.push_back(tr);
    for (int i = 1; i < pre_order.size(); ++i) {
        list *add = new list(0, pre_order[i], nullptr, nullptr);
        if (st.back()->ch[0] == nullptr) {
            (st.back())->ch[0] = add;
        } else {
            (st.back())->ch[1] = add;
            st.pop_back();
        }
        if (add->smb == gym) st.push_back(add);
    }
}

void encode() {
    std::string text;
    list *tr;
    read_encode(text, tr);
    list *now = tr;
    for (char &ch : text) {
        now = now->ch[ch - '0'];
        if (now->smb != gym) {
            end_text << char(now->smb);
            now = tr;
        }
    }
    //end_text << '\n';
}

int main() {
    //std::ofstream of1("/Users/alexsus/Desktop/Haffmun_Algorithm/f1.txt", std::ios::out |std::ios::trunc);
    //of1.close();
    //std::ofstream of2("/Users/alexsus/Desktop/f2.txt", std::ios::out |std::ios::trunc);
    //of2.close();
    //std::ofstream of3("/Users/alexsus/Desktop/f3.txt", std::ios::out |std::ios::trunc);
    //of3.close();
    //std::ofstream of4("/Users/alexsus/Desktop/f4.txt", std::ios::out |std::ios::trunc);
    //of4.close();
    //std::ofstream of5("/Users/alexsus/Desktop/f5.txt", std::ios::out |std::ios::trunc);
    //of5.close();
    start_text.open("/Users/alexsus/Desktop/f1.txt");
    decode_text.open("/Users/alexsus/Desktop/f2.txt");
    tabl.open("/Users/alexsus/Desktop/f3.txt");
    tree.open("/Users/alexsus/Desktop/f4.txt");
    end_text.open("/Users/alexsus/Desktop/f5.txt");
    int id;
    std::cin >> id;
    if (id == 1) generate();
    if (id == 2) decode();
    if (id == 3) encode();
    start_text.close();
    decode_text.close();
    tabl.close();
    tree.close();
    end_text.close();
    return 0;
}
