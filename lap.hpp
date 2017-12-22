#pragma once
#include<chrono>
#include<iomanip>
#ifdef LOCAL
#include"rprint2.hpp"
#else
template <class... T> void printl(T&&...){ }
template <class... T> void printc(T&&...){ }
template <class... T> void prints(T&&...){ }
template <class... T> void printd(T&&...){ }
#endif

class Lap{
    std::chrono::high_resolution_clock clk;
    decltype(clk.now()) start;
public:
    Lap(){
        init();
    }
    void init(){
        start = clk.now();
    }
    template<class T = std::chrono::milliseconds>
    T lap(){
        auto t = clk.now();
        auto ret = t - start;
        start = t;
        return std::chrono::duration_cast<T>(ret);
    }
    // template<class T = std::chrono::milliseconds>
    // long long lap_(T unit = T(1)){
    //     auto t = clk.now();
    //     auto ret = t - start;
    //     start = t;
    //     return ret / unit;
    // }
    template<class F>
    void dur(F f){
        init();
        f();
        prints(lap());
    }
    template<class F, class... Args>
    void dur(F f, Args&&... args){
        init();
        f(std::forward<Args>(args)...);
        prints(lap());
    }
    template<class F, class T>
    void graph(F f, int b, int e, T unit){
        auto prev = std::chrono::milliseconds(0);
        for(int i=b;i<e;i++){
            init();
            f(i);
            auto lp = lap();
            int len = lp / unit;
            std::cout << std::setw(4) << i << ' ';
            for(int i=0;i<len;i++){
                print('+');
            }
            print(lp - prev);
            prev = lp;
            std::cout << '\n';
        }
    }
};
