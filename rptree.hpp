#pragma once
#include<iostream>
#include<memory>
#include<queue>
#include<unordered_set>
#include<vector>

namespace rptree{

template <class Node, class F, class V = int>
void printTree(std::ostream& out, F&& f, Node* root, V Node::* value = &Node::value){
    struct N;
    using std::vector; using std::string; using std::queue;
    using std::shared_ptr; using std::make_shared;
    using uint = unsigned int;
    using Np = shared_ptr<N>;
    struct N{
        string value;
        size_t width = 0;
        int pos = 0;
        vector<Np> dests;
        N(int v): value(std::to_string(v)){ }
        int setWidth(int p = 0){
            pos = p;
            for(Np n : dests){
                width += n->setWidth(pos + width);
            }
            width = std::max(width, value.size() + 2);
            return width;
        }
        void print(string& s, string& s2, string& s3){
            int w = width - value.size();
            for(uint i = 0; i < value.size(); i++){
                s[pos + (w + 1) / 2 + i] = value[i];
            }
            if(dests.empty()){
                return;
            }
            s2[pos + (width + 0) / 2] = '|';
            if(dests.size() == 1){
                s3[pos + (width + 0) / 2] = '|';
            }else{
                for(uint i = dests[0]->width / 2; i < width - dests.back()->width / 2; i++){
                    s3[pos + i] = '-';
                }
            }
        }
    };

    Np nroot = make_shared<N>(root->*value);
    queue<std::pair<Node*, Np>> q;
    q.push({root, nroot});
    std::unordered_set<Node*> visited;
    while(q.size()){
        Node* n = q.front().first;
        Np n2 = q.front().second;
        q.pop();
        if(visited.count(n)){ continue; }
        visited.insert(n);
        for(Node* d : n->dests){
            if(visited.count(d)){ continue; }
            Np np = make_shared<N>(d->*value);
            n2->dests.push_back(np);
            q.emplace(d, np);
        }
    }
    int width = nroot->setWidth();

    queue<Np> q2, next;
    q2.emplace(nroot);
    vector<string> elems, elems2, elems3;
    out << '\n';
    while(1){
        string s(width, ' '), s2 = s, s3 = s;
        while(q2.size()){
            Np n = q2.front();
            q2.pop();
            n->print(s, s2, s3);
            for(Np d : n->dests){
                next.emplace(d);
            }
        }
        f(out, s); out << '\n';
        if(next.empty()){ break; }
        f(out, s2); out << '\n';
        f(out, s3); out << '\n';
        q2.swap(next);
    }
}

template <class Node, class V = int>
void printTree(std::ostream& out, Node* root, V Node::* value = &Node::value){
    printTree(out, [](std::ostream& out, auto e){ out << e; }, root, value);
}

} // namespace rptree

using rptree::printTree;

