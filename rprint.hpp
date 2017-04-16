#pragma once
#include<iostream>
#include<vector>

#include<numeric>
#include<map>
#include<set>
#include<chrono>

namespace rprint{

//params
#ifndef RP_CUSTOM
// #define RP_DEBUG
#define RP_USECOLOR
#define RP_WITHOUTNS
#endif

#ifndef RP_ELEMLIMIT
#define RP_ELEMLIMIT 0 //10
#endif

//helpers
#define atov(vec) rprint::_atov(rprint::elems(sizeof(vec), vec), vec)

//print functions
template <class T, T& out, class... F>
void print(F&&... f);
template <class T, T& out, class... F>
void printl(F&&... f);
template <class T, T& out, class... F>
void printc(F&&... f);
template <class T, T& out, class... F>
void prints(F&&... f);
template <std::ostream& out, class... F>
void print(F&&... f);
template <std::ostream& out = std::cout, class... F>
void printl(F&&... f);
template <std::ostream& out = std::cout, class... F>
void printc(F&&... f);
template <std::ostream& out = std::cout, class... F>
void prints(F&&... f);
// #define printd(var) prints(#var, var);
#define printa(arr) prints(atov(arr))
#define printd(...) prints(#__VA_ARGS__, __VA_ARGS__)
// #define printd(...) prints(#__VA_ARGS__), printc(__VA_ARGS__)

#ifdef RP_DEBUG
inline void testPrint();
#endif

struct Debug;
template <class... S>
auto olambda(S... s);


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

template <class... F>
struct OLambda : F...{
    OLambda(F... f): F(f)...{ }
};
//for gcc
//TODO variadic macro
template <class S>
struct OLambda<S> : S{
    OLambda(S s): S(s){ }
    using S::operator();
};
template <class S, class T>
struct OLambda<S, T> : S, T{
    OLambda(S s, T t): S(s), T(t){ }
    using S::operator();
    using T::operator();
};
template <class S, class T, class U>
struct OLambda<S, T, U> : S, T, U{
    OLambda(S s, T t, U u): S(s), T(t), U(u){ }
    using S::operator(); using T::operator(); using U::operator();
};

template <class... S>
auto olambda(S... s){
    return OLambda<S...>(s...);
}

inline auto defaultLambda(){
    return olambda(
        [](auto&& _out, bool b){ _out << (b ? "true" : "false"); },
        [](auto&& _out, auto&& a) -> decltype(_out << a, void()){ _out << a; },
#ifdef __clang__
        [](auto&& _out, std::chrono::milliseconds m){ _out << m.count() << "ms"; },
        [](auto&& _out, std::chrono::seconds s){ _out << s.count() << "sec"; },
#endif
        [](auto&& _out, auto&& d) -> decltype(std::chrono::duration_cast<std::chrono::nanoseconds>(d), void()){ _out << std::chrono::duration_cast<std::chrono::nanoseconds>(d).count() << "ns"; }
        );
}

struct Debug{
    std::ostream& out = std::cerr;
    Debug(std::ostream& out = std::cerr): out(out){}
    template<class T>
    Debug& operator << (T t){
#ifdef LOCAL
        out << t;
#endif
        return *this;
    }
};

template <class T>
struct _Raw{
    T t;
};

using _Char = _Raw<char>;
using _String = _Raw<const char*>;

//for gcc (operator !)
struct tTrue : public std::true_type{ };
struct tFalse : public std::false_type{ };

inline int getNestCnt(bool inc, int len = 1e9){
    static int cnt = 0;
    return inc ? cnt++ % len : (len + --cnt) % len;
}
constexpr _String _reset{"\e[00m"};
inline _String getColor(int nest){
#ifdef RP_USECOLOR
    const int colorLen = 6;
    // static _String colors[colorLen] = {{"\e[36m"}, {"\e[42m"},{"\e[41m"},{"\e[45m"},{"\e[43m"}, {"\e[46m"}};
    static _String colors[colorLen] = {{"\e[36m"}, {"\e[32m"},{"\e[31m"},{"\e[35m"},{"\e[33m"}, {"\e[36m"}};
    return colors[nest % colorLen];
#else
#pragma unused(nest)
    return _reset;
#endif
}

template <class... A>
void __print(A&&... args);

template <class T, class = typename T::iterator>
tTrue iterable(T);
tFalse iterable(...);

template <class S, class T>
auto outable(S& out, T t) -> decltype(out << t, tTrue());
tFalse outable(...);

// template <class F, class T>
// auto printable(F f, T&& s) -> decltype(__print(std::cout, f, s), tTrue());
// template <class F, class S, class T>
// auto printable(F f, S&& s, T&& t) -> decltype(__print(std::cout, f, s), tTrue());
// tFalse printable(...);

template <class F, class T>
auto callable(F f, T&& t) -> decltype(f(t), tTrue());
template <class F, class S, class T>
auto callable(F f, S&& s, T&& t) -> decltype(f(s, t), tTrue());
// template <class F, class S>
// auto callable(F f, S&& s, std::chrono::milliseconds t) -> decltype(tTrue());
tFalse callable(...);

// template <class <class A, class B> P, class S, class T>
// tTrue isPair(P<S, T>);
template <class S, class T>
tTrue isPair(std::pair<S, T>);
tFalse isPair(...);

tFalse vtPositive(...);
template <class... S>
auto vtPositive(S...) -> typename std::enable_if_t<sizeof...(S) != 0, tTrue>;

tFalse tAnd(...);
template <class S>
auto tAnd(S) -> std::enable_if_t<S::value, tTrue>;
template <class S, class... T>
auto tAnd(S, T... t) -> std::enable_if_t<S::value && decltype(tAnd(t...))::value, tTrue>;

tFalse tOr(...);
template <class S>
auto tOr(S) -> std::enable_if_t<S::value, tTrue>;
template <class S, class... T>
auto tOr(S, T... t) -> std::enable_if_t<S::value || decltype(tOr(t...))::value, tTrue>;

tTrue tNot(tFalse);
tFalse tNot(tTrue);

tTrue operator ! (tFalse);
tFalse operator ! (tTrue);

tFalse operator || (tFalse, tFalse);
tTrue operator || (tTrue, tFalse);
tTrue operator || (tFalse, tTrue);
tTrue operator || (tTrue, tTrue);

template <class R = void, class S>
auto ifAll(S) -> std::enable_if_t<S::value, R>;
template <class R = void, class S, class... T>
auto ifAll(S, T... t) -> std::enable_if_t<S::value && decltype(tAnd(t...))::value, R>;


template <class S, class = typename std::remove_reference<S>::type::iterator>
auto getAtom(S&& s);
template <class S>
auto getAtom(S&& s) -> decltype(ifAll<S>(!isPair(s), !iterable(s))){
    return s;
}
template <class S>
auto getAtom(S&& s) -> decltype(ifAll(isPair(s)), getAtom(s.first)){
    return getAtom(s.first);
}
template <class S, class>
auto getAtom(S&& s){
    return getAtom(*s.begin());
}
template <class T>
T getAtom(_Raw<T> c){ return c.t; }


template <class T>
int elems(int size, T []){
    return size / sizeof(T);
}
template <class T, int S>
int elems(int size, T (*)[S]){
    return size / sizeof(T) / S;
}
template <class T>
auto _atov(int size, T (*arr)){
    return std::vector<T>(arr, arr + size);
}
template <class T, int S>
auto _atov(int size, T (*arr)[S]){
    std::vector<decltype(_atov(S, arr[0]))> ret;
    ret.reserve(size);
    for(int i=0;i<size;i++){
        ret.emplace_back(_atov(S, arr[i]));
    }
    return ret;
}

//--------------------------------------------------------------------------------

template <class C, class F, class S, class T>
auto _printc(C& out, F f, S&& color, T&& t){
    __print(out, f, color, _Raw<T>{t}, _reset);
}

template <class C, class F, class T>
auto _print(C& out, F f, T&& t) -> decltype(ifAll(!isPair(t), !iterable(t) || outable(out, t))){
    f(out, t);
}
template <class C, class F, class T>
void _print(C& out, F, _Raw<T> r){
    out << r.t;
}
template <class C, class F, class T>
auto _print(C& out, F f, T&& t) -> decltype(ifAll(isPair(t))){
    __print(out, f, _Char{'{'}, t.first, _Char{','}, t.second, _Char{'}'});
}
template <class C, class F, class T>
auto _print(C& out, F f, T&& t) -> decltype(ifAll(!outable(out, t), iterable(t))){
    bool bottom = !decltype(iterable(*t.begin()))::value,
         nestPrint = !bottom && RP_ELEMLIMIT < t.size();
    int nest = getNestCnt(true);
    auto color = getColor(nest);
    _printc(out, f, color, '[');
    if(!bottom && RP_ELEMLIMIT == 0){ _print(out, f, _Char{'\n'}); }
    for(auto ite = t.begin(); ite != t.end(); ite++){
        auto& elem = *ite;
        if(nestPrint){
            for(int i=0;i<=nest;i++){ _print(out, f, _String{"	"}); }
        }
        __print(out, f, elem);
        auto e = ite;
        e++;
        if(&*e == &*t.end()){
            _printc(out, f, color, ']');
        }else{
            __print(out, f, _String{nestPrint ? "\n" : " "});
        }
    }
    if(t.empty()){ _printc(out, f, color, ']'); }
    getNestCnt(false);
}
template <class C, class F, class H, class... T>
auto _print(C& out, F f, H&& h, T&&... t) -> decltype(ifAll(vtPositive(t...))){
    __print(out, f, h);
    __print(out, f, t...);
}

template <class... A>
void __print(A&&... args){
    _print(std::forward<A>(args)...);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
template <class C, C& out, class... A>
void printHook(A&&... args){
    _print(out, std::forward<A>(args)...);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

template <class C = std::ostream, C& out = std::cout, class F, class S, class... T>
auto print(F f, S&& s, T&&... t) -> decltype(ifAll(callable(f, out, getAtom(s)))){
    printHook<C, out>(f, s, t...);
}
template <class C = std::ostream, C& out = std::cout, class... T>
void print(T&&... t){
    print<C, out>(defaultLambda(), t...);
}

template <char suffix, class C = std::ostream, C& out = std::cout, class... A>
void _sprint(A&&... args);

template <char suffix, class C = std::ostream, C& out = std::cout, class F, class S, class... T>
// auto sprint(F f, S&& s) -> decltype(f(out, getAtom(s)), void()){
auto sprint(F f, S&& s) -> decltype(ifAll(callable(f, out, getAtom(s)))){
    printHook<C, out>(f, s, _Char{'\n'});
}
template <char suffix, class C = std::ostream, C& out = std::cout, class F, class S, class... T>
auto sprint(F f, S&& s, T&&... t) -> decltype(ifAll(vtPositive(t...), callable(f, out, getAtom(s)))){
    printHook<C, out>(f, s, _Char{suffix});
    _sprint<suffix, C, out>(f, t...);
}
template <char suffix, class C = std::ostream, C& out = std::cout, class R, class S, class... T>
auto sprint(R&& r, S&& s, T&&... t) -> decltype(ifAll(!callable(r, out, getAtom(s)))){
    _sprint<suffix, C, out>(defaultLambda(), r, s, t...);
}
template <char suffix, class C = std::ostream, C& out = std::cout, class R>
void sprint(R&& r){
    _sprint<suffix, C, out>(defaultLambda(), r);
}

template <char suffix, class C, C& out, class... A>
void _sprint(A&&... args){
    sprint<suffix, C, out>(std::forward<A>(args)...);
}


// template <std::ostream& out, class... F>
// void printcs(F&&... f){ sprint<',', out>(std::forward<F>(f)...); }
// template <class T, T& out, class... F>
// void print(F&&... f){ print<T, out>(std::forward<F>(f)...); }
template <class T, T& out, class... F>
void printl(F&&... f){ sprint<'\n', T, out>(std::forward<F>(f)...); }
template <class T, T& out, class... F>
void printc(F&&... f){ sprint<',', T, out>(std::forward<F>(f)...); }
template <class T, T& out, class... F>
void prints(F&&... f){ sprint<' ', T, out>(std::forward<F>(f)...); }
template <std::ostream& out, class... F>
void print(F&&... f){ print<std::ostream, out>(std::forward<F>(f)...); }
template <std::ostream& out, class... F>
void printl(F&&... f){ printl<std::ostream, out>(std::forward<F>(f)...); }
template <std::ostream& out, class... F>
void printc(F&&... f){ printc<std::ostream, out>(std::forward<F>(f)...); }
template <std::ostream& out, class... F>
void prints(F&&... f){ prints<std::ostream, out>(std::forward<F>(f)...); }

//TODO ok?
// template <std::ostream& out = std::cout, class T, class... F>
// void printPtr(T* base, F&&... f){
//     prints<out>(olambda([](std::ostream& out_, auto a){ out_ << a;}, [base](std::ostream& out_, T* p){ out_ << p - base; }), std::forward<F>(f)...);
// }

//--------------------------------------------------------------------------------

// template <class T>
// void info(T&& t){
//     print("type ", typeid(t).name(), '\n');
//     print(t, '\n');
// }
// template <class H, class... T>
// void info(H&& h, T&&... t){
//     info(h);
//     info(t...);
// }



#ifdef RP_DEBUG
struct _Elem{
    int x, y;
};
inline void testPrint(){
    using namespace std;
    string s = "abc";
    vector<int> v = {1, 2, 3};
    vector<int*> vp = {&v[0], &v[1], &v[2]};
    vector<vector<int>> vv(3, v);
    vector<vector<vector<int>>> vvv(3, vv);
    vector<vector<vector<vector<int>>>> v4(3, vvv);
    vector<vector<vector<vector<vector<int>>>>> v5(3, v4);
    map<int, int> mp = {{1, 2}, {4, 5}};
    auto pvm = make_pair(vv, mp);
    set<decltype(pvm)> spvm{pvm};
    prints(s);
    print(1);
    print(vector<int>());
    print(1, 'a', v, vp, vv, vvv, mp);
    print([](ostream& out, auto i){ out << i; }, 1, 'a', "aiueo", v, vv, mp);
    cout<<endl;
    print(v);
    cout<<endl;
    print([](ostream& out, auto i){ out << i; }, v);
    cout<<endl;
    print([](ostream& out, auto i){ out << i; }, vp);
    cout<<endl;
    print([](ostream& out, auto i){ out << *i; }, vp);
    cout<<endl;
    prints(v4);
    prints(v5);
    prints(mp);
    prints(pvm);
    prints(spvm);
    tuple<int, char> t;
    //TODO
    // prints(t);
    printl(getAtom(mp));
    printl(getAtom(*mp.begin()));
    pair<int , char> p{1, 'a'};
    printl(p);
    printl(getAtom(p));
    printl(1, 2, "aaa", vv, mp);
    printc(1, 2, 3, 'b', "aaa", vv, mp, vvv);
    prints(1, 2, 3, 'c', "aaa", vv, mp, vvv);
    prints(olambda([](ostream& out, auto i){ out << i; }, [](ostream& out, char c){ out << '\'' << c << '\''; }), 1, 2, 3, 'c', "aaa", vv, mp, vvv);
    _Elem e{1, 2};
    prints([](ostream& out, _Elem e){out << e.x; }, e);
    vector<_Elem> ve(3, {1, 2});
    prints([](ostream& out, _Elem e){out << e.x; }, ve);
    set<pair<int, int>> sp = {{1, 2}};
    prints(sp);
    printd(v.back());
    vector<int> v2(20);
    iota(v2.begin(), v2.end(), 10);
    prints(v2);
    vector<vector<int>> v3(11, vector<int> (11));
    prints(v3);
    prints(getAtom(_Char{'a'}));
    prints(_Char{'a'});
    prints(_Raw<int>{1});
    // prints(decltype(outable<std::cout>([](auto a){;}))::value);
    int c[2][3][4];
    prints(atov(c));
    printa(c);
    int d[10];
    printa(d);
    printd(v3);
    printd(v2, p);
    prints(1s);
    prints(1ms);
    prints(1us);
    prints(1ns);
    prints<cerr>(1, 2, 3);
    prints<ostream, cerr>(1, 2, 3);
}
#endif

} //namespace rprint

#ifdef RP_WITHOUTNS
#ifdef RP_DEBUG
using rprint::testPrint;
#endif
using rprint::olambda;
using rprint::print;
using rprint::printl;
using rprint::printc;
using rprint::prints;
// using rprint::printd;
#endif
