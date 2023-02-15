#include <iostream>

using std::cout;
using std::endl;

class SortedCollection;
template <typename T>
class Collection {
    virtual const char* my_name() const { return "Collection"; }
    public:
    Collection() {}
    T filter() { cout << my_name() << "::filter" << endl; return *static_cast<T*>(this); }
    SortedCollection sort();
};

class SortedCollection : public Collection<SortedCollection> {
    const char* my_name() const override { return "SortedCollection"; }
    public:
    SortedCollection() {}
    SortedCollection(const Collection&) {}
    SortedCollection search() { cout << my_name() << "::search" << endl; return *this; }
    SortedCollection median() { cout << my_name() << "::median" << endl; return *this; }
};

template <typename T>
SortedCollection Collection<T>::sort() { cout << "Collection::sort" << endl; return SortedCollection(*this); }

int main() {
    Collection<SortedCollection> c;
    c.sort().search().filter().median();
}
