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

template<class T> auto iterable(T t) -> decltype(t.begin(), std::true_type());
std::false_type iterable(...);

template<class T> auto coutable(T t) -> decltype(std::cout << t, std::true_type());
std::false_type coutable(...);

template<class H, class... T>
void print(H&& h, T&&... t);

template<class T>
auto print(T& t) -> typename std::enable_if<decltype(coutable(t))::value, void>::type{
    std::cout << t;
}
template<class T>
auto print(T& t) -> typename std::enable_if<!decltype(coutable(t))::value && !decltype(iterable(*t.begin()))::value, void>::type{
    for(auto& elem : t){ print(elem, ' '); }
}
template<class T>
auto print(T& t) -> typename std::enable_if<!decltype(coutable(t))::value && decltype(iterable(*t.begin()))::value, void>::type{
    for(auto& elem : t){ print(elem, '\n'); }
}
template<class S, class T>
void print(std::pair<S, T>& p){
    print('{', p.first, ',', p.second, '}');
}

template<class H, class... T>
void print(H&& h, T&&... t){
    print(h);
    print(t...);
}

template<class T>
void println(T&& t){
    print(t, '\n');
}
template<class H, class... T>
void println(H&& h, T&&... t){
    println(h);
    println(t...);
}

template<class T>
void prints(T&& t){
    print(t);
}
template<class H, class... T>
void prints(H&& h, T&&... t){
    print(h, ' ');
    prints(t...);
}

template<class T>
void info(T&& t){
    print("type ", typeid(t).name(), '\n');
    print(t, '\n');
}
template<class H, class... T>
void info(H&& h, T&&... t){
    info(h);
    info(t...);
}

// template<class... T>
// void println(T&&... t){
//     print(forward<T>(t)..., '\n');
// }
