#include<deque>
#define Func(name)\
    template <class... A>\
    decltype(auto) name(A&&... args){ return data.name(std::forward<A>(args)...); }
template <class T>
struct DebugDeque {
    std::deque<T> data;
    // std::vector<T> data;
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
    void reserve(int){ }
    void swap(DebugDeque& dd) noexcept{
        data.swap(dd.data);
    }
    template <class I>
    T& operator [] (I idx){
        // return data[idx];
        return data.at(idx);
    }
    template <class I>
    const T& operator [] (I idx) const {
        // return data[idx];
        return data.at(idx);
    }
};
#undef Func

