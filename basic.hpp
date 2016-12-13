#include<iostream>
#include<vector>
#include<list>
#include<numeric>
#include<map>
#include<thread>
#include<queue>
#include<set>
#include<valarray>
#include<algorithm>
#include<numeric>
#define ALL(container) container.begin(), container.end()
#define REV(container) container.rbegin(), container.rend()
#define MAP(container, lambda) transform(ALL(container), container.begin(), lambda)
#define FOLDL(container, init, lambda) accumulate(ALL(vec), init, lambda)
#define FOLDR(container, init, lambda) accumulate(REV(vec), init, lambda)
#define FILTER(container, lambda) auto end = copy_if(ALL(container), container.begin(), lambda); container.resize(end - container.begin())
#define FOR_ALL(container, lambda) std::for_each(container.begin(), container.end(), lambda)
// using namespace std;

// #define DEBUG

struct _Char{
    char c;
};


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
auto callable(F f, S& s, T t) -> decltype(f(s, t), std::true_type());
std::false_type callable(...);


template <class S>
auto getAtom(S s) -> typename std::enable_if<!decltype(iterable(s))::value, S>::type{
    return s;
}
template <class S, class T>
auto getAtom(std::pair<S, T> s){
    return getAtom(s.first);
}
template <class S, class = typename S::iterator>
auto getAtom(S s){
    return getAtom(*s.begin());
}


template<std::ostream& out, class F, class H, class... T>
void _print(F f, H&& h, T&&... t);
template<std::ostream& out, class F, class T>
auto _print(F f, T t) -> typename std::enable_if<decltype(outable<out>(t))::value, void>::type{
    f(out, t);
}
template<std::ostream& out, class F>
void _print(F, _Char t){
    out << t.c;
}
template<std::ostream& out, class F, class T>
auto _print(F f, T t) -> typename std::enable_if<!decltype(outable<out>(t))::value && !decltype(iterable(*t.begin()))::value, void>::type{
    _print<out>(f, _Char{'['});
    for(auto& elem : t){
        _print<out>(f, elem, _Char{elem == *t.rbegin() ? ']' : ' '});
    }
}
template<std::ostream& out, class F, class T>
auto _print(F f, T t) -> typename std::enable_if<!decltype(outable<out>(t))::value && decltype(iterable(*t.begin()))::value, void>::type{
    _print<out>(f, _Char{'['});
    for(auto& elem : t){
        _print<out>(f, elem, _Char{&elem == &*t.rbegin() ? ']' : t.size() > 10 ? '\n' : ' '});
    }
}
template<std::ostream& out, class F, class S, class T>
void _print(F f, std::pair<S, T> p){
    _print<out>(f, _Char{'{'}, p.first, _Char{','}, p.second, _Char{'}'});
}
template<std::ostream& out, class F, class H, class... T>
void _print(F f, H&& h, T&&... t){
    _print<out>(f, h);
    _print<out>(f, t...);
}

template<std::ostream& out = std::cout, class F, class S, class... T>
auto print(F f, S s, T&&... t) -> decltype(f(out, getAtom(s)), void()){
    _print<out>(f, s, t...);
}
template<std::ostream& out = std::cout, class... T>
void print(T&&... t){
    print<out>([](std::ostream&, auto&& a){ out << a; }, t...);
}

#define _PRINT(func, suffix) \
template<std::ostream& out = std::cout, class F, class S, class... T> \
auto func(F f, S s) -> decltype(f(out, getAtom(s)), void()){ \
    _print<out>(f, s, _Char{'\n'}); \
} \
template<std::ostream& out = std::cout, class F, class S, class... T> \
auto func(F f, S s, T&&... t) -> decltype(f(out, getAtom(s)), void()){ \
    _print<out>(f, s, suffix); \
    func<out>(f, t...); \
} \
template<std::ostream& out = std::cout, class... T> \
void func(T&&... t){ \
    func<out>([](std::ostream&, auto&& a){ out << a; }, t...); \
}

_PRINT(printl, _Char{'\n'})
_PRINT(printc, _Char{','})
_PRINT(prints, _Char{' '})

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
void _test_print(){
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
}
#endif
