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
using namespace std;

// template<class T>
// auto iterable() -> decltype(T::iterator, true_type());
template<class T>
auto iterable(T t) -> decltype(t.begin(), true_type());
false_type iterable(...);
template<class T, bool = !is_same<typename T::iterator, typename T::iterator>::value>
// auto iterable() -> decltype(false_type());
false_type iterable();
// auto iterable() -> decltype(false_type());

template<class T>
auto coutable(T t) -> decltype(cout << t, true_type());
false_type coutable(...);

template<class H, class... T> void print(H h, T... t);
// template<class T> auto print(T t) -> decltype(cout << t, void()){ cout << t; }
// void ptint(){}
// template<> void print<string>(string t){ cout << t; }
// template<> void print(string t){ cout << t; }
// template<class T> auto print(T t) -> decltype(t.begin(), void()){ cout << endl; for(auto& elem : t){ print(elem, ' '); }  }
// template<class T> auto print(T t) -> typename enable_if<!decltype(coutable(t))::value, void>::type{ cout << endl; for(auto& elem : t){ print(elem, ' '); }  }
// template<class T> auto print(T t) -> typename enable_if<!decltype(iterable(t.begin()), coutable(t))::value, void>::type{ cout << endl; for(auto& elem : t){ print(elem, ' '); }  }
template<class T> auto print(T t) -> typename enable_if<decltype(coutable(t))::value, void>::type{ cout << t; }
template<class T> auto print(T t) -> typename enable_if<!decltype(coutable(t))::value && !decltype(iterable(*t.begin()))::value, void>::type{  for(auto& elem : t){ print(elem, ' '); }  }
template<class T> auto print(T t) -> typename enable_if<!decltype(coutable(t))::value && decltype(iterable(*t.begin()))::value, void>::type{  for(auto& elem : t){ print(elem, '\n'); }  }
// template<class T, class = typename T::iterator>
// template<class T, class = decltype(iterable<T>())>
// void print(T t){  for(auto& elem : t){ print(elem, ' '); }cout << endl;  }
template<class S, class T> void print(pair<S, T>& p)            { print('{', p.first, ',', p.second, '}'); }
template<class H, class... T> void print(H h, T... t)           { print(h); print(t...); }
template<class T> void println(T t)                             { print(t, '\n'); }

// template<class T>
// auto f(T t) -> void{
//     cout<<1<<endl;
// }
// template<>
// void f(string t){
//     cout<<2<<endl;
// }
//
// template<class T>
// auto has_begin(T t) -> decltype(t.begin(), true_type());
// auto has_begin(...) -> decltype(false_type());
//
// template<class T>
// auto iterable() -> decltype(T::iterator, true_type());
// auto iterable() -> decltype(false_type());
//
// template<class T> auto print(T t) -> decltype(cout << t, void()){ cout << t; }
// // template<class T> auto print(T t) -> decltype(t.begin(), void()){ cout << endl;    }
// template<> void print(string t){ cout << t; }
//
// template<class T, class = typename T::iterator>
// // auto aaa(T t) -> typename enable_if<decltype(has_begin(t))::value>::type{
// auto aaa(T t) -> decltype(typename T::iterator(), void()){
// // auto aaa(T t) -> typename enable_if<decltype(has_begin(t))::value>::type{
// // template<class T, typename enable_if<decltype(has_begin(t))::value, int>::type tt = 0>
// // void aaa(T t){
//     cout<<1<<endl;
// }
// // template<class T>
// // auto aaa(T t) -> typename enable_if<!has_begin(t)>::type{
// //     cout<<2<<endl;
// // }
// template<class T>
// auto aaa(T t) -> typename enable_if<!decltype(has_begin(t))::value>::type{
// // void aaa(T t){
//     cout<<2<<endl;
// }
