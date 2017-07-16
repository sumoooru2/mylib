//slow! unstable! undebugged! made in two or three hours!
#include<algorithm>
#include<cmath>
#include<iomanip>
#include<iostream>
#include<map>
#include<numeric>
#include<queue>
#include<set>
#include<sstream>
#include<vector>
using namespace std;

struct BigInt{
    //TODO BCD
    // vector<uint64_t> data;
    vector<bool> data;
    static BigInt lastMod;
    BigInt(){
    }
    BigInt(uint64_t n){
        data.resize(64);
        for(int i=0;i<64;i++){
            data[i] = (n >> i) & 1;
        }
        shrink();
    }
    BigInt(const BigInt& b){
        data = b.data;
    }
    auto operator [] (int idx){
        return data[idx];
    }
    bool operator [] (int idx) const{
        return data[idx];
    }
    // bool operator [] (int idx){
    //     return (data[idx / 64]  >> (idx % 64)) & 1;
    // }
    BigInt operator + (const BigInt& b) const{
        BigInt ret;
        auto& l = width() < b.width() ? b : *this;
        auto& s = width() >= b.width() ? b : *this;
        bool ex = 0;
        ret.resize(l.width() + 1);
        for(int i=0;i<s.width();i++){
            ret[i] = l[i] ^ s[i] ^ ex;
            ex = l[i] + s[i] + ex > 1;
        }
        for(int i=s.width();i<l.width();i++){
            ret[i] = l[i] ^ ex;
            ex = l[i] & ex;
        }
        ret[l.width()] = ex;
        ret.shrink();
        return ret;
    }
    BigInt operator * (const BigInt& b) const{
        BigInt ret;
        auto& l = width() < b.width() ? b : *this;
        auto& s = width() >= b.width() ? b : *this;
        ret.resize(l.width() + s.width());
        for(int i=0;i<s.width();i++){
            if(s[i]){
                ret = ret + (l << i);
            }
        }
        ret.shrink();
        return ret;
    }
    BigInt operator - (const BigInt& b) const{
        BigInt ret;
        auto& l = width() < b.width() ? b : *this;
        auto& s = width() >= b.width() ? b : *this;
        bool br = 0;
        ret.resize(l.width());
        for(int i=0;i<s.width();i++){
            ret[i] = l[i] ^ s[i] ^ br;
            br = l[i] - s[i] - br < 0;
        }
        for(int i=s.width();i<l.width();i++){
            ret[i] = l[i] ^ br;
            br = l[i] - br < 0;
        }
        //TODO minus
        ret.shrink();
        return ret;
    }
    BigInt operator / (const BigInt& r) const{
        BigInt ret;
        BigInt l(*this);
        if(r.zero()){ throw "divide by 0"; }
        ret.resize(l.width());
        for(int sft = -1; ; sft = -1){
            for(int i=0;i<=l.width();i++){
                if(l < (r << i)){
                    sft = i - 1;
                    // l.print64(); r.print64(); (r << i).print64(); l.print(); r.print(); (r << i).print();
                    break;
                }
            }
            if(sft == -1){ break; }
            l = l - (r << sft);
            ret[sft] = 1;
            // printd(sft); l.print(); ret.print(); ret.print64();
        }
        lastMod = l;
        ret.shrink();
        return ret;
    }
    BigInt operator % (const BigInt& b) const{
        *this / b;
        return lastMod;
    }
    BigInt operator << (int w) const{
        BigInt ret;
        ret.resize(width() + w);
        for(int i=0;i<width();i++){
            ret[i + w] = data[i];
        }
        return ret;
    }
    bool operator < (const BigInt& b) const{
        if(width() != b.width()){
            return width() < b.width();
        }
        for(int i=width()-1;i>=0;i--){
            if(!data[i] ^ b[i]){ continue; }
            return b[i];
        }
        return false;
    }
    int width() const{
        return data.size();
    }
    void print() const{
        for(int i=width() - 1;i>=0;i--){
            cout << data[i];
        }
        cout << '\n';
    }
    uint64_t get64() const{
        uint64_t ret = 0, p = 0;
        for(int i=0;i<width();i++){
            ret += data[i] << p;
            p++;
        }
        return ret;
    }
    void print64() const{
        cout << get64() << '\n';
    }
    void printDec() const{
        auto me(*this);
        stringstream ss;
        while(!me.zero()){
            // ss << (me % 10).get64();
            me = me / 10;
            ss << BigInt::lastMod.get64();
        }
        auto str = ss.str();
        reverse(str.begin(), str.end());
        cout << str << '\n';
    }
    void shrink(){
        for(int i=width()-1;i;i--){
            if(!data[i]){
                data.pop_back();
            }else{
                break;
            }
        }
    }
    void resize(int s){
        data.resize(s);
    }
    bool zero() const{
        return (width() == 1 && data[0] == 0);
    }
};
BigInt BigInt::lastMod;

int test(){
    // testPrint();
    BigInt bi(7);
    // prints(bi.data);
    // bi = bi + 1;
    bi.print();
    bi.print64();
    bi = bi * 8;
    bi.print64();
    bi = bi * 12;
    bi.print64();
    bi = bi - 3;
    bi.print();
    bi.print64();
    bi = bi / 2;
    bi.print64();
    // printd(BigInt(13) < (BigInt(2) << 2));
    BigInt c(1);
    c = c / 10;
    c.print64();
    (BigInt(6) * BigInt(4)).print64();
    BigInt b(1);
    for(int i=1;i<=100;i++){
        b = b * i;
        // b.printDec();
        // b.print();
    }
    // b.printDec();
    // b.print();
    return 0;
}
