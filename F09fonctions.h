#ifndef FONCTIONS_H_INCLUDED
#define FONCTIONS_H_INCLUDED

#include "F09structure.h"

template<typename T>
bool existe(list<T> mylist, T x){
    return find(mylist.begin(), mylist.end(), x) != mylist.end();

}

template<typename T>
bool existe(vector<T> mylist, T x){
    return find(mylist.begin(), mylist.end(), x) != mylist.end();

}

template<typename T1, typename T2>
bool existe(map<T1, T2> mymap, T1 x){
    return mymap.find(x) != mymap.end();

}

template<typename T>
std::string var2String(T x)
{
    std::stringstream type;
    type << x;
    return type.str();
}

template <typename T>
struct Assembler
{
    const set<T> _set;
    template <typename It> Assembler(const It& begin, const It& end) : _set(begin, end) {}
    bool operator()(const T& i) const
    {
        return _set.end() != _set.find(i);
    }
};

template <typename T>
void assemblerSansDoublons(vector<T>& a, vector<T>& b)
{
    std::remove_copy_if(b.begin(), b.end(), back_inserter(a), Assembler<T>(a.begin(), a.end()));
}

template <typename T>
void assemblerSansDoublons(vector<T>& a, T& x)
{
    vector<T> b;
    b.push_back(x);

    std::remove_copy_if(b.begin(), b.end(), back_inserter(a), Assembler<T>(a.begin(), a.end()));
}

template <typename T>
void supprimer(vector<T> & x, T value)
{
    x.erase(std::remove(x.begin(), x.end(), value), x.end());
}


#endif // FONCTIONS_H_INCLUDED
