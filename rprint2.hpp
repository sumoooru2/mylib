#pragma once
#include<iostream>
#include<vector>
#include<iomanip>
#include<queue>
#include<unordered_set>

#include<numeric>
#include<map>
#include<set>
#include<chrono>
#include<mutex>

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

#define COND(func) std::is_same<true_type, decltype(func(forward<A>(a)))>::value
#define H2(func, cond)\
    template <class A>\
    auto func(A&& a) -> std::enable_if_t<cond, void>
H2(rp_cout,     !COND(isArray) && COND(coutable));
H2(rp_str,      !COND(coutable) && COND(strCastable));
H2(rp_iter,     !COND(coutable) && COND(iterable));
H2(rp_pair,     COND(isPair));
H2(rp_array,    COND(isArray));

template <class F, class A>
void printOne(ostream& out, F&& f, A&&);

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
    printOne(out, f, Char{'{'});
    printOne(out, f, p.first);
    printOne(out, f, Char{','});
    printOne(out, f, p.second);
    printOne(out, f, Char{'}'});
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
        printOne(out, f, Char{' '});
        printOne(out, f, *i);
    }
}
static int cnt = 0;
constexpr int colorLen = 5;
static String reset{"\e[00m"};
static String colors[colorLen] = {{"\e[36m"}, {"\e[32m"}, {"\e[31m"}, {"\e[35m"}, {"\e[33m"}};
template <class F, class I>
auto print_container(ostream& out, F&& f, I&& b, I&& e){
    if(cnt){
        printOne(out, f, Char{'\n'});
        for(int i = 0; i < cnt; i++){
            printOne(out, f, Char{'	'});
        }
    }
    printOne(out, f, colors[cnt]);
    printOne(out, f, Char{'['});
    printOne(out, f, reset);
    cnt = (cnt + 1) % colorLen;
    if(b != e){
        printOne(out, f, *b);
        print_containerT(out, f, ++b, e);
    }
    cnt = (colorLen + cnt - 1) % colorLen;
    printOne(out, f, colors[cnt]);
    printOne(out, f, Char{']'});
    printOne(out, f, reset);
}

//--------------------------------------------------------------------------------

template <class F, class A>
void printOne(ostream& out, F&& f, A&& a){
    print(out, f, forward<A>(a));
}

template <class F, class A>
void _prints(ostream& out, F&& f, A&& a){
    printOne(out, f, forward<A>(a));
    printOne(out, f, Char{'\n'});
}
template <class F, class A1, class... As>
void _prints(ostream& out, F&& f, A1&& a, As&&... as){
    printOne(out, f, forward<A1>(a));
    printOne(out, f, Char{' '});
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
using rprint2::printu;

#define printd(...) prints(#__VA_ARGS__, __VA_ARGS__)

//--------------------------------------------------------------------------------

#ifdef RP_DEBUG
struct Elem {
    int a;
    operator std::string() {
        return "a";
    }
};

inline void testPrint(){
    using namespace std;
    using std::vector;
    prints(1);
    prints(1, 2, 3);
    vector<int> v = {1, 2, 3};
    prints(v);
    string s = "abc";
    prints(s);
    prints("abc");
    std::pair<int, int> p = {1, 2};
    prints(p);
    std::unordered_map<int, int> um;
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
    std::map<int, int> mp = {{1, 2}, {4, 5}};
    auto pvm = make_pair(vv, mp);
    std::set<decltype(pvm)> spvm{pvm};
    prints(vvv);
    prints(mp);
    prints(pvm);
    prints(spvm);
    std::set<std::pair<int, int>> sp = {{1, 2}};
    prints(sp);
    prints(1h, 1min, 1s, 1ms, 1us, 1ns);
    prints(1, 2, 3);
    printe(1, 2, 3);
    printw(2, 1, 2, 3);
    printew(2, 1, 2, v);
    printu(std::cout, [](ostream& out, auto e){ out << std::setw(3) << e; }, 1, 2, 3);
}

#endif
