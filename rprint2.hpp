#pragma once
#if __cplusplus < 201400
C++14 is required.
#else
#include<bitset>
#include<iomanip>
#include<iostream>
#include"rptree.hpp"

#include<chrono>
// #include<mutex>


namespace rprint2{

using std::string;
using std::true_type;
using std::false_type;
// using std::forward;
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
// H1(coutable2,   a >> std::cout);
H1(iterable,    (a.begin(), a.end()));
H1(strCastable, (string)a);
H1(isPair,      (a.first, a.second));
template <class A, size_t N>
true_type isArray(A (&)[N]);
template <class... A>
false_type isArray(A&&...);
template <class... A>
true_type isTuple(std::tuple<A...>);
template <class... A>
false_type isTuple(A&&...);
H1(isRoot,      (a->dests));

#define COND(func) std::is_same<true_type, decltype(func(std::forward<A>(a)))>::value
#define H2(func, cond)\
    template <class A>\
    auto func(A&& a) -> std::enable_if_t<cond, void>
H2(rp_cout,     !COND(isArray) && !COND(isRoot) && COND(coutable));
// H2(rp_cout,     !COND(coutable2) && !COND(isArray) && !COND(isRoot) && COND(coutable));
// H2(rp_cout2,    !COND(isArray) && !COND(isRoot) && COND(coutable2));
H2(rp_str,      !COND(coutable) && COND(strCastable));
H2(rp_iter,     !COND(coutable) && !COND(strCastable) && COND(iterable));
H2(rp_pair,     COND(isPair));
// H2(rp_str,      !COND(coutable) && !COND(coutable2) && COND(strCastable));
// H2(rp_iter,     !COND(coutable) && !COND(coutable2) && !COND(strCastable) && COND(iterable));
// H2(rp_pair,     !COND(coutable2) && COND(isPair));
H2(rp_tuple,    COND(isTuple));
H2(rp_array,    COND(isArray));
H2(rp_root,     COND(isRoot));
H2(rp_other,    !COND(coutable) && !COND(strCastable) && !COND(iterable) && !COND(isPair) && !COND(isArray) && !COND(isRoot) && !COND(isTuple));
// H2(rp_other,    !COND(coutable) && !COND(coutable2) && !COND(strCastable) && !COND(iterable) && !COND(isPair) && !COND(isArray) && !COND(isRoot) && !COND(isTuple));

#undef COND
#undef H2

template <class F, class A>
void __print(ostream& out, F&& f, A&&);

//--------------------------------------------------------------------------------

#define CHRONO(unit, ex)\
    template <class F>\
    auto _print(ostream& out, F&& f, std::chrono:##unit&& a){\
        f(out, a.count());\
        f(out, #ex);\
    }
CHRONO(:hours, h)
CHRONO(:minutes, min)
CHRONO(:seconds, s)
CHRONO(:microseconds, us)
CHRONO(:milliseconds, ms)
CHRONO(:nanoseconds, ns)
#undef CHRONO

//--------------------------------------------------------------------------------

template <class F, class I>
void print_container(ostream& out, F&& f, I&& b, I&& e);
template <class A>
void print_something(ostream& out, A&& a);
template <int N = 0, class F, class... S>
auto print_tuple(ostream& out, F&& f, std::tuple<S...> t) -> std::enable_if_t< N >= sizeof...(S), void>;
template <int N = 0, class F, class... S>
auto print_tuple(ostream& out, F&& f, std::tuple<S...> t) -> std::enable_if_t< N < sizeof...(S), void>;

template <class F, class T>
void _print(ostream& out, F&&, Raw<T> r){
    out << r.t;
}

template <class F, class A>
auto _print(ostream& out, F&& f, A&& a) -> decltype(rp_cout(a)){
    f(out, a);
}
//TODO
// template <class F, class A>
// auto _print(ostream& out, F&&, A&& a) -> decltype(rp_cout2(a)){
//     a >> out;
// }
template <class F, class A>
auto _print(ostream& out, F&& f, A&& a) -> decltype(rp_str(a)){
    f(out, (string)a);
}
template <class F, class P>
auto _print(ostream& out, F&& f, P&& p) -> decltype(rp_pair(p)){
    __print(out, f, Char{'{'}, p.first, Char{','}, p.second, Char{'}'});
}
template <class F, class A>
auto _print(ostream& out, F&& f, A&& a) -> decltype(rp_root(a)){
    printTree(out, f, a);
}
template <class F, class A, size_t N>
void _print(ostream& out, F&& f, A (&a)[N]){
    print_container(out, f, &a[0], &a[0] + N);
}
template <class F, class C>
auto _print(ostream& out, F&& f, C&& c) -> decltype(rp_iter(c)){
    print_container(out, f, c.begin(), c.end());
}
template <class F, size_t N>
void _print(ostream& out, F&& f, const char (&a)[N]){
    f(out, a);
}
template <class F, class T>
auto _print(ostream& out, F&& f, T&& t) -> decltype(rp_tuple(t)){
    // __print(out, f, Char{'{'}, p.first, Char{','}, p.second, Char{'}'});
    f(out, '{');
    print_tuple(out, f, t);
    f(out, '}');
}
template <class F, class A>
auto _print(ostream& out, F&&, A&& a) -> decltype(rp_other(a)){
    out << "dt " << decltype(coutable(a))::value << '\n';
    print_something(out, a);
}

template <class F, class I>
auto print_containerT(ostream& out, F&& f, I&& b, I&& e){
    for(auto& i = b; i != e; i++){
        __print(out, f, Char{' '}, *i);
    }
}
static int cnt = 0;
constexpr int colorLen = 5;
static const String reset{"\e[00m"};
static const String colors[colorLen] = {{"\e[36m"}, {"\e[32m"}, {"\e[31m"}, {"\e[35m"}, {"\e[33m"}};
template <class F, class I>
void print_container(ostream& out, F&& f, I&& b, I&& e){
    if(cnt){
        __print(out, f, Char{'\n'});
        for(int i = 0; i < cnt; i++){
            __print(out, f, Char{'	'});
        }
    }
    __print(out, f, colors[cnt], Char{'['}, reset);
    cnt = (cnt + 1) % colorLen;
    if(b != e){
        __print(out, f, *b);
        print_containerT(out, f, ++b, e);
    }
    cnt = (colorLen + cnt - 1) % colorLen;
    __print(out, f, colors[cnt], Char{']'}, reset);
}

template <class T, class T2 = T, class A>
void __print_something(ostream& out, uint bits, A&& arr){
    for(uint i = 0; i < bits / sizeof(T); i++){
        out << std::setw(4 * sizeof(T)) << (T2)((T*)arr)[i];
    }
    out << '\n';
}
template <class A>
void print_something(ostream& out, A&& a){
    out << '\n';
    out << std::setw(2 * sizeof(a)) << std::setfill('_') << "something size ";
    out << std::setw(2 * sizeof(a)) << std::left << sizeof(a) << std::setfill(' ') << std::right << '\n';
    // out << std::setw(2 * sizeof(a)) << sizeof(a) << std::setfill(' ') << '\n';
    auto ptr = (unsigned char*)(void*)&a;
    out << std::hex;
    __print_something<uint8_t, uint32_t>(out, sizeof(a), ptr);
    out << std::dec;
    __print_something<uint8_t, uint32_t>(out, sizeof(a), ptr);
    __print_something<uint32_t>(out, sizeof(a), ptr);
    __print_something<double>(out, sizeof(a), ptr);
}
template <int N, class F, class... S>
auto print_tuple(ostream&, F&&, std::tuple<S...>) -> std::enable_if_t< N >= sizeof...(S), void> {
}
template <int N, class F, class... S>
auto print_tuple(ostream& out, F&& f, std::tuple<S...> t) -> std::enable_if_t< N < sizeof...(S), void> {
    __print(out, f, std::get<N>(t), Char{','});
    print_tuple<N + 1>(out, f, t);
}

//--------------------------------------------------------------------------------

template <class F, class A>
void __print(ostream& out, F&& f, A&& a){
    _print(out, f, std::forward<A>(a));
}
template <class F, class A, class... As>
void __print(ostream& out, F&& f, A&& a, As&&... as){
    _print(out, f, std::forward<A>(a));
    __print(out, f, std::forward<As>(as)...);
}

template <class F>
void __prints(ostream& out, F&& f){
    __print(out, f, Char{'\n'});
}
template <class F, class A>
void __prints(ostream& out, F&& f, A&& a){
    __print(out, f, std::forward<A>(a), Char{'\n'});
}
template <class F, class A1, class... As>
void __prints(ostream& out, F&& f, A1&& a, As&&... as){
    __print(out, f, std::forward<A1>(a), Char{' '});
    __prints(out, f, std::forward<As>(as)...);
}

//--------------------------------------------------------------------------------

enum{
    trace = 1, debug, info, warn, error, fatal, off,
    TRACE = 1, DEBUG, INFO, WARN, ERROR, FATAL, OFF,
};

#ifndef RP_LOGLEVEL
#define RP_LOGLEVEL info
#endif
#define DEFAULT_LEVEL int Level = info
#define COND -> std::enable_if_t< Level >= RP_LOGLEVEL, void>
#define COND2 -> std::enable_if_t< Level < RP_LOGLEVEL, void>

// TODO
// printd<rprint2::WARN>(123);

template <DEFAULT_LEVEL, class... As>
auto print(As&&... as) COND {
    __print(std::cout, [](ostream& out, auto e){ out << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printe(As&&... as) COND {
    __prints(std::cerr, [](ostream& out, auto e){ out << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto prints(As&&... as) COND {
    __prints(std::cout, [](ostream& out, auto e){ out << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printe0(As&&... as) COND {
    __print(std::cerr, [](ostream& out, auto e){ out << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printse(As&&... as) COND {
    __prints(std::cerr, [](ostream& out, auto&& e){ out << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printw(int width, As&&... as) COND {
    __prints(std::cout, [width](ostream& out, auto e){ out << std::setw(width) << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printwe(int width, As&&... as) COND {
    __prints(std::cerr, [width](ostream& out, auto e){ out << std::setw(width) << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printwe0(int width, As&&... as) COND {
    __print(std::cerr, [width](ostream& out, auto e){ out << std::setw(width) << e; }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printb(As&&... as) COND {
    __prints(std::cout, [](ostream& out, auto e){ out << std::bitset<64>(e); }, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class... As>
auto printbe(As&&... as) COND {
    __prints(std::cerr, [](ostream& out, auto e){ out << std::bitset<8 * sizeof(e)>(e); }, std::forward<As>(as)...);
}
// template <DEFAULT_LEVEL, class... As>
// auto printbe(As&&... as) COND {
//     __prints(std::cerr, [](ostream& out, auto e){ out << std::bitset<64>(e); }, std::forward<As>(as)...);
// }
template <DEFAULT_LEVEL, class F, class... As>
auto printu(ostream& out, F&& f, As&&... as) COND {
    __prints(out, f, std::forward<As>(as)...);
}
template <DEFAULT_LEVEL, class F, class... As>
auto printu0(ostream& out, F&& f, As&&... as) COND {
    __print(out, f, std::forward<As>(as)...);
}

template <DEFAULT_LEVEL, class... As> auto prints(As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto print(As&&...) COND2 { }
// template <DEFAULT_LEVEL, class... As> auto printe() COND2 { }
template <DEFAULT_LEVEL, class... As> auto printe(As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto printe0(As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto printse(As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto printw(int, As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto printwe(int, As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto printwe0(int, As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto printb(As&&...) COND2 { }
template <DEFAULT_LEVEL, class... As> auto printbe(As&&...) COND2 { }
template <DEFAULT_LEVEL, class F, class... As> auto printu(ostream&, F&&, As&&...) COND2 { }
template <DEFAULT_LEVEL, class F, class... As> auto printu0(ostream&, F&&, As&&...) COND2 { }

#undef DEFAULT_LEVEL
#undef COND
#undef COND2

} //namespace rprint2

//--------------------------------------------------------------------------------

#ifdef LOCAL

using rprint2::print;
using rprint2::printe;
using rprint2::prints;
using rprint2::printse;
using rprint2::printe0;
// using rprint2::printle;
using rprint2::printw;
using rprint2::printwe;
using rprint2::printwe0;
using rprint2::printb;
using rprint2::printbe;
// using rprint2::printo;
using rprint2::printu;
using rprint2::printu0;

#define printWarn
#define printError

#define printd(...)                 prints(#__VA_ARGS__, ':', __VA_ARGS__)
#define printde(...)                printse(#__VA_ARGS__, ':', __VA_ARGS__)
#define printdbe(...)               printe0(#__VA_ARGS__, ": "); printbe(__VA_ARGS__)
#define printdwe(w, ...)            printe0(#__VA_ARGS__, ": "); printwe((w), __VA_ARGS__)
#define printdu(arg1, arg2, ...)    printu(arg1, arg2, #__VA_ARGS__, ':', __VA_ARGS__)
#define printd4(...)                prints<4>(#__VA_ARGS__, ':', __VA_ARGS__)
#define printde4(...)               printse<4>(#__VA_ARGS__, ':', __VA_ARGS__)
#define printd5(...)                prints<5>(#__VA_ARGS__, ':', __VA_ARGS__)
#define printde5(...)               printse<5>(#__VA_ARGS__, ':', __VA_ARGS__)

#endif

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
    printse(1, 2, 3);
    printw(2, 1, 2, 3);
    // printew(2, 1, 2, v);
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

    struct E2 {
        double v = 1.2;
        pair<int, int> p = {2, 3};
    } a;
    prints(a, a);
    prints(std::make_tuple(1, 3, "abc"));
    prints<rprint2::debug>(1);

    for(int i = 0; i < 3; i++){
        printe0(i);
    }
    printe();
}

#endif

#endif
