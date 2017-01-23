#pragma once
#include<algorithm>
#include<iostream>
#include<list>
#include<map>
#include<numeric>
#include<numeric>
#include<queue>
#include<set>
#include<thread>
#include<valarray>
#include<vector>
#define ALL(container) container.begin(), container.end()
#define REV(container) container.rbegin(), container.rend()
#define MAP(container, lambda) transform(ALL(container), container.begin(), lambda)
#define FOLDL(container, init, lambda) accumulate(ALL(vec), init, lambda)
#define FOLDR(container, init, lambda) accumulate(REV(vec), init, lambda)
#define FILTER(container, lambda) auto end = copy_if(ALL(container), container.begin(), lambda); container.resize(end - container.begin())
#define FOR_ALL(container, lambda) std::for_each(container.begin(), container.end(), lambda)
// using namespace std;

//TODO _ -> namespace

#define DEBUG
#define USECOLOR
#define ELEMLIMIT 0 //10

template <class... F>
struct _OLambda : F...{
    _OLambda(F... f): F(f)...{ }
};
//for gcc
template <class S>
struct _OLambda<S> : S{
    _OLambda(S s): S(s){ }
    using S::operator();
};
template <class S, class T>
struct _OLambda<S, T> : S, T{
    _OLambda(S s, T t): S(s), T(t){ }
    using S::operator();
    using T::operator();
};
template <class S, class T, class U>
struct _OLambda<S, T, U> : S, T, U{
    _OLambda(S s, T t, U u): S(s), T(t), U(u){ }
    using S::operator();
    using T::operator();
    using U::operator();
};

template <class T>
struct _Raw{
    T t;
};

using _Char = _Raw<char>;
using _String = _Raw<const char*>;

template <class... S>
auto olambda(S... s){
    return _OLambda<S...>(s...);
}

int _getNestCnt(bool inc, int len = 1e9){
    static int cnt = 0;
    return inc ? cnt++ % len : (len + --cnt) % len;
}
constexpr _String _reset{"\e[00m"};
_String _getColor2(int nest){
#ifdef USECOLOR
    const int colorLen = 6;
    static _String colors[colorLen] = {{"\e[36m"}, {"\e[42m"},{"\e[41m"},{"\e[45m"},{"\e[43m"}, {"\e[46m"}};
    return colors[nest % colorLen];
#else
    return _reset;
#endif
}

template <class T, class = typename T::iterator>
std::true_type iterable(T);
std::false_type iterable(...);

template <std::ostream& out, class T>
auto outable(T t) -> decltype(out << t, std::true_type());
template <std::ostream&>
std::false_type outable(...);

template <class F, class T>
auto callable(F f, T t) -> decltype(f(t), std::true_type());
template <class F, class S, class T>
auto callable(F f, S s, T t) -> decltype(f(s, t), std::true_type());
std::false_type callable(...);

template <class S, class T>
std::true_type is_pair(pair<S, T>);
std::false_type is_pair(...);

template <class S, class = typename std::remove_reference<S>::type::iterator>
auto getAtom(S&& s);

template <class S>
auto getAtom(S&& s) -> typename std::enable_if_t<!decltype(is_pair(s))::value && !decltype(iterable(s))::value, S>{
    return s;
}
template <class S>
auto getAtom(S&& s) -> typename std::enable_if_t<decltype(is_pair(s))::value, decltype(getAtom(s.first))>{
    return getAtom(s.first);
}
template <class T>
char getAtom(_Raw<T>& c){
    return c.t;
}
template <class T>
char getAtom(_Raw<T>&& c){
    return c.t;
}
template <class S, class>
// auto getAtom(S&& s) -> typename std::enable_if_t<!decltype(is_pair(s))::value, decltype(*s.begin())>{
auto getAtom(S&& s){
    return getAtom(*s.begin());
}


template <std::ostream& out, class F, class H, class... T>
void _print(F f, H&& h, T&&... t);

template <std::ostream& out, class F, class S, class T>
auto _printc(F f, S&& color, T&& t){
    _print<out>(f, color, _Raw<T>{t}, _reset);
}

