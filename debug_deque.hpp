#include<deque>
#define Func(name)\
    template <class... A>\
    decltype(auto) name(A... args){ return data.name(std::forward<A>(args)...); }
template <class T>
struct DebugDeque {
    std::deque<T> data;
    template <class... A>
    DebugDeque(A... args): data(forward<A>(args)...){ }
    Func(at);
    Func(insert);
    Func(push_back);
    Func(resize);
    Func(back);
    Func(pop_back);
    Func(pop_front);
    Func(size);
    Func(empty);
    Func(begin);
    Func(end);
    Func(rbegin);
    Func(rend);
    Func(clear);
    Func(emplace_back);
    T& operator [] (int idx){
        return data.at(idx);
    }
};
#undef Func

