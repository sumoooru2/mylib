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

#define DEBUG

template <class... F>
struct OLambda : F...{
    OLambda(F... f): F(f)...{ }
};
//for gcc
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
    using S::operator();
    using T::operator();
    using U::operator();
};

template <class... S>
auto olambda(S... s){
    return OLambda<S...>(s...);
}

template<class T>
struct _Raw{
    T t;
};

using _Char = _Raw<char>;


template<class T, class = typename T::iterator>
std::true_type iterable(T);
std::false_type iterable(...);

template<std::ostream& out, class T>
auto outable(T t) -> decltype(out << t, std::true_type());
template<std::ostream&>
std::false_type outable(...);

template<class F, class T>
auto callable(F f, T t) -> decltype(f(t), std::true_type());
template<class F, class S, class T>
auto callable(F f, S s, T t) -> decltype(f(s, t), std::true_type());
std::false_type callable(...);


template<class T>
char getAtom(_Raw<T>& c){
    return c.t;
}
template<class T>
char getAtom(_Raw<T>&& c){
    return c.t;
}
template <class S>
auto getAtom(S&& s) -> typename std::enable_if<!decltype(iterable(s))::value, S>::type{
    return s;
}
template <class S, class T>
auto getAtom(std::pair<S, T>& s){
    return getAtom(s.first);
}
template <class S, class T>
auto getAtom(std::pair<S, T>&& s){
    return getAtom(s.first);
}
template <class S, class T>
auto getAtom(const std::pair<S, T>& s){
    return getAtom(s.first);
}
template <class S, class T>
auto getAtom(const std::pair<S, T>&& s){
    return getAtom(s.first);
}
template <class S, class = typename std::remove_reference<S>::type::iterator>
auto getAtom(S&& s){
    return getAtom(*s.begin());
}


template<std::ostream& out, class F, class H, class... T>
void _print(F f, H&& h, T&&... t);
template<std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if<!decltype(outable<out>(t))::value && !decltype(iterable(t))::value, void>::type{
    f(out, t);
}
template<std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if<decltype(outable<out>(t))::value, void>::type{
// // auto _print(F f, T&& t) -> typename std::enable_if<decltype(callable(f, out, t))::value, void>::type{
// auto _print(F f, T&& t) -> decltype(f(out, t), void()){
    f(out, t);
}
template<std::ostream& out, class F>
void _print(F, _Char c){
    out << c.t;
}
template<std::ostream& out, class F, class T>
void _print(F, _Raw<T> r){
    out << r.t;
}
template<std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if<!decltype(outable<out>(t))::value && !decltype(iterable(*t.begin()))::value, void>::type{
    _print<out>(f, _Char{'['});
    for(auto& elem : t){
        _print<out>(f, elem, _Char{&elem == &*t.rbegin() ? ']' : ' '});
    }
    if(t.empty()){ _print<out>(f, _Char{']'}); }
}
template<std::ostream& out, class F, class T>
auto _print(F f, T&& t) -> typename std::enable_if<!decltype(outable<out>(t))::value && decltype(iterable(*t.begin()))::value, void>::type{
    _print<out>(f, _Raw<const char*>{t.size() > 10 ? "[	" : "["});
    for(auto& elem : t){
        _print<out>(f, elem, _Raw<const char*>{&elem == &*t.rbegin() ? "]" : t.size() > 10 ? "\n	" : " "});
    }
    if(t.empty()){ _print<out>(f, _Char{']'}); }
}
template<std::ostream& out, class F, class S, class T>
void _print(F f, const std::pair<S, T> p){
    _print<out>(f, _Char{'{'}, p.first, _Char{','}, p.second, _Char{'}'});
}
template<std::ostream& out, class F, class H, class... T>
void _print(F f, H&& h, T&&... t){
    _print<out>(f, h);
    _print<out>(f, t...);
}

template<std::ostream& out = std::cout, class F, class S, class... T>
// auto print(F f, S&& s, T&&... t) -> decltype(out << getAtom(s), void()){
auto print(F f, S&& s, T&&... t) -> decltype(f(out, getAtom(s)), void()){
    _print<out>(f, s, t...);
}
template<std::ostream& out = std::cout, class... T>
void print(T&&... t){
    print<out>([](std::ostream&, auto&& a){ out << a; }, t...);
}


template<char suffix, std::ostream& out = std::cout, class F, class S, class... T>
auto sprint(F f, S&& s) -> decltype(f(out, getAtom(s)), void()){
    _print<out>(f, s, _Char{'\n'});
}
template<char suffix, std::ostream& out = std::cout, class F, class S, class... T>
auto sprint(F f, S&& s, T&&... t) -> decltype(typename std::enable_if<sizeof...(T) != 0, void>::type(), f(out, getAtom(s)), void()){
    _print<out>(f, s, _Char{suffix});
    sprint<suffix, out>(f, t...);
}
template<char suffix, std::ostream& out = std::cout, class R, class S, class... T>
auto sprint(R&& r, S&& s, T... t) -> typename std::enable_if<!decltype(callable(r, out, getAtom(s)))::value, void>::type{
    sprint<suffix, out>([](std::ostream&, auto&& a){ out << a; }, r, s, t...);
}
template<char suffix, std::ostream& out = std::cout, class R, class S>
auto sprint(R&& r, S&& s) -> typename std::enable_if<!decltype(callable(r, out, getAtom(s)))::value, void>::type{
    sprint<suffix, out>([](std::ostream&, auto&& a){ out << a; }, r, s);
}
template<char suffix, std::ostream& out = std::cout, class R>
void sprint(R&& r){
    sprint<suffix, out>([](std::ostream&, auto&& a){ out << a; }, r);
}
//TODO forward
template<std::ostream& out = std::cout, class... F>
void printl(F&&... f){ sprint<'\n', out>(std::forward<F>(f)...); }
template<std::ostream& out = std::cout, class... F>
void printc(F&&... f){ sprint<',', out>(std::forward<F>(f)...); }
template<std::ostream& out = std::cout, class... F>
void prints(F&&... f){ sprint<' ', out>(std::forward<F>(f)...); }
#define printd(var) prints(#var, var);

template<std::ostream& out = std::cout, class T, class... F>
void printPtr(T* base, F&&... f){
    prints<out>(olambda([](std::ostream& out_, auto a){ out_ << a;}, [base](std::ostream& out_, T* p){ out_ << p - base; }), std::forward<F>(f)...);
}

// template<class T>
// void info(T&& t){
//     print("type ", typeid(t).name(), '\n');
//     print(t, '\n');
// }
// template<class H, class... T>
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
    map<int, int> mp = {{1, 2}, {4, 5}};
    print(1);
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
    print(mp);
    printl(getAtom(mp));
    printl(getAtom(*mp.begin()));
    pair<int , char> p{1, 'a'};
    printl(p);
    printl(getAtom(p));
    printl(1, 2, 3, 'a', "aaa", vv, mp, vvv);
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