template <std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if_t<decltype(is_pair(t))::value, void>{
    _print<out>(f, _Char{'{'}, t.first, _Char{','}, t.second, _Char{'}'});
}
template <std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if_t<!decltype(is_pair(t))::value && !decltype(outable<out>(t))::value && !decltype(iterable(t))::value, void>{
    f(out, t);
}
template <std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if_t<decltype(outable<out>(t))::value, void>{
// // auto _print(F f, T&& t) -> typename std::enable_if_t<decltype(callable(f, out, t))::value, void>{}
// auto _print(F f, T&& t) -> decltype(f(out, t), void()){}
    f(out, t);
}
template <std::ostream& out, class F, class T>
void _print(F, _Raw<T> r){
    out << r.t;
}
template <std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if_t<!decltype(outable<out>(t))::value && !decltype(iterable(*t.begin()))::value, void>{
    // _print<out>(f, _Char{'['});
    // auto color = _getColor(true);
    int nest = _getNestCnt(true);
    auto color = _getColor2(nest);
    _printc<out>(f, color, '[');
    for(auto& elem : t){
        // _print<out>(f, elem, _Char{&elem == &*t.rbegin() ? ']' : ' '});
        // for(int i=0;i<nest;i++){ _print<out>(f, _String{t.size() > ELEMLIMIT ? "	" : ""}); }
        _print<out>(f, elem);
        // _print<out>(f, elem, _Char{'a'});
        if(&elem == &*t.rbegin()){ _printc<out>(f, color, ']'); }else{ _print<out>(f, _Char{' '}); }
    }
    // if(t.empty()){ _print<out>(f, _Char{']'}); }
    // color = _getColor(false);
    nest = _getNestCnt(false);
    color = _getColor2(nest);
    if(t.empty()){ _printc<out>(f, color, ']'); }
}
template <std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if_t<!decltype(outable<out>(t))::value && decltype(iterable(*t.begin()))::value, void>{
    int nest = _getNestCnt(true);
    auto color = _getColor2(nest);
    // auto color = _getColor(true);
    _printc<out>(f, color, '[');
    if(ELEMLIMIT == 0){ _print<out>(f, _Char{'\n'}); }
    // _print<out>(f, color, _Char{'['}, _reset, _Char{'\n'});
    // _printc<out>(f, color, '[');
    // _printc<out>(f, color, "\n[");
    // _print<out>(f, _String{t.size() > ELEMLIMIT ? "	" : ""});
    for(auto& elem : t){
        for(int i=0;i<=nest;i++){ _print<out>(f, _String{t.size() > ELEMLIMIT ? "	" : ""}); }
        _print<out>(f, elem);
        if(&elem == &*t.rbegin()){ _printc<out>(f, color, ']'); }else{ _print<out>(f, _String{t.size() > ELEMLIMIT ? "\n" : " "}); }
    }
    // if(t.empty()){ _print<out>(f, color, _Char{']'}, _reset); }
    nest = _getNestCnt(false);
    color = _getColor2(nest);
    // color = _getColor(false);
    if(t.empty()){ _printc<out>(f, color, ']'); }
}
template <std::ostream& out, class F, class H, class... T>
void _print(F f, H&& h, T&&... t){
    _print<out>(f, h);
    _print<out>(f, t...);
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
template <std::ostream& out, class... A>
void printHook(A... args){
    // _getColor(true);
    _print<out>(std::forward<A>(args)...);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

template <std::ostream& out = std::cout, class F, class S, class... T>
// auto print(F f, S&& s, T&&... t) -> decltype(out << getAtom(s), void()){
auto print(F f, S&& s, T&&... t) -> decltype(f(out, getAtom(s)), void()){
    printHook<out>(f, s, t...);
}
template <std::ostream& out = std::cout, class... T>
void print(T&&... t){
    print<out>([](std::ostream&, auto&& a){ out << a; }, t...);
}


template <char suffix, std::ostream& out = std::cout, class F, class S, class... T>
auto sprint(F f, S&& s) -> decltype(f(out, getAtom(s)), void()){
    printHook<out>(f, s, _Char{'\n'});
}
template <char suffix, std::ostream& out = std::cout, class F, class S, class... T>
auto sprint(F f, S&& s, T&&... t) -> decltype(typename std::enable_if_t<sizeof...(T) != 0, void>(), f(out, getAtom(s)), void()){
    printHook<out>(f, s, _Char{suffix});
    sprint<suffix, out>(f, t...);
}
template <char suffix, std::ostream& out = std::cout, class R, class S, class... T>
auto sprint(R&& r, S&& s, T... t) -> typename std::enable_if_t<!decltype(callable(r, out, getAtom(s)))::value, void>{
    sprint<suffix, out>([](std::ostream&, auto&& a){ out << a; }, r, s, t...);
}
template <char suffix, std::ostream& out = std::cout, class R, class S>
auto sprint(R&& r, S&& s) -> typename std::enable_if_t<!decltype(callable(r, out, getAtom(s)))::value, void>{
    sprint<suffix, out>([](std::ostream&, auto&& a){ out << a; }, r, s);
}
template <char suffix, std::ostream& out = std::cout, class R>
void sprint(R&& r){
    sprint<suffix, out>([](std::ostream&, auto&& a){ out << a; }, r);
}
//TODO forward
template <std::ostream& out = std::cout, class... F>
void printl(F&&... f){ sprint<'\n', out>(std::forward<F>(f)...); }
template <std::ostream& out = std::cout, class... F>
void printc(F&&... f){ sprint<',', out>(std::forward<F>(f)...); }
template <std::ostream& out = std::cout, class... F>
void prints(F&&... f){ sprint<' ', out>(std::forward<F>(f)...); }
#define printd(var) prints(#var, var);

template <std::ostream& out = std::cout, class T, class... F>
void printPtr(T* base, F&&... f){
    prints<out>(olambda([](std::ostream& out_, auto a){ out_ << a;}, [base](std::ostream& out_, T* p){ out_ << p - base; }), std::forward<F>(f)...);
}

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

#ifdef DEBUG
struct _Elem{
    int x, y;
};
inline void _test_print(){
    using namespace std;
    string s = "abc";
    vector<int> v = {1, 2, 3};
    vector<int*> vp = {&v[0], &v[1], &v[2]};
    vector<vector<int>> vv(3, v);
    vector<vector<vector<int>>> vvv(3, vv);
    vector<vector<vector<vector<int>>>> v4(3, vvv);
    //TODO
    // vector<vector<vector<vector<vector<int>>>>> v5(3, v4);
    map<int, int> mp = {{1, 2}, {4, 5}};
    auto pvm = make_pair(vv, mp);
    set<decltype(pvm)> spvm{pvm};
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
    // prints(v5);
    prints(mp);
    prints(pvm);
    prints(spvm);
    printl(getAtom(mp));
    printl(getAtom(*mp.begin()));
    pair<int , char> p{1, 'a'};
    printl(p);
    printl(getAtom(p));
    printl(1, 2, "aaa", vv, mp);
    printc(1, 2, 3, 'b', "aaa", vv, mp, vvv);
    prints(1, 2, 3, 'c', "aaa", vv, mp, vvv);
    prints(olambda([](ostream& out, auto i){ out << i; }, [](ostream& out, char c){ out << '\'' << c << '\''; }), 1, 2, 3, 'c', "aaa", vv, mp, vvv);
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

}
#endif
