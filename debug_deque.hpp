#include<deque>
#define Func(name)\
    template <class... A>\
    decltype(auto) name(A&&... args){ return _data.name(std::forward<A>(args)...); }
template <class T>
struct DebugDeque {
    std::deque<T> _data;
    // std::vector<T> _data;
    template <class... A>
    DebugDeque(A... args): _data(std::forward<A>(args)...){ }
    DebugDeque(std::initializer_list<T>&& il): _data(il){ }
    Func(at);
    Func(insert);
    Func(push_back);
    void push_back(const T& il){ _data.push_back(il); }
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
    Func(erase);
    Func(data);
    void reserve(int){ }
    void swap(DebugDeque& dd) noexcept{
        _data.swap(dd._data);
    }
    template <class U>
    bool operator == (U&& v){ return _data == v._data; }
    template <class U>
    bool operator != (U&& v){ return _data != v._data; }
    template <class I>
    T& operator [] (I idx){
        // return _data[idx];
        return _data.at(idx);
    }
    template <class I>
    const T& operator [] (I idx) const {
        // return _data[idx];
        return _data.at(idx);
    }
};
#undef Func

