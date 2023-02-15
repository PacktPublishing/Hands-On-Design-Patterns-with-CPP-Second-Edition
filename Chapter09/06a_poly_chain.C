#include <iostream>

using std::cout;
using std::endl;

class SortedCollection;
class Collection {
    virtual const char* my_name() const { return "Collection"; }
    public:
    Collection() {}
    Collection filter();
    SortedCollection sort();
    virtual SortedCollection median();
};

class SortedCollection : public Collection {
    const char* my_name() const override { return "SortedCollection"; }
    public:
    SortedCollection() {}
    SortedCollection(const Collection&) {}
    SortedCollection search() { cout << my_name() << "::search" << endl; return *this; }
    SortedCollection median() override { cout << my_name() << "::median" << endl; return *this; }
};

Collection Collection::filter() { cout << my_name() << "::filter" << endl; return *this; }
SortedCollection Collection::sort() { cout << my_name() << "::sort" << endl; return SortedCollection(*this); }
SortedCollection Collection::median() { cout << my_name() << "::median!!!" << endl; return SortedCollection(*this); }

int main() {
    Collection c;
    c.sort().search().filter().median();
}
