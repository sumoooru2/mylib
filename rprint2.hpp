#pragma once
#include<iomanip>
#include<iostream>
#include"rptree.hpp"

// #include<chrono>
// #include<mutex>

namespace rprint2{

using std::string;
using std::true_type;
using std::false_type;
using std::forward;
using std::ostream;

template <class T>
struct Raw{ T t; };
using Char = Raw<char>;
using String = Raw<const char*>;

#define H1(func, cond)\
    template <class A>\
    auto func(A&& a) -> decltype(cond, true_type());\
    template <class... A>\
    false_type func(A&&...)
H1(coutable,    std::cout << a);
H1(iterable,    (a.begin(), a.end()));
H1(strCastable, (string)a);
H1(isPair,      (a.first, a.second));
template <class A, size_t N>
true_type isArray(A (&)[N]);
template <class... A>
false_type isArray(A&&...);
H1(isRoot,      (a->dests));

#define COND(func) std::is_same<true_type, decltype(func(forward<A>(a)))>::value
#define H2(func, cond)\
    template <class A>\
    auto func(A&& a) -> std::enable_if_t<cond, void>
H2(rp_cout,     !COND(isArray) && !COND(isRoot) && COND(coutable));
H2(rp_str,      !COND(coutable) && COND(strCastable));
H2(rp_iter,     !COND(coutable) && COND(iterable));
H2(rp_pair,     COND(isPair));
H2(rp_array,    COND(isArray));
H2(rp_root,    COND(isRoot));

template <class F, class A>
void _print(ostream& out, F&& f, A&&);

//--------------------------------------------------------------------------------

#define CHRONO(unit, ex)\
    template <class F>\
    auto print(ostream& out, F&& f, std::chrono:##unit&& a){\
        f(out, a.count());\
        f(out, #ex);\
    }
CHRONO(:hours, h)
CHRONO(:minutes, min)
CHRONO(:seconds, s)
CHRONO(:microseconds, us)
CHRONO(:milliseconds, ms)
CHRONO(:nanoseconds, ns)

//--------------------------------------------------------------------------------

template <class F, class I>
auto print_container(ostream& out, F&& f, I&& b, I&& e);

template <class F, class T>
void print(ostream& out, F&&, Raw<T> r){
    out << r.t;
}

template <class F, class A>
auto print(ostream& out, F&& f, A&& a) -> decltype(rp_cout(a)){
    f(out, a);
}
template <class F, class A>
auto print(ostream& out, F&& f, A&& a) -> decltype(rp_str(a)){
    f(out, (string)a);
}
template <class F, class P>
auto print(ostream& out, F&& f, P&& p) -> decltype(rp_pair(p)){
    _print(out, f, Char{'{'}, p.first, Char{','}, p.second, Char{'}'});
}
template <class F, class A>
auto print(ostream& out, F&& f, A&& a) -> decltype(rp_root(a)){
    printTree(out, f, a);
}
template <class F, class A, size_t N>
void print(ostream& out, F&& f, A (&a)[N]){
    print_container(out, f, &a[0], &a[0] + N);
}
template <class F, class C>
auto print(ostream& out, F&& f, C&& c) -> decltype(rp_iter(c)){
    print_container(out, f, c.begin(), c.end());
}
template <class F, size_t N>
void print(ostream& out, F&& f, const char (&a)[N]){
    f(out, a);
}

template <class F, class I>
auto print_containerT(ostream& out, F&& f, I&& b, I&& e){
    for(auto& i = b; i != e; i++){
        _print(out, f, Char{' '}, *i);
    }
}
static int cnt = 0;
constexpr int colorLen = 5;
static const String reset{"\e[00m"};
static const String colors[colorLen] = {{"\e[36m"}, {"\e[32m"}, {"\e[31m"}, {"\e[35m"}, {"\e[33m"}};
template <class F, class I>
auto print_container(ostream& out, F&& f, I&& b, I&& e){
    if(cnt){
        _print(out, f, Char{'\n'});
        for(int i = 0; i < cnt; i++){
            _print(out, f, Char{'	'});
        }
    }
    _print(out, f, colors[cnt], Char{'['}, reset);
    cnt = (cnt + 1) % colorLen;
    if(b != e){
        _print(out, f, *b);
        print_containerT(out, f, ++b, e);
    }
    cnt = (colorLen + cnt - 1) % colorLen;
    _print(out, f, colors[cnt], Char{']'}, reset);
}

//--------------------------------------------------------------------------------

template <class F, class A>
void _print(ostream& out, F&& f, A&& a){
    print(out, f, forward<A>(a));
}
template <class F, class A, class... As>
void _print(ostream& out, F&& f, A&& a, As&&... as){
    print(out, f, forward<A>(a));
    _print(out, f, forward<As>(as)...);
}

template <class F, class A>
void _prints(ostream& out, F&& f, A&& a){
    _print(out, f, forward<A>(a), Char{'\n'});
}
template <class F, class A1, class... As>
void _prints(ostream& out, F&& f, A1&& a, As&&... as){
    _print(out, f, forward<A1>(a), Char{' '});
    _prints(out, f, forward<As>(as)...);
}

//--------------------------------------------------------------------------------

template <class... As>
void prints(As&&... as){
    _prints(std::cout, [](ostream& out, auto e){ out << e; }, forward<As>(as)...);
}
template <class... As>
void printe(As&&... as){
    _prints(std::cerr, [](ostream& out, auto e){ out << e; }, forward<As>(as)...);
}
template <class... As>
void printw(int width, As&&... as){
    _prints(std::cout, [width](ostream& out, auto e){ out << std::setw(width) << e; }, forward<As>(as)...);
}
template <class... As>
void printew(int width, As&&... as){
    _prints(std::cerr, [width](ostream& out, auto e){ out << std::setw(width) << e; }, forward<As>(as)...);
}
template <class... As>
void printb(As&&... as){
    _prints(std::cout, [](ostream& out, auto e){ out << std::bitset<64>(e); }, forward<As>(as)...);
}
template <class F, class... As>
void printu(ostream& out, F&& f, As&&... as){
    _prints(out, f, forward<As>(as)...);
}


} //namespace rprint2

//--------------------------------------------------------------------------------

using rprint2::prints;
using rprint2::printe;
using rprint2::printw;
using rprint2::printew;
using rprint2::printb;
using rprint2::printu;

#define printd(...) prints(#__VA_ARGS__, __VA_ARGS__)

//--------------------------------------------------------------------------------

#ifdef RP_DEBUG
#include<map>
#include<numeric>
#include<random>
#include<set>
#include<unordered_map>
#include<vector>
// #include<chrono>
// #include<mutex>
struct Elem {
    int a;
    operator std::string() {
        return "a";
    }
};

struct Node{
    int value = 1;
    std::vector<Node*> dests;
    void add(Node* n){
        dests.push_back(n);
    }
    void add2(Node* n){
        add(n);
        n->add(this);
    }
    void dfs(Node* from = nullptr){
        for(Node* d : dests){
            if(d == from){ continue; }
            d -> dfs(this);
        }
    }
};

inline void testPrint(){
    using namespace std;
    prints(1);
    prints(1, 2, 3);
    vector<int> v = {1, 2, 3};
    prints(v);
    string s = "abc";
    prints(s);
    prints("abc");
    pair<int, int> p = {1, 2};
    prints(p);
    unordered_map<int, int> um;
    um[1] = 2;
    printd(um);
    Elem e;
    prints(e);
    int a1[] = {1, 2, 3, 4};
    prints(a1);
    int a3[2][3][4];
    prints(a3);
    printw(10, a3);
    
    vector<int*> vp = {&v[0], &v[1], &v[2]};
    vector<vector<int>> vv(3, v);
    vector<vector<vector<int>>> vvv(3, vv);
    map<int, int> mp = {{1, 2}, {4, 5}};
    auto pvm = make_pair(vv, mp);
    set<decltype(pvm)> spvm{pvm};
    prints(vvv);
    prints(mp);
    prints(pvm);
    prints(spvm);
    set<pair<int, int>> sp = {{1, 2}};
    prints(sp);
    prints(1h, 1min, 1s, 1ms, 1us, 1ns);
    prints(1, 2, 3);
    printe(1, 2, 3);
    printw(2, 1, 2, 3);
    printew(2, 1, 2, v);
    printu(cout, [](ostream& out, auto e){ out << setw(3) << e; }, 1, 2, 3);

    vector<Node> nodes(10);
    mt19937 mt(1);
    for(int i = 0; i < 10; i++){
        nodes[i].value = i;
        nodes[i].value = mt() % 10000;
    }
    nodes[0].add2(&nodes[1]);
    nodes[0].add2(&nodes[2]);
    nodes[1].add2(&nodes[3]);
    nodes[1].add2(&nodes[4]);
    nodes[2].add2(&nodes[5]);
    nodes[2].add2(&nodes[6]);
    nodes[6].add2(&nodes[7]);
    nodes[1].add2(&nodes[8]);
    // printTree(cout, &nodes[0]);
    prints(&nodes[0], &nodes[1]);
}

#endif
